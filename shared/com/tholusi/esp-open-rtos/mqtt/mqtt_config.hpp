/* 
 * The MIT License (MIT)
 * 
 * ESP8266 FreeRTOS Firmware
 * Copyright (c) 2015 Michael Jacobsen (github.com/mikejac)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * https://github.com/SuperHouse/esp-open-rtos
 * 
 */

#ifndef ESP_OPEN_RTOS_MQTT_CONFIG_HPP
#define	ESP_OPEN_RTOS_MQTT_CONFIG_HPP

namespace esp_open_rtos {
namespace mqtt {

const int MAX_HEADER_SIZE       = 32;
const int MAX_TOPIC_SIZE        = 128;
const int MAX_PAYLOAD_SIZE      = 256;
const int MAX_PACKET_SIZE       = MAX_HEADER_SIZE + MAX_TOPIC_SIZE + MAX_PAYLOAD_SIZE;
const int MAX_HOSTNAME_SIZE     = 64;
const int MAX_CLIENT_ID_SIZE    = 23;
const int MAX_USERNAME_SIZE     = 32;
const int MAX_PASSWORD_SIZE     = 32;
const int CLIENT_YIELD          = 100;

} // namespace mqtt {
} // namespace esp_open_rtos {

#endif	/* ESP_OPEN_RTOS_MQTT_CONFIG_HPP */

