#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <time.h>
#include <esp_system.h>
#include "logger_module.h"

// =====================================================
// SUPABASE SETTINGS
// =====================================================
const char* SUPABASE_URL =
"https://rucewsvsynjgaawypcer.supabase.co/rest/v1/mins5_solar_reading";

const char* API_KEY =
"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InJ1Y2V3c3ZzeW5qZ2Fhd3lwY2VyIiwicm9sZSI6ImFub24iLCJpYXQiOjE3ODQzMTIyMTEsImV4cCI6MjA5OTg4ODIxMX0.UgGiB5p68HfH-TGnDWjaez90zUGbYHitN4DY62bmvkc";

// =====================================================
// DATA STORAGE (accumulator for 5-min averaging)
// =====================================================
JsonDocument sumDoc;
JsonDocument lastDoc;
unsigned int sampleCount = 0;
bool hasData = false;

unsigned long lastStoreTime = 0;
const unsigned long STORE_INTERVAL = 300000UL; // 5 minutes

// =====================================================
// TIME
// =====================================================
String getTimestamp() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return String(millis());
  }
  char buffer[30];
  strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
  return String(buffer);
}

// =====================================================
// UNIQUE RECORD ID
// =====================================================
String makeRecordId() {
  time_t now;
  time(&now);
  uint32_t r = esp_random();
  char buffer[40];
  snprintf(buffer, sizeof(buffer), "%ld-%08x", (long)now, r);
  return String(buffer);
}

// =====================================================
// SUPABASE UPLOAD
// =====================================================
bool sendToSupabase(const String &payload) {
  HTTPClient http;
  http.begin(SUPABASE_URL);
  http.addHeader("apikey", API_KEY);
  http.addHeader("Authorization", String("Bearer ") + API_KEY);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(payload);
  String response = http.getString();

  Serial.printf("HTTP Code: %d\n", httpCode);
  Serial.println("Response: " + response);
  Serial.println("Payload sent: " + payload);

  http.end();
  return (httpCode >= 200 && httpCode < 300);
}

// =====================================================
// OFFLINE STORAGE
// =====================================================
void saveOffline(const String &payload) {
  File file = SPIFFS.open("/offline.txt", FILE_APPEND);
  if (!file) {
    Serial.println("Failed To Save");
    return;
  }
  file.println(payload);
  file.close();
  Serial.println("Saved Offline");
}

// =====================================================
// SYNC OFFLINE DATA
// =====================================================
void syncOffline() {
  if (WiFi.status() != WL_CONNECTED) return;

  File file = SPIFFS.open("/offline.txt");
  if (!file) return;

  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim();
    if (line.length() == 0) continue;

    if (!sendToSupabase(line)) {
      file.close();
      Serial.println("Sync Failed");
      return;
    }
  }
  file.close();
  SPIFFS.remove("/offline.txt");
  Serial.println("Offline Sync Complete");
}

// =====================================================
// FEED A READING INTO THE ACCUMULATOR
// (replaces the old UART/processSensorData ingestion)
// =====================================================
void loggerAddReading(
    float voltage,
    float current,
    float power,
    float energy,
    float frequency,
    float powerFactor,
    float temperature,
    float humidity
)
{
    auto accumulate = [](const char* key, double val) {
        double existing = sumDoc[key].is<double>() ? sumDoc[key].as<double>() : 0.0;
        sumDoc[key] = existing + val;
    };

    // Averaged fields
    accumulate("voltage", voltage);
    accumulate("current", current);
    accumulate("power", power);
    accumulate("frequency", frequency);
    accumulate("powerfactor", powerFactor);
    accumulate("temperature", temperature);
    accumulate("humidity", humidity);

    // Latest-value fields
    lastDoc["energy"]     = energy;

    sampleCount++;
    hasData = true;
}

// =====================================================
// STORE THE 5-MINUTE AVERAGE
// =====================================================
void storeLatestReading() {
  if (!hasData || sampleCount == 0) return;

  JsonDocument outDoc;

  for (JsonPair kv : sumDoc.as<JsonObject>()) {
    outDoc[kv.key()] = kv.value().as<double>() / sampleCount;
  }
  for (JsonPair kv : lastDoc.as<JsonObject>()) {
    outDoc[kv.key()] = kv.value();
  }

  outDoc["sample_count"] = sampleCount;
  outDoc["timestamp"] = getTimestamp();
  outDoc["record_id"] = makeRecordId();

  String payload;
  serializeJson(outDoc, payload);

  if (WiFi.status() == WL_CONNECTED) {
    if (!sendToSupabase(payload)) {
      saveOffline(payload);
    }
    syncOffline();
  } else {
    saveOffline(payload);
  }

  Serial.println("Averaged Reading Stored");

  sumDoc.clear();
  lastDoc.clear();
  sampleCount = 0;
  hasData = false;
}

// =====================================================
// SETUP
// =====================================================
void loggerBegin() {
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Failed");
    return;
  }

  configTime(0, 0, "pool.ntp.org", "time.nist.gov");

  Serial.println("Logger Ready");

  syncOffline();
}

// =====================================================
// LOOP
// =====================================================
void loggerUpdate() {
  if (millis() - lastStoreTime >= STORE_INTERVAL) {
    lastStoreTime = millis();
    storeLatestReading();
  }
}