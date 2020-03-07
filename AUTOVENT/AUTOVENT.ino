#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "ThingSpeak.h" //install library for thing speak
#include <WiFi.h>
#include <BME280I2C.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

int const tact1 = 16;
int const tact2 = 17;
int const tact3 = 35;

//Thing Speak neededd
char ssid[] = "PKIIEEE";        // your network SSID (name) 
char pass[] = "HACKUNO2020";   // your network password
int keyIndex = 0;           // your network key Index number (needed only for WEP)
WiFiClient  client;
unsigned long myChannelNumber = 985815;
const char * myWriteAPIKey = "77PYOKK03FXL15CW";
// Initialize our values
String myStatus = "";
float temp(NAN), hum(NAN), pres(NAN);

BME280I2C bme;


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void TempMenu(){
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("Temp Menu");
    display.setTextSize(1);
    display.println("\nConnecting to Wi-FI");
    display.display();
    delay(200);
    while(!bme.begin())
    {
      Serial.println("Could not find BME280 sensor!");
      delay(1000);
    }

    // bme.chipID(); // Deprecated. See chipModel().
    switch(bme.chipModel())
    {
      case BME280::ChipModel_BME280:
        Serial.println("Found BME280 sensor! Success.");
        break;
      case BME280::ChipModel_BMP280:
        Serial.println("Found BMP280 sensor! No Humidity available.");
        break;
      default:
        Serial.println("Found UNKNOWN sensor! Error!");
    }
    if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
    while(digitalRead(tact2)){
      BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
      BME280::PresUnit presUnit(BME280::PresUnit_Pa);

      bme.read(pres, temp, hum, tempUnit, presUnit);

      Serial.print("\nTemp: ");
      Serial.print(temp);
      Serial.print("°"+ String(tempUnit == BME280::TempUnit_Celsius ? 'C' :'F'));
      Serial.print("\t\tHumidity: ");
      Serial.print(hum);
      Serial.print("% RH");
      Serial.print("\t\tPressure: ");
      Serial.print(pres);
      Serial.println(" Pa");

      // set the fields with the values
      ThingSpeak.setField(1, temp);
      ThingSpeak.setField(2, hum);
      ThingSpeak.setField(3, pres);

      
      // set the status
      ThingSpeak.setStatus(myStatus);
      
      // write to the ThingSpeak channel
      int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
      if(x == 200){
        Serial.println("Channel update successful.");
      }
      else{
        Serial.println("Problem updating channel. HTTP error code " + String(x));
      }
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 10);
      display.println("Temp Menu");
      display.setTextSize(1);
      display.print("Temp :");
      display.println(temp);
      display.print("Hum  :");
      display.println(hum);
      display.print("Pres :");
      display.println(pres);
      display.display();
      delay(2500); // Wait 20 seconds to update the channel again
    }
    display.clearDisplay();
    display.display();
    delay(750); 
    return;
}

void sendHeart(){
    // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  // set the fields with the values
  ThingSpeak.setField(1, BPM);

  
  // set the status
  ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}
void setup() {
  Serial.begin(115200);  //Initialize serial
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  Wire.begin();
  
  pinMode(tact1, INPUT_PULLUP); 
  pinMode(tact2, INPUT_PULLUP); 
  pinMode(tact3, INPUT_PULLUP); 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(100); // Pause for 2 seconds
 
  // Clear the buffer.
  display.clearDisplay();
  
  // Draw bitmap on the screen
  display.drawBitmap(0, 15, Logo, 128, 31, 1);
  
  display.display();
  delay(2000);
}

void loop() {
  checkMenuTact();

}
