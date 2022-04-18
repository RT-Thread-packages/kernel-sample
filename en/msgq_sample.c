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
 * Demo: message queue
 *
 * This demo creates two threads and one message queue:
 *    1) thread #1: receive message(s) from message queue
 *    2) thread #2: send normal and urgent messages to message queue
 *
 * read more:
 *    https://www.rt-thread.io/document/site/thread-comm/thread-comm/#message-queue
 */

#include <rtthread.h>

#define THREAD_PRIORITY      25
#define THREAD_TIMESLICE     5

/* message queue control block */
static struct rt_messagequeue mq;
/* the memory pool used to place messages in the message queue */
static rt_uint8_t msg_pool[2048];

ALIGN(RT_ALIGN_SIZE)
static char thread1_stack[1024];
static struct rt_thread thread1;

/* thread #1 entry function */
static void thread1_entry(void *parameter)
{
    char buf = 0;
    rt_uint8_t cnt = 0;

    while (1)
    {
        /* pend and receive message(s) from message queue */
        if (rt_mq_recv(&mq, &buf, sizeof(buf), RT_WAITING_FOREVER) == RT_EOK)
        {
            rt_kprintf("thread1: recv msg from msg queue, the content:%c\n", buf);
            if (cnt == 19)
            {
                break;
            }
        }
        cnt++;

        /* delay for 50ms */
        rt_thread_mdelay(50);
    }
    rt_kprintf("thread1: detach mq \n");
    rt_mq_detach(&mq);
}

ALIGN(RT_ALIGN_SIZE)
static char thread2_stack[1024];
static struct rt_thread thread2;

/* thread #2 entry function */
static void thread2_entry(void *parameter)
{
    int result;
    char buf = 'A';
    rt_uint8_t cnt = 0;

    while (1)
    {
        if (cnt == 8)
        {
            /* send one URGENT message to the message queue */
            result = rt_mq_urgent(&mq, &buf, 1);
            if (result != RT_EOK)
            {
                rt_kprintf("rt_mq_urgent ERR\n");
            }
            else
            {
                rt_kprintf("thread2: send urgent message - %c\n", buf);
            }
        }
        else if (cnt >= 20) /* exit */
        {
            rt_kprintf("message queue stop send, thread2 quit\n");
            break;
        }
        else
        {
            /* send one message to the message queue */
            result = rt_mq_send(&mq, &buf, 1);
            if (result != RT_EOK)
            {
                rt_kprintf("rt_mq_send ERR\n");
            }

            rt_kprintf("thread2: send message - %c\n", buf);
        }
        buf++;
        cnt++;

        /* delay for 5ms */
        rt_thread_mdelay(5);
    }
}

int msgq_sample(void)
{
    rt_err_t result;

    /* initiate a message queue */
    result = rt_mq_init(&mq,
                        "mqt",
                        &msg_pool[0],               /* msg_pool's address */
                        1,                          /* the size of each message is 1 byte */
                        sizeof(msg_pool),           /* The size of the memory pool is the size of msg_pool */
                        RT_IPC_FLAG_PRIO);

    if (result != RT_EOK)
    {
        rt_kprintf("init message queue failed.\n");
        return -1;
    }

    rt_thread_init(&thread1,
                   "thread1",
                   thread1_entry,
                   RT_NULL,
                   &thread1_stack[0],
                   sizeof(thread1_stack),
                   THREAD_PRIORITY, THREAD_TIMESLICE);
#ifdef RT_USING_SMP
    /* Bind threads to the same core to avoid messy log output when multiple cores are enabled */
    rt_thread_control(&thread1, RT_THREAD_CTRL_BIND_CPU, (void*)0);
#endif
    rt_thread_startup(&thread1);

    rt_thread_init(&thread2,
                   "thread2",
                   thread2_entry,
                   RT_NULL,
                   &thread2_stack[0],
                   sizeof(thread2_stack),
                   THREAD_PRIORITY, THREAD_TIMESLICE);
#ifdef RT_USING_SMP
    /* Bind threads to the same core to avoid messy log output when multiple cores are enabled */
    rt_thread_control(&thread2, RT_THREAD_CTRL_BIND_CPU, (void*)0);
#endif
    rt_thread_startup(&thread2);

    return 0;
}

/* export the msh command */
MSH_CMD_EXPORT(msgq_sample, msgq sample);
