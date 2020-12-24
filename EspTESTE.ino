   //#include <SoftwareSerial.h>
   //use mega Serial 2 for serial monitor; Serial 1 on pins 19 (RX) and 18 (TX);// Serial2 on pins 17 (RX) and 16 (TX), Serial3 on pins 15 (RX) and 14 (TX).
   #define SSID "user"
   #define PASS "password"
   #define DST_IP "0.0.0.0" //baidu.com
   //SoftwareSerial dbgSerial(10, 11); // RX, TX
   void setup()
   {
     // Open serial communications and wait for port to open:
     //serial 2 is to esp8266 
     Serial1.begin(9600);//9600 (mine), 57600, 115200
     Serial1.setTimeout(1000);
    
     //serial 0 is to usb
     Serial.begin(115200);
     
          
     while(!Serial); 
     while(!Serial1);
     
     Serial.println("ESP8266 test on Mega2560");
    
    while(Serial1.available()>0)
    Serial1.read();
      
     delay(1000);
       //test if the module is ready
     Serial1.println("AT+RST");
     //delay(1000);
     //delay(1000);
     Serial.println("Resetting module");
     Serial1.flush();
     
     //if(Serial2.find("ready"))
     if(Serial1.find("Ready")||Serial1.find("ready"))
     {

       Serial.println("Module is ready");
     }
     else
     {

       Serial.println("Module have no response.");
       while(1);
     }
     delay(1000);
     //connect to the wifi
     boolean connected=false;
     for(int i=0;i<5;i++)
     {
       if(connectWiFi())
       {
         connected = true;
         break;
       }
     }
     if (!connected){while(1);}
     delay(5000);
     //print the ip addr
     /*
   Serial2.println("AT+CIFSR");
     Serial.println("ip address:");
     while (Serial2.available())
     Serial.write(Serial2.read());
   
   */
     //set the single connection mode
     Serial1.println("AT+CIPMUX=0");
   }
   void loop()
   {
     String cmd = "AT+CIPSTART=\"TCP\",\"";
     cmd += DST_IP;
     cmd += "\",80";
     Serial1.println(cmd);
     Serial.println(cmd);
     if(Serial1.find("Error")) return;
     cmd = "GET / HTTP/1.0\r\n\r\n";
     Serial1.print("AT+CIPSEND=");
     Serial1.println(cmd.length());
     if(Serial1.find(">"))
     {
       Serial.print(">");
       }else
       {
         Serial1.println("AT+CIPCLOSE");
         Serial.println("connect timeout");
         delay(1000);
         return;
       }
       Serial1.print(cmd);
       delay(2000);
       //Serial.find("+IPD");
       while (Serial1.available())
       {
         char c = Serial1.read();
         Serial.write(c);
         if(c=='\r') Serial.print('\n');
       }
       Serial.println("====");
       delay(1000);
     }
     boolean connectWiFi()
     {
       Serial1.println("AT+CWMODE=1");
       String cmd="AT+CWJAP=\"";
       cmd+=SSID;
       cmd+="\",\"";
       cmd+=PASS;
       cmd+="\"";
       Serial1.println(cmd);
       Serial.println(cmd);
       delay(2000);
       if(Serial1.find("OK"))
       {
         Serial.println("OK, Connected to WiFi.");
         return true;
         }else
         {
           Serial.println("Can not connect to the WiFi.");
           return false;
         }
       }
