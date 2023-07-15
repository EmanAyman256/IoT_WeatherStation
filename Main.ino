#include <ESPAsyncWebServer.h>
#include <ESP_Mail_Client.h>
#include <ESP8266WiFi.h>
#include "DHT.h"
#include <SFE_BMP180.h>
#include <Wire.h>
#define RainAnalog A0
#define ALTITUDE 1655.0
#define DHTPIN 0     
#define DHTTYPE DHT11   // DHT 11
#define AUTHOR_EMAIL "TeamIot23@gmail.com " /*change it to your email */
#define AUTHOR_PASSWORD "yxkkezgsozrzkqcs"  /*change it to your password*/
#define RecipientName "eman"
const char* ssid     = "Eman"; // Your ssid
const char* password = "Eman2077"; // Your Password
SFE_BMP180 pressure;
DHT dht(DHTPIN, DHTTYPE);
bool SendMail(String msg);
void smtpCallback(SMTP_Status status);
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

bool emailSent = true;
float tempthresh=20.0;
float HumThresh=45.0;
float Pressthresh=20.0;
float Altthresh=500.0;
float Rainthresh=30.0;

SMTPSession smtp;

double T,P,p0,alt;
double phg, pmb;
WiFiServer server(80);
void setup() {
Serial.begin(115200);
delay(100);
dht.begin();
Serial.print("Connecting to :: ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) 
{
delay(500);
}
server.begin();
Serial.println(WiFi.localIP());
 if (pressure.begin())
 Serial.println("BMP180 is Starting.."); 
 else
 {
 Serial.println("BMP180 FAILED.. \n\n");
 while(1); 
 }
}

void loop() 
{
float humidity = dht.readHumidity();
Serial.println("Humidity is :");
Serial.println(humidity);
// Read temperature as Celsius
float CelTemp = dht.readTemperature();
Serial.println("Tempreture is :");
Serial.println(CelTemp);
Serial.println(" C");     
int rain =analogRead(RainAnalog);
Serial.println(" Rain :");  
Serial.println(rain);     
   

char s = pressure.getPressure(P,T);
   if (s != 0) {
p0 = pressure.sealevel(P,ALTITUDE); 
Serial.print(" Pressure is : ");
Serial.print(p0,2);
Serial.print(" mb, ");
Serial.print(p0*0.0295333727,2);
Serial.println(" inHg");

alt = pressure.altitude(P,p0);
Serial.print(" Altitude is : ");
Serial.print(alt,0);
Serial.print(" meters, ");
Serial.print(alt*3.28084,0);
Serial.println(" Feet");
  }
   float Temp=CelTemp;
   float Pres=p0*0.0295333727;   
   float Altit=alt*3.28084;
  if(Temp > tempthresh && emailSent==true) {
  String emailMessage = String("Temperature Above Threshold And Current Temperature is : ") + String(Temp) +String("C");
      if (SendMail(emailMessage)) 
      {
              delay(10000);
      }   
 }
    if (Temp < tempthresh && emailSent==true) {
      String emailMessage = String("Temperature Below Threshold And Current Temperature is : ") + String(Temp) + String(" C");
      if (SendMail(emailMessage)) 
      {
              delay(10000);
      }
   }
  
  if(humidity > HumThresh && emailSent==true){
  String emailMessage = String("Humidity Above Threshold And Current Humidity is : ") + String(humidity) +String("%");
      if (SendMail(emailMessage)) 
      {
              delay(10000);
      }
 }
 
  if(humidity < HumThresh && emailSent==true) {
  String emailMessage = String("Humidity Below Threshold And Current Humidity is : ") + String(humidity) +String("%");
      if (SendMail(emailMessage)) 
      {
              delay(10000);
      }
 }
   if(Pres > Pressthresh && emailSent==true){
  String emailMessage = String("Pressure Above Threshold And Current Pressure is: ") + String(Pres) +String("Hg");
      if (SendMail(emailMessage)) 
      {
              delay(10000);
      }
 }
   if(Pres < Pressthresh && emailSent==true){
   String emailMessage = String("Pressure Above Threshold And Current Pressure is: ") + String(Pres) +String("Hg");
      if (SendMail(emailMessage)) 
      {
              delay(10000);
      }
 }
  if(Altit > Altthresh && emailSent==true){
   String emailMessage = String("Altitude Above Threshold And Current Altitude is: ") + String(Altit) +String("Hg");
      if (SendMail(emailMessage)) 
      {
              delay(10000);
      }
 }
  if(Altit < Altthresh && emailSent==true){
   String emailMessage = String("Altitude Below Threshold And Current Altitude is : ") + String(Altit) +String("Hg");
      if (SendMail(emailMessage)) 
      {
              delay(10000);
      }
 }

  if(rain > Rainthresh && emailSent==true){
   String emailMessage = String("Rain Percentage Above Threshold And Current Rain Percentage is : ") + String(rain) +String("%");
      if (SendMail(emailMessage)) 
      {
              delay(10000);
      }
 }
  if(rain < Rainthresh && emailSent==true){
   String emailMessage = String("Rain Percentage Below Threshold And Current Rain Percentage is : ") + String(rain) +String("%");
      if (SendMail(emailMessage)) 
      {
              delay(10000);
      }
 }
 delay(20000);
     
WiFiClient client = server.available();
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("Connection: close");  // the connection will be closed after completion of the response  
client.println();

            //WEB PAGE HTML And CSS 

client.println("<!DOCTYPE html> <html>");
client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
client.println("<style> *{ margin: 0; padding: 0; box-sizing: border-box;}");
client.println(" body{font-family: 'montserrat', sans-serif;background-image:url('https://images.pexels.com/photos/391522/pexels-photo-391522.jpeg');background-size: cover;}");
client.println(".webpage{ background-image: url('https://i.ibb.co/fHx4Dn5/BGBlurred2.jpg');  background-size: cover; background-position: top center;height:550px;background-repeat: no-repeat; padding: 10px; margin: auto; text-align: left;}");
client.println("h1 {  color: #fff;  font-size: 55px;  text-align: center;  padding: 90px;  font-weight: 900;margin: 30px 0px;  text-shadow: 2px 12px rgba(0, 0, 0, 0.6);}");
client.println(".side_adjust{  display: inline-block;  vertical-align: middle;  position: relative; padding: 5px; margin: 5px;}");
client.println(".text1{  padding-left: 10px;   font-size: 40px;   width: 190px; text-align: center; color: #ffffff; font-weight: 700; font-style: italic; margin-bottom: 15px; text-shadow: 0px 3px rgba(0, 0, 0, 0.4);}");
client.println(".data1{  font-weight: 140;  padding-left: 25px;   font-size: 50px; color: #ffffff;font-style: italic;   margin-bottom: 15px;   text-shadow: 0px 3px rgba(0, 0, 0, 0.4);}");
client.println(".text2{  padding-left: 10px;font-size: 40px; width: 190px; text-align: center;   color: #ffffff;  font-weight: 700;  font-style: italic;margin-bottom: 15px; text-shadow: 0px 3px rgba(0, 0, 0, 0.4);}");
client.println(".data2{font-weight: 140;padding-left: 100px;font-size: 50px; color: #ffffff; font-style: italic; margin-bottom: 15px; text-shadow: 0px 3px rgba(0, 0, 0, 0.4); }");
client.println(".text3{  padding-left: 0px;   font-size: 40px;width: 190px;text-align: center; color: #ffffff;font-weight: 700;font-style: italic;margin-bottom: 15px;text-shadow: 0px 3px rgba(0, 0, 0, 0.4);}");
client.println(".data3{  font-weight: 140;  padding-left: 25px;   font-size: 50px;   color: #ffffff;  font-style: italic; margin-bottom: 15px; text-shadow: 0px 3px rgba(0, 0, 0, 0.4);}");
client.println(".text4{ padding-left: 0px;   font-size: 40px;    width: 190px;margin: 0px;  text-align: center;  color: #ffffff; font-weight: 700; font-style: italic;margin-bottom: 15px;text-shadow: 0px 3px rgba(0, 0, 0, 0.4);}");
client.println(".data4{ font-weight: 140; padding-left: 25px;  font-size: 50px; font-weight: 700; font-style: italic; margin-bottom: 15px; text-shadow: 0px 3px rgba(0, 0, 0, 0.4);color: #ffffff;}");
client.println(".text5{ padding-left: 0px; font-size: 40px; width: 190px;margin: 0px;text-align: center; color: #ffffff;font-weight: 700;font-style: italic; margin-bottom: 15px;text-shadow: 0px 3px rgba(0, 0, 0, 0.4);}");
client.println(".data5{  font-weight: 140;  padding-left: 20px;   font-size: 50px;  font-weight: 700;  font-style: italic;  margin-bottom: 20px;  text-shadow: 0px 3px rgba(0, 0, 0, 0.4);color: #ffffff;}");
client.println(".data{padding: 10px;}");
client.println("</style>");
client.println("</head>");
client.println("<body>");
client.println("<h1> Weather Station </h1>");
client.println("<div class=\"webpage\">");   
client.println("<div class=\"data\">");
client.println("<div class=\"side_adjust text1\">Humidity:</div>");
client.println("<div class=\"side_adjust data1\">");
client.print(humidity);
client.println("<div class=\"side_adjust text1\">%</div>");
client.println("</div>");
client.println("<div class=\"data\">");
client.println("<div class=\"side_adjust text2\">Temperature:</div>");
client.println("<div class=\"side_adjust data2\">");
client.print(CelTemp);
client.println("<div class=\"side_adjust text2\">&deg;C</div>");
client.println("</div>");
client.println("<div class=\"data\">");
client.println("<div class=\"side_adjust text3\">Pressure:</div>");
client.println("<div class=\"side_adjust data3\">");
client.print(p0*0.0295333727,2);
client.println("<div class=\"side_adjust text3\">Hg</div>");
client.println("</div>");
client.println("<div class=\"data\">");
client.println("<div class=\"side_adjust text4\">Altitude:</div>");
client.println("<div class=\"side_adjust data4\">");
client.print(alt*3.28084,0);
client.println("<div class=\"side_adjust text3\">Feet</div>");
client.println("</div>");
client.println("<div class=\"data\">");
client.println("<div class=\"side_adjust text5\">Rain :</div>");
client.println("<div class=\"side_adjust data5\">");
client.print(rain);
client.println("<div class=\"side_adjust text5\">%</div>");
client.println("</div>");
client.println("</div>");
client.println("</div>");
client.println("</div>");
client.println("</div>");
client.println("</body>");
client.println("</html>");
delay(4000);
}
bool SendMail(String msg) {
  /** Enable the debug via Serial port
   * none debug or 0
   * basic debug or 1
  */
 // smtp.debug(1);
  smtp.callback(smtpCallback);
  /* Declare the session config data */
  ESP_Mail_Session session;
  /* Set the session config */
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  //session.login.user_domain = "";
  /* Declare the message class */
  SMTP_Message message;
/* Set the message headers */
  message.sender.name = "ESP8266";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = " ESP8266 NodeMCU Email ALERT !! ";
  message.addRecipient("Eman", "emanayman256@gmail.com");
 //Send raw text message
  String textMsg = msg;
  message.text.content = textMsg.c_str();
  message.text.charSet = "us-ascii";
 /* Connect to server with the session config */
  if (!smtp.connect(&session))
    return false;
  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message)) {
    //Serial.println("Error sending Email, " + smtp.errorReason());
    return false;
  } else {
   // Serial.println("Email Sent Successfullly");
    return true;
  }
}
/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status) {
  /* Print the current status */
  //Serial.println(status.info());

  /* Print the sending result */
  if (status.success()) {
    //Serial.println("----------------");
   // ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
   // ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    //Serial.println("----------------\n");
    struct tm dt;
  }
}
