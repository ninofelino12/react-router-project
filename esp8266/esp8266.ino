#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "LittleFS.h"
#include <EEPROM.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

//#define API_KEY "AIzaSyCmZ7ySVfRFKVOfLXBs8rKin3VkKLgcgOc"
#define API_KEY "AIzaSyCmZ7ySVfRFKVOfLXBs8rKin3VkKLgcgOc"
#define DATABASE_URL "https://myiot-5a297-default-rtdb.asia-southeast1.firebasedatabase.app" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "ninofelino12@gmail.com"
#define USER_PASSWORD "PASSWORD"

String parentPath;
String masterparentPath;
String hardwarePath;
// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;
FirebaseJson json;
FirebaseJson masterdata;
FirebaseJson hardwaredata;

bool signupOK = false;
byte address;
unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

const char* apSSID = "coolguard";
const char* apPassword = "12345678";
const char* staSSID = "MEGAVISION";
const char* staPassword = "nino2345";

ESP8266WebServer server(80);


// Handler untuk halaman web utama (kontrol pintu)
void handleRoot() {
  File file =LittleFS.open("/index.html", "r");
  if (!file) {
    Serial.println("Gagal membuka web/index.html");
    server.send(500, "text/plain", "Gagal membuka halaman utama");
    return;
  }
  server.streamFile(file, "text/html");
  file.close();
}


// Handler untuk aksi buka pintu (sama seperti sebelumnya)
void handleBuka() {
  Serial.println("Pintu Dibuka!");
  server.send(200, "text/plain", "Pintu Dibuka!");
}

// Handler untuk aksi tutup pintu (sama seperti sebelumnya)
void handleTutup() {
  Serial.println("Pintu Ditutup!");
  server.send(200, "text/plain", "Pintu Ditutup!");
}

void handleNotFound() {
  server.send(404, "text/plain", "Halaman Tidak Ditemukan!");
}

void handleStatus() {
  server.send(404, "text/plain", status());
}

String status() {
  Serial.println(signupOK); // Cetak nilai signupOK untuk debugging

  String jsonString = "{";

  // Tambahkan status signupOK
  jsonString += "\"signupOK\": ";
  jsonString += signupOK ? "\"true\"" : "\"false\"";
  jsonString += ","; // Tambahkan koma jika ada item lain setelah ini

  // Baca status pin 12
  int pin12Status = digitalRead(12);
  jsonString += "\"pin12\": ";
  jsonString += (digitalRead(12) == HIGH) ? "\"HIGH\"" : "\"LOW\"";
  jsonString += "\"pin13\": ";
  jsonString += (digitalRead(13) == HIGH) ? "\"HIGH\"" : "\"LOW\"";
    jsonString += "\"pin14\": ";
  jsonString += (digitalRead(14) == HIGH) ? "\"HIGH\"" : "\"LOW\"";
    jsonString += "\"pin15\": ";
  jsonString += (digitalRead(15) == HIGH) ? "\"HIGH\"" : "\"LOW\"";

  jsonString += "}";
  return jsonString;
}

void setup() {
  Serial.begin(115200);
   pinMode(12, INPUT_PULLUP); // Konfigurasi pin 12 sebagai input pull-up
   pinMode(13, INPUT_PULLUP); // Konfigurasi pin 12 sebagai input pull-up
pinMode(14, INPUT_PULLUP); // Konfigurasi pin 12 sebagai input pull-up
pinMode(15, INPUT_PULLUP); // Konfigurasi pin 12 sebagai input pull-up

  Serial.println();
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  // Konfigurasi sebagai Access Point
  WiFi.softAP(apSSID, apPassword);
  Serial.print("AP SSID: ");
  Serial.println(apSSID);
  Serial.print("AP IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Konfigurasi sebagai Station (mencoba terhubung ke internet)
  WiFi.mode(WIFI_AP_STA); // Set mode ke AP dan STA secara bersamaan
  WiFi.begin(staSSID, staPassword);
  Serial.print("Mencoba terhubung ke: ");
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(staSSID);
  Serial.println("local Ip");
  Serial.println(WiFi.localIP());

  // ... (kode untuk menunggu koneksi STA seperti sebelumnya) ...

  // Inisialisasi handler server (sama seperti sebelumnya)

  server.on("/", handleRoot);
// ...
server.on("/tutup", handleTutup);
server.on("/status", handleTutup);
// ...
  // ...
  server.begin();

    config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

   config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
    /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
 // Serial.println("HTTP server berjalan.");
 
  
}


void loop() {
  Serial.println(status());
  Serial.println(WiFi.softAPIP());
  if (WiFi.status() == WL_CONNECTED && Firebase.ready()) {
    Serial.println("ESP8266 terhubung ke Firebase!");
    // Lakukan operasi Firebase di sini (misalnya, set atau get data)
    delay(5000); // Jeda sebentar
  } else {
    Serial.println("ESP8266 tidak terhubung ke Firebase atau WiFi!");
    Serial.print("Status WiFi: ");
    Serial.println(WiFi.status());
    Serial.print("Status Firebase: ");
    Serial.println(Firebase.ready());
    delay(1000);
  }
  masterparentPath="test";
  json.set("sdk:",ESP.getSdkVersion() );
    json.set("mesinhidup",1 );
    json.set("overhead",1 );
    json.set("core:",ESP.getCoreVersion() );
    json.set("bahan bakar:", random(50, 70));
    json.set("suhu mesin:", random(10, 40) );
    json.set("suhu radiator:", random(90, 240) );
Serial.printf("Set master json... %s\n", Firebase.RTDB.setJSON(&fbdo, masterparentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
    
  server.handleClient();
}