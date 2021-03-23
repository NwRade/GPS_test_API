#include <DHTesp.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include <Dictionary.h>
#include <NodeArray.h>
#include <ESP8266WiFi.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)
#define DHT11_PIN D4

// String serverName = "http://192.168.0.157:3000/";
// String serverName = "http://192.168.0.194:3000/";
String serverName = "http://192.168.43.1:3000/";
static const int RXPin = D6, TXPin = D5;

static const uint32_t GPSBaud = 9600;

//const char* ssid = "Syntax Error";
//const char* password = "VpjBBcDfDesC7PF";

//const char* ssid = "Redmi";
//const char* password = "sit0972001";

const char* ssid = "realme 2"; //ssid of your wifi
const char* password = "87654321"; //password of your wifi

Dictionary &payload = *(new Dictionary());
Dictionary &GPS = *(new Dictionary());
Dictionary &BMP = *(new Dictionary());
Dictionary &DHT1 = *(new Dictionary());
// The TinyGPS++ object
TinyGPSPlus gps;

Adafruit_BMP280 bmp;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

LiquidCrystal_I2C lcd(0x3F, 16, 2);
DHTesp dht;

//LiquidCrystal_I2C lcd(0x3F, 20, 4);
int state = 0;
static int dataView = 0;

String sSatAtl,sSpeed, sLat, sLng, sDate, sTime, sAge, sHDOP, sPCS, sFCS, sCP, sSwFIX;
String sBAlt, sBTemp, sBAtm, sDHum, sDTemp, sDHInd;
void setup()
{
  // Setup The I2C LCD Dipslay
  lcd.begin(16, 2);
  //    lcd.begin(20,4);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Simple GPS model");
  lcd.setCursor(centerDo("by Adwait", 16), 1);
  lcd.print("by Adwait");

  
  pinMode(D7, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  ss.begin(GPSBaud);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); //connecting to wifi
  while (WiFi.status() != WL_CONNECTED)// while wifi not connected
  {
    delay(500);
    Serial.print("."); //print "...."
  }
  Serial.println("");
  Serial.println("Connection Established!!");
  Serial.println(WiFi.localIP());  // Print the IP address
  dht.setup(DHT11_PIN, DHTesp::DHT11);
  if (!bmp.begin(0x76)) { // havve to reconfigure this bmp library
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
  }
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  
  delay(1000);
  lcd.clear();
}
void loop() {

  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;
    // print the number of satellites visible to the sensors antenae
    while (ss.available() > 0) //while data is available
      if (gps.encode(ss.read())) //read gps data
      {
        state = digitalRead(D7);
        if (state == 1) {
          dataView++;
          if (dataView >= 6) {
            dataView = 0;
          }
        }
        
        GPS("sats", String (gps.satellites.value()));
        GPS("lati", String (gps.location.lat(), 8));
        GPS("longi", String (gps.location.lng(), 8));
        GPS("age", String (gps.location.age(), 5));
        GPS("date", String (gps.date.day()) + "/" + String (gps.date.month()) + "/" + String (gps.date.year()));
        GPS("time", String (gps.time.hour()) + ":" + String (gps.time.minute()) + ":" +  String (gps.time.second()));
        GPS("hdop", String (gps.hdop.value()));
        GPS("speed", String (gps.speed.kmph()));
        GPS("alt", String (gps.altitude.meters()));
        GPS("failedCheckSum", String (gps.failedChecksum()));
        GPS("passedCheckSum", String (gps.passedChecksum()));
        GPS("sentWithFix", String (gps.sentencesWithFix()));
        GPS("charsProcessed", String (gps.charsProcessed()));
        BMP("Temp", String (bmp.readTemperature()));
        BMP("Atm",String (bmp.readPressure() / 101325));
        BMP("Alt",String (bmp.readAltitude(1013.25)));
        DHT1("humidity",String (dht.getHumidity()));
        DHT1("Temp",String(dht.getTemperature()));
        DHT1("heatInd",String (dht.computeHeatIndex(dht.getTemperature(), dht.getHumidity(), false)));
        payload("GPS", GPS.json());
        payload("BMP",BMP.json());
        payload("DHT11",DHT1.json());
        
        
        smartDelay(dht.getMinimumSamplingPeriod());
        
        Serial.println(GPS.json());
Serial.println(BMP.json());
Serial.println(DHT1.json());
        // LCD Print the Data
        sSatAtl = "Sat:" + GPS["sats"] + " Alt:" + GPS["alt"];
        sSpeed = "Speed:" + GPS["speed"];
        sLat = "Lat:" + GPS["lati"];
        sLng = "Lng:" + GPS["longi"];
        sDate = "Date: " + GPS["date"];
        sTime = "Time: " + GPS["time"];
        sAge = "Age: " + GPS["age"];
        sHDOP = "HDOP: " + GPS["hdop"];
        sFCS = "FCS: " + GPS["failedCheckSum"];
        sCP = "CP: " + GPS["charsProcessed"];
        sSwFIX = "SwFIX: " + GPS["sentWithFix"];
        sPCS = "PCS: " + GPS["passedCheckSum"];
        
        sBAlt ="Alt2: " + BMP["Alt"];
        sBTemp = "Temp1: " + BMP["Temp"];
        sBAtm = "ATM: " + BMP["Atm"];
        
        sDHum = "Humi: " + DHT1["humidity"];
        sDTemp = "Temp2: " + DHT1["Temp"];
        sDHInd = "H.Index: " + DHT1["heatInd"];
//      {"sats":"5","lati":"26.76236017","longi":"88.38688600","age":"1","date":"19/3/2021","time":"14:15:27","hdop":"208","speed":"1.78","alt":"145.80","failedCheckSum":"121","passedCheckSum":"1780","sentWithFix":"473","charsProcessed":"121169"}
//      {"Temp":"29.48","Atm":"0.98","Alt":"172.62"}
//      {"humidity":"54.00","Temp":"29.90","heatInd":"31.55"}

        switch (dataView) {
          case 0:
            lcd.clear();
            //        |sats:__ Alt:____|
            //        |   speed:____   |
            lcd.setCursor(centerDo(sSatAtl, 16), 0);
            lcd.print(sSatAtl);
            lcd.setCursor(centerDo(sSpeed, 16), 1);
            lcd.print(sSpeed);
            break;
          case 1:
            lcd.clear();
            //        |lat:____________|
            //        |lng:____________|
            lcd.setCursor(centerDo(sLat, 16), 0);
            lcd.print(sLat);
            lcd.setCursor(centerDo(sLng, 16), 1);
            lcd.print(sLng);
            break;
          case 2:
            lcd.clear();
            //        |  Date: ______  |
            //        | Time: ________ |
            lcd.setCursor(centerDo(sDate, 16), 0);
            lcd.print(sDate);
            lcd.setCursor(centerDo(sTime, 16), 1);
            lcd.print(sTime);
            break;
          case 3:
            lcd.clear();
            //        | Age: _________ |
            //        |   HDOP:_____   |
            lcd.setCursor(centerDo(sAge, 16), 0);
            lcd.print(sAge);
            lcd.setCursor(centerDo(sHDOP, 16), 1);
            lcd.print(sHDOP);
            break;
          case 4:
            lcd.clear();
            //        |FCS:___________ |
            //        |CP:____________ |
            lcd.setCursor(centerDo(sFCS, 16), 0);
            lcd.print(sFCS);
            lcd.setCursor(centerDo(sCP, 16), 1);
            lcd.print(sCP);
            break;
          case 5:
            lcd.clear();
            //        | SwFIX: _______ |
            //        | PCS: _________ |
            lcd.setCursor(centerDo(sSwFIX, 16), 0);
            lcd.print(sSwFIX);
            lcd.setCursor(centerDo(sPCS, 16), 1);
            lcd.print(sPCS);
            break;
//             case 6:
//            lcd.clear();
//            //        |Alt2:____ Atm:__|
//            //        | PCS: _________ |
//            lcd.setCursor(centerDo(sSwFIX, 16), 0);
//            lcd.print(sSwFIX);
//            lcd.setCursor(centerDo(sPCS, 16), 1);
//            lcd.print(sPCS);
//            break;
          default:
            dataView = 0;
            break;
        }

        
                digitalWrite(LED_BUILTIN, LOW );
                String serverPath = serverName + "gps";
                http.begin(serverPath.c_str());
                http.addHeader("Content-Type", "application/json");
                int httpResponseCode = http.POST(payload.json());
                Serial.println(httpResponseCode);
                http.end();
                digitalWrite(LED_BUILTIN, HIGH );


      }
  }
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
int centerDo(String data, int screenLength) {
  int screenMid = screenLength / 2;
  int dataMid = data.length() / 2;
  int spacing = screenMid - dataMid;

  return spacing;

}
