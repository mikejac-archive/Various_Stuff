/* 
 * MQTT ESP8266 Slave Firmware
 * Copyright (C) 2015  Michael Jacobsen
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * https://github.com/mikejac/
 * 
 */

#include <com/tholusi/esp8266/wifi/wifi_t.hpp>
#include "com/tholusi/esp8266/mqtt_client/mqtt_client_t.hpp"
#include <com/espressif/esp8266/include/osapi.h>
#include <com/espressif/esp8266/include/os_type.h>
#include <com/espressif/esp8266/uart/uart.h>
#include <com/tholusi/esp8266/cplusplus/cplusplus.hpp>
#include <com/tholusi/esp8266/debug/debug_t.hpp>

//#define WITH_TESTCODE   1

#undef  DTXT
#define DTXT(...)   os_printf(__VA_ARGS__)

#define VERSION_MAJOR   1
#define VERSION_MINOR   0

extern "C" void user_init(void);

/******************************************************************************************************************
 * dispatcher_t
 *
 */
class dispatcher_t: public com::tholusi::esp8266::task1_t
{
public:
    /**
     * 
     * @param wifi
     * @return 
     */
    int ICACHE_FLASH_ATTR setup(com::tholusi::esp8266::wifi_t* wifi);
    
private:
    /**
     * 
     * @param params
     * @param req_id
     * @return 
     */
    int ICACHE_FLASH_ATTR wifiConnect(JVal* params, int32_t req_id);
    /**
     * 
     * @param params
     * @param req_id
     * @return 
     */
    int ICACHE_FLASH_ATTR wifiDisconnect(JVal* params, int32_t req_id);
    /**
     * 
     * @param params
     * @param req_id
     * @return 
     */
    int ICACHE_FLASH_ATTR wifiScan(JVal* params, int32_t req_id);
    /**
     * 
     * @param params
     * @param req_id
     * @return 
     */
    int ICACHE_FLASH_ATTR mqttConnect(JVal* params, int32_t req_id);
    /**
     * 
     * @param params
     * @param req_id
     * @return 
     */
    int ICACHE_FLASH_ATTR mqttDisconnect(JVal* params, int32_t req_id);
    /**
     * 
     * @param params
     * @param req_id
     * @return 
     */
    int ICACHE_FLASH_ATTR mqttPublish(JVal* params, int32_t req_id);
    /**
     * 
     * @param params
     * @param req_id
     * @return 
     */
    int ICACHE_FLASH_ATTR mqttSubscribe(JVal* params, int32_t req_id);
    /**
     * 
     * @param params
     * @param req_id
     * @return 
     */
    int ICACHE_FLASH_ATTR mqttUnsubscribe(JVal* params, int32_t req_id);
    /**
     * 
     * @param params
     * @param req_id
     * @return 
     */
    int ICACHE_FLASH_ATTR sysNop(JVal* params, int32_t req_id);
    /**
     * 
     * @param params
     * @param req_id
     * @return 
     */
    int ICACHE_FLASH_ATTR sysRestart(JVal* params, int32_t req_id);
    /**
     * 
     * @param e
     */
    void ICACHE_FLASH_ATTR runTask(os_event_t* e);
    
    com::tholusi::esp8266::mqtt_client_t    m_MqttClient;
    com::tholusi::esp8266::wifi_t*          m_Wifi;
    JErr*                                   m_Err;
    
    bool                                    m_FirstRun;
    uint32_t                                m_Seconds;
};

/**
 * 
 * @return 
 */
int ICACHE_FLASH_ATTR dispatcher_t::setup(com::tholusi::esp8266::wifi_t* wifi)
{
    setRunInterval(1000);
    
    m_FirstRun = true;
    m_Wifi     = wifi;
    m_Err      = new JErr;

    if(m_Err == 0) {
        os_printf(com::tholusi::esp8266::JSONRPC_ERR5, "JErr");
        return -1;
    }
    
    int ret = m_MqttClient.setup(os(), wifi);
    
    if(ret != EOK) {
        os_printf(com::tholusi::esp8266::JSONRPC_ERR5, "m_MqttClient.setup()");
    }
    
    return ret;
}
/**
 * 
 * @param e
 */
void ICACHE_FLASH_ATTR dispatcher_t::runTask(os_event_t* e)
{
    if(m_FirstRun == true) {
        m_FirstRun = false;

        os_printf("{\"jsonrpc\":\"2.0\",\"method\":\"system\",\"params\":{\"code\":1,\"sys\":\"system\",\"msg\":\"start\",\"p1\":null,\"p2\":null,\"n1\":0,\"n2\":0}}\n");
    }
    
    m_MqttClient.run(m_Seconds);
    
    int c = uart0_rx_one_char();

    //
    // TODO: handle too-long interval between indivdual characters - reset parser if interval too long
    //

    if(c != -1) {
        int ret = os()->jsonRpc()->parse(c);

        if(ret == EJSON_METHOD) {
            DTXT("%lu dispatcher_t::runTask(): parse() == EJSON_METHOD\n", millis());
            
            if(os_strcmp("mqtt_connect", os()->jsonRpc()->getMethod()) == 0) {
                DTXT("%lu dispatcher_t::runTask(): parse; mqtt_connect\n", millis());                
                mqttConnect(os()->jsonRpc()->getParams(), os()->jsonRpc()->getMsgId());
            }
            else if(os_strcmp("mqtt_disconnect", os()->jsonRpc()->getMethod()) == 0) {
                DTXT("%lu dispatcher_t::runTask(): parse; mqtt_disconnect\n", millis());                
                mqttDisconnect(os()->jsonRpc()->getParams(), os()->jsonRpc()->getMsgId());                
            }
            else if(os_strcmp("mqtt_publish", os()->jsonRpc()->getMethod()) == 0) {
                DTXT("%lu dispatcher_t::runTask(): parse; mqtt_publish\n", millis());                
                mqttPublish(os()->jsonRpc()->getParams(), os()->jsonRpc()->getMsgId());                
            }
            else if(os_strcmp("mqtt_subscribe", os()->jsonRpc()->getMethod()) == 0) {
                DTXT("%lu dispatcher_t::runTask(): parse; mqtt_subscribe\n", millis());                
                mqttSubscribe(os()->jsonRpc()->getParams(), os()->jsonRpc()->getMsgId());                
            }
            else if(os_strcmp("mqtt_unsubscribe", os()->jsonRpc()->getMethod()) == 0) {
                DTXT("%lu dispatcher_t::runTask(): parse; mqtt_unsubscribe\n", millis());                
                mqttUnsubscribe(os()->jsonRpc()->getParams(), os()->jsonRpc()->getMsgId());                
            }
            else if(os_strcmp("wifi_connect", os()->jsonRpc()->getMethod()) == 0) {
                DTXT("%lu dispatcher_t::runTask(): parse; wifi_connect\n", millis());                
                wifiConnect(os()->jsonRpc()->getParams(), os()->jsonRpc()->getMsgId());
            }
            else if(os_strcmp("wifi_disconnect", os()->jsonRpc()->getMethod()) == 0) {
                DTXT("%lu dispatcher_t::runTask(): parse; wifi_disconnect\n", millis());                
                wifiDisconnect(os()->jsonRpc()->getParams(), os()->jsonRpc()->getMsgId());
            }
            else if(os_strcmp("wifi_scan", os()->jsonRpc()->getMethod()) == 0) {
                DTXT("%lu dispatcher_t::runTask(): parse; wifi_scan\n", millis());                
                wifiScan(os()->jsonRpc()->getParams(), os()->jsonRpc()->getMsgId());
            }
            else if(os_strcmp("sys_nop", os()->jsonRpc()->getMethod()) == 0) {
                DTXT("%lu dispatcher_t::runTask(): parse; sys_nop\n", millis());                
                sysNop(os()->jsonRpc()->getParams(), os()->jsonRpc()->getMsgId());
            }
            else if(os_strcmp("sys_restart", os()->jsonRpc()->getMethod()) == 0) {
                DTXT("%lu dispatcher_t::runTask(): parse; sys_restart\n", millis());                
                sysRestart(os()->jsonRpc()->getParams(), os()->jsonRpc()->getMsgId());
            }
            else {
                DTXT("%lu dispatcher_t::runTask(): parse; unknown\n", millis());                
            }
            
            os()->jsonRpc()->parseReset();
        }
        else if(ret == EJSON_ERROR) {
            DTXT("%lu dispatcher_t::runTask(): parse; ret == EJSON_ERROR\n", millis());
        }
        else if(ret == EJSON_RESULT) {
            DTXT("%lu dispatcher_t::runTask(): parse; ret == EJSON_RESULT\n", millis());
        }
        else if(ret != EJSON_PARSE_INCOMPLETE && ret != EJSON_PARSE_ERR) {
            DTXT("%lu dispatcher_t::runTask(): parse; ret != EJSON_PARSE_INCOMPLETE && ret != EJSON_PARSE_ERR (%d)\n", millis(), ret);
        }
    }

#if defined(WITH_TESTCODE)
    int qos = 0;

    if(millis() - m_LastRun >= getRunInterval()) {
        DTXT("%lu dispatcher_t::runTask(): time to publish; freeHeap = %lu\n", millis(), os()->freeHeap());

        static char txt[60];
        
        os_sprintf(txt, "{\"seconds\":%lu,\"contact\":\"mqtt@visbyjacobsen.dk\"}", m_Seconds);
        
        for(int i = 0; i < m_MqttClient.getMaxConnections() - 1; ++i) {            
            DTXT("%lu dispatcher_t::runTask(): m_Command = %d, m_Status = %d\n", millis(), m_MqttClient.getCommand(i), m_MqttClient.getStatus(i));

            if(m_MqttClient.isReady(i) == 1) {
                DTXT("%lu dispatcher_t::runTask(): slot %d is connected\n", millis(), i);

                if(m_MqttClient.publish(i, "fabric/esp8266/seconds", txt, os_strlen(txt), qos, MQTT_RETAIN) != 0) {
                    DTXT("%lu dispatcher_t::runTask(): could not publish\n", millis());                
                }
            }
        }
        
        m_LastRun = millis();
        ++m_Seconds;
    }

    static int x0 = 0;
    static int x1 = 0;
    static int x2 = 0;
    static int x3 = 0;

    if(x0 == 0 && m_MqttClient.isReady(0) == 1) {
        if(m_MqttClient.publish(0, "fabric/esp8266/status", "online", 6, qos, MQTT_RETAIN) != 0) {
            DTXT("%lu dispatcher_t::runTask(): could not publish\n", millis());                
        }
        else {
            ++x0;
        }
    }
    
    if(x1 == 0 && m_MqttClient.isReady(1) == 1) {
        if(m_MqttClient.publish(1, "fabric/esp8266/status", "online", 6, qos, MQTT_RETAIN) != 0) {
            DTXT("%lu dispatcher_t::runTask(): could not publish\n", millis());                
        }
        else {
            ++x1;
        }
    }

    if(x2 == 0 && m_MqttClient.isReady(2) == 1) {
        if(m_MqttClient.publish(2, "fabric/esp8266/status", "online", 6, qos, MQTT_RETAIN) != 0) {
            DTXT("%lu dispatcher_t::runTask(): could not publish\n", millis());                
        }
        else {
            ++x2;
        }
    }
    
    if(x3 == 0 && m_MqttClient.isReady(3) == 1) {
        if(m_MqttClient.publish(3, "fabric/esp8266/status", "online", 6, qos, MQTT_RETAIN) != 0) {
            DTXT("%lu dispatcher_t::runTask(): could not publish\n", millis());                
        }
        else {
            ++x3;
        }
    }
#else
    if(millis() - m_LastRun >= getRunInterval()) {
        m_LastRun = millis();
        ++m_Seconds;
    }    
#endif
}

#define _WIFI_SSID      0x0001
#define _WIFI_PSW       0x0002

/**
 * 
 * @param params
 * @param req_id
 * @return 
 */
int ICACHE_FLASH_ATTR dispatcher_t::wifiConnect(JVal* params, int32_t req_id)
{
    // {"jsonrpc": "2.0", "method": "wifi_connect", "params": {"ssid": "MySSID", "password": "MyPaswd"}, "id": 100}
    
    uint16_t    flag = 0;
    
    /******************************************************************************************************************
     * extract data from JSON-RPC request
     *
     */
    const char* ssid      = 0;
    const char* password  = 0;
    
    m_Err->reset();
    JVal* jv = params;
    
    // extract all data
    while(jv != 0) {
        const char* name = jv->getName();
        
        if(os_strcmp("ssid", name) == 0) {
            DTXT("%lu dispatcher_t::wifiConnect(): ssid\n", millis());
            ssid  = jv->getString(m_Err);
            flag |= _WIFI_SSID;
        }
        else if(os_strcmp("password", name) == 0) {
            DTXT("%lu dispatcher_t::wifiConnect(): password\n", millis());
            password = jv->getString(m_Err);                
            flag    |= _WIFI_PSW;
        }
        else {
            DTXT("%lu dispatcher_t::mqttConnect(): unknown = %s\n", millis(), name);            
        }
        
        // check for errors
        if(m_Err->isError()) {        
            os()->jsonRpc()->errorReply(EJSON_DECODER, req_id, "JSON decoder error; %s", m_Err->msg);
            return EJSON_DECODER;
        }            
        
        jv = jv->getNextElem();
    }
    
    /******************************************************************************************************************
     * start connection
     *
     */
    int ret = EOK;
    
    if((flag & _WIFI_SSID) == _WIFI_SSID) {
        m_Wifi->connect(ssid, password);

        os()->jsonRpc()->successReply(req_id, "0");
    }
    else {
        ret = EWIFI_SSID;
        os()->jsonRpc()->errorReply(ret, req_id, "no ssid specified");
    }
    
    return ret;
}
/**
 * 
 * @param params
 * @param req_id
 * @return 
 */
int ICACHE_FLASH_ATTR dispatcher_t::wifiDisconnect(JVal* params, int32_t req_id)
{
    // {"jsonrpc": "2.0", "method": "wifi_disconnect", "params": {}, "id": 110}
    
    /******************************************************************************************************************
     * start disconnect
     *
     */
    int ret = EOK;
    
    m_Wifi->disconnect();

    os()->jsonRpc()->successReply(req_id, "0");
    
    return ret;
}
/**
 * 
 * @param params
 * @param req_id
 * @return 
 */
int ICACHE_FLASH_ATTR dispatcher_t::wifiScan(JVal* params, int32_t req_id)
{
    // {"jsonrpc": "2.0", "method": "wifi_scan", "params": {}, "id": 110}
    
    /******************************************************************************************************************
     * start scan
     *
     */
    int ret = EOK;
    
    m_Wifi->scan();

    os()->jsonRpc()->successReply(req_id, "0");
    
    return ret;
}

#define _CONN_BROKER        0x0001
#define _CONN_PORT          0x0002
#define _CONN_SECURE        0x0004
#define _CONN_CLIENT_ID     0x0008
#define _CONN_USERNAME      0x0010
#define _CONN_PASSWORD      0x0020
#define _CONN_KEEP_ALIVE    0x0040
#define _CONN_LWT_TOPIC     0x0080
#define _CONN_LWT_PAYLOAD   0x0100
#define _CONN_LWT_RETAIN    0x0200
/**
 * 
 * @param params
 * @param req_id
 * @return 
 */
int ICACHE_FLASH_ATTR dispatcher_t::mqttConnect(JVal* params, int32_t req_id)
{
    // {"jsonrpc": "2.0", "method": "mqtt_connect", "params": {"broker": "192.168.1.82", "port": 1883, "secure": false, "client_id": "esp8266_client", "username": "", "password": "", "keep_alive": 30}, "id": 1}
    // {"jsonrpc": "2.0", "method": "mqtt_connect", "params": {"broker": "test.mosquitto.org", "port": 1883, "secure": false, "client_id": "esp8266_client", "username": "", "password": "", "keep_alive": 60}, "id": 1}
    // {"jsonrpc": "2.0", "method": "mqtt_connect", "params": {"broker": "iot.eclipse.org", "port": 1883, "secure": false, "client_id": "esp8266_client", "username": "", "password": "", "keep_alive": 60}, "id": 1}
    
    
    if(m_Wifi->isConnected() == false) {
        os()->jsonRpc()->errorReply(EWIFI_NOT_CONNECTED, req_id, "wifi not connected");
        return EWIFI_NOT_CONNECTED;
    }
    
    uint16_t    flag = 0;
    
    /******************************************************************************************************************
     * extract data from JSON-RPC request
     *
     */
    const char* broker        = 0;
    int         port          = 1883;
    BaBool      secure        = false;
    const char* client_id     = 0;
    const char* username      = 0;
    const char* password      = 0;
    int         keep_alive    = 60;
    const char* lwt_topic     = 0;
    const char* lwt_payload   = 0;
    int         lwt_qos       = 0;
    BaBool      lwt_retain    = false;
    BaBool      clean_session = false;
    
    m_Err->reset();
    JVal* jv = params;
    
    // extract all data
    while(jv != 0) {
        const char* name = jv->getName();
        
        if(os_strcmp("broker", name) == 0) {
            broker = jv->getString(m_Err);
            flag  |= _CONN_BROKER;
            DTXT("%lu dispatcher_t::mqttConnect(): broker = %s\n", millis(), broker);
        }
        else if(os_strcmp("port", name) == 0) {
            port  = jv->getInt(m_Err);
            flag |= _CONN_PORT;
            DTXT("%lu dispatcher_t::mqttConnect(): port = %d\n", millis(), port);
        }
        else if(os_strcmp("secure", name) == 0) {
            secure = jv->getBoolean(m_Err);
            flag  |= _CONN_SECURE;
            DTXT("%lu dispatcher_t::mqttConnect(): secure = %s\n", millis(), secure ? "True" : "False");
        }
        else if(os_strcmp("client_id", name) == 0) {
            client_id = jv->getString(m_Err);                
            flag     |= _CONN_CLIENT_ID;
            DTXT("%lu dispatcher_t::mqttConnect(): client_id = %s\n", millis(), client_id);
        }
        else if(os_strcmp("username", name) == 0) {
            username = jv->getString(m_Err);                
            flag    |= _CONN_USERNAME;
            DTXT("%lu dispatcher_t::mqttConnect(): username = %s\n", millis(), username);
        }
        else if(os_strcmp("password", name) == 0) {
            password = jv->getString(m_Err);                
            flag    |= _CONN_PASSWORD;
            DTXT("%lu dispatcher_t::mqttConnect(): password = %s\n", millis(), password);
        }
        else if(os_strcmp("keep_alive", name) == 0) {
            keep_alive = jv->getInt(m_Err);            
            flag      |= _CONN_KEEP_ALIVE;
            DTXT("%lu dispatcher_t::mqttConnect(): keep_alive = %d\n", millis(), keep_alive);
        }
        else if(os_strcmp("lwt_topic", name) == 0) {
            lwt_topic = jv->getString(m_Err);
            DTXT("%lu dispatcher_t::mqttConnect(): lwt_topic = %s\n", millis(), lwt_topic);
        }
        else if(os_strcmp("lwt_payload", name) == 0) {
            lwt_payload = jv->getString(m_Err);            
            DTXT("%lu dispatcher_t::mqttConnect(): lwt_payload = %s\n", millis(), lwt_payload);
        }
        else if(os_strcmp("lwt_qos", name) == 0) {
            lwt_qos  = jv->getInt(m_Err);
            DTXT("%lu dispatcher_t::mqttConnect(): lwt_qos = %d\n", millis(), lwt_qos);
        }
        else if(os_strcmp("lwt_retain", name) == 0) {
            lwt_retain = jv->getBoolean(m_Err);            
            DTXT("%lu dispatcher_t::mqttConnect(): lwt_retain = %s\n", millis(), lwt_retain ? "True" : "False");
        }
        else if(os_strcmp("clean_session", name) == 0) {
            clean_session = jv->getBoolean(m_Err);            
            DTXT("%lu dispatcher_t::mqttConnect(): clean_session = %s\n", millis(), clean_session ? "True" : "False");
        }
        else {
            DTXT("%lu dispatcher_t::mqttConnect(): unknown = %s\n", millis(), name);            
        }
        
        // check for errors
        if(m_Err->isError()) {        
            os()->jsonRpc()->errorReply(EJSON_DECODER, req_id, "JSON decoder error; %s", m_Err->msg);
            return EJSON_DECODER;
        }            
        
        jv = jv->getNextElem();
    }

    //DTXT("%lu dispatcher_t::mqttConnect(): *1*\n", millis());
    //os_delay_us(2000000);
    
    /******************************************************************************************************************
     * start connection
     *
     */
    int ret = EOK;
    
    if(((flag & _CONN_BROKER) == _CONN_BROKER) && ((flag & _CONN_CLIENT_ID) == _CONN_CLIENT_ID)) {
        int slot = m_MqttClient.getFreeSlot();

        if(slot != EAVAILABLE_SLOT) {
            DTXT("%lu dispatcher_t::mqttConnect(): connecting\n", millis());
            
            if(m_MqttClient.connect(slot, broker, port, client_id, username, password, keep_alive, lwt_topic, lwt_payload, lwt_qos, lwt_retain, clean_session, secure) == 0) {
                os()->jsonRpc()->successReply(req_id, "%d", slot);
            }
            else {
                ret = ECONNECTION_INFO;
                os()->jsonRpc()->errorReply(ret, req_id, "failed to set connection info");
            }
        }
        else {
            ret = EAVAILABLE_SLOT;
            os()->jsonRpc()->errorReply(ret, req_id, "no available connection slot");
        }        
    }
    else {
        ret = EBROKER_CLIENT;
        os()->jsonRpc()->errorReply(ret, req_id, "no broker and/or client_id specified");
    }
    
    return ret;
}
/**
 * 
 * @param params
 * @param req_id
 * @return 
 */
int ICACHE_FLASH_ATTR dispatcher_t::mqttDisconnect(JVal* params, int32_t req_id)
{
    // {"jsonrpc": "2.0", "method": "mqtt_disconnect", "params": {"id": 0}, "id": 2}

    if(m_Wifi->isConnected() == false) {
        os()->jsonRpc()->errorReply(EWIFI_NOT_CONNECTED, req_id, "wifi not connected");
        return EWIFI_NOT_CONNECTED;
    }
    
    int slot = -1;

    /******************************************************************************************************************
     * extract data from JSON-RPC request
     *
     */
    m_Err->reset();
    JVal* jv = params;
    
    const char* name = jv->getName();
        
    if(os_strcmp("id", name) == 0) {
        DTXT("%lu dispatcher_t::mqttDisconnect(): id\n", millis());
        slot  = jv->getInt(m_Err);
    }
    
    int ret = EOK;
    
    // check for errors
    if(m_Err->isError()) {        
        ret = EJSON_DECODER;
        os()->jsonRpc()->errorReply(ret, req_id, "JSON decoder error; %s", m_Err->msg);
        return ret;
    }            

    /******************************************************************************************************************
     * start disconnect
     *
     */
    if(slot >= 0 && slot < m_MqttClient.getMaxConnections()) {
        ret = m_MqttClient.disconnect(slot);

        if(ret == EOK) {
            os()->jsonRpc()->successReply(req_id, "%d", slot);
        }
        else {
            os()->jsonRpc()->errorReply(ret, req_id, "disconnect error");
        }
    }
    else {
        ret = EID;
        os()->jsonRpc()->errorReply(ret, req_id, "invalid id");
    }
    
    return ret;
}

#define _PUB_SLOT       0x0001
#define _PUB_TOPIC      0x0002
#define _PUB_PAYLOAD    0x0004
#define _PUB_QOS        0x0008
#define _PUB_RETAIN     0x0010
/**
 * 
 * @param params
 * @param req_id
 * @return 
 */
int ICACHE_FLASH_ATTR dispatcher_t::mqttPublish(JVal* params, int32_t req_id)
{
    // {"jsonrpc": "2.0", "method": "mqtt_publish", "params": {"id": 0, "topic": "fabric/esp8266/test1", "payload": "data 1234", "qos": 0, "retain": false}, "id": 3}

    if(m_Wifi->isConnected() == false) {
        os()->jsonRpc()->errorReply(EWIFI_NOT_CONNECTED, req_id, "wifi not connected");
        return EWIFI_NOT_CONNECTED;
    }
    
    uint16_t    flag = 0;
    
    /******************************************************************************************************************
     * extract data from JSON-RPC request
     *
     */
    int         slot        = -1;
    const char* topic      = 0;
    const char* payload    = 0;
    int         qos        = 0;
    BaBool      retain     = false;
    
    m_Err->reset();
    JVal* jv = params;
    
    // extract all data
    while(jv != 0) {
        const char* name = jv->getName();
        
        if(os_strcmp("id", name) == 0) {
            slot  = jv->getInt(m_Err);
            flag |= _PUB_SLOT;
            DTXT("%lu dispatcher_t::mqttPublish(): slot = %d\n", millis(), slot);
        }
        else if(os_strcmp("topic", name) == 0) {
            topic = jv->getString(m_Err);
            flag |= _PUB_TOPIC;
            DTXT("%lu dispatcher_t::mqttPublish(): topic = %s\n", millis(), topic);
        }
        else if(os_strcmp("payload", name) == 0) {
            payload = jv->getString(m_Err);
            flag   |= _PUB_PAYLOAD;
            DTXT("%lu dispatcher_t::mqttPublisht(): payload = %s\n", millis(), payload);
        }
        else if(os_strcmp("qos", name) == 0) {
            qos  = jv->getInt(m_Err);
            flag |= _PUB_QOS;
            DTXT("%lu dispatcher_t::mqttPublish(): qos = %d\n", millis(), qos);
        }
        else if(os_strcmp("retain", name) == 0) {
            retain = jv->getBoolean(m_Err);
            flag  |= _PUB_RETAIN;
            DTXT("%lu dispatcher_t::mqttPublish(): retain = %s\n", millis(), retain ? "True" : "False");
        }
        else {
            DTXT("%lu dispatcher_t::mqttPublish(): unknown = %s\n", millis(), name);            
        }
        
        // check for errors
        if(m_Err->isError()) {        
            os()->jsonRpc()->errorReply(EJSON_DECODER, req_id, "JSON decoder error; %s", m_Err->msg);
            return EJSON_DECODER;
        }            
        
        jv = jv->getNextElem();
    }
    
    /******************************************************************************************************************
     * start publish
     *
     */
    int ret = EOK;
    
    if(((flag & _PUB_SLOT) == _PUB_SLOT) && ((flag & _PUB_TOPIC) == _PUB_TOPIC) && ((flag & _PUB_PAYLOAD) == _PUB_PAYLOAD)) {
        if(slot >= 0 && slot < m_MqttClient.getMaxConnections()) {
            ret = m_MqttClient.publish(slot, topic, payload, os_strlen(payload), qos, retain);

            if(ret == EOK) {
                os()->jsonRpc()->successReply(req_id, "%d", slot);
            }
            else {
                os()->jsonRpc()->errorReply(ret, req_id, "publish error");
            }
        }
        else {
            ret = EID;
            os()->jsonRpc()->errorReply(ret, req_id, "invalid id");
        }
    }
    else {
        ret = EMISSING_PARAMETER;
        os()->jsonRpc()->errorReply(ret, req_id, "no id and/or topic and/or payload specified");
    }
    
    return ret;
}

#define _SUB_SLOT       0x0001
#define _SUB_TOPIC      0x0002
#define _SUB_QOS        0x0004

/**
 * 
 * @param params
 * @param req_id
 * @return 
 */
int ICACHE_FLASH_ATTR dispatcher_t::mqttSubscribe(JVal* params, int32_t req_id)
{
    // {"jsonrpc": "2.0", "method": "mqtt_subscribe", "params": {"id": 0, "topic": "fabric/esp8266/ingress/#", "qos": 0}, "id": 4}

    if(m_Wifi->isConnected() == false) {
        os()->jsonRpc()->errorReply(EWIFI_NOT_CONNECTED, req_id, "wifi not connected");
        return EWIFI_NOT_CONNECTED;
    }
    
    uint16_t    flag = 0;
    
    /******************************************************************************************************************
     * extract data from JSON-RPC request
     *
     */
    int         slot        = -1;
    const char* topic      = 0;
    int         qos        = 0;
    
    m_Err->reset();
    JVal* jv = params;
    
    // extract all data
    while(jv != 0) {
        const char* name = jv->getName();
        
        if(os_strcmp("id", name) == 0) {
            DTXT("%lu dispatcher_t::mqttSubscribe(): slot\n", millis());
            slot  = jv->getInt(m_Err);
            flag |= _SUB_SLOT;
        }
        else if(os_strcmp("topic", name) == 0) {
            DTXT("%lu dispatcher_t::mqttSubscribe(): topic\n", millis());
            topic = jv->getString(m_Err);
            flag |= _SUB_TOPIC;
        }
        else if(os_strcmp("qos", name) == 0) {
            DTXT("%lu dispatcher_t::mqttSubscribe(): qos\n", millis());
            qos  = jv->getInt(m_Err);
            flag |= _SUB_QOS;
        }
        else {
            DTXT("%lu dispatcher_t::mqttSubscribe(): unknown = %s\n", millis(), name);            
        }
        
        // check for errors
        if(m_Err->isError()) {        
            os()->jsonRpc()->errorReply(EJSON_DECODER, req_id, "JSON decoder error; %s", m_Err->msg);
            return EJSON_DECODER;
        }            
        
        jv = jv->getNextElem();
    }
    
    /******************************************************************************************************************
     * start subscribe
     *
     */
    int ret = EOK;
    
    if(((flag & _SUB_SLOT) == _SUB_SLOT) && ((flag & _SUB_TOPIC) == _SUB_TOPIC)) {
        if(slot >= 0 && slot < m_MqttClient.getMaxConnections()) {
            ret = m_MqttClient.subscribe(slot, topic, qos);

            if(ret == EOK) {
                os()->jsonRpc()->successReply(req_id, "%d", slot);
            }
            else {
                os()->jsonRpc()->errorReply(ret, req_id, "subscribe error");
            }
        }
        else {
            ret = EID;
            os()->jsonRpc()->errorReply(ret, req_id, "invalid id");
        }
    }
    else {
        ret = EMISSING_PARAMETER;
        os()->jsonRpc()->errorReply(ret, req_id, "no id and/or topic specified");
    }
    
    return ret;
}

#define _USUB_SLOT      0x0001
#define _USUB_TOPIC     0x0002

/**
 * 
 * @param params
 * @param req_id
 * @return 
 */
int ICACHE_FLASH_ATTR dispatcher_t::mqttUnsubscribe(JVal* params, int32_t req_id)
{
    // {"jsonrpc": "2.0", "method": "mqtt_unsubscribe", "params": {"id": 0, "topic": "fabric/esp8266/ingress/#"}, "id": 5}

    if(m_Wifi->isConnected() == false) {
        os()->jsonRpc()->errorReply(EWIFI_NOT_CONNECTED, req_id, "wifi not connected");
        return EWIFI_NOT_CONNECTED;
    }
    
    uint16_t    flag = 0;
    
    /******************************************************************************************************************
     * extract data from JSON-RPC request
     *
     */
    int         slot       = -1;
    const char* topic      = 0;
    
    m_Err->reset();
    JVal* jv = params;
    
    // extract all data
    while(jv != 0) {
        const char* name = jv->getName();
        
        if(os_strcmp("id", name) == 0) {
            DTXT("%lu dispatcher_t::mqttSubscribe(): slot\n", millis());
            slot  = jv->getInt(m_Err);
            flag |= _USUB_SLOT;
        }
        else if(os_strcmp("topic", name) == 0) {
            DTXT("%lu dispatcher_t::mqttSubscribe(): topic\n", millis());
            topic = jv->getString(m_Err);
            flag |= _USUB_TOPIC;
        }
        else {
            DTXT("%lu dispatcher_t::mqttSubscribe(): unknown = %s\n", millis(), name);            
        }
        
        // check for errors
        if(m_Err->isError()) {        
            os()->jsonRpc()->errorReply(EJSON_DECODER, req_id, "JSON decoder error; %s", m_Err->msg);
            return EJSON_DECODER;
        }            
        
        jv = jv->getNextElem();
    }
    
    /******************************************************************************************************************
     * start unsubscribe
     *
     */
    int ret = EOK;
    
    if(((flag & _USUB_SLOT) == _USUB_SLOT) && ((flag & _USUB_TOPIC) == _USUB_TOPIC)) {
        if(slot >= 0 && slot < m_MqttClient.getMaxConnections()) {
            ret = m_MqttClient.unsubscribe(slot, topic);

            if(ret == EOK) {
                os()->jsonRpc()->successReply(req_id, "%d", slot);
            }
            else {
                os()->jsonRpc()->errorReply(ret, req_id, "unsubscribe error");
            }
        }
        else {
            ret = EID;
            os()->jsonRpc()->errorReply(ret, req_id, "invalid id");
        }
    }
    else {
        ret = EMISSING_PARAMETER;
        os()->jsonRpc()->errorReply(ret, req_id, "no id and/or topic specified");
    }
    
    return ret;
}
/**
 * 
 * @param params
 * @param req_id
 * @return 
 */
int ICACHE_FLASH_ATTR dispatcher_t::sysNop(JVal* params, int32_t req_id)
{
    // {"jsonrpc": "2.0", "method": "sys_nop", "params": {}, "id": 200}
    
    /******************************************************************************************************************
     * build JSON-RPC object
     *
     * {"jsonrpc": "2.0", "result": 19, "id": 1}
     */
    //os_printf("{\"jsonrpc\":\"2.0\",\"result\":{\"seconds\":%lu,\"freeheap\":%lu},\"id\":%lu}\n", m_Seconds, system_get_free_heap_size(), req_id);

    os()->jsonRpc()->successReply(req_id, "0");
    
    os_printf("{\"jsonrpc\":\"2.0\",\"method\":\"system\",\"params\":{\"code\":3,\"sys\":\"system\",\"msg\":\"nop\",\"p1\":null,\"p2\":null,\"n1\":%lu,\"n2\":%lu}}\n", m_Seconds, system_get_free_heap_size());
    
    return EOK;
}
/**
 * 
 * @param params
 * @param req_id
 * @return 
 */
int ICACHE_FLASH_ATTR dispatcher_t::sysRestart(JVal* params, int32_t req_id)
{
    // {"jsonrpc": "2.0", "method": "sys_restart", "params": {}, "id": 200}
    
    os()->jsonRpc()->successReply(req_id, "0");

    os()->reset();
    
    return EOK;
}
/******************************************************************************************************************
 * globals
 *
 */
com::tholusi::esp8266::esp8266_t        esp8266;
dispatcher_t                            dispatcher;

extern com::tholusi::esp8266::wifi_t    wifi;

/**
 * 
 */
extern "C" void ICACHE_FLASH_ATTR user_init(void)
{
    do_global_ctors();
    system_set_os_print(0);

    uart_init(BIT_RATE_115200, BIT_RATE_115200);
    //os_delay_us(1000000);
    
    int ret = esp8266.init(2048, 2048, 1024);
    
    os_printf("\n\n\n");
    
    //DTXT("MQTT ESP8266 Slave Firmware ver. %d.%d\n", VERSION_MAJOR, VERSION_MINOR);
    //DTXT("2015 Michael Jacobsen\n");
    //DTXT("\n");    
    //system_print_meminfo();
    //system_set_os_print(0);
    //DTXT("\n");
    
    if(ret == 0) {
        wifi.init(&esp8266);
        dispatcher.init(&esp8266);
        
        if(dispatcher.setup(&wifi) == EOK) {
            wifi.setup();

            //os_printf("{\"jsonrpc\":\"2.0\",\"method\":\"system\",\"params\":{\"code\":1,\"sys\":\"system\",\"msg\":\"start\",\"p1\":null,\"p2\":null,\"n1\":0,\"n2\":0}}\n");
        }
    }
    else {
        os_printf("{\"jsonrpc\":\"2.0\",\"method\":\"jsonrpc\",\"params\":{\"code\": -10,\"error\":\"esp8266.init() returns error; %d\"}}\n", ret);
    }
}
        