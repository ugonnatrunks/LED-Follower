#ifndef _APP_CONFIG_
#define _APP_CONFIG_

#include <stdio.h>
#include <zephyr/smf.h> 
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>

#define SLEEP_TIME                                  2000
#define MY_STACK_SIZE                               1024
#define MY_PRIORITY                                 5
#define HIGH_PRIORITY                               3

#define SHORT_PAUSE                                 50
#define MEDIUM_PAUSE                                100
#define LONG_PAUSE                                  800
#define INDICATOR_PAUSE                             500
#define INDEX_OF_STATES                             2
#define WAIT_TO_CLEAR_MESSAGE                       1500

#define NORMAL_RATE                                 1
#define FAST_RATE                                   3
#define SLOW_RATE                                   2

int blink_rate_get(void);
void blink_rate_set(int rate);
extern int blink_rate_multiplier;
int blink_pattern_rate_get(int f_rate, int blink_rate_multiplier);

extern const struct gpio_dt_spec LED2;
extern const struct gpio_dt_spec LED4;
extern const struct gpio_dt_spec BUTTON2;
extern const struct gpio_dt_spec BUTTON4;

extern struct k_mutex LED2_mutex;
extern struct k_event BUTTON2_Pressed_Event;
extern struct k_event BUTTON4_Pressed_Event;

#define BUTTON2_EVENT_BIT                           BIT(0)
#define BUTTON2_PATTERN1_BIT                        BIT(1)
#define BUTTON2_PATTERN2_BIT                        BIT(2)
#define BUTTON2_PATTERN3_BIT                        BIT(3)
#define BUTTON4_EVENT_BIT                           BIT(0)
#define RESET_BIT                                   BIT(4)

void ISRs_Thread(void *p1, void *p2, void *p3);
void patterns_on_LED2(void *p1, void *p2, void *p3);
void GPIO_initializations(void *p1, void *p2, void *p3);
void button2_event_handler(void *p1, void *p2, void *p3);
void button4_event_handler(void *p1, void *p2, void *p3);
void default_time_indicator(void *p1, void *p2, void *p3);

void time_indicator_pattern(void);
void blink_pattern1(int blink_rate_multiplier);
void blink_pattern2(int blink_rate_multiplier);
void blink_pattern3(int blink_rate_multiplier);

// -----------------------------
// Logic Levels and Booleans
// -----------------------------
#define ON              (1u)
#define OFF             (0u)

#define TRUE            (1u)
#define FALSE           (0u)

#define ENABLED         (1u)
#define DISABLED        (0u)

#define HIGH            (1u)
#define LOW             (0u)

#endif