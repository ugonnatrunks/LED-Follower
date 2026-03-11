#include "app_config.h"
K_THREAD_DEFINE(GPIO_initializations_id, MY_STACK_SIZE, GPIO_initializations, NULL, NULL, NULL, HIGH_PRIORITY, 0, 0);

const struct gpio_dt_spec BUTTON2 = GPIO_DT_SPEC_GET(DT_NODELABEL(button1), gpios);
const struct gpio_dt_spec BUTTON4 = GPIO_DT_SPEC_GET(DT_NODELABEL(button3), gpios);
const struct gpio_dt_spec LED2 = GPIO_DT_SPEC_GET(DT_NODELABEL(led1), gpios);
const struct gpio_dt_spec LED4 = GPIO_DT_SPEC_GET(DT_NODELABEL(led3), gpios);

int ret;

void GPIO_initializations(void *p1, void *p2, void *p3) {

if ((!gpio_is_ready_dt(&BUTTON2)) || (!gpio_is_ready_dt(&BUTTON4)) || (!gpio_is_ready_dt(&LED2)) || (!gpio_is_ready_dt(&LED4))) {
    printk("A GPIO is not ready!\n");
}

if ((gpio_pin_configure_dt(&BUTTON2, GPIO_INPUT)) || (gpio_pin_configure_dt(&BUTTON4, GPIO_INPUT)) 
    || (gpio_pin_configure_dt(&LED2, GPIO_OUTPUT)) || (gpio_pin_configure_dt(&LED4, GPIO_OUTPUT))) {
        printk("A GPIO pin was not successfully configured!\n");
    }

if ( (gpio_pin_interrupt_configure_dt(&BUTTON2, GPIO_INT_EDGE_TO_INACTIVE)) || (gpio_pin_interrupt_configure_dt(&BUTTON4, GPIO_INT_EDGE_TO_INACTIVE))) {
        printk("GPIO Interrupt to successfully configured!\n");
    }
}