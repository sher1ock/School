#ifndef MENUHEADER
    #define MENUHEADER
    // for use with the menu system for 16X2 displays
    //create a structure for the variables
    typedef struct pidVars
    {
        int seeDelay, menu, update, pwm;
        int SP, kp, ki, kd, *currentVariable;
        float temperature;
        char* title;
        char* author;
    }pidVars_t;
    
    // this does not create memory space for PID, but only the external linkage
    extern struct pidVars PID;

    void showSplashScreen(void);
    void mainMenu(void);
    void menuInit(void);
#endif