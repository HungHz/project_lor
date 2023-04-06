#include <DHT.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include "ThingSpeak.h"
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

//wifi
const char* ssid     = "Tang2";
const char* password = "567891011";
//tele
#define BOTtoken "5789561318:AAFIodbmY1OiPhktSnr7bv8LcBI8Dv1rcV4"  // your Bot Token (Get from Botfather)
#define CHAT_ID "1497151151"
WiFiClientSecure client1;
WiFiClient  client;
UniversalTelegramBot bot(BOTtoken, client1);

//thinkspeak
#define SECRET_CH_ID 1904636      // replace 0000000 with your channel number
#define SECRET_WRITE_APIKEY "VNZG4WY9WW9G0QOU" 
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

//config firebase
FirebaseData firebaseData;
FirebaseJson json;

#define fb_config "mypro-593ab-default-rtdb.asia-southeast1.firebasedatabase.app"
#define fb_auth "uP4pZmNReaIa2OIVeSZARijgkFBEGY5kjDL6ChVz"

// pin define
#define dhtPin 26
#define mqPin 39
#define ledPower 13
#define dustPin 36


#define DHTTYPE DHT11
DHT dht(dhtPin, DHTTYPE);


//khai bao bien

volatile unsigned long sendTime = 0; //thoi gian send notification
unsigned long sysTime = 0;  //thoi gian delay he thong do luong
unsigned long botTime = 0; //thoi gian quet tin nhan
volatile int count, notiTime = 1;
volatile float pmValue, coValue, totalPm, totalCo, pmAvg, coAvg, aqi,e=2.71828;
String myStatus = "";
String myStatus2 = "";

//khai bao ham
void initWiFi(); // ket noi wifi
void measureDht();
void measurePm2_5();
void measureCo();
void calAqi();

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(ledPower,OUTPUT);
  pinMode(dustPin,INPUT);
  analogReadResolution(10);

  initWiFi();
  client1.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  ThingSpeak.begin(client);
  Firebase.begin(fb_config,fb_auth);
  Firebase.reconnectWiFi(true);

  delay(100);
}

void loop() {
  if((millis()- botTime) > 2000)
  {
    
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
  if((millis()- sendTime) >= notiTime*60000){
      sendIndex();
      sendTime = millis();
    }
  if((millis() - sysTime) > 3000)
  {
      measureDht();
      measurePm2_5();
      measureCo();

      count ++;
      if (count >= 3){
        calAqi();
            ThingSpeak.setField(1,aqi);
             ThingSpeak.setStatus(myStatus2);
             int x2 = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
            if(x2 == 200){
              Serial.println("Channel update successful2.");
            }
            else{
              Serial.println("Problem updating channel. HTTP error code2 " + String(x2));
            }
        totalCo = 0;
        totalPm = 0;
        count = 0;
        }
      ThingSpeak.setField(2,pmValue);
      ThingSpeak.setField(3,coValue);
      ThingSpeak.setStatus(myStatus);
   int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  // if(x == 200){
  //   Serial.println("Channel update successful-1.");
  // }
  // else{
  //   Serial.println("Problem updating channel. HTTP error code-1 " + String(x));
  // }
    sysTime = millis();
  }
    botTime = millis();  
  }

}

///////////////
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void measureDht(){
  int temp = dht.readTemperature();
  int humi = dht.readHumidity();
    Serial.print(temp);
    Serial.println(" °C");
    Serial.print(humi);
    Serial.println(" %");
    Firebase.setInt(firebaseData,"/dht/temp",temp);
    Firebase.setInt(firebaseData,"/dht/humi",humi);   
  }

void measurePm2_5(){
  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(280);
  int dustVal=analogRead(dustPin)+85; // read the dust value//102
  delayMicroseconds(40);

  
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(9680);
  
  float voltage = dustVal*0.00489;//dustval*5/1024
  pmValue = (0.172*voltage-0.1)*1000.00;
  
  if (pmValue < 0 )
  pmValue = 0;
  if (pmValue > 500)
  pmValue = 500;
  
  Serial.print(pmValue);
  Serial.println(" ug/m3");
  totalPm +=pmValue;
  delay(10);
  Firebase.setFloat(firebaseData,"/air/pm",pmValue);
}
  
void measureCo(){
//  coValue = mq7.getPPM();
  float sensorValue = analogRead(mqPin);
  float sensor_volt = sensorValue/1024*5.0;
  coValue = 3.027*pow(e,(1.0698*sensor_volt));
  Serial.print(coValue);
  Serial.println(" ppm");
  totalCo += coValue;
  delay(10);
  Firebase.setFloat(firebaseData,"/air/co",coValue);
}
  
void calAqi(){
  pmAvg = totalPm/count;
  coAvg = totalCo/count;
  float pmAqi = pmAvg*2.103004+25.553648;
  float coAqi = coAvg*16.89655-59.51724;
  delay(10);
  if(pmAqi > coAqi) aqi = pmAqi;
  else aqi = coAqi;
  Firebase.setFloat(firebaseData,"/air/aqi",aqi);
}

void handleNewMessages(int numNewMessages){
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

     if (text == "/start")
    {
      String welcome = "Welcome to Telegram Bot.\n";
      welcome += "/number : to set up timer to send nitification \n";
      welcome += "/state : Returns current air index \n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
    else if (text == "/state"){
      String mess;
      mess += "AQI index: ";
      mess.concat(aqi);
      mess += "\n";
      mess += "PM2.5 index: ";
      mess.concat(pmValue);
      mess += "\n";
      mess += "CO index: ";
      mess.concat(coValue);
      mess += "\n";
      mess += "Notifications are sent every : ";
      mess.concat(notiTime);
      mess += " min";
      mess += "\n";
      bot.sendMessage(CHAT_ID, mess, "");
    }
    else {
      text.remove(0,1);
      notiTime = text.toInt();
      bot.sendMessage(CHAT_ID,"OK. I got it!!! \n", "");
    }
  }
}
void sendIndex(){
  String mess;
  mess += "AQI index: ";
  mess.concat(aqi);
  mess += "\n";
  mess += "PM2.5 index: ";
  mess.concat(pmValue);
  mess += "\n";
  mess += "CO index: ";
  mess.concat(coValue);
  mess += "\n";
  bot.sendMessage(CHAT_ID, mess, "");
}
