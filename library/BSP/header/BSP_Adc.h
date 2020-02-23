/************************************************************
 * File:    BSP_Adc.h                                       *
 * Author:  Asst.Prof.Dr.Santi Nuratch                      *
 *          Embedded Computing and Control Laboratory       *
 *          ECC-Lab, INC, KMUTT, Thailand                   *
 * Update:  22 February 2020                                *
 ************************************************************/

#ifndef __ADC_H_ADC__

    #define __ADC_H_ADC__

    #include <BSP_Mcu.h>

    /*******************************************************
     * ADC Channel ID
     *******************************************************/
    #define ADC_ID_0            0       // ADC Channel 0.
    #define ADC_ID_1            1       // ADC Channel 1.
    #define ADC_ID_2            2       // ADC Channel 2.
    #define ADC_ID_3            3       // ADC Channel 3.

    /*******************************************************
     * ADC TOTAL CHANNELS
     *******************************************************/
    #define ADC_NUM_CHANNELS    4       // Number of channel.

    /*******************************************************
     * ADC VOLTAGE RANGE
     *******************************************************/
    #define ADC_VOLTAGE_MAX     3.3     // Maximum voldate
    #define ADC_VOLTAGE_MIN     0.0     // Minumum voltage
    #define ADC_VOLTAGE_REF     0.0     // Refferent voltage

    /*******************************************************
     * ADC OBJECT STRUCTURE
     *******************************************************/
    typedef struct {
        int16_t     id;                 // Id of ADC.
        int16_t     value;              // 10-bit current value.
        int16_t     previous;           // 10-bit previous value.
        int16_t     delta;              // Delta value.
        uint16_t    ticks;              // Tick counter.
        int16_t     direction;          // Changed direction (+1: Up, -1: Down).
        uint16_t    threshold;          // Changed threshold.
        uint16_t    interval;           // Changed calculation interval.
        callback_t  callback;           // Changed callback function.
    }adc_t;

    /*******************************************************
     * ADC EVENT STRUCTURE
     *******************************************************/
    typedef struct {
        uint16_t    id;                 // Id of ADC.
        int16_t     value;              // 10-bit current value.
        int16_t     delta;              // Delta value.
        int16_t     direction;          // Changed direction (+1: Up, -1: Down).
        adc_t       *sender;            // ADC object.
    }adc_event_t;




    void Adc_Init(void);

    int16_t Adc_Get(uint16_t id);
    float Adc_GetVoltage(uint16_t id);
    void Adc_SetChangedCallback(uint16_t id, callback_t callback);
    void Adc_SetChangedThreshold(uint16_t id, uint16_t threshold);
    void Adc_SetChangedInterval(uint16_t id, uint16_t interval);

    inline void ADC_TickedExecutor(void);

#endif // __ADC_H_ADC__
