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
 * Demo: dynamic memory management
 *
 * This demo creates a dynamic thread to allocate and free memory.
 * Each time it allocates more memory, and it will end when it can't allocate any memory.
 *
 * read more:
 *    https://www.rt-thread.io/document/site/memory/memory/#memory-management
 */

#include <rtthread.h>

#define THREAD_PRIORITY      25
#define THREAD_STACK_SIZE    512
#define THREAD_TIMESLICE     5

/* thread #1 entry function*/
void thread1_entry(void *parameter)
{
    int i;
    char *ptr = RT_NULL; /* memory's pointer */

    for (i = 0; ; i++)
    {
        /* allocate memory of (1 << i) bytes  */
        ptr = rt_malloc(1 << i);

        if (ptr != RT_NULL)
        {
            /* if memory allocated successfully */
            rt_kprintf("get memory :%d byte\n", (1 << i));
            rt_free(ptr); /* free memory */
            rt_kprintf("free memory :%d byte\n", (1 << i));
            ptr = RT_NULL;
        }
        else
        {
            rt_kprintf("try to get %d byte memory failed!\n", (1 << i));
            return;
        }
    }
}

int dynmem_sample(void)
{
    rt_thread_t tid = RT_NULL;

    /* create thread #1 */
    tid = rt_thread_create("thread1",
                           thread1_entry, RT_NULL,
                           THREAD_STACK_SIZE,
                           THREAD_PRIORITY,
                           THREAD_TIMESLICE);
    /*start thread #1 */
    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}

/* export the msh command */
MSH_CMD_EXPORT(dynmem_sample, dynmem sample);
