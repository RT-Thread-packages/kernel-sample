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
 * Demo: producer-consumer problem (or bounded-buffer problem)
 *
 * this demo creates two threads to demonstrate producer-consumer problem:
 *     1) producer thread: adds 1 to the variable "cnt" and stores it into the array.
 *     2) consumer thread: prints out the value and adds it up
 *
 * read more:
 *    https://www.rt-thread.io/document/site/thread-sync/thread-sync/#semaphores
 */

#include <rtthread.h>

#define THREAD_PRIORITY       6
#define THREAD_STACK_SIZE     512
#define THREAD_TIMESLICE      5

/* define the maximum 5 elements can be produced */
#define MAXSEM 5

rt_uint32_t array[MAXSEM];

/* the pointers of producer and consumer's position in the array */
static rt_uint32_t set, get;

/* thread handler */
static rt_thread_t producer_tid = RT_NULL;
static rt_thread_t consumer_tid = RT_NULL;

struct rt_semaphore sem_lock;
struct rt_semaphore sem_empty, sem_full;

/* producer thread entry function */
void producer_thread_entry(void *parameter)
{
    int cnt = 0;

    while (cnt < 10)
    {
        /* get a "empty" mark */
        rt_sem_take(&sem_empty, RT_WAITING_FOREVER);

        /* protect the critial section */
        rt_sem_take(&sem_lock, RT_WAITING_FOREVER);
        array[set % MAXSEM] = cnt + 1;
        rt_kprintf("the producer generates a number: %d\n", array[set % MAXSEM]);
        set++;
        rt_sem_release(&sem_lock);

        /* release a "full" mark */
        rt_sem_release(&sem_full);
        cnt++;

        rt_thread_mdelay(20);
    }

    rt_kprintf("the producer exit!\n");
}

/* consumer thread entry function */
void consumer_thread_entry(void *parameter)
{
    rt_uint32_t sum = 0;

    while (1)
    {
        /* get a "full" mark */
        rt_sem_take(&sem_full, RT_WAITING_FOREVER);

        /* protect the critial section */
        rt_sem_take(&sem_lock, RT_WAITING_FOREVER);
        sum += array[get % MAXSEM];
        rt_kprintf("the consumer[%d] get a number: %d\n", (get % MAXSEM), array[get % MAXSEM]);
        get++;
        rt_sem_release(&sem_lock);

        /* release a "empty" mark */
        rt_sem_release(&sem_empty);

        if (get == 10) break;

        rt_thread_mdelay(50);
    }

    rt_kprintf("the consumer sum is: %d\n", sum);
    rt_kprintf("the consumer exit!\n");
}

int producer_consumer(void)
{
    set = 0;
    get = 0;

    rt_sem_init(&sem_lock, "lock",     1,      RT_IPC_FLAG_PRIO);
    rt_sem_init(&sem_empty, "empty",   MAXSEM, RT_IPC_FLAG_PRIO);
    rt_sem_init(&sem_full, "full",     0,      RT_IPC_FLAG_PRIO);

    producer_tid = rt_thread_create("producer",
                                    producer_thread_entry, RT_NULL,
                                    THREAD_STACK_SIZE,
                                    THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    if (producer_tid != RT_NULL)
        rt_thread_startup(producer_tid);


    consumer_tid = rt_thread_create("consumer",
                                    consumer_thread_entry, RT_NULL,
                                    THREAD_STACK_SIZE,
                                    THREAD_PRIORITY + 1, THREAD_TIMESLICE);
    if (consumer_tid != RT_NULL)
        rt_thread_startup(consumer_tid);

    return 0;
}

/* export the msh command */
MSH_CMD_EXPORT(producer_consumer, producer_consumer sample);
