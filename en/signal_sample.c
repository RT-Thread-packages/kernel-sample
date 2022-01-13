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
 * Demo: signal
 *
 * This demo creates one thread. When the signal is installed, the signal
 * processing mode is set to custom processing. The processing function of
 * the signal is defined to be thread1_signal_handler(). After the thread is
 * running and the signal is installed, a signal is sent to this thread.
 * This thread will receive the signal and print the message.
 *
 * read more:
 *    https://www.rt-thread.io/document/site/thread-comm/thread-comm/#signal
 */

#include <rtthread.h>

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

static rt_thread_t tid1 = RT_NULL;

/* callback function of thread #1's signal */
void thread1_signal_handler(int sig)
{
    rt_kprintf("thread1 received signal %d\n", sig);
}

/* thread #1 entry function */
static void thread1_entry(void *parameter)
{
    int cnt = 0;

    /* install signal*/
    rt_signal_install(SIGUSR1, thread1_signal_handler);
    rt_signal_unmask(SIGUSR1);

    while (cnt < 10)
    {
        rt_kprintf("thread1 count : %d\n", cnt);
        cnt++;
        rt_thread_mdelay(100);
    }
}

int signal_sample(void)
{
    tid1 = rt_thread_create("thread1",
                            thread1_entry, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);

    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    rt_thread_mdelay(300);

    /* send the signal "SIGUSR1" to thread #1 */
    rt_thread_kill(tid1, SIGUSR1);

    return 0;
}

/* export the msh command */
MSH_CMD_EXPORT(signal_sample, signal sample);
