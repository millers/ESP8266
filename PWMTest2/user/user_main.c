#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_config.h"
#include "user_interface.h"
#include "driver/uart.h"

// There are 16 "virtual" PWM channels (PWM0...PWM15)
#define PLATFORM_PWM_TOTAL                    16
// TODO: PLATFORM_PWM_TOTAL is not used - figure out purpose, or remove?

#define NORMAL_PWM_DEPTH  PWM_DEPTH
#define NORMAL_DUTY(d) (((unsigned)(d)*NORMAL_PWM_DEPTH) / PWM_DEPTH)
#define DUTY(d) ((uint16_t)( ((unsigned)(d)*PWM_DEPTH) / NORMAL_PWM_DEPTH) )

os_event_t    user_procTaskQueue[user_procTaskQueueLen];
static volatile os_timer_t channelHop_timer;
LOCAL os_timer_t hello_timer;



int myDuty = 50;
int dir = 10;
uint8 PWM_CH[] = {0,0,0};
uint16 myFreq = 1000;


LOCAL void ICACHE_FLASH_ATTR hello_cb(void *arg)
{
	//os_printf("13Hello World!\r\n");
	myDuty = myDuty + dir;

	pwm_set_duty(myDuty, 0);
	pwm_start();

	if (myDuty > 250) {
		dir = dir * -1;
		os_printf("over 250. dir is now %d", dir);
	}

	if (myDuty < 10) {
		dir = dir * -1;
		os_printf("under 10. dir is now %d", dir);
	}

    os_delay_us(10000);
    os_printf("Duty: %d\n",myDuty);
}

//Init function 
void ICACHE_FLASH_ATTR
user_init()
{
	uint8 tempDuty[4];
	tempDuty[0] = 0;
	tempDuty[1] = 0;
	tempDuty[2] = 0;
	tempDuty[3] = 0;

	uart_init(BIT_RATE_115200,BIT_RATE_115200);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);

	// start sequence
		GPIO_OUTPUT_SET(2, 1);
		os_delay_us(1000000);
		GPIO_OUTPUT_SET(2, 0);
		os_delay_us(1000000);
		GPIO_OUTPUT_SET(2, 1);
		os_delay_us(1000000);
		GPIO_OUTPUT_SET(2, 0);
		os_delay_us(1000000);
		GPIO_OUTPUT_SET(2, 1);
		os_delay_us(1000000);
		GPIO_OUTPUT_SET(2, 0);
		os_delay_us(1000000);
		GPIO_OUTPUT_SET(2, 1);
		os_delay_us(1000000);
		GPIO_OUTPUT_SET(2, 0);




		os_printf("pwm_init\r");

	 pwm_init(500, tempDuty);
	 os_printf("pwm_set_freq");
	 pwm_set_freq((uint16_t)myFreq, 0);
	 os_printf("pwm_start");
	 pwm_start();

    //Start os task
	os_printf("system_os_task");
    //system_os_task(loop, user_procTaskPrio,user_procTaskQueue, user_procTaskQueueLen);
    os_printf("user_init complete");


	os_timer_disarm(&hello_timer);
	// os_timer_setfn(ETSTimer *ptimer, ETSTimerFunc *pfunction, void *parg)
	os_timer_setfn(&hello_timer, (os_timer_func_t *)hello_cb, (void *)0);
    // void os_timer_arm(ETSTimer *ptimer,uint32_t milliseconds, bool repeat_flag)
    os_timer_arm(&hello_timer, 30, 1);
}
