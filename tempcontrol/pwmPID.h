#ifndef pidPIDINCLUDE
    #define pidPIDINCLUDE
    #define PWM_OUTPUT_PIN 18
    void pwmPID_init(uint pin);
    void pwmPID_output(double value);
#endif    