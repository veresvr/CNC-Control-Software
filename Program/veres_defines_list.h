/*			discription:
    List of errors, which cnc freeze return to PC.
*/
// errors
#define OK              			0
//#define NO_ERROR	      			0
#define START_STOP_UNCORR 		19
#define CRC_UNCORR      			18
#define UNDEF_COMMAND   			0x04
#define COMMAND_LESS_5_CHAR   21
#define EXCESS_NUMB     			0x06
#define EXCESS_DELAY    			0x07
#define X_SENSOR_MAX    			0x08
#define X_SENSOR_MIN    			0x09
#define Y_SENSOR_MAX    			0x0A
#define Y_SENSOR_MIN    			0x0B
#define Z_SENSOR_MAX    			0x0C
#define Z_SENSOR_MIN    			0x0D
#define SH_CANT_ON						0x0E
#define SH_OVERCURRENT	 			0x0F
#define LASER_CANT_START 			0x10
#define SET_DIR_ERROR 				31


//data packet commands
#define INSTRUCTION						1		// coz the 0th is start sign
#define DATA									2
#define MOV_STEPS_X_PLUS			65
#define MOV_STEPS_X_MINUS			30
#define MOV_STEPS_Y_PLUS			31
#define MOV_STEPS_Y_MINUS			32
#define MOV_STEPS_Z_PLUS			33
#define MOV_STEPS_Z_MINUS			34
#define MOV_CARRIAGE_FREE			48
#define MOV_CARRIAGE_WORK			47
#define MOV_STEPS_XY					49
#define MOV_CARRIAGE_AT_Z_FREE 50
#define MOV_CARRIAGE_AT_Z_WORK 51
#define SHPINDLE_MODE					26
#define GOTO_COMMAND                            52
#define END_SONSORS_STATUS		53

// GOTO variants
#define NONE                                    0
#define GOTO_HOME                               1
#define GOTO_LOCAL_ZERO                         2
#define GOTO_X_PLUS                             3
#define GOTO_X_MINUS                            4
#define GOTO_Y_PLUS                             5
#define GOTO_Y_MINUS                            6
#define GOTO_Z_PLUS                             7
#define GOTO_Z_MINUS                            8

#define REPETITION_VALUE_MAX	255

// INSTRUCTION variants:
#define GET_END_SENSORS_STATUS						101				// ASCII e
#define SET_LOCAL_COORDINATES_TO_ZERO			98				// ASCII b
#define ACKNOWLEDGE												6					// ASCII control character
#define NEGATIVE_ACKNOWLEDGE							21				// ASCII control character
#define MAKE_BELL													7					// ASCII control character























