#include "STDrone.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PID gain and limit settings
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float pid_p_gain_roll = 1.3;               //Gain setting for the pitch and roll P-controller (default = 1.3).
float pid_i_gain_roll = 0.04;              //Gain setting for the pitch and roll I-controller (default = 0.04).
float pid_d_gain_roll = 18.0;              //Gain setting for the pitch and roll D-controller (default = 18.0).
int pid_max_roll = 400;                    //Maximum output of the PID-controller (+/-).

float pid_p_gain_pitch = pid_p_gain_roll;  //Gain setting for the pitch P-controller.
float pid_i_gain_pitch = pid_i_gain_roll;  //Gain setting for the pitch I-controller.
float pid_d_gain_pitch = pid_d_gain_roll;  //Gain setting for the pitch D-controller.
int pid_max_pitch = pid_max_roll;          //Maximum output of the PID-controller (+/-).

float pid_p_gain_yaw = 4.0;                //Gain setting for the pitch P-controller (default = 4.0).
float pid_i_gain_yaw = 0.02;               //Gain setting for the pitch I-controller (default = 0.02).
float pid_d_gain_yaw = 0.0;                //Gain setting for the pitch D-controller (default = 0.0).
int pid_max_yaw = 400;                     //Maximum output of the PID-controller (+/-).

float pid_p_gain_head = 3.0;                //Gain setting for the head P-controller (default = 4.0).
float pid_i_gain_head = 0.02;               //Gain setting for the head I-controller (default = 0.02).
float pid_d_gain_head = 0.0;                //Gain setting for the head D-controller (default = 0.0).
int pid_max_head = 400;                     //Maximum output of the PID-controller (+/-).

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Declaring global variables
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//int16_t = signed 16 bit integer
//uint16_t = unsigned 16 bit integer

uint8_t start;
uint8_t error, error_counter, error_led;

int16_t esc_1, esc_2, esc_3, esc_4;
int16_t throttle;
int16_t temperature;

int32_t receiver_input[6], reveiver_input_previous[6];
uint8_t channel_select_counter;
int32_t measured_time, measured_time_start;
uint32_t loop_timer, error_timer;

float roll_level_adjust, pitch_level_adjust;
float pid_error_temp;
float pid_i_mem_roll, pid_roll_setpoint, gyro_roll, pid_output_roll, pid_last_roll_d_error;
float pid_i_mem_pitch, pid_pitch_setpoint, gyro_pitch, pid_output_pitch, pid_last_pitch_d_error;
float pid_i_mem_yaw, pid_yaw_setpoint, gyro_yaw, pid_output_yaw, pid_last_yaw_d_error;
float pid_i_mem_head, pid_head_setpoint, gyro_head, pid_output_head, pid_last_head_d_error;
float angle_roll_acc, angle_pitch_acc, angle_pitch, angle_roll, angle_yaw;
float battery_voltage;

float campassX, campassY, campassZ;
float compassX_horizontal, compassY_horizontal, actual_compass_heading;

enum flightType {MANUAL, HEAD_LOCK, GPS_HOLD, VIO} flightMode = MANUAL;

char buffer[20];

uint32_t timer;

/*
 *  Variables for GPS
 */
float lattitude, longitude;
bool north_location = true;
bool east_location = false;
int32_t l_lat_gps, l_lon_gps, lat_gps_previous, lon_gps_previous;
float lat_gps_loop_add, lon_gps_loop_add, lat_gps_add, lon_gps_add;



/*
 *  send telegram data
 */
bool telegram_IMU;
uint8_t telegram_loop_counter, telegram_send_byte, telemetry_bit_counter;
uint32_t telegram_data_buffer;
