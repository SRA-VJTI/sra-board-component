#ifndef PIN_DEFS_H
#define PIN_DEFS_H

//////// accelerometer //////
#define ACCL_SDA 22
#define ACCL_SCL 23
/////////////////////////////

//////// bar graph //////////
#define BG_LED_1 32
#define BG_LED_2 33
#define BG_LED_3 25
#define BG_LED_4 26
#define BG_LED_5 27
#define BG_LED_6 14
#define BG_LED_7 12
#define BG_LED_8 13
/////////////////////////////

////////// switches //////////
#define SWITCH_1 4
#define SWITCH_2 2
#define SWITCH_3 17
#define SWITCH_4 16
/////////////////////////////

/////////// lsa /////////////
#define LSA_A0 36
#define LSA_A1 39
#define LSA_A2 34
#define LSA_A3 35
/////////////////////////////

/////// motor driver ////////
//****** normal mode ******//
#define MDA_NORMAL_IN_1 32  //MCPWM_UNIT_0 MCPWM0A
#define MDA_NORMAL_IN_2 33  //MCPWM_UNIT_0 MCPWM0B
#define MDA_NORMAL_IN_3 25  //MCPWM_UNIT_0 MCPWM1A
#define MDA_NORMAL_IN_4 26  //MCPWM_UNIT_0 MCPWM1B
#define MDB_NORMAL_IN_5 27  //MCPWM_UNIT_0 MCPWM2A
#define MDB_NORMAL_IN_6 14  //MCPWM_UNIT_0 MCPWM2B
#define MDB_NORMAL_IN_7 12  //MCPWM_UNIT_1 MCPWM0A
#define MDB_NORMAL_IN_8 13  //MCPWM_UNIT_1 MCPWM0B

//***** parallel mode *****//
#define MDA_PARALLEL_IN_1_2 25
#define MDA_PARALLEL_IN_3_4 26
#define MDB_PARALLEL_IN_5_7 12
#define MDB_PARALLEL_IN_6_8 13
/////////////////////////////

/////////// servos //////////
#define SERVO_1 5
#define SERVO_2 18
#define SERVO_3 19
/////////////////////////////

#endif