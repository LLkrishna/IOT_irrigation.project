//https://api.openweathermap.org/data/2.5/find?q=vijayawada&units=imperial&type=accurate&mode=json&APPID=8a131d11dbb800262a5e593494ee3110



#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>

#define BLYNK_TEMPLATE_ID "TMPL3dU4y12JY"
#define BLYNK_TEMPLATE_NAME "temp work"
char auth[] = "GClx2d0fpzsCtYVE-tLKbV7u_TWddKIl";

#define VIRTUAL_PIN_TEMPERATURE V0;
BlynkTimer timer;
byte switchStatus;

BLYNK_WRITE(V0) {
   switchStatus = param.asInt();
}
float pressure ;
  float Speed ;
  int degree ;
   String result;
   String temp = "";
   bool b = false;

const char* ssid     = "TAP ON WIFI";                 // SSID of local network
const char* password = "QWERTYUIOP@";                    // Password on network
String APIKEY = "..............................";
String CityID = "1255647";          //Your City ID
WiFiClient client;
char servername[] = "api.openweathermap.org";            



void setup() {


   Serial.begin(9600);

  Serial.print("Connecting to ");
  WiFi.mode(WIFI_STA);  
  WiFi.begin(ssid, password);
  Blynk.begin(auth, ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  timer.setInterval(500L, turnOffLed);

}
void loop()                    
{
  Blynk.run();
  timer.run();
   if (b){
   Blynk.virtualWrite(V1, temp);
   temp = "";
   b = false;
   }
}


 

void turnOffLed() {
 if(switchStatus == 1){
  
   
   Blynk.virtualWrite(V0, 0);
 
   switchStatus = 0;
if (client.connect(servername, 80))
  { 
    client.println("GET /data/2.5/weather?id=" + CityID + "&units=metric&APPID=" + APIKEY);
    client.println("Host: api.openweathermap.org");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
  }
  else {
    Serial.println("connection failed");        //error message if no client connect
    Serial.println();
  }

  while (client.connected() && !client.available())
    delay(1);                                          //waits for data
  while (client.connected() || client.available())
  { //connected or data available
    char c = client.read();                     //gets byte from ethernet buffer
    result = result + c;
  }

  client.stop();                                      //stop client
  result.replace('[', ' ');
  result.replace(']', ' ');
  
  char jsonArray [result.length() + 1];
  result.toCharArray(jsonArray, sizeof(jsonArray));
  jsonArray[result.length() + 1] = '\0';
  StaticJsonDocument<1024> doc;
  DeserializationError  error = deserializeJson(doc, jsonArray);


  if (error) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(error.c_str());
    return;
  }


   pressure = doc["main"]["pressure"];
   Speed = doc["wind"]["speed"];
   degree = doc["wind"]["deg"];
   String mode = doc["weather"]["description"];
  mode.replace(' ', '.');
   Serial.println();
  Serial.printf("Pressure: %.2f hpa\r\n", pressure);
  Serial.printf("Wind speed: %.1f m/s\r\n", Speed);
  Serial.printf("Wind degree: %d°\r\n", degree);
  Serial.printf("weather discription: ");
  Serial.println(mode);
  Blynk.virtualWrite(V2, pressure);
  Blynk.virtualWrite(V3, Speed);
  Blynk.virtualWrite(V4, degree);
   temp = mode;
  mode = "";
  result = "";
  b = true;
 }
 
}
