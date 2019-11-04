/*
  ESC_Calibration
    It's important to calibrate the ESC with the values (uS / microseconds) it will expect for Min and Max speed.
    This one requires a little procedure:  
      1 - Connect your Arduino board and your Adafruit PCA9685 16-Channel 12-bit PWM/Servo Driver - I2C interface
      2 - Load the sketch to your Arduino board, then remove power
      2 - Connect your ESC to the Arduino board
      3 - Power your Arduino board
      4 - Once the LED (LED_PIN) is HIGH/ON connect the power to your ESC, you have 5sec to do so
      5 - Once the LED is LOW/OFF the calibration should be done
      6 - Should now be calibrated between 1000us and 2000us
    
  Based on the 27 April 2017 Sketch by Eric Nantel and his RC_ESC library for Robot shop
  rewritten for use in the I2C_ESC library with the Adafruit PCA9685 16-Channel 12-bit PWM/Servo Driver - I2C interface
  
  Servo & Knob links
  http://www.arduino.cc/en/Tutorial/Knob
  http://people.interaction-ivrea.it/m.rinott
 */

#include "I2C_ESC.h"
#define LED_PIN (13)            // Pin for the LED 
#define SPEED_MIN (1000)        // Set the Minimum Speed in microseconds
#define SPEED_MAX (2000)        // Set the Maximum Speed in microseconds
#define ARM_VALUE (500)         // Set the Arm value in microseconds
#define SERVO_FREQ (60)         // Analog servos run at ~60 Hz updates

I2C_ESC myESC (0x40, 0, 1, SPEED_MIN, SPEED_MAX, ARM_VALUE);    // ESC_Name (I2C_address, ESC PIN, Minimum Value, Maximum Value, Default Speed, Arm Value)

void setup() {
  // Start serial port
  Serial.begin(9600);
  Serial.println("ESC calibration over the I2C based PWM/Servo control");
  // Set up the I2C based PWM/Servo extenders
  myESC.begin();

    /*
   * In theory the internal oscillator (clock) is 25MHz but it really isn't that precise. 
   * You can 'calibrate' by tweaking this number till you get the frequency you're expecting!
   * The int.osc. is closer to 27MHz and is used for calculating things like writeMicroseconds
   */
  myESC.setOscillatorFrequency(26075000);

  /*
  * Set the analog servo PWM frequency
  * alternativly you could set this using the prescale 60Hz is a prescale of about 105 (depending on the internal oscillator frequency)
  */
  myESC.setPWMFreq(SERVO_FREQ);
  
  // Start calibration cycle
  pinMode(LED_PIN, OUTPUT);       // LED Visual Output (can be removed)
  digitalWrite(LED_PIN, HIGH);    // LED High while signal is High (can be removed)
  myESC.calib();                  // Calibration of the Max and Min value the ESC is expecting
  myESC.stop();                   // Stop the ESC to avoid damage or injuries
  digitalWrite(LED_PIN, LOW);     // LED Low when the calibration is done (can be removed)
}

void loop() {
  // Noting to do in the loop, Calibration all happens in set up.
}
