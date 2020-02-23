# ECC-RTLOS-2020 (v0.1.0, The First Release)

>This repository is continuously updated. It is still under the development process. All files will be changed continuously.

# ECC-RTLOS Components

### 1) BSP

The `BSP` is the `Board Support Package` (drivers). All files prefixed with the `BSP` are targeted to the `ECC Embedded Development Boards` that use the `PIC24FJ` series. Most of the functions that access to CPU and peripherals are implemented using `event-driven` and `callback operations`. There fully support real-time asynchronous operations.

### 2) RTL

The `RTL` is the `Real-Time Library`. All files prefixed with the `RTL` are designed and implemented to support real-time operations. All functions are implemented using `Overlapped Operation` or `Non-Blocking Operation`. They can operate on all embedded platforms. They are the hardware-independent modules.

### 3) RTOS

The `RTOS` is the modified version of the `FreeRTOS`. The core functions of the `FreeRTOS` are modified to perform with the `BSP` and `RTL`. All functions of the RTOS modules can be used as the same way as the standard `FreeRTOS`.

# ECC-RTLOS Modes

The `ECC-RTLOS` has three main modes can be configured to fit a user application. Changing the operation modes can be done by modifying one or two lines of code in the header file `ap.h`.

### Mode 1) Pure BSP

Use the `BSP` only, no `RTL`, no `RTOS`. In this mode, the system tick directly drives the ticked functions of all modules that need to be performed by the system tick.  See `ex01_bsp` for more details.

### Mode 2) BSP+RTL

Use the `BSP` and `RTL`, no `RTOS`. The RTL module is included in the application and it is directly driven by the system tick. See `ex02_rtl` for more details.

### Mode 3) BSP+RTL+RTOS

Use all the modules, `BSP`, `RTL` and `RTOS`. In this mode, the `BPS` and `RTL` modules are driven by a `CoRutine` task and `TickHook` function of the `FreeRTOS`. See `ex03_rtos` for more details.


# ECC-RTLOS Application Examples

### ex01_bsp
Using the `BPS`.

### ex02_rtl
Using the `RTL` with the `BSP`.

### ex03_rtos
Using the `RTOS` with the `BSP` and the `RTL`.

### ex04_pwm
Using the programable 4-channel PWMs.

### ex05_beep
Using the programable beep sound generator (PWM-based)

### ex06_adc
Using the event-driven based 4-channel ADCs.

### ex07_blink
Using the three techniques, `Flasher`, `Blinker`, and programable `PWM` to control the LED blinkig.

### Other examples will be provided here soon.

---

### ex01_bsp

```c
/************************************************************
 * Example 1. Using the BSP with the BSP System Tick        *
 ************************************************************
 * File:    main.c                                          *
 * Description:                                             *
 *          Demonstrates how to use the BSP with the SYS.   *
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
 *   2) Change value of the TARGET_EXAMPLE to 1.
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
 * Keys (PSW0,..., PSW3) changed callback function.
 ************************************************************/
void KeyChanged_Callback( void *event ) {
    switch_event_t *evt = (switch_event_t *)event;
    Uart1_Printf( "id: %d, stateCode: 0x%.2X, stateName: %s\r\n", evt->id, evt->state, evt->sname );
    if( evt->state == PSW_STATE_DOWN ) {
        Led_Inv(evt->id);
        Beep_Play(500 + (400*evt->id), 100);
    }
}


/************************************************************
 * Uarts (UART1 & UART2) ISR byte received callback function.
 * This function will be called direcly from the UART ISR.
 ************************************************************/
void Uarts_RxIsrCallback(void *event) {
    uart_event_t *evt = (uart_event_t *)event;                          // Get the uart isr event.
    Uart_Printf(evt->id, "UART%i-ISR: %c\r\n", evt->id, evt->byte);     // Print the information.

    Led_Flash(LED_ID_2, 200);
}


/************************************************************
 * Uarts (UART1 & UART2) byte received callback function.
 * This function will be called from the internal BSP executor.
 ************************************************************/
void Uarts_RxCallback(void *event) {
    uart_event_t *ue = (uart_event_t *)event;                           // Get the uart event.
    Uart_Printf(ue->id, "UART%i-STD: %c\r\n", ue->id, ue->byte);        // Print the information.

    Led_Flash(LED_ID_3, 200);
}


/************************************************************
 * Main function
 * Intitializes all peripherals and softwares modules.
 ************************************************************/
int main(void) {

    /*********************************
     * 1. INITIALIZE THE SYSTEM
     *********************************/
    System_Init();                                  // Initializes system modules.

    /*********************************
     * 2. UART1
     *********************************/
    Uart1_Init(115200, 64, 128);                    // Initializes Uart1 with the given baoudrate and buffer sizes of the RX and TX.
    Uart1_SetRxIsrCallback(Uarts_RxIsrCallback);    // Sets Rx ISR callback function of the Uart1.
    Uart1_SetRxCallback(Uarts_RxCallback);          // Sets callback function of the Uart1 Rx.
    Uart1_Printf("UART1, Example1: Using the BSP.\r\n");

    // Note: Tx callback functions of the Uart1 can be used if required.
    // Uart1_SetTxIsrCallback(Uarts_TxIsrCallback);
    // Uart1_SetTxCallback(Uarts_TxCallback);

    /*********************************
     * 3. UART2
     *********************************/
    Uart2_Init(115200, 64, 128);                    // Initializes Uart2 with the given baoudrate and buffer sizes of the RX and TX.
    Uart2_SetRxIsrCallback(Uarts_RxIsrCallback);    // Sets Rx ISR callback function of the Uart2.
    Uart2_SetRxCallback(Uarts_RxCallback);          // Sets callback function of the Uart2 Rx.
    Uart2_Printf("UART2, Example1: Using the BSP.\r\n");        // Prints a message to the Uart2.

    // Note: Tx callback functions of the Uart2 can be used if required.
    // Uart2_SetTxIsrCallback(Uarts_TxIsrCallback);
    // Uart2_SetTxCallback(Uarts_TxCallback);


    /*********************************
     * 4. SWITCHES
     *********************************/
    Psw_SetKeyChangedCallback(PSW_ID_0, KeyChanged_Callback);   // Add callback function of PSW0.
    Psw_SetKeyChangedCallback(PSW_ID_1, KeyChanged_Callback);   // Add callback function of PSW1.
    Psw_SetKeyChangedCallback(PSW_ID_2, KeyChanged_Callback);   // Add callback function of PSW2.
    Psw_SetKeyChangedCallback(PSW_ID_3, KeyChanged_Callback);   // Add callback function of PSW3.

    /*********************************
     * 5. START THE SYSTEM
     *********************************/
    System_Start();                                 // Starts the system.

    return 0;
}

```

---

### ex02_rtl

```c
/************************************************************
 * Example 2. Using the RTL, BSP and SYS                    *
 ************************************************************
 * File:    main.c                                          *
 * Description:                                             *
 *          Demonstrates how to use the RTL, BSP and SYS .  *
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
 *   2) Change value of the TARGET_EXAMPLE to 2.
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
 * Callback function of the software timers.
 ************************************************************/
void Timer_Callback(void *event) {
    timer_event_t *te = (timer_event_t *)event;                     // Get the timer event.
    Uart1_Printf( "TId: %i, TCnt: %d\r\n", te->id, te->counter);    // Print the information.
}

/************************************************************
 * Main function
 * Intitializes all peripherals and modules.
 ************************************************************/
int main(void) {

    /*********************************
     * 1. INITIALIZE THE SYSTEM
     *********************************/
    System_Init();                                  // Initializes system modules.

    /*********************************
     * 2. UART1
     *********************************/
    Uart1_Init(115200, 64, 128);                    // Initializes Uart1.
    Uart1_Printf("Example2: Using the RTL.\r\n");   // Prints a message.


    /*********************************
     * 3. CREATE SOFTWARE TIMERS
     *********************************/
    Timer_Create(1000,  Timer_Callback);            // Create a software timer.
    Timer_Create(1500,  Timer_Callback);            // Create a software timer.

    /*********************************
     * 4. START THE SYSTEM
     *********************************/
    System_Start();                                 // Starts the system.

    return 0;
}
```

---

### ex03_rtos

```c
/************************************************************
 * Example 3. Using the RTOS, BSP, and TRL libraries        *
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
 *   2) Change value of the TARGET_EXAMPLE to 3.
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
 * Callback function of the software timers.
 ************************************************************/
void Timer_Callback(void *event) {
    timer_event_t *te = (timer_event_t *)event; // Get the timer event.
    Uart1_Printf( "Timer: %i, Counter: %d\r\n", te->id, te->counter);   // Prints the information.
    Led_Inv(te->id);    // Toggle the LED spedified by the te->id
}


/************************************************************
 * Keys (PSW0,..., PSW3) changed callback function.
 ************************************************************/
void KeyChanged_Callback(void *event) {
    switch_event_t *evt = (switch_event_t *)event;   // Get the switch event.
    Uart1_Printf( "id: %d, stateCode: 0x%.2X, stateName: %s\r\n", evt->id, evt->state, evt->sname);    // Print the information.

    if( evt->state == PSW_STATE_DOWN ) {
        Beep_Play( 500 + (evt->id) * 300, 50 );
    }
}


/************************************************************
 * Uarts (UART1 & UART2) ISR byte received callback function.
 * This function will be called direcly from the UART ISR.
 ************************************************************/
void Uarts_RxCallback(void *event) {
    Uart1_Printf( "recv-std: %c\r\n", ((uart_event_t *)event)->byte);
}


/************************************************************
 * Uarts (UART1 & UART2) byte received callback function.
 * This function will be called from the internal BSP executor.
 ************************************************************/
void Uarts_RxIsrCallback(void *event) {
    Uart1_Printf( "recv-isr: %c\r\n", ((uart_event_t *)event)->byte);
}


/************************************************************
 * Main function
 * Intitializes all peripherals and softwares modules.
 ************************************************************/
int main(void) {

    /*********************************
     * 1. INITIALIZE THE SYSTEM
     *********************************/
    System_Init();                                  // Initializes system modules.


    /*********************************
     * 2. UART1
     *********************************/
    Uart1_Init(115200, 64, 128);                    // Initializes Uart1.
    Uart1_SetRxIsrCallback(Uarts_RxIsrCallback);    // Sets Rx ISR callback function of the Uart1.
    Uart1_SetRxCallback(Uarts_RxCallback);          // Sets callback function of the Uart1 Rx.
    Uart1_Printf("Example 3: Using the RTOS, BSP and RTL.\r\n");   // Prints a message.


    /*********************************
     * 3. Create Software Timers
     *********************************/
    Timer_Create(200,  Timer_Callback);             // Create a software timer.
    Timer_Create(500,  Timer_Callback);             // Create a software timer.
    Timer_Create(700,  Timer_Callback);             // Create a software timer.
    Timer_Create(900,  Timer_Callback);             // Create a software timer.


    /*********************************
     * 5. SWITCHES
     *********************************/
    Psw_SetKeyChangedCallback(PSW_ID_0, KeyChanged_Callback);   // Add callback function of PSW0.
    Psw_SetKeyChangedCallback(PSW_ID_1, KeyChanged_Callback);   // Add callback function of PSW1.
    Psw_SetKeyChangedCallback(PSW_ID_2, KeyChanged_Callback);   // Add callback function of PSW2.
    Psw_SetKeyChangedCallback(PSW_ID_3, KeyChanged_Callback);   // Add callback function of PSW3.


    /*********************************
     * 6. START THE SYSTEM
     *********************************/
    System_Start();                                 // Starts the system.

    return 0;
}
```

---

### ex04_pwm

```c
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
```

---

### ex05_beep

```c
/************************************************************
 * Example 5. PWM-based beep sound generator                *
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
 *   2) Change value of the TARGET_EXAMPLE to 5.
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
 * Simple RTOS Task
 ************************************************************/
void Beep_TaskCode( void * pvParameters ) {
    for(;;) {
        Led_Inv(LED_ID_0);
        vTaskDelayMs(500);
    }
}

/************************************************************
 * Beep Ending Callback (Play notes)
 ************************************************************/
void Beep_Callback( void *event ) {

    const float Notes[] = {
        BEEP_Do, BEEP_Re, BEEP_Me, BEEP_Fa,
        BEEP_So, BEEP_La, BEEP_Si, BEEP_DO
    };

    beep_event_t *evt = (beep_event_t *)event;  // Get event.

    uint16_t idx = evt->counter%8;  // Index (the counter is increased by 1 evey time the callback is performed)
    float freq   = Notes[idx]*1.0;  // Frequency
    Beep_Play(freq, 200 + idx*10);  // Play the target note.

    // Note:
    //   This function requires a lot of requorces and activates Tx ISR.
    //   To avoid the stack overflow, it should be called at the last line,
    //   or apply it in the PERFORM_CRITICAL_SECTION()
    Uart1_Printf("idx: %d, freq: %4.2f\r\n", idx, freq);
}


/************************************************************
 * Main function
 * Intitializes all peripherals and modules.
 ************************************************************/
int main(void) {

    /*********************************
     * 1. INITIALIZE THE SYSTEM
     *********************************/
    System_Init();

    /*********************************
     * 2. UART1
     *********************************/
    Uart1_Init(115200, 128, 128);
    Uart1_Printf("Example 5: Beep Sound\r\n");

    /*********************************
     * 3. BEEP
     *********************************/
    Beep_SetCallback(Beep_Callback);

    /************************************************
     * 4. Create task (If the task uses printf(), or
     * complex methematic operations, the stack size
     * should be larg enough, e.e., 256 or 512 bytes
     ************************************************/
    xTaskCreate(Beep_TaskCode, "Beep", 512, NULL, tskIDLE_PRIORITY, NULL);

    /*********************************
     * 8. START THE SYSTEM
     *********************************/
    System_Start();                                 // Starts the system.

    return 0;
}
```

---

### ex06_adc

```c
/************************************************************
 * Example 6. ADCs                                          *
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
 *   2) Change value of the TARGET_EXAMPLE to 6.
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
 * RTOS Task used to read all channels of ADCs.
 ************************************************************/
void ADC_TaskCode( void * pvParameters ) {
    for( ;; ) {
        Uart1_Printf("TSK: %3d %3d %3d %3d\r\n", Adc_Get(0), Adc_Get(1), Adc_Get(2), Adc_Get(3));
        Led_Set(LED_ID_0); vTaskDelayMs(50);
        Led_Clr(LED_ID_0); vTaskDelayMs(950);
    }
}

/************************************************************
 * Callback function (OnChanedCallback), called when value is changed.
 ************************************************************/
void ADC_Changed( void *event ) {

    adc_event_t *evt = (adc_event_t *)event;

    if( evt->direction > 0 ) {
        Beep_Play(1200, 50);    // High frequency beep
        Led_Set(LED_ID_1);

    }
    else {
        Beep_Play(500, 50);     // Low frequency beep
        Led_Clr(LED_ID_1);
    }

    // Note:
    //   Be careful to use the Uart1_Printf() with out critical section protection.
    Uart1_Printf("CBK: %d %4d %3d (%c)\r\n", evt->id, evt->value, evt->delta, evt->direction>0?'+':'-');
}

/************************************************************
 * Main function
 * Intitializes all peripherals and modules.
 ************************************************************/
int main(void) {

    /*********************************
     * 1. INITIALIZE THE SYSTEM
     *********************************/
    System_Init();

    /*********************************
     * 2. UART1
     *********************************/
    Uart1_Init(115200, 128, 128);
    Uart1_Printf("Example 6: ADCs\r\n");

    /*********************************
     * 3. Changed Callbacks
     *********************************/
    Adc_SetChangedCallback(ADC_ID_0, ADC_Changed);  // Add callback function to channel 0
    Adc_SetChangedCallback(ADC_ID_1, ADC_Changed);  // Add callback function to channel 1
    Adc_SetChangedCallback(ADC_ID_2, ADC_Changed);  // Add callback function to channel 2
    Adc_SetChangedCallback(ADC_ID_3, ADC_Changed);  // Add callback function to channel 3

    /*********************************
     * 4. Changed Interval and Threahold
     *    ADC1 connected to LDR.
     *********************************/
    Adc_SetChangedInterval(ADC_ID_1, 5);    // 5mS
    Adc_SetChangedThreshold(ADC_ID_1, 20);  // Threahold value


    /************************************************
     * 5. Create task (If the task uses printf(), or
     * complex methematic operations, the stack size
     * should be larg enough, e.e., 256 or 512 bytes
     ************************************************/
    xTaskCreate(ADC_TaskCode, "ADC", 512, NULL, tskIDLE_PRIORITY, NULL);

    /*********************************
     * 6. START THE SYSTEM
     *********************************/
    System_Start();                                 // Starts the system.

    return 0;
}
```

---

### ex07_blink

```c
/************************************************************
 * Example 7. Blinker                                       *
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
 *   2) Change value of the TARGET_EXAMPLE to 7.
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
 * RTOS Task calls the Flashers and Blinkers
 ************************************************************/
void Blink_Flash_TaskCode( void * pvParameters ) {

    for(;;) {

        /****************************************************
         * Flash (Uncomment to check the result)
         ****************************************************/
        /*
        Led_Flash(LED_ID_0, 100);
        Led_Flash(LED_ID_1, 200);
        Led_Flash(LED_ID_2, 300);
        Led_Flash(LED_ID_3, 400);
        */

        /****************************************************
         * Blink (Uncomment to check the result)
         ****************************************************/
        /*
        Led_Blink(LED_ID_0, 100, 10);
        Led_Blink(LED_ID_1, 200, 10);
        Led_Blink(LED_ID_2, 300, 10);
        Led_Blink(LED_ID_3, 400, 10);
        */

        vTaskDelayMs(1000);
    }
}


/************************************************************
 * State Changed Callback for all LEDs
 ************************************************************/
void LED_Callback(void *event) {

    led_event_t *evt = (led_event_t *)event;

    if(evt->state == LED_STATE_BEGIN) {
        //Beep_Play(500+((evt->counter*20)%1000), 20+((evt->counter*4)%200));
    }

    // Note:
    //   Be careful to use the Uart1_Printf() with out critical section protection.
    Uart1_Printf("led: %d, sta: 0x%.2X, cnt: %d\r\n", evt->id, evt->state, evt->counter);
}

/************************************************************
 * Main function
 * Intitializes all peripherals and modules.
 ************************************************************/
int main(void) {

    /*********************************
     * 1. INITIALIZE THE SYSTEM
     *********************************/
    System_Init();

    /*******************************************************
     * 2. UART1
     *******************************************************/
    Uart1_Init(115200, 128, 128);
    Uart1_Printf("Example 8: Flasher\r\n");

    /*******************************************************
     * 3. LED Blinking with PWM
     *******************************************************/
    Led_Pwm(LED_ID_0, 0,   10, 500);
    Led_Pwm(LED_ID_1, 100, 10, 500);
    Led_Pwm(LED_ID_2, 200, 10, 500);
    Led_Pwm(LED_ID_3, 300, 10, 500);

    /*******************************************************
     * 4. State changed callbacks
     *******************************************************/
    Led_SetChangedCallback(LED_ID_0, LED_Callback);
    Led_SetChangedCallback(LED_ID_1, LED_Callback);
    Led_SetChangedCallback(LED_ID_2, LED_Callback);
    Led_SetChangedCallback(LED_ID_3, LED_Callback);


    /************************************************
     * 5. Create task (If the task uses printf(), or
     * complex methematic operations, the stack size
     * should be larg enough, e.e., 256 or 512 bytes
     ************************************************/
    xTaskCreate(Blink_Flash_TaskCode, "Blink", 512, NULL, tskIDLE_PRIORITY, NULL);

    /*********************************
     * 6. START THE SYSTEM
     *********************************/
    System_Start();                                 // Starts the system.

    return 0;
}
```

> Please check in the `examples` directory for more details.

---
