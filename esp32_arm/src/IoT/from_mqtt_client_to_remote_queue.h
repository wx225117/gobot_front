#pragma once

#include "IoT/mqtt_message_consumer.h"
void mono_remote_queue_bridge_via_mqtt_setup(const char* topic, MessageQueue* local_gcode_queue, MqttMessageConsumer* local_gcode_consumer);
void mono_remote_queue_bridge_spin_once();

#include "IoT/wifi_mqtt_client.h"

