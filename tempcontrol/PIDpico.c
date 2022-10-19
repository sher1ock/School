#include <stdio.h>
#include "pico/stdlib.h"
#include "NewLCDLibrary.h"
#include "encoder.h"
// #include "menu.h"
// #include "MAX6675.h"
// #include "pwmPID.h"
#include "PIDCalc.h"

#define START_SETPOINT 150
#define START_KP 5
#define START_KI 0
#define START_KD 0



/*
nick.mccloud:
Pico/RP2040 ≠ Arduino
Pico = hot rod kit car 
Arduino = hot rod kit car wrapped in cotton wool with buoyancy aids & parachute
*/


//create the PID variables in global memory
//pidVars_t PID;


int main()
{
    float current_temp;
    stdio_init_all();
    LCDinit(6,7,8,9,13,11,16,2);
    Encoder_init();
    menuInit();
    MAX6675Init(MAX6675_SCLK, MAX6675_MISO, MAX6675_CS);
    pwmPID_init(PWM_OUTPUT_PIN);

    PID.SP = START_SETPOINT;
    PID.kp = START_KP;
    PID.ki = START_KI;
    PID.kd = START_KD;

    PID_init(&PID.temperature, &PID.pwm, &PID.SP, PID.kp, PID.ki, PID.kd, P_ON_E, DIRECT );
    SetMode(AUTOMATIC);
        
    // Here is how we put out serial data: puts("MECH PID!");
    //LCDclear();
    //LCDprintWrapped("Welcome to the  PIDPico");
    showSplashScreen();
    while(1)
    {
        //sleep_ms(500);
        mainMenu();
        PID.temperature = readFahrenheit();
        Compute();
        printf("%f, %f\r\n", PID.temperature, PID.SP);
        pwmPID_output(PID.pwm); //just a test
    }

    return 0;
}