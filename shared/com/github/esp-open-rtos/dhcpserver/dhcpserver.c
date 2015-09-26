/* Very basic LWIP & FreeRTOS-based DHCP server
 *
 * Based on RFC2131 http://www.ietf.org/rfc/rfc2131.txt
 * ... although not fully RFC compliant yet.
 *
 * TODO
 * * Allow binding on a single interface only (for mixed AP/client mode), lwip seems to make it hard to
 *   listen for or send broadcasts on a specific interface only.
 *
 * * Probably allocates more memory than it should, it should be possible to reuse netbufs in most cases.
 *
 * Part of esp-open-rtos
 * Copyright (C) 2015 Superhouse Automation Pty Ltd
 * BSD Licensed as described in the file LICENSE
 */
#include <string.h>

#include <FreeRTOS.h>
#include <task.h>
#include <lwip/netif.h>
#include <lwip/api.h>

/* Grow the size of the lwip dhcp_msg struct's options field, as LWIP
   defaults to a 68 octet options field for its DHCP client, and most
   full-sized clients send us more than this. */
#define DHCP_OPTIONS_LEN 312

#include <lwip/dhcp.h>

//_Static_assert(sizeof(struct dhcp_msg) == offsetof(struct dhcp_msg, options) + 312, "dhcp_msg_t should have extended options size");

#include <lwip/netbuf.h>

#include "dhcpserver.h"

#define DTXT(...)       printf(__VA_ARGS__)

typedef struct {
    uint8_t hwaddr[NETIF_MAX_HWADDR_LEN];
    uint32_t expires;
} dhcp_lease_t;

typedef struct {
    struct netconn *nc;
    uint8_t max_leases;
    ip_addr_t first_client_addr;
    struct netif *server_if;
    dhcp_lease_t *leases; /* length max_leases */
} server_state_t;

/* Only one DHCP server task can run at once, so we have global state
   for it.
*/
static xTaskHandle dhcpserver_task_handle;
static server_state_t *state;

/* Handlers for various kinds of incoming DHCP messages */
static void handle_dhcp_discover(struct dhcp_msg *received);
static void handle_dhcp_request(struct dhcp_msg *dhcpmsg);
static void handle_dhcp_release(struct dhcp_msg *dhcpmsg);

static void send_dhcp_nak(struct dhcp_msg *dhcpmsg);

static void dhcpserver_task(void *pxParameter);

/* Utility functions */
static uint8_t *find_dhcp_option(struct dhcp_msg *msg, uint8_t option_num, uint8_t min_length, uint8_t *length);
static uint8_t *add_dhcp_option_byte(uint8_t *opt, uint8_t type, uint8_t value);
static uint8_t *add_dhcp_option_bytes(uint8_t *opt, uint8_t type, void *value, uint8_t len);
static dhcp_lease_t *find_lease_slot(dhcp_lease_t *leases, uint8_t *hwaddr);

void dhcpserver_start(const ip_addr_t *first_client_addr, uint8_t max_leases)
{
    /* Stop any existing running dhcpserver */
    if(dhcpserver_task_handle) {
        dhcpserver_stop();
    }
    
    state = malloc(sizeof(server_state_t));
    state->max_leases = max_leases;
    state->leases = calloc(max_leases, sizeof(dhcp_lease_t));
    
    // state->server_if is assigned once the task is running - see comment in dhcpserver_task()
    ip_addr_copy(state->first_client_addr, *first_client_addr);

    xTaskCreate(dhcpserver_task, (signed char *)"DHCPServer", 768, NULL, 8, &dhcpserver_task_handle);
}

void dhcpserver_stop(void)
{
    if(dhcpserver_task_handle) {
        vTaskDelete(dhcpserver_task_handle);
        free(state);
        dhcpserver_task_handle = NULL;
    }
}

static void dhcpserver_task(void *pxParameter)
{
    DTXT("dhcpserver_task(): start\n");
    
    // netif_list isn't assigned until after user_init completes, which is why we do it inside the task
    state->server_if = netif_list; // TODO: Make this configurable

    state->nc = netconn_new(NETCONN_UDP);
    
    if(!state->nc) {
        DTXT("dhcpserver_task(): failed to allocate socket\n");
        return;
    }

    netconn_bind(state->nc, IP_ADDR_ANY, DHCP_SERVER_PORT);

    while(1) {
        struct netbuf *netbuf;
        struct dhcp_msg received = { 0 };

        // Receive a DHCP packet
        err_t err = netconn_recv(state->nc, &netbuf);
        
        if(err != ERR_OK) {
            DTXT("dhcpserver_task(): failed to receive DHCP packet; err = %d\n", err);
            continue;
        }

        DTXT("dhcpserver_task(): got DHCP packet\n");
        
        // expire any leases that have passed
        uint32_t now = xTaskGetTickCount();
        
        for(int i = 0; i < state->max_leases; i++) {
            uint32_t expires = state->leases[i].expires;
            
            if(expires && expires < now) {
                state->leases[i].expires = 0;
                DTXT("dhcpserver_task(): lease expired; i = %d\n", i);
            }
        }

        ip_addr_t received_ip;
        u16_t     port;
        
        netconn_addr(state->nc, &received_ip, &port);

        if(netbuf_len(netbuf) < offsetof(struct dhcp_msg, options)) {
            DTXT("dhcpserver_task(): too short DHCP packet\n");
            // too short to be a valid DHCP client message
            netbuf_delete(netbuf);
            continue;
        }
        
        if(netbuf_len(netbuf) >= sizeof(struct dhcp_msg)) {
            DTXT("dhcpserver_task(): client sent more options than we know how to parse; len = %d\n", netbuf_len(netbuf));
        }

        netbuf_copy(netbuf, &received, sizeof(struct dhcp_msg));
        netbuf_delete(netbuf);

        uint8_t *message_type = find_dhcp_option(&received, DHCP_OPTION_MESSAGE_TYPE, DHCP_OPTION_MESSAGE_TYPE_LEN, NULL);
        
        if(!message_type) {
            DTXT("dhcpserver_task(): no message type field found\n");
            continue;
        }
        
        switch(*message_type) {
            case DHCP_DISCOVER:
                DTXT("dhcpserver_task(): DHCP_DISCOVER\n");
                handle_dhcp_discover(&received);
                break;
                
            case DHCP_REQUEST:
                DTXT("dhcpserver_task(): DHCP_REQUEST\n");
                handle_dhcp_request(&received);
                break;
                
            case DHCP_RELEASE:
                DTXT("dhcpserver_task(): DHCP_RELEASE\n");
                handle_dhcp_release(&received);
                break;
                
            default:
                DTXT("dhcpserver_task(): unsupported message type %d\n", *message_type);
                break;
        }
    }
}

static void handle_dhcp_discover(struct dhcp_msg *dhcpmsg)
{
    if(dhcpmsg->htype != DHCP_HTYPE_ETH) {
        return;
    }
    
    if(dhcpmsg->hlen > NETIF_MAX_HWADDR_LEN) {
        return;
    }
    
    dhcp_lease_t *freelease = find_lease_slot(state->leases, dhcpmsg->chaddr);
    
    if(!freelease) {
        DTXT("handle_dhcp_discover(): all leases taken\n");
        return; // Nothing available, so do nothing
    }

    // Reuse the DISCOVER buffer for the OFFER response
    dhcpmsg->op = DHCP_BOOTREPLY;
    bzero(dhcpmsg->options, DHCP_OPTIONS_LEN);

    ip_addr_copy(dhcpmsg->yiaddr, state->first_client_addr);
    ip4_addr4(&(dhcpmsg->yiaddr)) += (freelease - state->leases);

    uint8_t *opt = (uint8_t *)&dhcpmsg->options;
    opt = add_dhcp_option_byte( opt, DHCP_OPTION_MESSAGE_TYPE, DHCP_OFFER);
    opt = add_dhcp_option_bytes(opt, DHCP_OPTION_SERVER_ID,    &state->server_if->ip_addr, 4);
    opt = add_dhcp_option_bytes(opt, DHCP_OPTION_SUBNET_MASK,  &state->server_if->netmask, 4);
    opt = add_dhcp_option_bytes(opt, DHCP_OPTION_END, NULL,    0);

    struct netbuf *netbuf = netbuf_new();
    
    if(netbuf == NULL) {
        DTXT("handle_dhcp_discover(): netbuf == NULL\n");
    }
    else {
        netbuf_alloc(netbuf, sizeof(struct dhcp_msg));
        netbuf_take(netbuf, dhcpmsg, sizeof(struct dhcp_msg));
        netconn_sendto(state->nc, netbuf, IP_ADDR_BROADCAST, 68);
        netbuf_delete(netbuf);
    }
}

static void handle_dhcp_request(struct dhcp_msg *dhcpmsg)
{
    if(dhcpmsg->htype != DHCP_HTYPE_ETH) {
        return;
    }
    
    if(dhcpmsg->hlen > NETIF_MAX_HWADDR_LEN) {
        return;
    }
    
    ip_addr_t requested_ip;
    uint8_t *requested_ip_opt = find_dhcp_option(dhcpmsg, DHCP_OPTION_REQUESTED_IP, 4, NULL);
    
    if(requested_ip_opt) {
        memcpy(&requested_ip.addr, requested_ip_opt, 4);
    } 
    else if(ip_addr_cmp(&requested_ip, IP_ADDR_ANY)) {
        ip_addr_copy(requested_ip, dhcpmsg->ciaddr);
    } 
    else {
        DTXT("handle_dhcp_request(): no requested IP\n");
        send_dhcp_nak(dhcpmsg);
        return;
    }

    // Test the first 4 octets match
    if(ip4_addr1(&requested_ip) != ip4_addr1(&state->first_client_addr)
       || ip4_addr2(&requested_ip) != ip4_addr2(&state->first_client_addr)
       || ip4_addr3(&requested_ip) != ip4_addr3(&state->first_client_addr)) {
        DTXT("handle_dhcp_request(): 0x%08x not an allowed IP\n", (unsigned int)requested_ip.addr);
        send_dhcp_nak(dhcpmsg);
        return;
    }
    
    // Test the last octet is in the MAXCLIENTS range
    int16_t octet_offs = ip4_addr4(&requested_ip) - ip4_addr4(&state->first_client_addr);
    if(octet_offs < 0 || octet_offs >= state->max_leases) {
        DTXT("handle_dhcp_request(): address out of range\n");
        send_dhcp_nak(dhcpmsg);
        return;
    }

    dhcp_lease_t *requested_lease = state->leases + octet_offs;
    if(requested_lease->expires != 0 && memcmp(requested_lease->hwaddr, dhcpmsg->chaddr,dhcpmsg->hlen))
    {
        DTXT("handle_dhcp_request(): lease for address already taken\n");
        send_dhcp_nak(dhcpmsg);
        return;
    }

    memcpy(requested_lease->hwaddr, dhcpmsg->chaddr, dhcpmsg->hlen);
    
    DTXT("handle_dhcp_request(): lease addr %d.%d.%d.%d assigned to MAC %02x:%02x:%02x:%02x:%02x:%02x\n",
            ip4_addr1(&requested_ip.addr), ip4_addr2(&requested_ip.addr), ip4_addr3(&requested_ip.addr), ip4_addr4(&requested_ip.addr),
            //(unsigned int)requested_ip.addr, 
            requested_lease->hwaddr[0],
            requested_lease->hwaddr[1], 
            requested_lease->hwaddr[2], 
            requested_lease->hwaddr[3], 
            requested_lease->hwaddr[4],
            requested_lease->hwaddr[5]);
    
    requested_lease->expires = DHCPSERVER_LEASE_TIME * configTICK_RATE_HZ;

    DTXT("handle_dhcp_request(): expires = %d\n", requested_lease->expires);
    
    // Reuse the REQUEST message as the ACK message
    dhcpmsg->op = DHCP_BOOTREPLY;
    bzero(dhcpmsg->options, DHCP_OPTIONS_LEN);

    ip_addr_copy(dhcpmsg->yiaddr, requested_ip);

    uint32_t expiry = htonl(DHCPSERVER_LEASE_TIME);

    uint8_t *opt = (uint8_t *)&dhcpmsg->options;
    
    opt = add_dhcp_option_byte( opt, DHCP_OPTION_MESSAGE_TYPE, DHCP_ACK);
    opt = add_dhcp_option_bytes(opt, DHCP_OPTION_LEASE_TIME,   &expiry, 4);
    opt = add_dhcp_option_bytes(opt, DHCP_OPTION_SERVER_ID,    &state->server_if->ip_addr, 4);
    opt = add_dhcp_option_bytes(opt, DHCP_OPTION_SUBNET_MASK,  &state->server_if->netmask, 4);
    
    opt = add_dhcp_option_bytes(opt, DHCP_OPTION_ROUTER,       &state->server_if->ip_addr, 4);
    opt = add_dhcp_option_bytes(opt, DHCP_OPTION_DNS_SERVER,   &state->server_if->ip_addr, 4);
    
    
    opt = add_dhcp_option_bytes(opt, DHCP_OPTION_END,          NULL, 0);

    
//#define DHCP_OPTION_ROUTER 3
//#define DHCP_OPTION_DNS_SERVER 6 
    
    struct netbuf *netbuf = netbuf_new();
    
    if(netbuf == NULL) {
        DTXT("handle_dhcp_request(): netbuf == NULL\n");
    }
    else {
        netbuf_alloc(netbuf, sizeof(struct dhcp_msg));
        netbuf_take(netbuf, dhcpmsg, sizeof(struct dhcp_msg));
        netconn_sendto(state->nc, netbuf, IP_ADDR_BROADCAST, 68);
        netbuf_delete(netbuf);
    }
}

static void handle_dhcp_release(struct dhcp_msg *dhcpmsg)
{
    dhcp_lease_t *lease = find_lease_slot(state->leases, dhcpmsg->chaddr);
    
    if(lease) {
        DTXT("handle_dhcp_release(): lease expired\n");
        lease->expires = 0;
    }
}

static void send_dhcp_nak(struct dhcp_msg *dhcpmsg)
{
    DTXT("send_dhcp_nak(): begin\n");
    
    // Reuse 'dhcpmsg' for the NAK
    dhcpmsg->op = DHCP_BOOTREPLY;
    bzero(dhcpmsg->options, DHCP_OPTIONS_LEN);

    uint8_t *opt = (uint8_t *)&dhcpmsg->options;
    opt = add_dhcp_option_byte(opt, DHCP_OPTION_MESSAGE_TYPE, DHCP_NAK);
    opt = add_dhcp_option_bytes(opt, DHCP_OPTION_SERVER_ID, &state->server_if->ip_addr, 4);
    opt = add_dhcp_option_bytes(opt, DHCP_OPTION_END, NULL, 0);

    struct netbuf *netbuf = netbuf_new();
    netbuf_alloc(netbuf, sizeof(struct dhcp_msg));
    netbuf_take(netbuf, dhcpmsg, sizeof(struct dhcp_msg));
    netconn_sendto(state->nc, netbuf, IP_ADDR_BROADCAST, 68);
    netbuf_delete(netbuf);
    
    DTXT("send_dhcp_nak(): end\n");
}

static uint8_t *find_dhcp_option(struct dhcp_msg *msg, uint8_t option_num, uint8_t min_length, uint8_t *length)
{
    uint8_t *start = (uint8_t *)&msg->options;
    uint8_t *msg_end = (uint8_t *)msg + sizeof(struct dhcp_msg);

    for(uint8_t *p = start; p < msg_end-2;) {
        uint8_t type = *p++;
        uint8_t len = *p++;
        if(type == DHCP_OPTION_END)
            return NULL;
        if(p+len >= msg_end)
            break; // We've overrun our valid DHCP message size, or this isn't a valid option
        if(type == option_num) {
            if(len < min_length)
                break;
            if(length)
                *length = len;
            return p; // start of actual option data
        }
        p += len;
    }
    return NULL; // Not found
}

static uint8_t *add_dhcp_option_byte(uint8_t *opt, uint8_t type, uint8_t value)
{
    *opt++ = type;
    *opt++ = 1;
    *opt++ = value;
    return opt;
}

static uint8_t *add_dhcp_option_bytes(uint8_t *opt, uint8_t type, void *value, uint8_t len)
{
    *opt++ = type;
    if(len) {
        *opt++ = len;
        memcpy(opt, value, len);
    }
    return opt+len;
}

/* Find a free DHCP lease, or a lease already assigned to 'hwaddr' */
static dhcp_lease_t *find_lease_slot(dhcp_lease_t *leases, uint8_t *hwaddr)
{
    dhcp_lease_t *empty_lease = NULL;
    for(int i = 0; i < state->max_leases; i++) {
        if(leases->expires == 0 && !empty_lease)
            empty_lease = &leases[i];
        else if (memcmp(hwaddr, leases[i].hwaddr, 6) == 0)
            return &leases[i];

    }
    return empty_lease;
}
