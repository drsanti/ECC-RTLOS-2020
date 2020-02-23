/************************************************************
 * Example 4. PWMs with OCs                                 *
 ************************************************************
 * File:    main.c                                          *
 * Description:                                             *
 *          Demonstrates how to use the RTOS, BSP and RTL.  *
 *          (Please see closely in the app.h and config.cfg)*
 * Author:  Asst.Prof.Dr.Santi Nuratch                      *
 *          Embedded Computing and Control Laboratory       *
 *          ECC-Lab, INC, LMUTT, Thailand                   *
 ************************************************************
 * Update:  18 February 2020 (Initial)                      *
 * Update:  23 February 2020                                *
 *          Use the app.h to include/exclude modules.       *
 ************************************************************/

/************************************************************
 * Instuction:
 *   1) Open the library/ECC/app.h.
 *   2) Change value of the TARGET_EXAMPLE to 4.
 ************************************************************/


/************************************************************
 * Include the application header.
 ************************************************************/
#include <app.h>

/************************************************************
 * Include the system header.
 ************************************************************/
#include <ecc.h>


/************************************************************
 * PWM Update Task (Generate Sevo Motor Driving Signal).
 ************************************************************/
void PWM_TaskCode( void * pvParameters ) {

    // Servo motor requires 50 Hz (20 mS) of PWM
    // Minumum of t-on is 1.0 mS (1/20 duty ratio)
    // Maximum of t-on is 2.0 mS (2/20 duty ratio)
    #define T_MIN 1.0
    #define T_MAX 2.0

    float t_on = 1.0;   // On-time interval.
    float dir  = 1.0;   // Changing direction.
    float step = 0.25;  // Step size.

    for( ;; ) {
        t_on += step*dir;

        if(t_on >= T_MAX || t_on <= T_MIN) {
            dir *= -1;
        }

        PERFORM_CRITICAL_SECTION(
            Uart1_Printf("TSK: t-on: %.2f, ratio: %.3f\r\n", t_on, t_on/20.0);
            Pwm_SetDuty(PWM_ID_3, t_on/20.0);
        )

        vTaskDelayMs(1000);
    }
}


/************************************************************
 * PSW State Changed Callback function.
 ************************************************************/
void  PSW_Callback( void *event ) {

    static float t_on = 1.0;

    switch_event_t *evt = (switch_event_t *)event;

    if(evt->id == 0) {
        t_on += 0.1;
    }
    else if(evt->id == 1) {
        t_on = 1.5;
    }
    else if(evt->id == 2) {
       t_on-=0.1;
    }

    PERFORM_CRITICAL_SECTION(
        Uart1_Printf("CBK: t-on: %.2f, ratio: %.3f\r\n", t_on, t_on/20.0);
        Pwm_SetDuty(PWM_ID_3, t_on/20.0);
    );
}

int main(void) {

    /*********************************
     * 1. INITIALIZE THE SYSTEM
     *********************************/
    System_Init();                                  // Initializes system modules.


    /*********************************
     * 2. UART1
     *********************************/
    Uart1_Init(115200, 64, 128);
    Uart1_Printf("Example 4: PWMs (OCs)\r\n");


    /************************************************
     * 3. Initial PWMs with frequency and duty ratio.
     ************************************************/
    Pwm_Init(50, 0.1);


    /************************************************
     * 4. Change the PWMs frequency to all channels.
     ************************************************/
    Pwm_SetFrequency(1.0/20e-3);

    /************************************************
     * 5. Change the duty ratio of the PWM
     ************************************************/
    Pwm_SetDuty(PWM_ID_0, 0.1);
    Pwm_SetDuty(PWM_ID_1, 0.5);
    Pwm_SetDuty(PWM_ID_2, 0.8);
    Pwm_SetDuty(PWM_ID_3, 1.0/20.0);    // Connected to Servo.


    /************************************************
     * 6. Create task (If the task uses printf(), or
     * complex methematic operations, the stack size
     * should be larg enough, e.e., 256 or 512 bytes
     ************************************************/
    xTaskCreate(PWM_TaskCode, "PWM", 512, NULL, tskIDLE_PRIORITY, NULL);


    /*********************************
     * 7. ADD CALLBACKS OF SWITCHES
     *********************************/
    Psw_SetKeyDownCallback(PSW_ID_0, PSW_Callback);
    Psw_SetKeyDownCallback(PSW_ID_1, PSW_Callback);
    Psw_SetKeyDownCallback(PSW_ID_2, PSW_Callback);
    Psw_SetKeyDownCallback(PSW_ID_3, PSW_Callback);


    /*********************************
     * 8. START THE SYSTEM
     *********************************/
    System_Start();                                 // Starts the system.
    return 0;
}
