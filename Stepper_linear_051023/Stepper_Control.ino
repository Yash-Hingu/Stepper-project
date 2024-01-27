//  Created by Yash Hingu
// https://github.com/Yash-Hingu/Stepper-project

//  {'row': 112, 'motor1': 20.0, 'motor2': 20.0, 'motor3': 20.0}

#include <AccelStepper.h>
#include <ArduinoJson.h>
#include <MultiStepper.h>

#define STEP_PIN_M1 A0
#define DIR_PIN_M1 A1
#define EN_PIN_M1 38

#define STEP_PIN_M2 A6
#define DIR_PIN_M2 A7
#define EN_PIN_M2 A2

#define STEP_PIN_M3 46
#define DIR_PIN_M3 48
#define EN_PIN_M3 A8

#define pedal_btn 4
#define disengage_btn 5
#define ENCODER_DT_PIN 6

#define Interrupt_pin_1 18
#define Interrupt_pin_2 19
#define Interrupt_pin_3 2

#define home_sens_m1 40
#define home_sens_m2 42
#define home_sens_m3 44

MultiStepper multiStepper;

// Define the capacity for the JSON object based on your data structure
const size_t capacity = JSON_OBJECT_SIZE(5); // Adjust based on your data structure

// Create a JSON object
DynamicJsonDocument jsonDoc(200);

// Create an instance of the AccelStepper class
AccelStepper stepper1(1, STEP_PIN_M1, DIR_PIN_M1);
AccelStepper stepper2(1, STEP_PIN_M2, DIR_PIN_M2);
AccelStepper stepper3(1, STEP_PIN_M3, DIR_PIN_M3);

String incomingData = "";
bool newData = false;
bool pedal_state;
bool disengage_state;

volatile int encoderPos = 0;
volatile boolean rotating = false;
int axisToggle = 1;

void setup() {
  Serial.begin(115200);
  
  pinMode(EN_PIN_M1, OUTPUT);
  digitalWrite(EN_PIN_M1, LOW);
  
  pinMode(EN_PIN_M2, OUTPUT);
  digitalWrite(EN_PIN_M2, LOW);
 
  pinMode(EN_PIN_M3, OUTPUT);
  digitalWrite(EN_PIN_M3, LOW);
  
  String incomingData = "";
  bool newData = false;
  
  stepper1.setMaxSpeed(16000);      // Set the maximum speed in steps per second
  stepper1.setAcceleration(16000);    // Set the acceleration in steps per second^2
  stepper1.setSpeed(16000); // 24 * 16 = 384
                                   // Set the target position (3600 steps = 360 degrees)
  stepper2.setMaxSpeed(2000);      // Set the maximum speed in steps per second
  stepper2.setAcceleration(1000);    // Set the acceleration in steps per second^2
  stepper2.setSpeed(1000); // 24 * 16 = 384
  
  stepper3.setMaxSpeed(2000);      // Set the maximum speed in steps per second
  stepper3.setAcceleration(1000);    // Set the acceleration in steps per second^2
  stepper3.setSpeed(1000); // 24 * 16 = 384
  
  multiStepper.addStepper(stepper1);
  multiStepper.addStepper(stepper2);
  multiStepper.addStepper(stepper3);
  
  pinMode(pedal_btn, INPUT_PULLUP);
  pinMode(disengage_btn, INPUT_PULLUP);
  pinMode(ENCODER_DT_PIN, INPUT);
  
  pinMode(Interrupt_pin_1, INPUT);
  attachInterrupt(digitalPinToInterrupt(Interrupt_pin_1), Interrupt_1, CHANGE);
  pinMode(Interrupt_pin_2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Interrupt_pin_2), Interrupt_2, FALLING);
  pinMode(Interrupt_pin_2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Interrupt_pin_3), Interrupt_3, RISING);

}

long positions[3];

void loop() {
  
pedal_state = digitalRead(pedal_btn);
disengage_state = digitalRead(disengage_btn);

stepper2.setSpeed(16000);
stepper2.setSpeed(16000);
stepper2.setSpeed(16000);


if (!disengage_state) {digitalWrite(EN_PIN_M1, HIGH);}
else {digitalWrite(EN_PIN_M1, LOW);}
 
  if (Serial.available() > 0 ) {

     String incomingData = Serial.readStringUntil('\n');
     Serial.println("Sending data: " + incomingData);
        if(!pedal_state) json_data(incomingData);

}
  
//  Serial.print(stepper1.distanceToGo());
//  Serial.print("  ");
//  Serial.print(stepper2.distanceToGo());
//  Serial.print("  ");
//  Serial.print(stepper3.distanceToGo());
//  Serial.println();

   
 
}

void stepperRun(long M1,long M2,long M3){
    Serial.print("Motor_1: ");
    Serial.print(M1);
    Serial.print(" Motor_2: ");
    Serial.print(M2);
    Serial.print(" Motor_3: ");
    Serial.println(M3);
    
    positions[0] = M1;
    positions[1] = M2;
    positions[2] = M3;
    multiStepper.moveTo(positions);
    multiStepper.runSpeedToPosition();
   
}

void set_motor_zero(){
  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);
  stepper3.setCurrentPosition(0);
  }
  
void json_data(String serialData){
      DeserializationError error = deserializeJson(jsonDoc, serialData);
      if (!error) {
      // Check if the parsed JSON has the expected fields
      if (jsonDoc.containsKey("row") && jsonDoc.containsKey("motor1") && jsonDoc.containsKey("motor2") && jsonDoc.containsKey("motor3")) {
        // Access and print individual JSON values
        int rowValue = jsonDoc["row"];
        long motor1Value = jsonDoc["motor1"];
        long motor2Value = jsonDoc["motor2"];
        long motor3Value = jsonDoc["motor3"];

        // Do something with the parsed values
        Serial.print("Parsed JSON Data - row: ");
        Serial.println(rowValue);
        Serial.print("motor1: ");
        Serial.println(motor1Value);
        Serial.print("motor2: ");
        Serial.println(motor2Value);
        Serial.print("motor3: ");
        Serial.println(motor3Value);
        if (stepper1.distanceToGo()== stepper2.distanceToGo()== stepper3.distanceToGo() == 0) {
          
               stepperRun(motor1Value,motor2Value,motor3Value);
              }
        else{
      //    stepper1.moveTo(5000);
      //    stepper2.moveTo(5000);
          }
      } else {
        Serial.println("Invalid JSON format or missing fields");
      }
    } else {
      Serial.println("Error parsing JSON");
    }
}

bool m1_reached = false;
bool m2_reached = false;
bool m3_reached = false;

void Interrupt_1() {
  rotating = true;
  
  // Read the state of DT pin to determine the direction
  if (digitalRead(ENCODER_DT_PIN) == HIGH) {
    positions[axisToggle]++;
  } else {
    positions[axisToggle]--;
  }
   
   multiStepper.moveTo(positions);
   multiStepper.runSpeedToPosition();
  // Uncomment the next line if you want to print the encoder position to the serial monitor
  // Serial.println(encoderPos);

  rotating = false;
  }
  
void Interrupt_2() {
delay(50);  // Debounce
  if (digitalRead(Interrupt_pin_2) == LOW) {
    if (!rotating) {
      // Toggle between axes
      axisToggle = (axisToggle == 0) ? 1 : (axisToggle == 1) ? 2 : 0;

      Serial.print("Toggled to Axis ");
      Serial.println(axisToggle);
    }
  }
}
void Interrupt_3() {
m3_reached = true;
}



void homing_stepper(){
  unsigned long previousMillis = millis();
  while (m1_reached!=true && m2_reached!=true && m3_reached!=true){
    
    unsigned long currentMillis = millis();
    m1_reached = !digitalRead(home_sens_m1);
    m2_reached = !digitalRead(home_sens_m2);
    m3_reached = !digitalRead(home_sens_m3);
     
     if (currentMillis - previousMillis >= 60000) {
            if (!m1_reached){
                  positions[0]--;
                    }
            if (!m2_reached){
                  positions[1]--;
                    }
            if (!m3_reached){
                  positions[2]--;
                    }
          
           multiStepper.moveTo(positions);
           multiStepper.runSpeedToPosition();
      
           if (m1_reached==true && m2_reached==true && m3_reached==true){
                  set_motor_zero();
              }
      } else
              {
                Serial.println("Homing Failed : Taking too much time");
              }

  }
}
