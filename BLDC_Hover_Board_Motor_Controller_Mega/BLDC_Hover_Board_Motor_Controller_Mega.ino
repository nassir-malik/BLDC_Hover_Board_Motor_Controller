#include <ArduinoJson.h>
//#######################################
//#######################################
// GPIO mappings for Arduino Mega 2560
//#######################################
int m1_EL_Start_Stop=5;  //EL 
int m1_Signal_hall=3;   // Signal - Hall sensor
int m1_ZF_Direction=4;  // ZF 
int m1_VR_speed=2;    //VR 
//#######################################
//#######################################
int pos=0;int steps=0;int speed1=0;
String direction1;
//#######################################
//#######################################
void plus() {
  pos++; //count steps
  Serial.println(pos);
    if(pos>=steps){
    wheelStop();
    pos=0;
  }
}

void setup() {

// put your setup code here, to run once:
Serial.begin(115200);

//wheel 1 - Setup pin mode
pinMode(m1_EL_Start_Stop, OUTPUT);//stop/start - EL 
pinMode(m1_Signal_hall, INPUT);   //plus       - Signal  
pinMode(m1_ZF_Direction, OUTPUT); //direction  - ZF 
    
    //Hall sensor detection - Count steps
    attachInterrupt(digitalPinToInterrupt(m1_Signal_hall), plus, CHANGE);
}


void drive(){
// {"direction":"forward","steps":"30","speed":"50"}
// {"direction":"backword","steps":"30","speed":"50"}
// {"direction":"stop","steps":"0","speed":"0"}--
 
      if(direction1=="forward" && pos<steps){
        wheelMoveForward();
      }else if(direction1=="backword" && pos<steps){
        wheelMoveBackward();
      }else{
        Serial.println("Stop");
        wheelStop();
        pos=0;
      }        
 }


void wheelStop(){
  digitalWrite(m1_EL_Start_Stop,LOW);
}

void wheelMoveForward(){
      analogWrite(m1_VR_speed, speed1);
      digitalWrite(m1_EL_Start_Stop,LOW);
      delay(1000);
      digitalWrite(m1_ZF_Direction,HIGH);
      delay(1000);
      digitalWrite(m1_EL_Start_Stop,HIGH);
}

void wheelMoveBackward(){
      analogWrite(m1_VR_speed, speed1);
      digitalWrite(m1_EL_Start_Stop,LOW);
      delay(1000);
      digitalWrite(m1_ZF_Direction,LOW);
      delay(1000);
      digitalWrite(m1_EL_Start_Stop,HIGH);
}

void loop() {

  if (Serial.available()>0) {
    String command=Serial.readString();
    
      DynamicJsonBuffer jsonBuffer;
      JsonObject& root= jsonBuffer.parseObject(command);
      
       if (root.success()) {
          direction1 = root["direction"].asString();
          Serial.println(direction1);
          steps = atoi(root["steps"]);
          Serial.println(steps);
          speed1 = atoi(root["speed"]);
          Serial.println(speed1);
          drive();
       }
}


}










