//mb_mishu

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

WiFiClient wifiClient;


#define HOST "cse360.bracu-duburi.com"

#define WIFI_SSID "PasaMota"
#define WIFI_PASSWORD "123456789@"

float val1 = 28;
float val2 = 80;
float val3 = 99;
float val4 = 0.6;
float val5 = 99;


String status_sound, pressure, dustDensity, temp, alt, postData;
char rdata; // received charactors

String myString = "";

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}

void loop() { // run over and over
  HTTPClient http;
  String x = "";
  if (Serial.available()) {

    rdata = Serial.read();
    //    Serial.write(rdata);
    myString = myString + rdata;
    if ( rdata == '\n')
    {
      String val1 = getValue(myString, ',', 0);
      String val2 = getValue(myString, ',', 1);
      String val3 = getValue(myString, ',', 2);

      // Convert  variables to string
      status_sound = String(val1);
      pressure = String(val2);
      dustDensity = String(val3);
      temp = String(val4);
      alt = String(val5);

      postData = "status_sound=" + status_sound + "&pressure=" + pressure +  "&dustDensity=" + dustDensity + "&temp=" + temp + "&alt=" + alt ;
      http.begin(wifiClient, "http://cse360.bracu-duburi.com/write.php");
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");            //Specify content-type header

      int httpCode = http.POST(postData);

      // if connection eatablished then do this
      if (httpCode == 200) {
        Serial.println("Values uploaded successfully.");
        Serial.println(httpCode);
        String webpage = http.getString();    // Get html webpage output and store it in a string
        Serial.println(webpage + "\n");
      }
      else {
        Serial.println(httpCode);
        Serial.println("Failed to upload values. \n");
        http.end();
        return;
      }
      myString = "";
    }
  }
}
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
