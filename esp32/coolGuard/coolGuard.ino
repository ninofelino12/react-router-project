//#include <WiFi.h>
#define KENDARAAN "BOLDUZER"
#define OPERATOR "Jhoni"
#define MCU "ESP8266"


#define API_KEY "AIzaSyCmZ7ySVfRFKVOfLXBs8rKin3VkKLgcgOc"
#define DATABASE_URL "https://myiot-5a297-default-rtdb.asia-southeast1.firebasedatabase.app" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define NTP_SERVER "pool.ntp.org"
#define UTC_OFFSET_SEC 25200 // Waktu Indonesia Barat (WIB)
//#define API_KEY "AIzaSyBjSTTlLJHED34kiRKX32IyHK1LKy7m18M"
//#define DATABASE_URL "https://esp32-s3-control-system-default-rtdb.firebaseio.com/" 

#define PHOTO "https://raw.githubusercontent.com/ninofelino12/nino_ecu_iot/refs/heads/main/lolin.jpeg"

#define GITHUB "https://raw.githubusercontent.com/ninofelino12/react-router-project/refs/heads/master/img/"
//              https://raw.githubusercontent.com/ninofelino12/react-router-project/refs/heads/master/img/13597457.jpg
#include <LOLIN_I2C_BUTTON.h>

const int output = 2;
const int buttonPin = 4;
int ledState = LOW;          // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
int lcdColumns = 16;
int lcdRows = 2;
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
  #include "esp_chip_info.h"
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
 
#include <ESP8266WebServer.h>
#endif
#include <NTPClient.h>
    #include <WiFiUdp.h>

#include <SPI.h>

//#include <WebServer.h>
#include "time.h"
#include <lwip/ip_addr.h>
#include <lwip/netif.h>
#include <lwip/opt.h>
#include <lwip/tcpip.h>
#include <lwip/ip4_addr.h>
#include <lwip/dns.h>
#include <Firebase_ESP_Client.h>
//#include <SoftwareSerial.h>#include <Adafruit_Sensor.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <TinyGPS++.h>
#define DHTTYPE DHT11   // DHT 11
const int DHTPin = 5;
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 48

#define OLED_RESET -1 

//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_SSD1306 display(OLED_RESET);
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

    //SoftwareSerial sim800(10, 11); // RX, TX
//const char* apSSID = "ECU"+ESP.getChipId();
const char* apSSID = "CCTVSAMPING";
const char* apPassword = "11111111";
const char* staSSID = "MEGAVISION";
const char* staPassword = "nino2345";

     WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP, NTP_SERVER, UTC_OFFSET_SEC);

String parentPath;
String masterparentPath;
String hardwarePath;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;
bool ismodem = false;
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
//WebServer server(80);
int timestamp;
FirebaseJson json;
FirebaseJson masterdata;
FirebaseJson hardwaredata;

byte address;

 // the previous reading from the input pin
int reading ;
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
//unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
//unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
const int BUZZZER_PIN = T8;
const int buzzerPin = T8;
byte buzzer = T8;  
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

TinyGPSPlus gps;
//ESP8266WebServer server(80);   //instantiate server at port 80 (http port)

  // Konfigurasi Broker MQTT
    const char* mqtt_server = "broker.emqx.io";
    const int mqtt_port = 1883;
    const char* mqtt_topic = "topik/esp8266";

    //WiFiClient espClient;
   // PubSubClient client(espClient);
    
void handleRoot() {
  Serial.print("handle root");
 //server.send(200, "text/html", "<h1>ESP32 WiFi Extender & Web Server</h1>");
}
 int nctl = 0;
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}
void setup() {
   Wire.begin(5, 4);

  
  
   dht.begin();
    pinMode(buzzerPin, OUTPUT);
  Serial.begin(115200);
  //scani2c() ;
  tone(buzzer,262);
  delay(50);

delay(10);
  WiFi.mode(WIFI_AP_STA);

  WiFi.softAP(apSSID, apPassword);
  IPAddress apIP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(apIP);
  WiFi.mode(WIFI_STA);
  WiFi.begin(staSSID, staPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Konfigurasi NAT
  //tcpip_adapter_ip_info_t ipInfo;
  //tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ipInfo);
  ip4_addr_t ip4_gw;
  ip4_addr_t ip4_sn;
  ip4_addr_t ip4_ip;
  //ip4_addr_set_u32(&ip4_ip, ipInfo.ip.addr);
  //ip4_addr_set_u32(&ip4_gw, ipInfo.gw.addr);
  //ip4_addr_set_u32(&ip4_sn, ipInfo.netmask.addr);
 // netif_set_gw(netif_find(TCPIP_ADAPTER_IF_AP), &ip4_gw);
//  netif_set_netmask(netif_find(TCPIP_ADAPTER_IF_AP), &ip4_sn);
//  netif_set_ipaddr(netif_find(TCPIP_ADAPTER_IF_AP), &ip4_ip);

  //server.on("/", handleRoot);
 // server.begin();
   //server.on("/", handleRoot);

  Serial.println("Web server started!");
    config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

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
  String mac=WiFi.macAddress();
  parentPath="data-"+WiFi.macAddress();
  masterparentPath="master-"+mac;
  hardwarePath="hardware-"+mac;
  
 
     

}


void scani2c() {
  byte error;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  delay(5000);          
}


void loop() {
  char buffer[50];
  buttonState = digitalRead(buttonPin); // Baca status push button

 // Serial.println(WiFi.softAPIP());
 // Serial.println(WiFi.localIP());
   Serial.println("OLED");
display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
     display.println(F("scroll"));
    display.setTextColor(WHITE);

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("scroll"));
  display.display();      // Show initial text
  if (buttonState == LOW) { // Jika tombol ditekan (LOW karena pull-up)
    Serial.println("Tombol ditekan!");
    delay(200); // Debouncing (menghindari pembacaan ganda)
  }
   tone(buzzerPin, 3000); // Buzzer berbunyi dengan frekuensi 2000 Hz
  delay(100); // Durasi bunyi detak
  noTone(buzzerPin); // Buzzer mati
  delay(200); // Jeda antar detak
    lcd.setCursor(0, 0);
  // print message
  lcd.print("cool guard");
  delay(1000);
     lcd.setCursor(0,1);

  // clears the display to print new message
  //lcd.clear();
  timeClient.update();
    unsigned long epochTime = timeClient.getEpochTime();
    time_t rawtime = epochTime;

  struct tm * timeinfo = localtime (&rawtime);
      String formattedDate = timeClient.getFormattedTime();
reading = digitalRead(buttonPin);
//cekSIM800();
Serial.println(address,HEX);
  String mac=WiFi.macAddress();
Serial.print(ESP.getCoreVersion());
   //Serial.println(WiFi.macAddress());
  //server.handleClient();
   
   if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    json.set("sdk:",ESP.getSdkVersion() );
    json.set("mesinhidup",1 );
    json.set("overhead",1 );
    json.set("core:",ESP.getCoreVersion() );
    json.set("bahan bakar:", random(50, 70));
    json.set("suhu mesin:", random(10, 40) );
    json.set("suhu radiator:", random(90, 240) );

    //masterdata.set("Id"+parentPath,"MAC");
    //masterdata.set("mac",mac);
    masterdata.set("03-type","Excavator");
    masterdata.set("03-alatberat","Excavator");
    masterdata.set("02-ACC ",digitalRead(buttonPin));
    masterdata.set("01-OVERHEAD",digitalRead(buttonPin+1));

    masterdata.set("40-suhumesin",random(60, 150));
    
    masterdata.set("04-nama",KENDARAAN);
    masterdata.set("05-operator",OPERATOR);
    
   
   // masterdata.set("photo",PHOTO);
    masterdata.set("37-lokasi","gps");
    masterdata.set("08-reading",reading);
    masterdata.set("timestamp",timeClient.getEpochTime());
        masterdata.set("00-tanggal",asctime(timeinfo));
    masterdata.set("32-mcu",MCU);
    // masterdata.set("33-idmcu",ESP.getChipId());
    
    

     masterdata.set("link",parentPath.c_str());
    // String alamat= String(ESP.)+".jpg";
     //masterdata.set("img",GITHUB+alamat);
     //masterdata.set("img2",GITHUB+alamat);
     // masterdata.set("photo",GITHUB+String("P")+alamat);
     //   masterdata.set("photo2",GITHUB+String("P")+alamat);
      //masterdata.set("33-Flash ",ESP.getFlashChipSize() / (1024 * 1024));
      masterdata.set("z50-PIND8_BUZZER",digitalRead(buttonPin));
      //masterdata.set("z50-PIND7",digitalRead(D7));
      //masterdata.set("z50-PIND6",digitalRead(D6));
      //masterdata.set("z50-PIND5",digitalRead(D5));
      masterdata.set("z51 Count", nctl++);
      int d5Value = digitalRead(14);
 
//binary_string = str(d8_val) + str(d7_val) + str(d6_val) + str(d5_val) // #most significant bit is d8
 //binary_integer = int(binary_string, 2)
  // Gabungkan nilai-nilai menjadi satu nilai biner
 // int binaryValue = (d8Value << 3) | (d7Value << 2) | (d6Value << 1) | d5Value;
//int angka[] = {10, 20, 30, 40, 50}; 
  // sprintf(buffer, "Values: %d, %d, %d", digitalRead(5),digitalRead(6),digitalRead(7));
   masterdata.set("Led", buffer);
     


    //masterdata.set("Humidity",dht.readHumidity());
    hardwaredata.set("reading",reading);
    hardwaredata.set("modem",ismodem);
    hardwaredata.set("I2C",address);
   // hardwaredata.set("Chipid",ESP.getChipId());
     hardwaredata.set("freq",ESP.getCpuFreqMHz());
    hardwaredata.set("Flash ",ESP.getFlashChipSize() / (1024 * 1024));
//    hardwaredata.set("Led",{digitalRead(D4),digitalRead(D5)});


  //  json.set("timePath", String(timestamp));
    Serial.printf("Set json... %s\n", Firebase.RTDB.pushJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set master json... %s\n", Firebase.RTDB.setJSON(&fbdo, masterparentPath.c_str(), &masterdata) ? "ok" : fbdo.errorReason().c_str());
  //  Serial.printf("Hardware... %s\n", Firebase.RTDB.setJSON(&fbdo, hardwarePath.c_str(), &hardwaredata) ? "ok" : fbdo.errorReason().c_str());
     lcd.setCursor(0, 1);
  // print message
     lcd.print(fbdo.errorReason().c_str());
      delay(10000);
   
}}
void macToString(uint8_t mac[], char *macString) {
  sprintf(macString, "%02X%02X%02X%02X%02X%02X",
          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}
void removeColons(char *str) {
  char *token = strtok(str, ":");
  char temp[100] = ""; // Asumsikan string hasil tidak akan lebih dari 100 karakter
  while (token != NULL) {
    strcat(temp, token);
    token = strtok(NULL, ":");
  }
  strcpy(str, temp); // Salin string hasil kembali ke string asli
}


int esp32PinsToBinary(int d5, int d6, int d7, int d8) {
  /*
    Converts the digital states of ESP32 pins D5, D6, D7, and D8 to an integer.

    Args:
      d5: State of pin D5 (HIGH or LOW, 1 or 0).
      d6: State of pin D6 (HIGH or LOW, 1 or 0).
      d7: State of pin D7 (HIGH or LOW, 1 or 0).
      d8: State of pin D8 (HIGH or LOW, 1 or 0).

    Returns:
      The integer representation of the binary value.
  */

  return (d8 << 3) | (d7 << 2) | (d6 << 1) | d5;
}

 void cekSIM800() {
     
      
      Serial.println("SIM800 tidak terdeteksi!");
       lcd.setCursor(0,1);
      lcd.print("SIM800 tidak terdeteksi!");
    }