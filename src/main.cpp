#include <Arduino.h>

/* Debug code 
If you want to print on the serial, set DEBUG to 1, 
otherwise set it to 0 to suppress all Serial print statements
*/
#define DEBUG 1

#ifdef DEBUG == 1

#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)

#else

#define debug(x)
#define debugln(x)

#endif

/* End of debug statements */
// encoder pins
#define ENCODER_A_PIN  2 // interrupt
#define ENCODER_B_PIN  4 // direction

#define PULSES_PER_REV  150  // this was measured manually before-hand
#define RPM_TO_RADS 0.1047198; // 1 rpm  = 0.1047198 rad/s

volatile long pulse_count = 0;
bool direction = false;  // true if motor is moving forward, false if motor is moving reverse

/* Sampling interval 1 second */
int sampling_interval = 1000;
long current_millis = 0;
long previous_millis = 0;

/* velocity variables 
  we need angular velocity in radians, in degrees and the linear velocity
*/
float angular_velocity_in_rads = 0.0;
float angular_velocity_in_degrees = 0.0;
float linear_velocity = 0.0;
float distance = 0.0;

void pulse_isr(){
  // get read the direction encoder => ENCODER_B
  int val = digitalRead(ENCODER_B_PIN);

  /* check if the motor is oving forward or reverse
   if reverse, val is going to be LOW, otherwise val will be HIGH
   If val is LOW, decrement the pulse count, if val is HIGH, increment the pulse count
  */

  if (val == HIGH){
    direction = true;
  } else{
    direction = false;
  }

  if (direction){
    /* code */
    pulse_count++;
  }else{
    pulse_count--;
  }
  
  
  // update pulse count
  // (direction) ? pulse_count++ : pulse_count-- ;
  
}

int calculate_distance(volatile long p){
  /* p => pulses that the ISR has registered */
  
  return 0;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(ENCODER_A_PIN, INPUT_PULLUP);
  pinMode(ENCODER_B_PIN, INPUT);

  // Attach interrupt, every time the pin goes high, register a pulse
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN), pulse_isr, RISING);
}

void loop() {
  
  // get the elapsed time
  current_millis = millis();

  // Take a sample
  if ((current_millis - previous_millis) > sampling_interval){
    /* code */

    /* update the current time for the next iteration */
    previous_millis = current_millis;

    /* calculate Revs per minute */ 
    float revs_per_minute = (float)((pulse_count * 60) / PULSES_PER_REV);

    /* calculate angular velocity in rad/s of the wheel in radians */
    angular_velocity_in_rads = revs_per_minute * RPM_TO_RADS;

    /* calculate angular velocity in degrees */
    angular_velocity_in_degrees = angular_velocity_in_rads * RAD_TO_DEG; // 1 rad = 57.2958 deg, inbuilt

    /* debug on the serial monitor */
    debug("RPM: "); debugln(revs_per_minute);

    /* end of debug */

    /* reset the pulse count */
    pulse_count = 0;

  }

}
