# Stepper Motor Controlling Project with Arduino Mega

This project involves the control of stepper motors using an Arduino Mega board. The project includes features such as motor rotation triggered by a pedal button, homing of motors using switches, teaching positions with a button, and control using a knob encoder.

## Motors

The stepper motors in this project are denoted as M1, M2, and M3.

## Inputs Connection

1. **Pedal Button (D4):**
   - The pedal button on pin D4 triggers motor rotation by a specific number of steps, as directed by a PySerial script.

2. **Homing Switches:**
   - Home switch for M1: Connected to pin D40.
   - Home switch for M2: Connected to pin D42.
   - Home switch for M3: Connected to pin A11.
   - These switches are used for homing each of the three axes of the motors.

3. **Teaching Button (X+ Servo - Red Connector):**
   - The teaching button is connected to the signal pin of the X+ servo on the onboard.

4. **Knob Encoder (Z- Servo - Blue Connector):**
   - The CLK pin of the knob encoder is connected to the signal pin of the Z- servo (Blue connector).

5. **Motor Toggling Button (Z+ Servo - Blue Connector):**
   - The motor toggling button is connected to the signal pin of the Z+ servo (Blue connector).

6. **Pedal Button to Run Motors to Taught Position (D6):**
   - A pedal button is connected to pin D6 and is used to run the motors to the previously taught position.

7. **Homing Pin (D5):**
   - The homing pin is connected to pin D5 and is used to perform the homing procedure for stepper motors.

8. **Encoder Direction (DT) Pin (D11):**
   - The DT pin of the encoder is connected to pin D11.

## Usage

1. Connect the Arduino Mega board to the specified pins according to the project description.

2. Upload the Arduino sketch to the board.

3. Execute the PySerial script to control the stepper motors based on the pedal button input.

4. Use the homing pin to perform the homing procedure for stepper motors.

5. Utilize the teaching button to store positions to variables.

6. Control the motors using the knob encoder and motor toggling button.

7. Run the motors to the previously taught position using the pedal button connected to D6.

## Author

Yash Hingu

