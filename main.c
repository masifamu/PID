#include <stdio.h>
#include <stdlib.h>
#include "PID.h"



/* Simulated dynamical system (first order) */
float TestSystem_Update(float inp);
int mapFunction(int throttle);
float mapToPWM(float pidOut);
float getMotorSpeed(float pwmWidth);

int main()
{

    //file handling
    FILE *filePointer;
    filePointer=fopen("output.txt","w");

    /* Initialise PID controller */
    PIDController pid = { PID_KP, PID_KI, PID_KD,
                          PID_TAU,
                          PID_LIM_MIN, PID_LIM_MAX,
			  PID_LIM_MIN_INT, PID_LIM_MAX_INT,
                          SAMPLE_TIME_S };

    PIDController_Init(&pid);

    /* Simulate response using test system */
    float targetRPM = mapFunction(500);

    printf("Time (s)\tSystem Output\tControllerOutput\r\n");
    for (float t = 0.0f; t <= SIMULATION_TIME_MAX; t += SAMPLE_TIME_S) {
        /* making pwm width */
        float pwmWidth = mapToPWM(pid.out);
        /* Get measurement from system */
        float measuredRPM = getMotorSpeed(pwmWidth);

        /* Compute new control signal */
        PIDController_Update(&pid, targetRPM, measuredRPM);

        printf("t=%f\tMEAS_RPM=%f\tPID_OUT=%f\tTARG_RPM=%fPWM_WIDTH=%f\r\n", t, measuredRPM, pid.out,targetRPM,pwmWidth);
        fprintf(filePointer,"%f,%f,%f,%f,%f\n", t, measuredRPM, pid.out,targetRPM,pwmWidth);

    }

    fclose(filePointer);
    return 0;

}

float TestSystem_Update(float inp) {

    static float output = 0.0f;
    static const float alpha = 0.02f;

    output = (SAMPLE_TIME_S * inp + output) / (1.0f + alpha * SAMPLE_TIME_S);

    return output;
}

int mapFunction(int throttle){
    int y=0;
    y=((throttle-MIN_THROTTLE)*(MAX_RPM-MIN_RPM)/(MAX_THROTTLE-MIN_THROTTLE))+MIN_RPM;
    return y;
}

float mapToPWM(float pidOut){
/*    float y=0;
    y=((float)(pidOut-PID_LIM_MIN)*(MAX_PWM_WIDTH-MIN_PWM_WIDTH)/(PID_LIM_MAX-PID_LIM_MIN))+MIN_PWM_WIDTH;
    return y;
*/
/*    static float pwmW;
    static float prevPIDOut;
    if(pidOut-prevPIDOut>0)//meaning that the PID output is increasing
        pwmW += 1;
    else if(pidOut-prevPIDOut<0)
        pwmW -= 1;
    return pwmW;
    */
    return pidOut;
}

float getMotorSpeed(float pwmWidth){

    static int count;
    static float output = 0.0f;
    static float alpha = 0.02f;

    output = (SAMPLE_TIME_S * pwmWidth + output) / (1.0f + alpha * SAMPLE_TIME_S);

    //creating a load increasing mechanism that decreases the RPM
    count++;
    if(count == 1500) alpha = 10;
    if(count == 1510) alpha = 0.02;

    if(count == 3000) alpha = 10;
    if(count == 3100) alpha = 0.02;

    if(count == 4500) alpha = 10;
    if(count == 4505) alpha = 0.02;

    return output;
}


