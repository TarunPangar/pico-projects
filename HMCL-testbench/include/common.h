#ifndef __COMMON_H__
#define __COMMON_H__

#define LED_PIN     25
#define GPIO1_PIN   1
#define GPIO2_PIN   2
#define MAX_PWM     ((1U << 16) - 1)

typedef enum sensor_types {
    NTC_TEMP1 = 1,
    NTC_TEMP2,
    NTC_TEMP3,
    CURR_HR,
    CURR_LR,
    PDU_TEMP,
    MAX_SENSORS
} sensor_types_e;

extern const char* sensorNames[];

typedef struct sensor_val {
    int id;
    float value;
} sensor_val_t;

#endif
