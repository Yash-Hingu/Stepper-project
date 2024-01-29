//  Created by Yash Hingu
//  https://github.com/Yash-Hingu/Stepper-project
//  yashhingu32@gmail.com

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
#define homing_pin 5
#define run_teach_pedal_btn 6
#define ENCODER_DT_PIN 11
#define enc_steps 222
#define homing_timeout 60000
#define homing_step 50

#define Interrupt_pin_1 18 //CLK
#define Interrupt_pin_2 19 //Encode switch (toggle motor)
#define Interrupt_pin_3 2 //Teaching button

#define home_sens_m1 40
#define home_sens_m2 42
#define home_sens_m3 44

#define motor1_speed 4000
#define motor2_speed 4000
#define motor3_speed 4000


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
bool teach_pedal_state;

volatile int encoderPos = 0;
volatile boolean rotating = false;
int axisToggle = 0;

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
  stepper1.setAcceleration(8000);    // Set the acceleration in steps per second^2
  stepper1.setSpeed(motor1_speed); // 24 * 16 = 384
                                   // Set the target position (3600 steps = 360 degrees)
  stepper2.setMaxSpeed(16000);      // Set the maximum speed in steps per second
  stepper2.setAcceleration(8000);    // Set the acceleration in steps per second^2
  stepper2.setSpeed(motor2_speed); // 24 * 16 = 384
  
  stepper3.setMaxSpeed(16000);      // Set the maximum speed in steps per second
  stepper3.setAcceleration(8000);    // Set the acceleration in steps per second^2
  stepper3.setSpeed(motor3_speed); // 24 * 16 = 384
  
  multiStepper.addStepper(stepper1);
  multiStepper.addStepper(stepper2);
  multiStepper.addStepper(stepper3);

  pinMode(home_sens_m1,INPUT_PULLUP);
  pinMode(home_sens_m2,INPUT_PULLUP);
  pinMode(home_sens_m3,INPUT_PULLUP);
  pinMode(homing_pin,INPUT_PULLUP);
  
  pinMode(pedal_btn, INPUT_PULLUP);
  pinMode(run_teach_pedal_btn, INPUT_PULLUP);
  pinMode(ENCODER_DT_PIN, INPUT_PULLUP);
  
  pinMode(Interrupt_pin_1, INPUT_PULLUP); // CLK_PIN
  attachInterrupt(digitalPinToInterrupt(Interrupt_pin_1), Interrupt_1, RISING);
  pinMode(Interrupt_pin_2, INPUT_PULLUP); // SW_PIN
  attachInterrupt(digitalPinToInterrupt(Interrupt_pin_2), Interrupt_2, FALLING);
  pinMode(Interrupt_pin_3, INPUT_PULLUP); // Teaching Pin
  attachInterrupt(digitalPinToInterrupt(Interrupt_pin_3), Interrupt_3, FALLING);

}

long positions[3];
long teached_positions[3];
bool knob_turn = false;
bool homing_state = false;
void loop() {

pedal_state = digitalRead(pedal_btn);
teach_pedal_state = digitalRead(run_teach_pedal_btn);
homing_state = !digitalRead(homing_pin);
if (homing_state){
  homing_stepper();
  }
 if (knob_turn){
    multiStepper.moveTo(positions);
    multiStepper.runSpeedToPosition();
    knob_turn = false;
}
  if (Serial.available() > 0 ) {

     String incomingData = Serial.readStringUntil('\n');
     Serial.println("Sending data: " + incomingData);
        if(!pedal_state) json_data(incomingData);
        else if (!teach_pedal_state) {
                    stepperRun(teached_positions[0],teached_positions[1],teached_positions[2]);
        }

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

bool m1_reached;
bool m2_reached;
bool m3_reached;

void Interrupt_1() {
    rotating = true;
    Serial.println("---- ---- ----");
    Serial.print("Motor No.: ");
    Serial.println(axisToggle+1);
    Serial.print("position_1: ");
    Serial.println(positions[0]);
    Serial.print("position_2: ");
    Serial.println(positions[1]);
    Serial.print("position_3: ");
    Serial.println(positions[2]);
    
  
  // Read the state of DT pin to determine the direction
  if (digitalRead(ENCODER_DT_PIN) == HIGH) {

          positions[axisToggle] = positions[axisToggle] + enc_steps;
  
  } else {
    
    positions[axisToggle] = positions[axisToggle] - enc_steps;
  
  }
   
//   multiStepper.moveTo(positions);
//   multiStepper.runSpeedToPosition();
   knob_turn = true;

   

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
    teached_positions[0] = positions[0];
    teached_positions[1] = positions[1];
    teached_positions[2] = positions[2];
}

void homing_stepper(){
  unsigned long previousMillis = millis();
    m1_reached = !digitalRead(home_sens_m1);
    m2_reached = !digitalRead(home_sens_m2);
    m3_reached = !digitalRead(home_sens_m3);

  while (m1_reached==false || m2_reached==false || m3_reached==false){
    
    unsigned long currentMillis = millis();
    m1_reached = !digitalRead(home_sens_m1);
    m2_reached = !digitalRead(home_sens_m2);
    m3_reached = !digitalRead(home_sens_m3);
     
     if (currentMillis - previousMillis <= homing_timeout) {
            if (!m1_reached){
                  positions[0] = positions[0] - homing_step;
                    }
            if (!m2_reached){
                  positions[1] = positions[1] - homing_step;
                    }
            if (!m3_reached){
                  positions[2] = positions[2] - homing_step;
                    }

           multiStepper.moveTo(positions);
           multiStepper.runSpeedToPosition();
      
           if (m1_reached==true && m2_reached==true && m3_reached==true){
                  Serial.println("Home Position Set");
                  set_motor_zero();
                  break;
              }
      } else
              {
                Serial.println("Homing Failed : Taking too much time!!");
                break;
              }

  }
  Serial.println("Homing over!!");
}
