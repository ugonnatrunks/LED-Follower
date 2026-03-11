#include "app_config.h"

// Short-on / Long-off
void blink_pattern3(int blink_rate_multiplier) {
    gpio_pin_set_dt(&LED2, 1);
    k_msleep(blink_pattern_rate_get(MEDIUM_PAUSE, blink_rate_multiplier));
    gpio_pin_set_dt(&LED2, 0);
    k_msleep(blink_pattern_rate_get(LONG_PAUSE, blink_rate_multiplier));
}
// Heartbeat Blink (Double Pulse)
void blink_pattern1(int blink_rate_multiplier) {
    gpio_pin_set_dt(&LED2, 1);
    k_msleep(blink_pattern_rate_get(SHORT_PAUSE, blink_rate_multiplier));
    gpio_pin_set_dt(&LED2, 0);
    k_msleep(blink_pattern_rate_get(MEDIUM_PAUSE, blink_rate_multiplier));
    gpio_pin_set_dt(&LED2, 1);
    k_msleep(blink_pattern_rate_get(SHORT_PAUSE, blink_rate_multiplier));
    gpio_pin_set_dt(&LED2, 0);
    k_msleep(blink_pattern_rate_get(LONG_PAUSE, blink_rate_multiplier));
}

// Blink-Hold (Activity + Ready Indicator)
void blink_pattern2(int blink_rate_multiplier) {
    gpio_pin_set_dt(&LED2, 1);
    k_msleep(blink_pattern_rate_get(MEDIUM_PAUSE, blink_rate_multiplier));
    gpio_pin_set_dt(&LED2, 0);
    k_msleep(blink_pattern_rate_get(MEDIUM_PAUSE, blink_rate_multiplier));
    gpio_pin_set_dt(&LED2, 1);
    k_msleep(blink_pattern_rate_get(MEDIUM_PAUSE, blink_rate_multiplier));
    gpio_pin_set_dt(&LED2, 0);
    k_msleep(blink_pattern_rate_get(MEDIUM_PAUSE, blink_rate_multiplier));
    gpio_pin_set_dt(&LED2, 1);
    k_msleep(blink_pattern_rate_get(INDICATOR_PAUSE, blink_rate_multiplier));
    gpio_pin_set_dt(&LED2, 0);
    k_msleep(blink_pattern_rate_get(INDICATOR_PAUSE, blink_rate_multiplier));
}

// Blink to give a sense of time
void time_indicator_pattern(void) {
    gpio_pin_set_dt(&LED4, 1);
    k_msleep(INDICATOR_PAUSE);
    gpio_pin_set_dt(&LED4, 0);
    k_msleep(INDICATOR_PAUSE);
}

// Functions for safely managing the blink rate multiplier
void blink_rate_set(int rate) {
    k_mutex_lock(&LED2_mutex, K_FOREVER);
    blink_rate_multiplier = rate;
    k_mutex_unlock(&LED2_mutex);
}

int blink_rate_get(void) {
    k_mutex_lock(&LED2_mutex, K_FOREVER);
    float rate = blink_rate_multiplier;
    k_mutex_unlock(&LED2_mutex);
    return rate;
}

// Preparing the blink rate for use by the main pattern functions
int blink_pattern_rate_get(int pause, int blink_rate_multiplier) {
    int pause_time = pause;
    if (blink_rate_multiplier > 2) {
        pause_time /= 2;
        return pause_time;
    } else {
        pause_time *= blink_rate_multiplier;
        return pause_time;
    }
}