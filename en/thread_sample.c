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
 * Demo: thread(s)
 *
 * This demo creates two threads:
 *    1) create thread #1 dynamically, and delete automatically when the thread #1 finished;
 *    2) create thread #2 statically, and print counting numbers continuously.
 *
 * read more:
 *    https://www.rt-thread.io/document/site/thread/thread/
 */

#include <rtthread.h>

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

/* thread handler */
static rt_thread_t tid1 = RT_NULL;

/* thread #1 entry function */
static void thread1_entry(void *parameter)
{
    rt_uint32_t count = 0;

    while (1)
    {
        /* thread #1 occupies low priority and prints counting numbers continuously */
        rt_kprintf("thread1 count: %d\n", count ++);
        rt_thread_mdelay(500);
    }
}

ALIGN(RT_ALIGN_SIZE)
static char thread2_stack[1024];
static struct rt_thread thread2;

/* thread #2 entry function */
static void thread2_entry(void *param)
{
    rt_uint32_t count = 0;

    /* thread #2 occupies higher priority than that of thread #1 */
    for (count = 0; count < 10 ; count++)
    {
        /* thread #2 prints counting numbers */
        rt_kprintf("thread2 count: %d\n", count);
    }
    rt_kprintf("thread2 exit\n");

    /* RT-Thread allows thread entry function to return directly */
    /* thread #2 will be deleted automatically by idle thread once it finishes */
}

int thread_sample(void)
{
    /* create thread #1 dynamically */
    tid1 = rt_thread_create("thread1",
                            thread1_entry, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);
#ifdef RT_USING_SMP
    /* Bind threads to the same core to avoid messy log output when multiple cores are enabled */
    rt_thread_control(tid1, RT_THREAD_CTRL_BIND_CPU, (void*)0);
#endif
    /* start thread #1 */
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    /* create thread #2 statically */
    rt_thread_init(&thread2,
                   "thread2",
                   thread2_entry,
                   RT_NULL,
                   &thread2_stack[0],
                   sizeof(thread2_stack),
                   THREAD_PRIORITY - 1, THREAD_TIMESLICE);
#ifdef RT_USING_SMP
    /* Bind threads to the same core to avoid messy log output when multiple cores are enabled */
    rt_thread_control(&thread2, RT_THREAD_CTRL_BIND_CPU, (void*)0);
#endif
    rt_thread_startup(&thread2); /* start thread #2 */

    return 0;
}

/* export the msh command */
MSH_CMD_EXPORT(thread_sample, thread sample);
