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
 * Demo: memory pool
 *
 * This demo creates one memory pool and two threads:
 *   1) thread #1: allocate memory pool
 *   2) thread #2: free memory pool
 *
 * read more:
 *    https://www.rt-thread.io/document/site/memory/memory/#memory-pool
 */

#include <rtthread.h>

static rt_uint8_t *ptr[50];
static rt_uint8_t mempool[4096];
static struct rt_mempool mp;

#define THREAD_PRIORITY      25
#define THREAD_STACK_SIZE    512
#define THREAD_TIMESLICE     5

/* thread handler */
static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2 = RT_NULL;

/* thread #1 entry function */
static void thread1_mp_alloc(void *parameter)
{
    int i;
    for (i = 0 ; i < 50 ; i++)
    {
        if (ptr[i] == RT_NULL)
        {
            /* allocate memory from the memory pool */
            ptr[i] = rt_mp_alloc(&mp, RT_WAITING_FOREVER);
            if (ptr[i] != RT_NULL)
                rt_kprintf("allocate No.%d\n", i);
        }
    }
}

/* thread #2 entry function */
static void thread2_mp_release(void *parameter)
{
    int i;

    rt_kprintf("thread2 try to release block\n");
    for (i = 0; i < 50 ; i++)
    {
        /* free all memory */
        if (ptr[i] != RT_NULL)
        {
            rt_kprintf("release block %d\n", i);
            rt_mp_free(ptr[i]);
            ptr[i] = RT_NULL;
        }
    }
}

int mempool_sample(void)
{
    int i;
    for (i = 0; i < 50; i ++) ptr[i] = RT_NULL;

    /* initiate memory pool */
    rt_mp_init(&mp, "mp1", &mempool[0], sizeof(mempool), 80);

    /* create thread #1 */
    tid1 = rt_thread_create("thread1", thread1_mp_alloc, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);
#ifdef RT_USING_SMP
    /* Bind threads to the same core to avoid messy log output when multiple cores are enabled */
    rt_thread_control(tid1, RT_THREAD_CTRL_BIND_CPU, (void*)0);
#endif
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1); /* start thread #1 */

    /* create thread #2 */
    tid2 = rt_thread_create("thread2", thread2_mp_release, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY + 1, THREAD_TIMESLICE);
#ifdef RT_USING_SMP
    /* Bind threads to the same core to avoid messy log output when multiple cores are enabled */
    rt_thread_control(tid2, RT_THREAD_CTRL_BIND_CPU, (void*)0);
#endif
    if (tid2 != RT_NULL)
        rt_thread_startup(tid2); /* start thread #2 */

    return 0;
}

/* export the msh command */
MSH_CMD_EXPORT(mempool_sample, mempool sample);
