#include <Arduino.h> // Required for all code
#include <WiFi.h> // Required for all wifi code

#include "heltec.h"

#include "TinyMqtt.h"    // https://github.com/hsaturn/TinyMqtt
#include "TinyStreaming.h" // https://github.com/hsaturn/TinyConsole

const char* BROKER = "192.168.4.1";
const uint16_t BROKER_PORT = 1883;

const char* ssid = "Group12";
const char* password = "";

static MqttClient client;

const byte ledPin = 35;
const byte interruptPin = 0;
volatile byte state = LOW;
volatile boolean button_pressed = false;

void blink() {
    button_pressed = true;
}

void onPublishA(const MqttClient* /* source */, const Topic& topic, const char* payload, size_t /* length */){ 
    Serial.printf("Received from Broker\nTopic: ");
    Serial.printf(topic.c_str());
    Serial.printf("\nPayload: ");
    Serial.printf(payload);
    Serial.printf("\n\n");
}

void setup()
{
    // Set WiFi to station mode and disconnect from an AP if it was previously connected.
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    // Initialize display and serial. 
    Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
	{ delay(500); Serial << '.'; }

    Serial.printf("Connected to ");
    Serial.printf(ssid);
    Serial.printf(" IP address: ");
    Serial.println(WiFi.localIP());

    pinMode(interruptPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(interruptPin), blink, RISING);

	client.connect(BROKER, BROKER_PORT);
    client.setCallback(onPublishA);
    client.subscribe("temp-data");
}

void loop()
{ 
    digitalWrite(ledPin, state);
    client.loop();

    static auto next_send = millis();

    if(button_pressed){
        button_pressed = !button_pressed;
        Serial.print("--> Publishing a new control-msgs value: pressed\n");
        client.publish("control-msgs", "pressed");
    }

	if (millis() > next_send)
	{
		next_send += 1000;

		if (not client.connected())
		{
			Serial << millis() << ": Not connected to broker" << endl;
			return;
		}
	}
    // if (n == 0) {
    //     Serial.println("no networks found");

    //     Heltec.display->clear();
    //     Heltec.display->drawStringMaxWidth(0, 0, 128, "no networks found.");
    //     Heltec.display->display();
    // } else {

    //     Heltec.display->clear();
    //     Heltec.display->drawStringMaxWidth(0, 0, 128, "WiFi Networks");
    //     Heltec.display->display();

    //     Serial.print(n);
    //     Serial.println(" Networks found:");

    //     for (int i = 0; i < n; ++i) {

    //         // Format network names for the OLED display.
    //         int column = i / 5;
    //         int row = i % 5;

    //         char ssid_name[100];
    //         uint16_t width;
    //         uint16_t char_count;
    //         int ssid_len;

    //         ssid_len = strlen(WiFi.SSID(i).c_str());
    //         for (char_count=5; char_count<ssid_len; char_count++) {
    //             width = Heltec.display->getStringWidth(WiFi.SSID(i).c_str(), char_count);

    //             if (width > 63) {
    //                 char_count = char_count - 1;
    //                 break;
    //             }
    //         }

    //         memcpy(ssid_name, WiFi.SSID(i).c_str(), char_count);
    //         ssid_name[char_count] = '\0';

    //         // Display network names on the screen until the screen is full.
    //         if (column < 2) {
    //             Heltec.display->drawStringMaxWidth(column * 64, (row+1) * 10, 63, ssid_name);
    //             Heltec.display->display();
    //         }

    //         // Also print on serial.
    //         Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
    //     }
    // }
}

