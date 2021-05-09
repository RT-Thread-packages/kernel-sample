/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-08-24     yangjie      the first version
 * 2020-10-17     Meco Man     translate to English comment
 */

/*
 * Demo: timer(s)
 *
 * This demo creates two dynamic timers:
 *    1) timer #1: one-shot mode timer
 *    2) timer #2: periodic mode timer
 *
 * read more:
 *    https://www.rt-thread.io/document/site/timer/timer/#timer-management
 */

#include <rtthread.h>

/* timer handle */
static rt_timer_t timer1;
static rt_timer_t timer2;
static int cnt = 0;

/* timer #1 timeout callback function */
static void timeout1(void *parameter)
{
    rt_kprintf("periodic timer is timeout %d\n", cnt);

    if (cnt++ >= 9)
    {
        rt_timer_stop(timer1);
        rt_kprintf("periodic timer was stopped! \n");
    }
}

/* timer #2 timeout callback function */
static void timeout2(void *parameter)
{
    rt_kprintf("one shot timer is timeout\n");
}

int timer_sample(void)
{
    /* create timer #1, periodic mode */
    timer1 = rt_timer_create("timer1", timeout1,
                             RT_NULL, 10,
                             RT_TIMER_FLAG_PERIODIC);

    /* start timer #1 */
    if (timer1 != RT_NULL)
        rt_timer_start(timer1);

    /* create timer #2, one-shot mode */
    timer2 = rt_timer_create("timer2", timeout2,
                             RT_NULL,  30,
                             RT_TIMER_FLAG_ONE_SHOT);

    /* start timer #2 */
    if (timer2 != RT_NULL)
        rt_timer_start(timer2);

    return 0;
}

/* export the msh command */
MSH_CMD_EXPORT(timer_sample, timer sample);
