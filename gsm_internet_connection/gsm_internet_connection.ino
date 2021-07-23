/*
 * Making GET and POST requests to servers through API's
*/

#include <SoftwareSerial.h>

#define RX 9
#define TX 10

SoftwareSerial GSMSerial(RX, TX);

char* simAPN = "xxxxxxxxxxxxx";
String databuffer = "This is the data to be sent in body of request";

void setup() {
   GSMSerial.begin(9600);
   Serial.begin(9600);
   gsm_init();
   Serial.println("GSM configured....\n");
}

void loop() {
  if(Serial.available() == 0) {}
  switch(Serial.read()) {
    case 'g': getData();
              break;
    case 'p': postData();
  }
  if(GSMSerial.available() > 0){
    Serial.write(GSMSerial.read());
  }
}

void gsm_init() {
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

  Serial.println("Setting up internet connection");
  GSMSerial.println("AT+CIPSHUT");  // starts a fresh connection
  delay(1000);
  printSerialResponse();

  Serial.println("Attaching the GPRS");
  GSMSerial.println("AT+CGATT=1"); // attaching GPRS
  delay(200);
  printSerialResponse();

  Serial.println("Setting content type to GPRS");
  GSMSerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");//setting the SAPBR,connection type is GPRS
  delay(1000);
  printSerialResponse();

  Serial.println("Setting APN");
  GSMSerial.println("AT+SAPBR=3,1,\"APN\",\"" + String(simAPN) + "\"");
  delay(1000);
  printSerialResponse();

  Serial.println("Enabling GPRS and APN");
  GSMSerial.println("AT+SAPBR=1,1");
  delay(10000);
  printSerialResponse();
}

void printSerialResponse() {
  while(GSMSerial.available()!=0)
    Serial.write(GSMSerial.read());
}

void getData() {
  Serial.println("Initializing http request");
  GSMSerial.println("AT+HTTPINIT"); //init the HTTP request
  delay(2000);
  printSerialResponse();
               
  GSMSerial.println("AT+HTTPPARA=\"URL\",\"http:xxxxxxxxxxxxxxxxapi_urlxxxxxxxxxxxxxxxxxxxxxxxxxx\"");// setting the httppara,
  delay(1000);
  printSerialResponse();
               
  GSMSerial.println();
  Serial.println("Submitting get request");
  GSMSerial.println("AT+HTTPACTION=0");//submit the GET request
  delay(8000);//the delay is important if the return datas are very large, the time required longer.
  printSerialResponse();
              
  GSMSerial.println("AT+HTTPREAD=0,20");// read the data from the website you access
  delay(3000);
  printSerialResponse();
               
  GSMSerial.println("");
  delay(1000);
  Serial.println("Terminating http service");
  GSMSerial.println("AT+HTTPTERM");// terminate HTTP service
  printSerialResponse();
}

void postData() {
  Serial.println("Querying if the bearer is connected");
  GSMSerial.println("AT+SAPBR=2,1"); //init the HTTP request
  delay(2000);
  printSerialResponse();

  GSMSerial.println("AT+CMEE=2");   // Enable +CME ERROR: <err> result code               
  delay(1000);  
             
  Serial.println("Initializing http request");
  GSMSerial.println("AT+HTTPINIT"); //init the HTTP request
  delay(2000);
  printSerialResponse();

  GSMSerial.println("AT+HTTPPARA=\"CID\",1");// setting the httppara,
  delay(1000);
  printSerialResponse();

  GSMSerial.println("AT+HTTPPARA=\"URL\",\"http://xxxxxxxapi_urlxxxxxxxxxxxxxxxxxxx\"");  // setting the httpparameter,
  delay(1000);
  printSerialResponse();

//  if content to be sent is in json format
//  GSMSerial.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");  // setting the httpparameter,
//  delay(1000);
//  printSerialResponse();

  GSMSerial.println("AT+HTTPDATA=" + String(databuffer.length()) + ",100000");// setting the httppara,
  delay(2000);
  printSerialResponse();

  GSMSerial.println(databuffer);
  delay(8000);

  GSMSerial.println("AT+HTTPACTION=1");  //submit the POST request
  delay(8000);//the delay is important if the return datas are very large, the time required longer.
  printSerialResponse();
  GSMSerial.println("AT+HTTPREAD=0,20"); // reading server response. 20 -> Number of characters read from the server
  delay(3000);
  printSerialResponse();
               
  GSMSerial.println("");
  delay(1000);
  Serial.println("Terminating http service");
  GSMSerial.println("AT+HTTPTERM"); // terminate HTTP service
  printSerialResponse();
  delay(1000);
}
