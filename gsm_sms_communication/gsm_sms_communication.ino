/*
 * Sending and recieving messages through GSM
*/

#include <SoftwareSerial.h>

#define RX 9
#define TX 10

SoftwareSerial GSMSerial(RX, TX);

String number="+91xxxxxxxxxx";
String msg="This is a GSM generated message";

void setup() {
     GSMSerial.begin(9600);
   Serial.begin(9600);
   gsm_init();
   Serial.println("GSM configured....\n");
}

void loop() {
  if(Serial.available() == 0) {}
  switch(Serial.read()) {
    case 's': GSMSerial.println("AT+CMGF=1");
              delay(1000);
              GSMSerial.println("AT+CMGS=\"+919420427695\"\r");
              delay(1000);
              GSMSerial.println("Hello, how are you?");
              delay(100);
              GSMSerial.println("This is a GSM generated message");
              delay(100);
              GSMSerial.println((char)26);
              delay(1000);
              break;
    case 'r': GSMSerial.println("AT+CNMI=2,2,0,0,0");
              delay(1000);
              break;
  }
  if(GSMSerial.available() > 0){
    Serial.write(GSMSerial.read());
  }
}

void  gsm_init() {
  Serial.println("Finding module");
  boolean at_flag = 1, echo_flag = 1, net_flag = 1;

  while(at_flag) {
    GSMSerial.println("AT");
    while(GSMSerial.available() > 0) {
      if(GSMSerial.find("OK")) {
        Serial.println("Module sucessfully found");
        at_flag = 0;
      }
    }

    delay(1000);
  }

  Serial.println("Module connected.....\n");
  Serial.println("Disabling echo");

  while(echo_flag) {
    GSMSerial.println("ATE1");
    while(GSMSerial.available() > 0) {
      if(GSMSerial.find("OK"))
        echo_flag = 0;
    }
    delay(1000);
  }

  Serial.println("Echo is off.....\n");
  Serial.println("Finding network");

  while(net_flag) {
    GSMSerial.println("AT+CPIN?");
    while(GSMSerial.available() > 0) {
      if(GSMSerial.find("+CPIN: READY"))
        net_flag = 0;
    }
    delay(1000);
  }

  Serial.println("Network found.....\n");
  delay(1000);
}
