#include <Arduino.h>
#include <WiFi.h> // arduino-esp32 wifi api
#include <esp_wifi.h> // esp-idf wifi api
#include "TinyMqtt.h"   // https://github.com/hsaturn/TinyMqtt

#define PORT 1883
MqttBroker broker(PORT);

const char* ssid = "Group12";
const char* password = "";

void setup()
{
    Serial.begin(115200);
    delay(10);

    Serial.println("Started!");
    Serial.print("MAC Address is: ");
    Serial.println(WiFi.macAddress());

    Serial.println("Setting up Access Point...");
    if (!WiFi.softAP("Group12")) {
        Serial.println("Soft AP creation failed.");
        while(1);
    }
    
    Serial.println("AP Started!");
    IPAddress myIP = WiFi.softAPIP();
    Serial.println(myIP);

	if (strlen(ssid)==0){
        Serial.println("****** PLEASE MODIFY ssid/password *************");
    }

    broker.begin();
    Serial.print("Broker ready : ");
    Serial.println(WiFi.localIP());
    Serial.print(" on port ");
    Serial.println(PORT);
}

void get_client_details() {
    wifi_sta_list_t clients;
    esp_wifi_ap_get_sta_list(&clients);
    Serial.printf("\n\nConnected clients: %d\n", clients.num);
    IPAddress myIP = WiFi.softAPIP();
    Serial.printf("IP: ");
    Serial.println(myIP);
    Serial.printf("Port: %d\n", PORT);

    for (size_t i=0; i<clients.num; i++) {
        wifi_sta_info_t* info = &(clients.sta[i]);

        const char* phys = "Unknown";
        if (info->phy_11b && info->phy_11g && info->phy_11n) {
            phys = "802.11b/g/n";
        }

        Serial.printf("%d\tRSSI=%d\tPHY=%s\n", i,
            info->rssi, phys);
    }
}

void loop()
{
    get_client_details();
    broker.loop();
    delay(10000);
}
