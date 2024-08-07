#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <PubSubClient.h>

const int RELAY_1 = 18;
const int RELAY_2 = 19;
const int RELAY_3 = 21;
const int RELAY_4 = 23;
const int RELAY_5 = 22;
const int RELAY_6 = 25;
const int RELAY_7 = 33;
const int RELAY_8 = 32;

#define switch1 34
#define switch2 35
#define switch3 26
#define switch4 27
#define switch5 13
#define switchf 4

int switch_ON_Flag1_previous_I = 0;
int switch_ON_Flag2_previous_I = 0;
int switch_ON_Flag3_previous_I = 0;
int switch_ON_Flag4_previous_I = 0;
int switch_ON_Flag5_previous_I = 0;
int switch_ON_Flagf_previous_I = 0;

const char *ssid = "B_402_2.4G";   // Enter your WiFi name
const char *password = "B402@123"; // Enter WiFi password
const char *mqtt_broker = "192.168.1.10";
const char *mqtt_username = "misara";
const char *mqtt_password = "openhab";
const int mqtt_port = 1883;

#define pub1 "/home/switch1/command"
#define pub2 "/home/switch2/command"
#define pub3 "/home/switch3/command"
#define pub4 "/home/switch4/command"
#define pub5 "/home/switch5/command"
#define pubf "/home/switchf/command"

WiFiClient espClient;
PubSubClient client(espClient);

bool ledState_1 = false;
bool ledState_2 = false;
bool ledState_3 = false;
bool ledState_4 = false;
bool ledState_5 = false;

void reconnect_mqtt()
{
    while (!client.connected())
    {
        String client_id = "esp8266-client--";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connecting to the mqtt broker\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password))
        {
            Serial.println("mqtt broker connected");
            digitalWrite(BUILTIN_LED, HIGH);
            delay(10000);
        }
        else
        {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
            digitalWrite(RELAY_1, digitalRead(switch1));
            digitalWrite(RELAY_2, digitalRead(switch2));
            digitalWrite(RELAY_3, digitalRead(switch3));
            digitalWrite(RELAY_4, digitalRead(switch4));
            digitalWrite(RELAY_5, digitalRead(switch5));
            digitalWrite(RELAY_6, digitalRead(switchf));
            digitalWrite(RELAY_7, digitalRead(switchf));
            digitalWrite(RELAY_8, digitalRead(switchf));
        }
        client.setKeepAlive(60);
    }
    client.subscribe("/home/switch1/command");
    client.subscribe("/home/switch2/command");
    client.subscribe("/home/switch3/command");
    client.subscribe("/home/switch4/command");
    client.subscribe("/home/switch5/command");
    client.subscribe("/home/switchf/command");
}

// connecting to a WiFi network
void setup_WiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");
}

void reconnect_wifi()
{
    // Loop until we're reconnected
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print("Reconnecting to WiFi...");
        WiFi.begin(ssid, password);
        // Wait 5 seconds before retrying
        delay(5000);
    }
    Serial.println("WiFi reconnected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void setup()
{
    // Defin output Relay
    pinMode(RELAY_1, OUTPUT);
    pinMode(RELAY_2, OUTPUT);
    pinMode(RELAY_3, OUTPUT);
    pinMode(RELAY_4, OUTPUT);
    pinMode(RELAY_5, OUTPUT);
    pinMode(RELAY_6, OUTPUT);
    pinMode(RELAY_7, OUTPUT);
    pinMode(RELAY_8, OUTPUT);
    pinMode(BUILTIN_LED, OUTPUT);

    // Define input Manual Switch
    pinMode(switch1, INPUT);
    pinMode(switch2, INPUT);
    pinMode(switch3, INPUT);
    pinMode(switch4, INPUT);
    pinMode(switch5, INPUT);
    pinMode(switchf, INPUT);

    // Set software serial baud to 115200;
    Serial.begin(115200);
    setup_WiFi();

    // connecting to a mqtt broker
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    client.setKeepAlive(60);
}

void callback(char *topic, byte *payload, unsigned int length)
{
    String message;
    if (strcmp(topic, "/home/switch1/command") == 0)
    {
        for (int i = 0; i < length; i++)
        {
            message += (char)payload[i]; // Convert *byte to string
        }

        if (message == "6" && !ledState_1)
        {
            digitalWrite(RELAY_1, LOW);
            ledState_1 = true;
            delay(500);
        }
        else if (message == "7" && ledState_1)
        {
            digitalWrite(RELAY_1, HIGH);
            ledState_1 = false;
            delay(500);
        }
    }
    else if (strcmp(topic, "/home/switch2/command") == 0)
    {
        for (int i = 0; i < length; i++)
        {
            message += (char)payload[i];
        }

        if (message == "11" && !ledState_2)
        {
            digitalWrite(RELAY_2, LOW);
            ledState_2 = true;
            delay(500);
        }
        else if (message == "00" && ledState_2)
        {
            digitalWrite(RELAY_2, HIGH);
            ledState_2 = false;
            delay(500);
        }
    }
    else if (strcmp(topic, "/home/switch3/command") == 0)
    {
        for (int i = 0; i < length; i++)
        {
            message += (char)payload[i];
        }

        if (message == "111" && !ledState_3)
        {
            digitalWrite(RELAY_3, LOW);
            ledState_3 = true;
            delay(500);
        }
        else if (message == "000" && ledState_3)
        {
            digitalWrite(RELAY_3, HIGH);
            ledState_3 = false;
            delay(500);
        }
    }
    else if (strcmp(topic, "/home/switch4/command") == 0)
    {
        for (int i = 0; i < length; i++)
        {
            message += (char)payload[i];
        }
        if (message == "1111" && !ledState_4)
        {
            digitalWrite(RELAY_4, LOW);
            ledState_4 = true;
            delay(500);
        }
        else if (message == "0000" && ledState_4)
        {
            digitalWrite(RELAY_4, HIGH);
            ledState_4 = false;
            delay(500);
        }
    }
    else if (strcmp(topic, "/home/switch5/command") == 0)
    {
        for (int i = 0; i < length; i++)
        {
            message += (char)payload[i];
        }

        if (message == "11111" && !ledState_5)
        {
            digitalWrite(RELAY_5, LOW);
            ledState_5 = true;
            delay(500);
        }
        else if (message == "00000" && ledState_5)
        {
            digitalWrite(RELAY_5, HIGH);
            ledState_5 = false;
            delay(500);
        }
    }
    else if (strcmp(topic, "/home/switchf/command") == 0)
    {
        for (int i = 0; i < length; i++)
        {
            message += (char)payload[i];
        }

        if (message == "4")
        {
            digitalWrite(RELAY_8, LOW);
            digitalWrite(RELAY_6, HIGH);
            digitalWrite(RELAY_7, HIGH);
            delay(500);
        }
        else if (message == "3")
        {
            digitalWrite(RELAY_8, HIGH);
            digitalWrite(RELAY_6, LOW);
            digitalWrite(RELAY_7, LOW);
            delay(500);
        }
        else if (message == "2")
        {
            digitalWrite(RELAY_8, LOW);
            digitalWrite(RELAY_6, HIGH);
            digitalWrite(RELAY_7, LOW);
            delay(500);
        }
        else if (message == "1")
        {
            digitalWrite(RELAY_8, LOW);
            digitalWrite(RELAY_6, LOW);
            digitalWrite(RELAY_7, HIGH);
            delay(500);
        }
        else if (message == "0")
        {
            digitalWrite(RELAY_8, HIGH);
            digitalWrite(RELAY_6, HIGH);
            digitalWrite(RELAY_7, HIGH);
            delay(500);
        }
    }
}

void manual_control()
{
    if (digitalRead(switch1) == LOW)
    {
        if (switch_ON_Flag1_previous_I == 0)
        {
            digitalWrite(RELAY_1, LOW);
            client.publish(pub1, "7");
            switch_ON_Flag1_previous_I = 1;
        }
    }
    if (digitalRead(switch1) == HIGH)
    {
        if (switch_ON_Flag1_previous_I == 1)
        {
            digitalWrite(RELAY_1, HIGH);
            client.publish(pub1, "6");
            switch_ON_Flag1_previous_I = 0;
        }
    }
    if (digitalRead(switch2) == LOW)
    {
        if (switch_ON_Flag2_previous_I == 0)
        {
            digitalWrite(RELAY_2, LOW);
            client.publish(pub2, "00");
            switch_ON_Flag2_previous_I = 1;
        }
    }
    if (digitalRead(switch2) == HIGH)
    {
        if (switch_ON_Flag2_previous_I == 1)
        {
            digitalWrite(RELAY_2, HIGH);
            client.publish(pub2, "11");
            switch_ON_Flag2_previous_I = 0;
        }
    }
    if (digitalRead(switch3) == LOW)
    {
        if (switch_ON_Flag3_previous_I == 0)
        {
            digitalWrite(RELAY_3, LOW);
            client.publish(pub3, "000");
            switch_ON_Flag3_previous_I = 1;
        }
    }
    if (digitalRead(switch3) == HIGH)
    {
        if (switch_ON_Flag3_previous_I == 1)
        {
            digitalWrite(RELAY_3, HIGH);
            client.publish(pub3, "111");
            switch_ON_Flag3_previous_I = 0;
        }
    }
    if (digitalRead(switch4) == LOW)
    {
        if (switch_ON_Flag4_previous_I == 0)
        {
            digitalWrite(RELAY_4, LOW);
            client.publish(pub4, "0000");
            switch_ON_Flag4_previous_I = 1;
        }
    }
    if (digitalRead(switch4) == HIGH)
    {
        if (switch_ON_Flag4_previous_I == 1)
        {
            digitalWrite(RELAY_4, HIGH);
            client.publish(pub4, "1111");
            switch_ON_Flag4_previous_I = 0;
        }
    }
    if (digitalRead(switch5) == LOW)
    {
        if (switch_ON_Flag5_previous_I == 0)
        {
            digitalWrite(RELAY_5, LOW);
            client.publish(pub5, "00000");
            switch_ON_Flag5_previous_I = 1;
        }
    }
    if (digitalRead(switch5) == HIGH)
    {
        if (switch_ON_Flag5_previous_I == 1)
        {
            digitalWrite(RELAY_5, HIGH);
            client.publish(pub5, "11111");
            switch_ON_Flag5_previous_I = 0;
        }
    }
    if (digitalRead(switchf) == LOW)
    {
        if (switch_ON_Flagf_previous_I == 0)
        {
            digitalWrite(RELAY_6, LOW);
            digitalWrite(RELAY_7, LOW);
            digitalWrite(RELAY_8, LOW);
            client.publish(pubf, "0");
            switch_ON_Flagf_previous_I = 1;
        }
    }
    if (digitalRead(switchf) == HIGH)
    {
        if (switch_ON_Flagf_previous_I == 1)
        {
            digitalWrite(RELAY_8, HIGH);
            client.publish(pubf, "1");
            switch_ON_Flagf_previous_I = 0;
        }
    }
    delay(100);
}

void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        reconnect_wifi();
    }
    if (!client.connected())
    {
        reconnect_mqtt();
    }
    manual_control();
    client.loop();
    delay(100);
}
