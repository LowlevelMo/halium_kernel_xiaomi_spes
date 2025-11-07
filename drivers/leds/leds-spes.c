#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/leds.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_gpio.h>

static int gpio_a = -1;  // GPIO for brightness < 50
static int gpio_b = -1;  // GPIO for brightness >= 50

static void flashlight_brightness_set(struct led_classdev *led_cdev, enum led_brightness brightness)  // Fixes return type
{
    if (brightness > 0 && brightness < 50) {
        gpio_set_value(gpio_a, 1);
        gpio_set_value(gpio_b, 0);
    } else if (brightness >= 50) {
        gpio_set_value(gpio_a, 0);
        gpio_set_value(gpio_b, 1);
    } else {
        gpio_set_value(gpio_a, 0);
        gpio_set_value(gpio_b, 0);
    }
}

static struct led_classdev flashlight_led = {
    .name = "led:torch",
    .brightness_set = flashlight_brightness_set,
};

static int __init flashlight_init(void)
{
    struct device_node *np;
    int ret;

    np = of_find_compatible_node(NULL, NULL, "qcom,camera-flash");
    if (!np)
        return -ENODEV;

    gpio_a = of_get_named_gpio(np, "qcom,flash-gpios", 0);
    gpio_b = of_get_named_gpio(np, "qcom,flash-gpios", 1);

    if (!gpio_is_valid(gpio_a) || !gpio_is_valid(gpio_b)) {
        pr_err("Flashlight: Invalid GPIOs\n");
        return -EINVAL;
    }

    ret = gpio_request(gpio_a, "flashlight_gpio_a");
    if (ret) {
        pr_err("Failed to request GPIO A\n");
        return ret;
    }
    ret = gpio_request(gpio_b, "flashlight_gpio_b");
    if (ret) {
        gpio_free(gpio_a);
        pr_err("Failed to request GPIO B\n");
        return ret;
    }

    gpio_direction_output(gpio_a, 0);
    gpio_direction_output(gpio_b, 0);

    return led_classdev_register(NULL, &flashlight_led);
}

static void __exit flashlight_exit(void)
{
    led_classdev_unregister(&flashlight_led);
    gpio_free(gpio_a);
    gpio_free(gpio_b);
}

late_initcall(flashlight_init);
