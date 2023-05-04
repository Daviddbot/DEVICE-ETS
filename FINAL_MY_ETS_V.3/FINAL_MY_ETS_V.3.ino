#include <HTTPClient.h>
#include <SoftwareSerial.h> 
#include <TinyGPS++.h> 
#include <EEPROM.h>
#include <rdm6300.h> 
#include <WiFi.h> 
#include "SPI.h"
#include "FS.h"
#include "SD.h"
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"

AsyncWebServer server(80);
WiFiManager wifiManager;
//WebServer server(80);

//void handleRoot() {
//  String html = "<html><body>";
//  html += "<h1>ESP32 WiFi Manager</h1>";
//  html += "<p>Alamat IP: ";
//  html += WiFi.localIP().toString();
//  html += "</p>";
//  html += "<p><a href='/reset'>Reset WiFi</a></p>";
//  html += "</body></html>";
//  server.send(200, "text/html", html);
//}
//

//void handleReset() {
//  wifiManager.resetSettings();
//  server.send(200, "text/plain", "Koneksi WiFi telah direset. Silakan koneksikan ke AP ESP32 untuk mengatur ulang koneksi WiFi.");
//  delay(5000);
//  ESP.restart();
//}

void handleReset(AsyncWebServerRequest *request) {
  wifiManager.resetSettings();
  request->send(200, "text/plain", "Koneksi WiFi telah direset. Silakan koneksikan ke AP ESP32 untuk mengatur ulang koneksi WiFi.");
  delay(5000);
  ESP.restart();
}

String ledState;
#define ledPin 18


#define EEPROM_SIZE 512
#define RDM6300_RX_PIN 26
//#define RDM6300_TX_PIN 27
#define GPS_TX_PIN 16
#define GPS_RX_PIN 17
#define BUZZ 2
#define SD_CS 5
//inisialisasi ke software serial 
SoftwareSerial gpsSerial(GPS_RX_PIN, GPS_TX_PIN); 
Rdm6300 rdm6300; 
 
TinyGPSPlus gps; 
double latitude, longitude; 
String latStr;
String lonStr; 
String tagId;

String clubno="DA1234DF";
String raceno="1354";
// String deviceno="DA212121";
String deviceno = WiFi.macAddress();

// Replaces placeholder with GPS values
String processor(const String& var){
//Serial.println(var);
      if(var == "LATITUDE"){
        return String(lonStr);
      }
      else if(var == "LONGITUDE"){
       return String(lonStr);
      }
      
        return String();
      }

// const char* ssid = "TEKNOLAB Office";
// const char* password = "selamatdatang";
const char* serverName = "http://elgaarisprastyo.com/myets/get_data.php?";
const char* host = "elgaarisprastyo.com"; 


const int ledPin1 = 33; //22
const int ledPin2 = 4;
const int ledPin3 =  21;
const int ledPin4 = 22;

bool flag1 = true; // inisialisasi variabel flag sebagai true untuk LED 1
bool flag2 = true; // inisialisasi variabel flag sebagai true untuk LED 2
bool flag3 = true; // inisialisasi variabel flag sebagai true untuk LED 3
bool flag4 = true; // inisialisasi variabel flag sebagai true untuk LED 4
int status;

int  number;
#define Afile 0
int count;
#define Acount 10
String numb;
 
File file; 

void setup() {


  Serial.begin(115200);
//  wifiManager.setShouldClearFileSystem(false);
  wifiManager.autoConnect("MY ETS WIFI SETUP");
  
  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  Serial.println("Web server dimulai.");
  Serial.println("Terkoneksi ke WiFi dengan IP: " + WiFi.localIP().toString());

  String deviceno = WiFi.macAddress();
  Serial.print("ESP32 MAC ADDRES :" + deviceno );

//    while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }

    // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/wifimanager.html", String(), false, processor);
  });
      // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/wifiConfig.html", String(), false, processor);
  });
      // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.css", String(), false, processor);
  });
       // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/reset.html", String(), false, processor);
  });
  

  server.on("/latitude", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(latStr).c_str());
  });

  server.on("/longitude", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(lonStr).c_str());
  });

//  server.on("/check-connection", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send_P(200, "text/plain", "connected");
//  });

  
  if (WiFi.status() == WL_CONNECTED) {
    server.on("/check-connection", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/plain", "connected");
    });
  } else {
    server.on("/check-connection", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/plain", "not connected");
    });
  }

  server.on("/get-ip-address", HTTP_GET, [](AsyncWebServerRequest *request){
  String ipAddress = WiFi.localIP().toString();
  request->send_P(200, "text/plain", ipAddress.c_str());
  });

  server.on("/reset", HTTP_GET, handleReset);
  


  server.serveStatic("/", SPIFFS, "/");


  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  
//  tulisan gj 
  EEPROM.begin(EEPROM_SIZE);
  Serial.setDebugOutput(false); 
  gpsSerial.begin(9600); 
   
  pinMode(BUZZ, OUTPUT); 
  digitalWrite(BUZZ, LOW); 
 
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, LOW);
  SD.begin(SD_CS);
//   mendeteksi sdcard 
  if (!SD.begin(SD_CS)) {
    Serial.println("Gagal Memuat Kartu SD");
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("Tidak Ada Kartu SD");
    return;
  }

  delay(1000); 
  rdm6300.begin(RDM6300_RX_PIN);  
  Serial.println("\nPlace RFID tag near the RDM6300..."); 
}
void loop() {

//        server.handleClient();
//        LOAD GPS
      while (gpsSerial.available() > 0)     { 
      if (gps.encode(gpsSerial.read()))   { 
      if (gps.location.isUpdated())     {

//        menampilkan latitude dan longitude 
//              Serial.print("la : ");  
//              Serial.println(latStr); 
//              Serial.print("lo : ");  
//              Serial.println(lonStr);   
     
         
  if (WiFi.status() == WL_CONNECTED)  {
    HTTPClient http;
    http.begin(serverName);
    int httpCode = http.GET();
    if (httpCode > 0) {      
      String data = http.getString();
      Serial.println(data);
      if (data == "0" && flag1 == true) {
        for (int i = 0; i < 4; i++) {
          digitalWrite(ledPin1, HIGH);
          delay(500);
          digitalWrite(ledPin1, LOW);
          delay(500);
        }
        status = 0;
      }
      if (data == "1" && flag2 == true) {
        for (int i = 0; i < 3; i++) {
          digitalWrite(ledPin2, HIGH);
          delay(500);
          digitalWrite(ledPin2, LOW);
          delay(500);
        }
        status = 1;
      }
      if (data == "2" && flag3 == true) {
        for (int i = 0; i < 2; i++) {
          digitalWrite(ledPin3, HIGH);
          delay(500);
          digitalWrite(ledPin3, LOW);
          delay(500);
        }
        status = 2;
      }
      if (data == "3" && flag4 == true) {
        for (int i = 0; i < 1; i++) {
          digitalWrite(ledPin4, HIGH);
          delay(500);
          digitalWrite(ledPin4, LOW);
          delay(500);
        }
        status = 3;
      }
        if(status == 0){
        Serial.println("MODE BASKETING");
        basketing();
        flag1 = false;
        flag2 = true;
        flag3 = true;
        flag4 = true;
        }
          if(status == 1){
        Serial.println("MODE ABSENSI");
        utama();
        flag2 = false;
        flag1 = true;
        flag3 = true;
        flag4 = true;
        }
          if(status == 2){
        Serial.println("MODE TRAINING");
         utama();
        flag1 = true;
        flag2 = true;
        flag3 = false;
        flag4 = true;
        }
         if(status == 3){
        Serial.println("MODE RACE");
         utama();
        flag1 = true;
        flag2 = true;
        flag3 = true;
        flag4 = false;
        }
    }
    http.end();
        } 
      }
    }
  }
}


void utama() {
   
      lonStr = String(gps.location.lng(), 6);
      latStr = String(gps.location.lat(), 6);
      
           if (rdm6300.get_new_tag_id()) 
              {               
                for(int i = 0; i < 3 ; i++)  
              { 
                digitalWrite(BUZZ, HIGH); // nyalakan LED 
                delay(100); // tunggu selama 1 detik 
                digitalWrite(BUZZ, LOW); // matikan LED 
                delay(100); // tunggu selama 1 detik 
              }              
              Serial.print("ringno : "); 
              tagId = String(rdm6300.get_tag_id(), HEX); 
              Serial.println(tagId); 
//            menampilkan latitude dan longitude 
              Serial.print("la : "); 
              latStr = String(gps.location.lat(), 6); 
              Serial.println(latStr); 
              Serial.print("lo : "); 
              lonStr = String(gps.location.lng(), 6); 
              Serial.println(lonStr); 
              Serial.print("clubno : ");
              Serial.println(clubno);
              Serial.print("raceno : ");
              Serial.println(raceno);
              String deviceno = WiFi.macAddress();
              Serial.print("deviceno : ");
              Serial.println(deviceno);

 
 // Connect to server 
    WiFiClient client;    
    const int httpPort = 80; 
    if (client.connect(host, httpPort)) { 
      sdTodb();
    }             
   // Send HTTP request 
    client.print(String("GET http://elgaarisprastyo.com/myets/konek.php?") + 
                 ("ringno=") + tagId + 
                 ("&la=") + latStr + 
                 ("&lo=") + lonStr + 
                 ("&clubno=") + clubno +
                 ("&raceno=") + raceno +
                 ("&deviceno=") + deviceno +
                 " HTTP/1.1\r\n" + 
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n"); 
 
    // tunggu respon 
    unsigned long timeout = millis(); 
    while (client.available() == 0) { 
      if (millis() - timeout > 1000) { 
        Serial.println(">>> Client Timeout !"); 
        client.stop(); 
     kirimSd();
     return;
      } 
    }   
    // Read server response 
    while (client.available()) { 
      String line = client.readStringUntil('\r'); 
      Serial.print(line); 
    } 
    Serial.println(); 
              } 
       
       
  // digitalWrite(BUZZ, rdm6300.get_tag_id()); 
  // delay(10);  
 
}

void appendFile(fs::FS &fs, String path, String message){
    file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Tidak ada sd card untuk menulis");
        return;
    }
    Serial.printf("Appending to file: %s\n", path);
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}


void readFile(fs::FS &fs, String path){
   

    File file = fs.open(path);
    if(file){
    Serial.println("ada file");
    Serial.printf("Reading file: %s\n", path);
    Serial.println("Read from file: ");
    while(file.available()){
      String line = file.readStringUntil('\n');
      int separatorIndex = line.indexOf(",");
      if (separatorIndex != -1) {
        String tagId = line.substring(0, separatorIndex);
        String gpsLocation = line.substring(separatorIndex + 1);

        int separatorIndex2 = gpsLocation.indexOf(",");
        if (separatorIndex2 != -1) {
          String latStr = gpsLocation.substring(0, separatorIndex2);
          String lonStr = gpsLocation.substring(separatorIndex2 + 1);

          Serial.print("ringno : ");
          Serial.println(tagId);
          Serial.print("la : ");
          Serial.println(latStr);
          Serial.print("Lo : ");
          Serial.println(lonStr);
              Serial.print("clubno : ");
              Serial.println(clubno);
              Serial.print("raceno : ");
              Serial.println(raceno);
              String deviceno = WiFi.macAddress();
              Serial.print("deviceno : ");
              Serial.println(deviceno);
              
          // Kirim data ke database
           WiFiClient client;
          const int httpPort = 80;
          if (client.connect(host, 80)) {
            client.print(String("GET http://elgaarisprastyo.com/myets/konek.php?") +
                         "ringno=" + tagId +
                         "&la=" + latStr +
                         "&lo=" + lonStr +
                         "&clubno=" + clubno +
                         "&raceno=" + raceno +
                         "&deviceno=" + deviceno +
                         " HTTP/1.1\r\n" +
                         "Host: " + host + "\r\n" +
                         "Connection: close\r\n\r\n");
            Serial.println("Data berhasil dikirim ke server");

            // Tunggu respon dari server
            while (client.connected()) {
              if (client.available()) {
                String response = client.readStringUntil('\n');
                Serial.println(response);
              }
            }
            file.close();
            SD.remove(numb);
            Serial.println("Data berhasil dikirim dari SD card");
          } else {
            Serial.println("Gagal terkoneksi dengan server");
          }
        }
      }
    }
  }
}

void sdTodb(){
   for(number= 0; number<=count; number++){
    numb = String() + "/datalog" + number + ".txt";
    readFile(SD, numb);
    delay(1000);
  }
  count = 0;
  number =0;
  EEPROM.put(Afile, number);
  EEPROM.put(Acount, count);
  EEPROM.commit();
  Serial.println("Data written to EEPROM: " + String()+"COUNT=" + count);
}
void kirimSd(){
  count++;
  number++;
  EEPROM.put(Afile, number);
  EEPROM.put(Acount, count);
  EEPROM.commit();
  Serial.println("Data written to EEPROM: " + String()+"COUNT=" + count);
  numb = String() + "/datalog" + number + ".txt";
  appendFile(SD, numb, String() + tagId + "," + latStr  + "," + lonStr + "," + clubno + "," + raceno + "," + deviceno );
}

void basketing(){

      lonStr = String(gps.location.lng(), 6);
      latStr = String(gps.location.lat(), 6);
      
//          baca rfidtag 
           if (rdm6300.get_new_tag_id()) {              
                for(int i = 0; i < 3 ; i++)  
              { 
                digitalWrite(BUZZ, HIGH); // nyalakan LED 
                delay(100);               // tunggu selama 1 detik 
                digitalWrite(BUZZ, LOW);  // matikan LED 
                delay(100);               // tunggu selama 1 detik 
              }               
              Serial.print("ringno : "); 
              tagId = String(rdm6300.get_tag_id(), HEX); 
              Serial.println(tagId); 
//            menampilkan latitude dan longitude 
              Serial.print("la : "); 
              latStr = String(gps.location.lat(), 6); 
              Serial.println(latStr); 
              Serial.print("lo : "); 
              lonStr = String(gps.location.lng(), 6); 
              Serial.println(lonStr);
              Serial.print("clubno : ");
              Serial.println(clubno);
              Serial.print("raceno : ");
              Serial.println(raceno);
              String deviceno = WiFi.macAddress();
              Serial.print("deviceno : ");
              Serial.println(deviceno);
          } 
        }  
