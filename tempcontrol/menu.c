//PID.menu for 16x2 LCD with encoder
// David Frame Mechatronics Utah Valley University

// Currently this is using golbal variables.
#include "NewLCDLibrary.h"
#include "menu.h"
#define NAN 0.0/0.0

pidVars_t PID;

void menuInit(void)
{
    PID.menu = PID.update = PID.pwm = PID.SP = 0;
    PID.kp = PID.ki = PID.kd = 0;
    PID.currentVariable = &PID.SP;
    PID.seeDelay = 250;
    PID.menu = 0;
    PID.update = 0;
    sprintf(PID.title,"Temperature PID v0.5 ");
    sprintf(PID.author, "Dan");

}
void showSplashScreen(void)
{
	// Clear the screen
	LCDclear();
	// Simple string printing
	LCDWriteStringXY(0, 0, PID.title);
	// A string on line 2
	LCDWriteStringXY(0, 1, PID.author);
	sleep_ms(2000);
	// Clear the screen
	LCDclear();
}

void mainMenu(void)
{
	int debounce_delay = PID.seeDelay;
	if (PID.menu > 0)
	{
		// Clear the screen
		if (PID.update == 0)
		{
			LCDclear();

			// PID.menu
			LCDWriteStringXY(0, 0, "SP:"); // Set Point
			LCDWriteStringXY(8, 0, "ki:"); // Integer Gain
			LCDWriteStringXY(0, 1, "kd:"); // Differential Gain
			LCDWriteStringXY(8, 1, "kp:"); // Proportional Gain
			PID.update = 1;
		}
		else
		{
			if (PID.menu == 1)
			{
				sleep_ms(debounce_delay);
				LCDWriteStringXY(3, 0, "    ");
				sleep_ms(debounce_delay);
				LCDWriteIntXY(3, 0, PID.SP);
				PID.currentVariable = &PID.SP; // connect to interrupt
			}
			if (PID.menu == 2)
			{
				sleep_ms(debounce_delay);
				LCDWriteStringXY(11, 0, "    ");
				sleep_ms(debounce_delay);
				LCDWriteIntXY(11, 0, PID.ki);
				PID.currentVariable = &PID.ki; // connect to interrupt
			}
			if (PID.menu == 3)
			{
				sleep_ms(debounce_delay);
				LCDWriteStringXY(3, 1, "    ");
				sleep_ms(debounce_delay);
				LCDWriteIntXY(3, 1, PID.kd);
				PID.currentVariable = &PID.kd; // connect to interrupt
			}
			if (PID.menu == 4)
			{
				sleep_ms(debounce_delay);
				LCDWriteStringXY(11, 1, "    ");
				sleep_ms(debounce_delay);
				LCDWriteIntXY(11, 1, PID.kp);
				PID.currentVariable = &PID.kp; // connect to interrupt
			}

			LCDWriteIntXY(3, 0, PID.SP);  // Set Point
			LCDWriteIntXY(11, 0, PID.ki);  // Integer Gain
			LCDWriteIntXY(3, 1, PID.kd);  // Differential Gain
			LCDWriteIntXY(11, 1, PID.kp); // Proportional Gain
		}
	}
	else
	{
		if (PID.update == 0)
		{
			LCDclear();
			LCDWriteStringXY(0, 0, "Temperature:");
			LCDWriteStringXY(0, 1, "PWM:");
			PID.update = 1;
		}
		else
		{
			if (PID.temperature != NAN)
			{
				if (PID.temperature < 100) // needed to kill revanent digit
				{
					LCDWriteStringXY(14, 0, " ");
				}
				LCDWriteFloatXY(12, 0, PID.temperature); // Current Temp
				if (PID.pwm < 100)							// needed to kill revanent digit
				{
					LCDWriteStringXY(12, 1, " ");
				}
				LCDWriteIntXY(4, 1, PID.pwm);
			}
			else
			{
				LCDWriteStringXY(12, 0, "NAN");
			}
		}
	}
}