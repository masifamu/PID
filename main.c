#include <stdio.h>
#include <stdlib.h>
#include "PID.h"



/* Simulated dynamical system (first order) */
float TestSystem_Update(float inp);
float mapFunction(float x,float x1,float x2, float y1, float y2);

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
    float setpoint = 500.0f;

    printf("Time (s)\tSystem Output\tControllerOutput\r\n");
    for (float t = 0.0f; t <= SIMULATION_TIME_MAX; t += SAMPLE_TIME_S) {

        /* Get measurement from system */
        float measurement = TestSystem_Update(pid.out);

        /* Compute new control signal */
        PIDController_Update(&pid, setpoint, measurement);

        printf("t=%f\tSYSTEM_OUT=%f\tPID_OUT=%f\tMAPPED_OUT=%f\r\n", t, measurement, pid.out,mapFunction(pid.out,PID_LIM_MIN,PID_LIM_MAX,0,4096));
        //fprintf(filePointer,"t=%f\tSYSTEM_OUT=%f\tPID_OUT=%f\tMAPPED_OUT=%f\r\n", t, measurement, pid.out,mapFunction(pid.out,PID_LIM_MIN,PID_LIM_MAX,0,4096));
        fprintf(filePointer,"%f,%f,%f,%f\n", t, measurement, pid.out,mapFunction(pid.out,PID_LIM_MIN,PID_LIM_MAX,0,4096));

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

float mapFunction(float x,float x1,float x2, float y1, float y2){
    float y=0;
    y=((x2-x1)/(y2-y1))*(x-x1)+y1;
    return y;
}


