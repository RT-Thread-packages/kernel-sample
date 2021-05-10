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
 * Demo: scheduler hook
 *
 * This demo sets a scheduler hook function and prints context-switching information.
 *
 * read more:
 *    https://www.rt-thread.io/document/site/thread/thread/#set-the-scheduler-hook
 */

#include <rtthread.h>

#define THREAD_STACK_SIZE   1024
#define THREAD_PRIORITY     20
#define THREAD_TIMESLICE    10

/* counter for each thread */
volatile rt_uint32_t count[2];

/* thread entry function */
/* threads #1 and #2 share one entry, but the entry parameter is different */
static void thread_entry(void *parameter)
{
    rt_uint32_t value;

    value = (rt_uint32_t)parameter;
    while (1)
    {
        rt_kprintf("thread %d is running\n", value);
        rt_thread_mdelay(1000);
    }
}

/* thread(s) handler */
static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2 = RT_NULL;

static void hook_of_scheduler(struct rt_thread *from, struct rt_thread *to)
{
    rt_kprintf("from: %s -->  to: %s \n", from->name, to->name);
}

int scheduler_hook(void)
{
    /* set a scheduler hook function */
    rt_scheduler_sethook(hook_of_scheduler);

    tid1 = rt_thread_create("thread1",
                            thread_entry, (void *)1,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    tid2 = rt_thread_create("thread2",
                            thread_entry, (void *)2,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE - 5);
    if (tid2 != RT_NULL)
        rt_thread_startup(tid2);
    return 0;
}

/* export the msh command */
MSH_CMD_EXPORT(scheduler_hook, scheduler_hook sample);
