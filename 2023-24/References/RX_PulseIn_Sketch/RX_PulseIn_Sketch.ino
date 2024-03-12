/*
    @Author: Caleb W. Taylor
    Date: 12/25/2023
    Organization: Bengal-Bots at Louisiana State University

    The purpose of this sketch is to demonstrate how to receive and interpret pulse width 
    modulated (PWM) signals from devices that output such a signal using the "PulseIn" function within 
    the Arduino framework. 

    In our case (Bengal-Bots), we intend to receive these pulses from a RC receiver used in the past for 
    RC planes and drones to drive and control different functionalities on our 2023/2024 demonstration robot. 

    The link to the full official Arduino reference can be found here:
    https://www.arduino.cc/reference/en/language/functions/advanced-io/pulsein/
*/



/* 
    Each direction of the left and right joystick has its own dedicated output channel. 
    Here, we map each to its own dedicated digital input pin on the Arduino.
*/

#define rHorizontalPin 2 // RX Channel 1
#define rVerticalPin 3 // RX Channel 2
#define lHorizontalPin 5 // RX Channel 4
#define lVerticalPin 4 // RX Channel 3

// ---------------------------------------------------------

void setup() 
{
  // Set all pins to digital input mode.
  pinMode(rHorizontalPin, INPUT);
  pinMode(rVerticalPin, INPUT);
  pinMode(lHorizontalPin, INPUT);
  pinMode(lVerticalPin, INPUT);

  // Begin serial communication at 9600 baud.
  Serial.begin(9600);
}

void loop() 
{
  // Set all pins to pulse in, reading when a pulse goes from high to low. 
  unsigned long rHorizPulse = pulseIn(rHorizontalPin, LOW);
  unsigned long rVertPulse = pulseIn(rVerticalPin, LOW);
  unsigned long lHorizPulse = pulseIn(lHorizontalPin, LOW);
  unsigned long lVertPulse = pulseIn(lVerticalPin, LOW);

  // Print all values out:
  Serial.print(rHorizPulse + " ");
  Serial.print(rVertPulse + " ");
  Serial.print(lHorizPulse + " ");
  Serial.println(lVertPulse);

  // Note: Not printing all values on the same line to avoid data type parsing issues. 
}
