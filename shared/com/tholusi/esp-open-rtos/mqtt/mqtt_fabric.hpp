/* 
 * ESP8266 FreeRTOS Firmware
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
 * https://github.com/SuperHouse/esp-open-rtos
 * 
 */

#ifndef COM_THOLUSI_ESP_OPEN_RTOS_MQTT_FABRIC_HPP
#define	COM_THOLUSI_ESP_OPEN_RTOS_MQTT_FABRIC_HPP

#include <org/eclipse/paho/mqtt/MQTTClient/src/MQTTClient.h>
#include <stdint.h>
#include <string.h>

#include "mqtt_config.hpp"

namespace esp_open_rtos {
namespace mqtt {

const int ROOT_TOPIC_SIZE           = 20;
const int NODENAME_SIZE             = 40;
const int PLATFORM_ID_SIZE          = 20;

const int ACTOR_ID_SIZE             = 20;
const int ACTOR_PLATFORM_ID_SIZE    = 20;
const int TASK_ID_SIZE              = 20;

const int SUBTOPIC_SIZE             = 30;

/******************************************************************************************************************
 * fabric_t
 *
 */
class fabric_t
{
public:
    int init(const char* root_topic, const char* nodename, const char* platform_id);
    
    typedef enum {
        fabric_online,
        fabric_offline,
        fabric_disconnected
    } fabric_status_t;

    /**
     * 
     * @param fabric_status
     * @param seconds
     * @param topic
     * @param message
     * @return 
     */
    int fabricStatusMessage(fabric_status_t fabric_status, int seconds, char* topic, char* message);
    /**
     * 
     * @param service_id
     * @param feed_id
     * @param topic
     * @return 
     */
    int fabricOnrampTopic(const char* service_id, const char* feed_id, char* topic);
    /**
     * 
     * @param service_id
     * @param feed_id
     * @param topic
     * @return 
     */
    int fabricOfframpTopic(const char* service_id, const char* feed_id, char* topic);
    /**
     * 
     * @return 
     */
    const char* fabricRootTopic()
    {
        return m_RootTopic;
    }
    /**
     * 
     * @param md
     * @return 
     */
    int fabricTokenBegin(const MQTT::MessageData* md);
    /**
     * 
     * @param subtopic
     * @param index
     * @return 
     */
    int fabricTokenNext(char **subtopic, int& index);
    /**
     * 
     * @return 
     */
    int fabricTokenReset();
    
protected:
    char                m_RootTopic[ROOT_TOPIC_SIZE];
    char                m_NodeName[NODENAME_SIZE];
    char                m_PlatformId[PLATFORM_ID_SIZE];

    char                m_ActorId[ACTOR_ID_SIZE];
    char                m_ActorPlatformId[ACTOR_PLATFORM_ID_SIZE];
    char                m_TaskId[TASK_ID_SIZE];
    
private:
    MQTT::MessageData*  m_Md;
    char*               m_P;
    char*               m_Pp;
    int                 m_Words;
    int                 m_I;
    
    char                m_LastSubtopic[SUBTOPIC_SIZE];
};

} // namespace mqtt {
} // namespace esp_open_rtos {

#endif	/* COM_THOLUSI_ESP_OPEN_RTOS_MQTT_FABRIC_HPP */

