///////////////////////////////////////////////////////////////////////////////////////
//Terms of use
///////////////////////////////////////////////////////////////////////////////////////
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////////////
//Safety note
///////////////////////////////////////////////////////////////////////////////////////
//Always remove the propellers and stay away from the motors unless you
//are 100% certain of what you are doing.
///////////////////////////////////////////////////////////////////////////////////////

//Let's declare some variables so we can use them in the complete program.
//int16_t = signed 16 bit integer
//uint16_t = unsigned 16 bit integer
#include <JY901.h>

#define THROTTLE_INPUT  2
#define YAW_INPUT       3
#define PITCH_INPUT     1
#define ROLL_INPUT      0
#define TWIST_INPUT     5
#define SWITCH_INPUT    4

#define IMU_SCALE                       32768
#define GYRO_SCALE                      2000
#define ANGLE_SCALE                     180

#define ANGLE_ROLL_CALIBRATE_VALUE      -2.04
#define ANGLE_PITCH_CALIBRATE_VALUE     0.02

//Manual accelerometer calibration values for IMU angles:
int16_t manual_acc_pitch_cal_value = 0;
int16_t manual_acc_roll_cal_value = 0;

//Manual gyro calibration values.
//Set the use_manual_calibration variable to true to use the manual calibration variables.
uint8_t use_manual_calibration = false;
int16_t manual_gyro_pitch_cal_value = 0;
int16_t manual_gyro_roll_cal_value = 0;
int16_t manual_gyro_yaw_cal_value = 0;

uint8_t disable_throttle, flip32;
uint32_t loop_timer;
float angle_roll_acc, angle_pitch_acc, angle_pitch, angle_roll;
float battery_voltage;
int16_t loop_counter;
uint8_t data, start, warning;
float acc_axis[4], gyro_axis[4], temperature;
int32_t gyro_axis_cal[4], acc_axis_cal[4];
int32_t cal_int;
int32_t receiver_input[6], reveiver_input_previous[6];

//The I2C address of the MPU-6050 is 0x68 in hexadecimal form.
uint8_t gyro_address = 0x68;

uint8_t channel_select_counter;
int32_t measured_time, measured_time_start;

void setup() {
  pinMode(4, INPUT_ANALOG);
  //Port PB3 and PB4 are used as JTDO and JNTRST by default.
  //The following function connects PB3 and PB4 to the alternate output function.
  afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY);                     //Connects PB3 and PB4 to output function.

  //On the Flip32 the LEDs are connected differently. A check is needed for controlling the LEDs.
  pinMode(PB3, INPUT);                                         //Set PB3 as input.
  pinMode(PB4, INPUT);                                         //Set PB4 as input.
  if (digitalRead(PB3) || digitalRead(PB3))flip32 = 1;         //Input PB3 and PB4 are high on the Flip32
  else flip32 = 0;

  pinMode(PB3, OUTPUT);                                         //Set PB3 as output.
  pinMode(PB4, OUTPUT);                                         //Set PB4 as output.

  green_led(LOW);                                               //Set output PB3 low.
  red_led(LOW);                                                 //Set output PB4 low.
  Serial1.begin(115200);
  Serial.begin(57600);                                          //Set the serial output to 57600 kbps.
  delay(100);                                                    //Give the serial port some time to start to prevent data loss.
  timer_setup();                                                //Setup the timers for the receiver inputs and ESC's output.
  delay(50);                                                    //Give the timers some time to start.

  print_intro();                                                //Print the intro on the serial monitor.
}

void loop() {
  delay(10);

  if (Serial.available() > 0) {
    data = Serial.read();                                       //Read the incomming byte.
    delay(100);                                                 //Wait for any other bytes to come in.
    while (Serial.available() > 0)loop_counter = Serial.read(); //Empty the Serial buffer.
    disable_throttle = 1;                                       //Set the throttle to 1000us to disable the motors.
  }

  if (!disable_throttle) {                                      //If the throttle is not disabled.
    TIMER4_BASE->CCR1 = receiver_input[THROTTLE_INPUT];                              //Set the throttle receiver input pulse to the ESC 1 output pulse.
    TIMER4_BASE->CCR2 = receiver_input[THROTTLE_INPUT];                              //Set the throttle receiver input pulse to the ESC 2 output pulse.
    TIMER4_BASE->CCR3 = receiver_input[THROTTLE_INPUT];                              //Set the throttle receiver input pulse to the ESC 3 output pulse.
    TIMER4_BASE->CCR4 = receiver_input[THROTTLE_INPUT];                              //Set the throttle receiver input pulse to the ESC 4 output pulse.
  }
  else {                                                        //If the throttle is disabled
    TIMER4_BASE->CCR1 = 1000;                                   //Set the ESC 1 output to 1000us to disable the motor.
    TIMER4_BASE->CCR2 = 1000;                                   //Set the ESC 2 output to 1000us to disable the motor.
    TIMER4_BASE->CCR3 = 1000;                                   //Set the ESC 3 output to 1000us to disable the motor.
    TIMER4_BASE->CCR4 = 1000;                                   //Set the ESC 4 output to 1000us to disable the motor.
  }

  if (data == 'a') {
    Serial.println(F("Reading receiver input pulses."));
    Serial.println(F("You can exit by sending a q (quit)."));
    delay(2500);
    reading_receiver_signals();
  }


  if (data == 'c') {
    Serial.println(F("Reading raw gyro data."));
    Serial.println(F("You can exit by sending a q (quit)."));
    read_gyro_values();
  }

  if (data == 'd') {
    Serial.println(F("Reading the raw accelerometer data."));
    Serial.println(F("You can exit by sending a q (quit)."));
    delay(2500);
    read_gyro_values();
  }

  if (data == 'e') {
    Serial.println(F("Reading the IMU angles."));
    Serial.println(F("You can exit by sending a q (quit)."));
    check_imu_angles();
  }

  if (data == 'f') {
    Serial.println(F("Test the LEDs."));
    test_leds();
  }

  if (data == 'g') {
    Serial.println(F("Reading the battery voltage."));
    Serial.println(F("You can exit by sending a q (quit)."));
    check_battery_voltage();
  }

  if (data == 'h') {
    Serial.println(F("Get manual gyro and accelerometer calibration values."));
    manual_imu_calibration();
  }

  if (data == '1') {
    Serial.println(F("Check motor 1 (front right, counter clockwise direction)."));
    Serial.println(F("You can exit by sending a q (quit)."));
    delay(2500);
    check_motor_vibrations();
  }

  if (data == '2') {
    Serial.println(F("Check motor 2 (rear right, clockwise direction)."));
    Serial.println(F("You can exit by sending a q (quit)."));
    delay(2500);
    check_motor_vibrations();
  }

  if (data == '3') {
    Serial.println(F("Check motor 3 (rear left, counter clockwise direction)."));
    Serial.println(F("You can exit by sending a q (quit)."));
    delay(2500);
    check_motor_vibrations();
  }

  if (data == '4') {
    Serial.println(F("Check motor 4 (front lefft, clockwise direction)."));
    Serial.println(F("You can exit by sending a q (quit)."));
    delay(2500);
    check_motor_vibrations();
  }

  if (data == '5') {
    Serial.println(F("Check motor all motors."));
    Serial.println(F("You can exit by sending a q (quit)."));
    delay(2500);
    check_motor_vibrations();
  }
}

void gyro_signalen(void) {
  while (Serial1.available()) JY901.CopeSerialData(Serial1.read()); //Call JY901 data cope function
  gyro_axis[1]  = -(float)JY901.stcGyro.w[0] / IMU_SCALE * GYRO_SCALE;
  gyro_axis[2] = -(float)JY901.stcGyro.w[1] / IMU_SCALE * GYRO_SCALE;
  gyro_axis[3]   = -(float)JY901.stcGyro.w[2] / IMU_SCALE * GYRO_SCALE;

  acc_axis[1] = -(float)JY901.stcAngle.Angle[0] / IMU_SCALE * ANGLE_SCALE + ANGLE_PITCH_CALIBRATE_VALUE;
  acc_axis[2]  = -(float)JY901.stcAngle.Angle[1] / IMU_SCALE * ANGLE_SCALE + ANGLE_ROLL_CALIBRATE_VALUE;
  acc_axis[3]  = -(float)JY901.stcAngle.Angle[2] / IMU_SCALE * ANGLE_SCALE;
}

void red_led(int8_t level) {
  if (flip32)digitalWrite(PB4, !level);
  else digitalWrite(PB4, level);
}
void green_led(int8_t level) {
  if (flip32)digitalWrite(PB3, !level);
  else digitalWrite(PB3, level);
}
