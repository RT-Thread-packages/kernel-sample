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
 * Demo: prevent priority inversions
 *
 * This demo creates 3 threads and 1 mutex to
 * demonstrate how the mutex prevents priority inversions.
 *
 * read more:
 *    https://www.rt-thread.io/document/site/thread-sync/thread-sync/#mutex
 */

#include <rtthread.h>

/* thread(s) handler */
static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2 = RT_NULL;
static rt_thread_t tid3 = RT_NULL;
static rt_mutex_t mutex = RT_NULL;

#define THREAD_PRIORITY       10
#define THREAD_STACK_SIZE     512
#define THREAD_TIMESLICE      5

/* thread #1 entry function */
static void thread1_entry(void *parameter)
{
    /* let the lower priority thread run first */
    rt_thread_mdelay(100);

    /* at this point, thread #3 holds the mutex and thread #2 is pending on holding the mutex */

    /* check the priority level of thread #2 and thread #3 */
    if (tid2->current_priority != tid3->current_priority)
    {
        /* failure */
        rt_kprintf("the priority of thread2 is: %d\n", tid2->current_priority);
        rt_kprintf("the priority of thread3 is: %d\n", tid3->current_priority);
        rt_kprintf("test failed.\n");
        return;
    }
    else
    {
        rt_kprintf("the priority of thread2 is: %d\n", tid2->current_priority);
        rt_kprintf("the priority of thread3 is: %d\n", tid3->current_priority);
        rt_kprintf("test OK.\n");
    }
}

/* thread #2 entry function */
static void thread2_entry(void *parameter)
{
    rt_err_t result;

    rt_kprintf("the priority of thread2 is: %d\n", tid2->current_priority);

    /* let the lower priority thread run first */
    rt_thread_mdelay(50);

    /*
     * pending the mutex
     * At this time, kernel raises the priority of thread #3 to same priority
     * as thread #2
     */
    result = rt_mutex_take(mutex, RT_WAITING_FOREVER);

    if (result == RT_EOK)
    {
        /* release the mutex */
        rt_mutex_release(mutex);
    }
}

/* thread #3 entry function */
static void thread3_entry(void *parameter)
{
    rt_tick_t tick;
    rt_err_t result;

    rt_kprintf("the priority of thread3 is: %d\n", tid3->current_priority);

    result = rt_mutex_take(mutex, RT_WAITING_FOREVER);
    if (result != RT_EOK)
    {
        rt_kprintf("thread3 take a mutex, failed.\n");
    }

    /* delay for 500ms without scheduling */
    tick = rt_tick_get();
    while (rt_tick_get() - tick < (RT_TICK_PER_SECOND / 2)) ;

    /* release the mutex */
    rt_mutex_release(mutex);
}

int pri_inversion(void)
{
    /* create mutex */
    mutex = rt_mutex_create("mutex", RT_IPC_FLAG_PRIO);
    if (mutex == RT_NULL)
    {
        rt_kprintf("create dynamic mutex failed.\n");
        return -1;
    }

    /* create thread #1 */
    tid1 = rt_thread_create("thread1",
                            thread1_entry,
                            RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY - 1, THREAD_TIMESLICE);
#ifdef RT_USING_SMP
    /* Bind threads to the same core to avoid messy log output when multiple cores are enabled */
    rt_thread_control(tid1, RT_THREAD_CTRL_BIND_CPU, (void*)0);
#endif
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    /* create thread #2 */
    tid2 = rt_thread_create("thread2",
                            thread2_entry,
                            RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);
#ifdef RT_USING_SMP
    /* Bind threads to the same core to avoid messy log output when multiple cores are enabled */
    rt_thread_control(tid2, RT_THREAD_CTRL_BIND_CPU, (void*)0);
#endif
    if (tid2 != RT_NULL)
        rt_thread_startup(tid2);

    /* create thread #3 */
    tid3 = rt_thread_create("thread3",
                            thread3_entry,
                            RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY + 1, THREAD_TIMESLICE);
#ifdef RT_USING_SMP
    /* Bind threads to the same core to avoid messy log output when multiple cores are enabled */
    rt_thread_control(tid3, RT_THREAD_CTRL_BIND_CPU, (void*)0);
#endif
    if (tid3 != RT_NULL)
        rt_thread_startup(tid3);

    return 0;
}

/* export the msh command */
MSH_CMD_EXPORT(pri_inversion, pri_inversion sample);
