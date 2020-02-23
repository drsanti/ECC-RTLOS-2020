/************************************************************
 * File:    app.h (used for all examples)                   *
 * Description:                                             *
 *          Demonstrates how to use the RTOS, BSP and RTL.  *
 *          (Please see closely in the app.h and config.cfg)*
 * Author:  Asst.Prof.Dr.Santi Nuratch                      *
 *          Embedded Computing and Control Laboratory       *
 *          ECC-Lab, INC, LMUTT, Thailand                   *
 ************************************************************
 * Update:  23 February 2020                                *
 ************************************************************/

#ifndef ECC_APP_CONFIGURATION
#define ECC_APP_CONFIGURATION

/************************************************************
 * TARGET APPLICATION TO BE COMPILED
 ************************************************************/
// #define TARGET_EXAMPLE           1   /* BSP, SYS         */
// #define TARGET_EXAMPLE           2   /* BSP, SYS, TRL    */
// #define TARGET_EXAMPLE           3   /* BSP, RTL, RTOS   */
// #define TARGET_EXAMPLE           4   /* BSP, RTOS        */
// #define TARGET_EXAMPLE           5   /* BSP, RTOS        */
// #define TARGET_EXAMPLE           6   /* BSP, RTOS        */
#define TARGET_EXAMPLE              7   /* BSP, RTOS        */




/************************************************************
 * No RTOS
 ************************************************************/
#if TARGET_EXAMPLE > 2
    #define ECC_SYSTEM_USE_RTOS     1
#else
    #define ECC_SYSTEM_USE_RTOS     0
#endif

/************************************************************
 * No RTL
 ************************************************************/
#if TARGET_EXAMPLE == 2 || TARGET_EXAMPLE == 3
    #define ECC_SYSTEM_USE_RTL      1
#else
    #define ECC_SYSTEM_USE_RTL      0
#endif


#endif // ECC_APP_CONFIGURATION
