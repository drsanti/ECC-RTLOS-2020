/************************************************************
 * File:    ecc.h (system core)                             *
 * Description:                                             *
 *          Demonstrates how to use the RTOS, BSP and RTL.  *
 *          (Please see closely in the app.h and config.cfg)*
 * Author:  Asst.Prof.Dr.Santi Nuratch                      *
 *          Embedded Computing and Control Laboratory       *
 *          ECC-Lab, INC, KMUTT, Thailand                   *
 ************************************************************
 * Update:  23 February 2020                                *
 ************************************************************/

#ifndef __ECC_H__
    #define __ECC_H__

    /************************************************************
     * These two lines must be defined in the app.h
     ************************************************************/
    //#define ECC_SYSTEM_USE_RTL      0
    //#define ECC_SYSTEM_USE_RTOS     0

    #include <app.h>
    #include <bsp.h>

    #if ECC_SYSTEM_USE_RTL > 0
        #include <rtl.h>
    #endif

    #if ECC_SYSTEM_USE_RTOS > 0
        #include <rtos.h>
        void System_StartCoRutine(void);

        #define vTaskDelayMs(ms) vTaskDelay(ms / portTICK_PERIOD_MS)

    #endif

    #if ECC_SYSTEM_USE_RTOS > 0
        #define System_Init() {		        \
        	Mcu_Init();				        \
            Beep_Init();                    \
            Led_BlinkInit();                \
            Adc_Init();                     \
        }
        #define System_Start(){		        \
            System_StartCoRutine();         \
        	vTaskStartScheduler();          \
        }
    #else
        #define System_Init() {		        \
        	Mcu_Init();				        \
            Beep_Init();                    \
            Led_BlinkInit();                \
            Adc_Init();                     \
        	System_TimerInit();		        \
        }
        #if ECC_SYSTEM_USE_RTL > 0
            #define System_Start(){		    \
            	while(1) {				    \
                    BSP_Executor();		    \
                    RTL_Executor();		    \
                }						    \
            }
        #else
           #define System_Start(){		    \
            	while(1) {				    \
                    BSP_Executor();		    \
                }						    \
            }
        #endif
    #endif

#endif // __ECC_H__
