#include <SoftwareSerial.h>
#define RX 2
#define TX 3
#define echoPin 4                                  
#define trigPin 5                                  
long duration;                                   
int distance;                                    
int sensorPin = A0;                                
int sensorValue = 0;                              

String AP = "WIFI_NAME";                          
String PASS = "PASSWORD";                          
String API = "BZR9FU3PUIKOPWZZ";                 
String HOST = "api.thingspeak.com";
String PORT = "80";
String field1 = "field1";
String field2 = "field2";

int countTrueCommand;
int countTimeCommand; 
boolean found = false;
String values1;
String values2;
SoftwareSerial esp8266(RX,TX); 
  
void setup() {
  pinMode(trigPin, OUTPUT);                        
  pinMode(echoPin, INPUT);                         
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}

void loop() 
{
 values1 = getSensor1Data();                                    
 values2= getSensor2Data() ;                                    
 
 String getData = "GET /update?api_key="+ API +"&"+ field1 +"="+String(values1)+"&"+ field2 +"="+ String(values2);
 
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
}

String getSensor1Data(){
 sensorValue = analogRead(sensorPin);                           
Serial.println(sensorValue);                                    

delay (10);
return String(sensorValue);                                     
}
String getSensor2Data(){
 
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
 
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
 
  // Calculating the distance
  distance = duration * 0.034 / 2; 
 
  // Displays the distance on the Serial Monitor
  distance = 12-distance;// height of container used is 12cm

  return String(distance);// ultrasonic data
}


void sendCommand(String command, int maxTime, char readReplay[]) {
 
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
 
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
