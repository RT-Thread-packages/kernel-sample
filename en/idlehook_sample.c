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
 * Demo: idle hook
 *
 * This demo creates a thread and set an idle thread hook function. The idle
 * thread hook function will be invoked when thread #1 is delaying.
 *
 * read more:
 *    https://www.rt-thread.io/document/site/thread/thread/#set-and-delete-idle-hooks
 *    https://www.rt-thread.io/document/site/thread/thread/#idle-thread
 */

#include <rtthread.h>

#define THREAD_PRIORITY      20
#define THREAD_STACK_SIZE    1024
#define THREAD_TIMESLICE     5

/* thread handler */
static rt_thread_t tid = RT_NULL;

/* idle thread hook function running number of times */
volatile static int hook_times = 0;

/* idle thread hook function */
static void idle_hook()
{
    if (0 == (hook_times % 10000))
    {
        rt_kprintf("enter idle hook %d times.\n", hook_times);
    }

    rt_enter_critical();
    hook_times++;
    rt_exit_critical();
}

/* thread entry function */
static void thread_entry(void *parameter)
{
    int i = 5;
    while (i--)
    {
        rt_kprintf("enter thread1.\n");
        rt_enter_critical();
        hook_times = 0;
        rt_exit_critical();

        /* sleep for 500ms */
        rt_kprintf("thread1 delay 500ms.\n");
        rt_thread_mdelay(500);
    }
    rt_kprintf("delete idle hook.\n");

    /* remove idle thread hook function */
    rt_thread_idle_delhook(idle_hook);
    rt_kprintf("thread1 finish.\n");
}

int idle_hook_sample(void)
{
    /* set idle thread hook function */
    rt_thread_idle_sethook(idle_hook);

    /* create thread #1 */
    tid = rt_thread_create("thread1",
                           thread_entry, RT_NULL,
                           THREAD_STACK_SIZE,
                           THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}

/* export the msh command */
MSH_CMD_EXPORT(idle_hook_sample, idle hook sample);
