#include "app_config.h"

K_MUTEX_DEFINE(LED2_mutex);
K_EVENT_DEFINE(BUTTON2_Pressed_Event);
K_EVENT_DEFINE(BUTTON4_Pressed_Event);

int blink_rate_multiplier = NORMAL_RATE;
static int i = 0;
static int idx = 1;

/*************************Thread running LED4 to give a sense of time**********************************/
K_THREAD_DEFINE(LED4_Patterns_id, MY_STACK_SIZE, default_time_indicator, NULL, NULL, NULL, MY_PRIORITY, 0, 0);
void default_time_indicator(void *p1, void *p2, void *p3) {
    while (true) {
        time_indicator_pattern();
        k_yield();
    }
}

/*************************Thread running the three patterns on LED2*****************************************/
K_THREAD_DEFINE(LED2_Patterns_id, MY_STACK_SIZE, patterns_on_LED2, NULL, NULL, NULL, MY_PRIORITY, 0, 0);
void patterns_on_LED2(void *p1, void *p2, void *p3) {
    static int use_blink_rate;
    uint32_t event_holder;

    while (true) {
        use_blink_rate = blink_rate_get();
        event_holder = k_event_test(&BUTTON2_Pressed_Event, BUTTON2_PATTERN1_BIT | BUTTON2_PATTERN2_BIT | BUTTON2_PATTERN3_BIT);
        if ( (k_event_test(&BUTTON2_Pressed_Event, BUTTON2_PATTERN1_BIT)) == FALSE) {
            blink_pattern1(use_blink_rate);
            printk("\033[1;1HPattern 1 Executing!");
        }
        else if ( (k_event_test(&BUTTON2_Pressed_Event, BUTTON2_PATTERN2_BIT | BUTTON2_PATTERN1_BIT)) == (BUTTON2_PATTERN2_BIT | BUTTON2_PATTERN1_BIT)) {
            blink_pattern2(use_blink_rate);
            printk("\033[1;1HPattern 2 Executing!");
        }
        else if ( (k_event_test(&BUTTON2_Pressed_Event, BUTTON2_PATTERN3_BIT)) == BUTTON2_PATTERN3_BIT) {
            blink_pattern3(use_blink_rate);
            printk("\033[1;1HPattern 3 Executing!");
        }
    }
}

/*************************Button2 Pressed ISR/Event********************************************************/
void button2_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins) 
{
    k_event_post(&BUTTON2_Pressed_Event, BUTTON2_EVENT_BIT);
}

/*************************Button4 Pressed ISR/Event********************************************************/
void button4_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins) 
{
    k_event_post(&BUTTON4_Pressed_Event, BUTTON4_EVENT_BIT);
} 

/*************************Button ISR Initializations********************************************************/
K_THREAD_DEFINE(ISRs_thread_id, MY_STACK_SIZE, ISRs_Thread, NULL, NULL, NULL, MY_PRIORITY, 0, 0);
void ISRs_Thread(void *p1, void *p2, void *p3) {
    static struct gpio_callback button2_cb;
    gpio_init_callback(&button2_cb, button2_pressed, BIT(BUTTON2.pin));
    gpio_add_callback(BUTTON2.port, &button2_cb);

    static struct gpio_callback button4_cb;
    gpio_init_callback(&button4_cb, button4_pressed, BIT(BUTTON4.pin));
    gpio_add_callback(BUTTON4.port, &button4_cb);
}

/*************************Button2 Event handler thread*************************************************************/
K_THREAD_DEFINE(LED2_Pattern_Events_id, MY_STACK_SIZE, button2_event_handler, NULL, NULL, NULL, MY_PRIORITY, 0, 0);
void button2_event_handler(void *p1, void *p2, void *p3) {

    while (true) {
        uint32_t button_event = k_event_wait(&BUTTON2_Pressed_Event, BUTTON2_EVENT_BIT, false, K_FOREVER);
        if (button_event & BUTTON2_EVENT_BIT) {
            k_msleep(50);
            printk("\033[2;1HButton 2 Pressed, Pattern Changed!");
            switch (idx) {
            case 1:
                idx++;
                k_event_post(&BUTTON2_Pressed_Event, BUTTON2_PATTERN1_BIT | BUTTON2_PATTERN2_BIT);
                k_event_set_masked(&BUTTON2_Pressed_Event, LOW, BUTTON2_EVENT_BIT);
                k_msleep(WAIT_TO_CLEAR_MESSAGE);
                printk("\033[2;1H\033[2K");
                continue;
            case 2:
                idx++;
                k_event_set_masked(&BUTTON2_Pressed_Event, LOW, BUTTON2_PATTERN2_BIT);
                k_event_post(&BUTTON2_Pressed_Event, BUTTON2_PATTERN3_BIT);
                k_event_set_masked(&BUTTON2_Pressed_Event, LOW, BUTTON2_EVENT_BIT);
                k_msleep(WAIT_TO_CLEAR_MESSAGE);
                printk("\033[2;1H\033[2K"); 
                continue;           
            case 3:
                idx = 1;
                k_event_set_masked(&BUTTON2_Pressed_Event, LOW, BUTTON2_PATTERN1_BIT | BUTTON2_PATTERN2_BIT | BUTTON2_PATTERN3_BIT);
                k_event_set_masked(&BUTTON2_Pressed_Event, LOW, BUTTON2_EVENT_BIT);
                k_msleep(WAIT_TO_CLEAR_MESSAGE);
                printk("\033[2;1H\033[2K");
                continue;
            }
        }
    }  
}

/*************************Button4 Event handler thread*************************************************************/
K_THREAD_DEFINE(LED4_Pattern_Events_id, MY_STACK_SIZE, button4_event_handler, NULL, NULL, NULL, MY_PRIORITY, 0, 0);
void button4_event_handler(void *p1, void *p2, void *p3) {
    int rates[3] = { FAST_RATE, NORMAL_RATE, SLOW_RATE };

    while (true) {
        uint32_t button_event = k_event_wait(&BUTTON4_Pressed_Event, BUTTON4_EVENT_BIT, true, K_FOREVER);
        if (button_event & BUTTON4_EVENT_BIT) {
            k_msleep(50);
            if (i < INDEX_OF_STATES) {
                i++;
                blink_rate_set(rates[i]);
            } else {
                i = 0;
                blink_rate_set(rates[i]);
            }
        }
        printk("\033[2;1HButton 4 Pressed, Rate Changed!");
        k_msleep(WAIT_TO_CLEAR_MESSAGE);
        printk("\033[2;1H\033[2K");
    }
}
