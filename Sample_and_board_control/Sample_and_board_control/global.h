#ifndef global
#define global

#define CHARGE_EN PD2
#define STATUS_CHARGE PD3
#define STATUS_DONE PD4
#define BOOST_EN PD5
#define RUNNING PD6
#define RESET_PI PD7

#define START_STOPBUTTON PB0
#define SAMPLE PB1

#define SET_PRE_0 (TCCR1B=0b00011000)
#define SET_PRE_1 (TCCR1B=0b00011001)
#define SET_PRE_8 (TCCR1B=0b00011010)
#define SET_PRE_64 (TCCR1B=0b00011011)
#define SET_PRE_256 (TCCR1B=0b00011100)
#define SET_PRE_1024 (TCCR1B=0b00011101)

float samplefreq;

#endif