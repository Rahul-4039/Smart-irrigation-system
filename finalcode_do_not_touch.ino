#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <DHT.h>

// Wi-Fi credentials
#define WIFI_SSID "Thorat"
#define WIFI_PASSWORD "15012006"

// Firebase Database URL and API key
#define FIREBASE_HOST "smart-irrigation-5ac56-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "zSZE2PNqlBz9cUyJJslxKb9djbt1htmfeWv5htcN"

// Firebase objects
FirebaseData firebaseData;
FirebaseConfig config;
FirebaseAuth auth;

// DHT sensor settings
#define DHTPIN D2          // Pin connected to DHT sensor
#define DHTTYPE DHT11      // DHT 11 (or DHT22 for other sensors)
DHT dht(DHTPIN, DHTTYPE);

// Moisture sensor settings
#define MOISTURE_SENSOR_PIN A0  // Pin connected to moisture sensor

// Motor control settings
#define MOTOR_PIN D3             // Pin connected to the relay or motor control circuit

// Manual control state
bool manualControl = false;
bool motorState = false;

void setup() {
  Serial.begin(115200);

  // Start DHT sensor
  dht.begin();

  // Setup motor control pin
  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, LOW);  // Ensure motor is off initially

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi.");

  // Firebase configuration
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  // Initialize Firebase
  Firebase.begin(&config, &auth);

  if (Firebase.ready()) {
    Serial.println("Connected to Firebase");
  } else {
    Serial.println("Failed to connect to Firebase");
  }
}

void loop() {
  checkWiFi();

  // Get humidity and temperature from DHT sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Get moisture level from sensor (0-1023)
  int moisture = analogRead(MOISTURE_SENSOR_PIN);

  // Check if the readings are valid
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Send humidity data to Firebase
  if (Firebase.setFloat(firebaseData, "/sensors/humidity", humidity)) {
    Serial.println("Humidity data written to Firebase.");
  } else {
    Serial.print("Failed to write humidity data: ");
    Serial.println(firebaseData.errorReason());
  }

  // Send moisture data to Firebase
  if (Firebase.setInt(firebaseData, "/sensors/moisture", moisture)) {
    Serial.println("Moisture data written to Firebase.");
  } else {
    Serial.print("Failed to write moisture data: ");
    Serial.println(firebaseData.errorReason());
  }

  // Check if manual control is enabled from Firebase
  if (Firebase.getBool(firebaseData, "/control/manual_control")) {
    manualControl = firebaseData.boolData(); // Get manual control state
  }

  // If manual control is enabled, read the motor state from Firebase
  if (manualControl) {
    if (Firebase.getBool(firebaseData, "/control/motor_state")) {
      motorState = firebaseData.boolData();  // Get motor state
    }

    if (motorState) {
      digitalWrite(MOTOR_PIN, HIGH);  // Turn motor ON
      Serial.println("Motor ON (Manual control)");
    } else {
      digitalWrite(MOTOR_PIN, LOW);   // Turn motor OFF
      Serial.println("Motor OFF (Manual control)");
    }
  } else {
    // Motor control logic based on moisture level (Automatic mode)
    if (moisture < 800) {  // Example threshold, adjust based on your sensor
      digitalWrite(MOTOR_PIN, HIGH);  // Turn the motor on
      Serial.println("Motor ON (Watering plants in auto mode)");
    } else {
      digitalWrite(MOTOR_PIN, LOW);  // Turn the motor off
      Serial.println("Motor OFF (Soil moisture is sufficient in auto mode)");
    }
  }

  // Add delay to avoid rapid updates
  delay(2000);
}

void checkWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected. Trying to reconnect...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Reconnected to Wi-Fi.");
  }
}