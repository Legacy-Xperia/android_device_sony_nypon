char const*const RED_LED_FILE = "/sys/class/leds/red/brightness";
char const*const GREEN_LED_FILE = "/sys/class/leds/green/brightness";
char const*const BLUE_LED_FILE = "/sys/class/leds/blue/brightness";

char const*const LED_FILE_TRIGGER[] = {
 "/sys/class/leds/red/trigger",
 "/sys/class/leds/green/trigger",
 "/sys/class/leds/blue/trigger",
};

char const*const LED_FILE_DELAYON[] = {
 "/sys/class/leds/red/delay_on",
 "/sys/class/leds/green/delay_on",
 "/sys/class/leds/blue/delay_on",
};

char const*const LED_FILE_DELAYOFF[] = {
 "/sys/class/leds/red/delay_off",
 "/sys/class/leds/green/delay_off",
 "/sys/class/leds/blue/delay_off",
};

char const*const BUTTON_BACKLIGHT_FILE[] = {
};
// "/sys/class/leds/so34-led0/brightness",
// "/sys/class/leds/so34-led1/brightness",
// "/sys/class/leds/so34-led2/brightness",

char const*const ON = "timer";
char const*const OFF = "timer";
