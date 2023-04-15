#include <SoftwareSerial.h> 
#include <TinyGPS++.h> 
#include <rdm6300.h> 
#include <WiFi.h> 
#include "FS.h"
#include "SD.h"
#include "SPI.h"

//definisi pin 


 #define RDM6300_RX_PIN 26
//#define RDM6300_TX_PIN 27
#define GPS_TX_PIN 16
#define GPS_RX_PIN 17
#define READ_LED_PIN 2

//#define led1 22 
//#define led2 21 
//#define led3 25 
//#define led4 33 
#define SD_CS 5
//inisialisasi ke software serial 
SoftwareSerial gpsSerial(GPS_RX_PIN, GPS_TX_PIN); 
Rdm6300 rdm6300; 
 
TinyGPSPlus gps; 
double latitude, longitude; 
String latStr, lonStr; 
String tagId;
 
//set wifi 
const char* ssid = "your ssid"; 
const char* password = "yourpass"; 
const char* host = "192.168.1.15"; 

int  number = 0;
int count = 0;
String numb;
 
File file; 
 
void setup() { 
  Serial.begin(115200); 
//  tulisan gj 
 Serial.setDebugOutput(false); 
  gpsSerial.begin(9600); 
   
  pinMode(READ_LED_PIN, OUTPUT); 
  pinMode(22, OUTPUT); // lampu pertama 
  pinMode(21, OUTPUT); // lampu kedua 
  pinMode(4, OUTPUT); // lampu ketiga 
  pinMode(33, OUTPUT); // lampu keempat 
  digitalWrite(READ_LED_PIN, LOW); 
 
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
 
  // Connect to WiFi network 
  Serial.println();   
  Serial.print("Connecting to "); 
  Serial.println(ssid); 
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); 
    Serial.print("."); 
  } 
  Serial.println(""); 
  Serial.println("WiFi connected"); 
    for (int i = 0; i < 4; i++) { 
    digitalWrite(22, HIGH); 
    delay(100); 
    digitalWrite(22, LOW); 
    delay(100); 
  } 
 
  Serial.println("\nPlace RFID tag near the RDM6300..."); 
      for (int i = 0; i < 3; i++) { 
    digitalWrite(4, HIGH); 
    delay(100); 
    digitalWrite(4, LOW); 
    delay(100); 
  }   
} 
 
void loop() { 
//  membaca gps  
    while (gpsSerial.available() > 0) { 
      if (gps.encode(gpsSerial.read())) { 
        if (gps.location.isUpdated()) { 
//          baca rfidtag 
           if (rdm6300.get_new_tag_id()) { 
        
              
                for(int i = 0; i < 3 ; i++)  
              { 
                digitalWrite(READ_LED_PIN, HIGH); // nyalakan LED 
                delay(100); // tunggu selama 1 detik 
                digitalWrite(READ_LED_PIN, LOW); // matikan LED 
                delay(100); // tunggu selama 1 detik 
              }
               
              Serial.print("RFID Tag: "); 
//            Serial.println(rdm6300.get_tag_id(), HEX); 
              tagId = String(rdm6300.get_tag_id(), HEX); 
              Serial.println(tagId); 
//            menampilkan latitude dan longitude 
              Serial.print("latitude: "); 
              latStr = String(gps.location.lat(), 6); 
              Serial.println(latStr); 
              Serial.print("longitude: "); 
              lonStr = String(gps.location.lng(), 6); 
              Serial.println(lonStr); 
 
  
 //cobaa ke dtb 
 // Connect to server 
    WiFiClient client;    
    const int httpPort = 80; 
    if (client.connect(host, httpPort)) { 
      sdTodb();
    } 
             
   // Send HTTP request 
    client.print(String("GET http://192.168.1.15/iotprojek/konek.php?") + 
                 ("tagId=") + tagId + 
                 ("&latitude=") + latStr + 
                 ("&longitude=") + lonStr + 
                 " HTTP/1.1\r\n" + 
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n"); 
 
    // tunggu respon 
    unsigned long timeout = millis(); 
    while (client.available() == 0) { 
      if (millis() - timeout > 1000) { 
              for (int i = 0; i < 3; i++) { 
    digitalWrite(33, HIGH); 
    delay(100); 
    digitalWrite(33, LOW); 
    delay(100); 
  }   
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
            }       
          } 
        } 
       
  digitalWrite(READ_LED_PIN, rdm6300.get_tag_id()); 
  delay(10);  
 
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

          Serial.print("RFID Tag: ");
          Serial.println(tagId);
          Serial.print("Latitude: ");
          Serial.println(latStr);
          Serial.print("Longitude: ");
          Serial.println(lonStr);
          
          // Kirim data ke database
           WiFiClient client;
          const int httpPort = 80;
          if (client.connect(host, 80)) {
            client.print(String("GET http://192.168.1.15/iotprojek/konek.php?") +
                         "tagId=" + tagId +
                         "&latitude=" + latStr +
                         "&longitude=" + lonStr +
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
}
void kirimSd(){
  number++;
  count++;
  numb = String() + "/datalog" + number + ".txt";
  appendFile(SD, numb, String() + tagId + "," + latStr  + "," + lonStr);
}






  
