#ifndef pidPIDINCLUDE
    #define pidPIDINCLUDE
    #define PWM_OUTPUT_PIN 18
    #define PWM_SLICE 65535
    void pwmPID_init(uint pin);
    void pwmPID_output(double value);
#endif    