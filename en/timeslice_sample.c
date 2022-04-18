/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-08-24     yangjie      the first version
 * 2020-10-17     Meco Man     translate to English comment
 */

/*
 * Demo: Time-Slicing (or Round-Robin Scheduling)
 *
 * This demo creates two threads to show how Time-Slicing works.
 *
 * read more:
 *    https://www.rt-thread.io/document/site/thread/thread/#time-slice
 */

#include <rtthread.h>

#define THREAD_STACK_SIZE   1024
#define THREAD_PRIORITY     20
#define THREAD_TIMESLICE    10

/* thread entry function */
/* threads #1 and #2 share one entry, but the entry parameter is different */
static void thread_entry(void *parameter)
{
    rt_uint32_t value;
    rt_uint32_t count = 0;

    value = (rt_uint32_t)parameter;
    while (1)
    {
        if (0 == (count % 5))
        {
            rt_kprintf("thread %d is running ,thread %d count = %d\n", value, value, count);

            if (count > 200)
                return;
        }
        count++;
    }
}

int timeslice_sample(void)
{
    rt_thread_t tid = RT_NULL; /* thread handle */

    /* create thread #1 */
    tid = rt_thread_create("thread1",
                           thread_entry, (void *)1,
                           THREAD_STACK_SIZE,
                           THREAD_PRIORITY, THREAD_TIMESLICE);
#ifdef RT_USING_SMP
    /* Bind threads to the same core to avoid messy log output when multiple cores are enabled */
    rt_thread_control(tid, RT_THREAD_CTRL_BIND_CPU, (void*)0);
#endif
    if (tid != RT_NULL)
        rt_thread_startup(tid); /* start thread #1 */

    /* create thread #2 */
    tid = rt_thread_create("thread2",
                           thread_entry, (void *)2,
                           THREAD_STACK_SIZE,
                           THREAD_PRIORITY, THREAD_TIMESLICE - 5);
#ifdef RT_USING_SMP
    /* Bind threads to the same core to avoid messy log output when multiple cores are enabled */
    rt_thread_control(tid, RT_THREAD_CTRL_BIND_CPU, (void*)0);
#endif
    if (tid != RT_NULL)
        rt_thread_startup(tid); /* start thread #2 */

    return 0;
}

/* export the msh command */
MSH_CMD_EXPORT(timeslice_sample, timeslice sample);
