#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

// WiFi
const char* ssid = "LoraChoco";
const char* wifi_password = "MRB3HBM0R28";

// MQTT
const char* mqtt_server = "192.168.1.132";
const char* temperature_topic = "esp32/temperature";
const char* mqtt_username = "Ronan";
const char* mqtt_password = "Ronanmalo01";
const char* clientID = "client_cter_esp32_classroom";

// Configuration NeoPixel
#define PIN_NEOPIXEL 0 
#define NUMPIXELS 1
Adafruit_NeoPixel pixels(NUMPIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// Paramètres Alerte (Suppression de 'const' pour pouvoir le modifier via Node-RED)
float SEUIL = 30.0; 
unsigned long precedentMillis = 0;
const long intervalle = 200;
bool ledAllumee = false;

// Timer pour envoi MQTT sans bloquer le clignotement
unsigned long dernierEnvoiMQTT = 0;

WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient);

// Fonction de réception des messages (Slider Node-RED)
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  if (String(topic) == "esp32/reglage/seuil") {
    SEUIL = message.toFloat(); 
    Serial.print("Nouveau seuil reçu : ");
    Serial.println(SEUIL);
  }
}

void connect_MQTT(){
  if (!client.connected()) {
    if (client.connect(clientID, mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT Broker!");
      client.subscribe("esp32/reglage/seuil"); // On se réabonne après connexion
    }
  }
}

void setup() {
  Serial.begin(9600);
  
  pixels.begin();
  pixels.clear();
  pixels.show();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  
  client.setCallback(callback);
}

void loop() {
  connect_MQTT();
  client.loop(); // Indispensable pour traiter le callback

  int raw = analogRead(33);
  float volts = (float)raw * 3.3 / 4095;
  float degres = (volts / 0.01) + 13; // Ton calcul actuel
  
  // --- LOGIQUE CLIGNOTEMENT ---
  if (degres > SEUIL) {
    unsigned long actuelMillis = millis();
    if (actuelMillis - precedentMillis >= intervalle) {
      precedentMillis = actuelMillis;
      ledAllumee = !ledAllumee;
      
      if (ledAllumee) {
        pixels.setPixelColor(0, pixels.Color(255, 0, 0)); // ROUGE
      } else {
        pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      }
      pixels.show();
    }
  } else {
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();
  }

  // --- ENVOI MQTT (Toutes les 2 secondes sans bloquer) ---
  if (millis() - dernierEnvoiMQTT > 2000) {
    String temperature_string = String(degres);
    client.publish(temperature_topic, temperature_string.c_str());
    Serial.print("Degres : ");
    Serial.println(degres);
    dernierEnvoiMQTT = millis();
  }
}