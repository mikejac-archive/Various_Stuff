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

#ifndef COM_THOLUSI_ESP8266_ERRORCODES_T_HPP
#define	COM_THOLUSI_ESP8266_ERRORCODES_T_HPP

#define EOK                         0
#define ESTART                      1
#define ESYS_RESTART                2
#define ESYS_NOP_DONE               3
#define EPUB_DATA                   4
#define EPUB_DONE                   5
#define ESUB_DONE                   6
#define EUSUB_DONE                  7
#define ECONNECT_DONE               8
#define EDISCONNECT_DONE            9
#define EWIFI_CONNECTED             10
#define EWIFI_DISCONNECTED          11
#define EWIFI_LIST                  12
#define EJSON_PARSE_INCOMPLETE      13
#define EJSON_METHOD                14
#define EJSON_RESULT                15
#define EJSON_ERROR                 16

#define ETIMEOUT                    -1000
#define EINVALID_PACKET             -1001
#define EPROTOCOL_VERSION           -1002
#define EIDENTIFIER_REJECTED        -1003
#define ESERVER_UNAVAILABLE         -1004
#define EBAD_USERNAME_PASSWORD      -1005
#define ENOT_AUTHORIZED             -1006
#define ERESERVED                   -1007
#define EPING_TIMEOUT               -1008
#define EDISCONNECTED               -1009
#define ERECONNECT                  -1010
#define EDNS                        -1011
#define EPACKET_SIZE                -1012

#define EJSON_DECODER               -2000
#define ECONNECTION_INFO            -2001
#define EAVAILABLE_SLOT             -2002
#define EBROKER_CLIENT              -2003
#define EID                         -2004
#define EMISSING_PARAMETER          -2005
#define EJSON_SET                   -2006
#define EJSON_SET_ERR               -2007
#define EJSON_COMMIT                -2008
#define EJSON_PARSE_1               -2009
#define EJSON_PARSE_2               -2010
#define EJSON_PARSE_3               -2011
#define EJSON_PARSE_4               -2012
#define EJSON_PARSE_5               -2013
#define EJSON_PARSE_6               -2014
#define EJSON_PARSE_7               -2015
#define EJSON_PARSE_8               -2016
#define EJSON_PARSE_9               -2017
#define EJSON_PARSE_10              -2018
#define EJSON_PARSE_11              -2019
#define EJSON_PARSE_12              -2020
#define EJSON_PARSE_13              -2021
#define EJSON_PARSE_14              -2022
#define EJSON_PARSE_15              -2023
#define EJSON_PARSE_16              -2024
#define EJSON_PARSE_17              -2025
#define EJSON_PARSE_ERR             -2026
#define ENOT_READY_STATE            -2027

const int EWIFI_CONNECT_TIMEOUT     = -2050;
const int EWIFI_DISCONNECT_TIMEOUT  = -2051;
const int EMQTT_CONNECT_TIMEOUT     = -2052;
const int EMQTT_DISCONNECT_TIMEOUT  = -2053;
const int EMQTT_SUBSCRIBE_TIMEOUT   = -2054;
const int EMQTT_PUBLISH_TIMEOUT     = -2055;
const int ESYS_NOP_TIMEOUT          = -2056;
const int ESYS_RESTART_TIMEOUT      = -2057;

#define ENOT_CONNECTED              -2100
#define EOUTBOUNDBUSY               -2101
#define EMEMORY                     -2102
#define EPING_IN_PROGRESS           -2103
#define ENO_RECV_MSG                -2014

#define EWIFI_WRONG_PASSWORD        -2200
#define EWIFI_NO_AP_FOUND           -2201
#define EWIFI_CONNECT_FAIL          -2202
#define EWIFI_SSID                  -2203
#define EWIFI_NOT_CONNECTED         -2204
#define EWIFI_SCAN_FAILED           -2205

#endif	/* COM_THOLUSI_ESP8266_ERRORCODES_T_HPP */

