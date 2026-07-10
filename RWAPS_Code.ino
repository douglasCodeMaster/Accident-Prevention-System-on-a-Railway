// Railway Accident Prevention System (RWAPS)
#include "gsm.h"
int count; // used to ensure SMS are sent only once 

uint8_t trigPin = 9; // uint8_t uses less memory space than int since the single numbers are just 1 byte using a data type that reserves 4 bytes for the value is abit overkill 
uint8_t echoPin = 8; 
long ultrasonicTime;
int distanceInCm;    // maximum distance 400cm

uint8_t redLed = 10;           // indicator LED for obstacle detected
uint8_t trainControl_pin = 6;  // activates and deactivates the train

void setup(){
      gsmSetUp();
      Serial.begin(9600); 
      pinMode(redLed, OUTPUT);
      pinMode(trainControl_pin, OUTPUT);
      
      // ultrasonic sensor setup
      pinMode(trigPin, OUTPUT); 
      pinMode(echoPin, INPUT); 
}

void loop(){
      gsmFunction();
      
      int sensorDistance = getDistanceInCm();
      Serial.print("\nDistance in cm: "); 
      Serial.println(sensorDistance); 

      if (sensorDistance < 20 and count != 8){
          digitalWrite(redLed, HIGH);
          digitalWrite(trainControl_pin, LOW); // train stopped
          Serial.println("Obstacle detected on railway!!!"); 
          count += 1;
          delay(300);
      }
      else if (count == 8){
          sendSMS(phoneNumber, "Obstacle detected on railway");
          count = 0;
      }
      else{
          digitalWrite(trainControl_pin, HIGH); // train moving
          digitalWrite(redLed, LOW);
      }
      delay(300); 
}

int getDistanceInCm(){
      digitalWrite(trigPin, LOW);  
      delayMicroseconds(2);        // sets Trig pin LOW for 2 microseconds
      
      digitalWrite(trigPin, HIGH); 
      delayMicroseconds(10);       // sets Trig pin HIGH for 10 microseconds
      
      digitalWrite(trigPin, LOW);  

      ultrasonicTime = pulseIn(echoPin, HIGH);   // function for storing pulse time 
      distanceInCm = ultrasonicTime * 0.034 / 2; // formular for calculating distance in centimeter(cm) // speed of sound in air -> 343m/s
                                                 // divided by 2 because its a (1)to and (2)fro movement of the 
                                                 // wave but we only need the "to" movemment once
      return distanceInCm;
}
