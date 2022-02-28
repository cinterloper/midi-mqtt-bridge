//
// Created by g on 2/28/22.
//


#ifndef MIDI_AMQP_PAHO_MQTT_HPP
#define MIDI_AMQP_PAHO_MQTT_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "MQTTClient.h"

#define CLIENTID    "ExampleClientPub"
#define TOPIC       "MQTT Examples"
#define QOS         1
#define TIMEOUT     10000L
MQTTClient client;

int phao_connect(std::string address) {
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, address.c_str(), CLIENTID,
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
    }
    return rc;
}

void phao_disconnect() {
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
}

int phao_publish(const std::string& channel, std::string midi_event_bytes) {
    MQTTClient_deliveryToken token;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;

    pubmsg.payload = static_cast<void*>(midi_event_bytes.data());
    pubmsg.payloadlen = midi_event_bytes.size();
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, channel.c_str(), &pubmsg, &token);
    return MQTTClient_waitForCompletion(client, token, TIMEOUT);
}

#endif //MIDI_AMQP_PAHO_MQTT_HPP
