This is testing out PWM on GPIO2 of the ESP-01 board.

I'm sure I was just doing something wrong but I couldnt get it to work straight up so I made some changes...

I made the below change in pwm.h:
#define PWM_0_OUT_IO_NUM 12
#define PWM_0_OUT_IO_FUNC FUNC_GPIO12

change to:
\#define PWM_0_OUT_IO_NUM 2
\#define PWM_0_OUT_IO_FUNC FUNC_GPIO2

In user_main.c:

pwm_init(500, 0);
pwm_set_freq((uint16_t)myFreq, 0);
pwm_start();

Now the pwm_init command is not right… the code in pwm.c expect an array to be passed instead of just ‘0’ but I found when doing that I receive: Fatal Exception(28) every time so I made an additional change in pwm.c.
From:


LOCAL void ICACHE_FLASH_ATTR
pwm_set_freq_duty(uint16 freq, uint8 *duty)
{
uint8 i;

pwm_set_freq(freq);

for (i = 0; i < PWM_CHANNEL; i++) {
pwm_set_duty(duty[i], i);
}
}

to:

LOCAL void ICACHE_FLASH_ATTR
pwm_set_freq_duty(uint16 freq, uint8 *duty)
{
uint8 i;
pwm_set_freq(freq);

for (i = 0; i < PWM_CHANNEL; i++) {
pwm_set_duty(0, i);
}
}
