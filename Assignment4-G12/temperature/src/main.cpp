// prints the Heltec WiFi Lora ESP32's internal temperature
#include <Arduino.h>
#include "driver/temp_sensor.h"    
#include "TinyMqtt.h"    // https://github.com/hsaturn/TinyMqtt
#include "TinyStreaming.h" // https://github.com/hsaturn/TinyConsole

const char* BROKER = "192.168.4.1";
const uint16_t BROKER_PORT = 1883;
const int interval = 5000;

const char* ssid = "Group12";
const char* password = "";

static float temp=19;
static MqttClient client;

void initTempSensor(){
    temp_sensor_config_t temp_sensor = TSENS_CONFIG_DEFAULT();
    temp_sensor.dac_offset = TSENS_DAC_L2; //TSENS_DAC_L2 is default, range: -10 to 80 degree celcius
    temp_sensor_set_config(temp_sensor);
    temp_sensor_start();
}

void onPublishControl(const MqttClient* /* source */, const Topic& topic, const char* payload, size_t /* length */){ 
    Serial.printf("Received from Broker\nTopic: ");
    Serial.printf(topic.c_str());
    Serial.printf("\nPayload: ");
    Serial.printf(payload);
    Serial.printf("\n\n");
}

void setup() {
  Serial.begin(115200);
  delay(500);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
	{ delay(500); Serial << '.'; }

  Serial.printf("Connected to ");
  Serial.printf(ssid);
  Serial.printf(" IP address: ");
  Serial.println(WiFi.localIP());

	client.connect(BROKER, BROKER_PORT);
  client.setCallback(onPublishControl);
  client.subscribe("control-msgs");

  initTempSensor();
}

void loop() {
  client.loop();	// Don't forget to call loop() for each broker and client

	// delay(1000);		please avoid usage of delay (see below how this done using next_send and millis())
	static auto next_send = millis();  // publishes every 5s (please avoid usage of delay())
	
	if (millis() > next_send)
	{
		next_send += interval;

		if (not client.connected())
		{
			Serial << millis() << ": Not connected to broker" << endl;
			return;
		}

    float temp = 0;
    temp_sensor_read_celsius(&temp); 
    Serial.print("--> Publishing a new temp-data value: ");
    Serial.println(temp);

		client.publish("temp-data", String(temp));
	}
}