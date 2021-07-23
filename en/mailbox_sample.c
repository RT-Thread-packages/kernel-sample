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
 * Demo:
 *
 * This demo creates two threads and one boxmail (static):
 *    1) thread #1: receive mails
 *    2) thread #2: send mails
 *
 * read more:
 *    https://www.rt-thread.io/document/site/thread-comm/thread-comm/#mailbox
 */

#include <rtthread.h>

#define THREAD_PRIORITY      10
#define THREAD_TIMESLICE     5

/* mailbox control block */
static struct rt_mailbox mb;

/* memory pool for mails storage */
static char mb_pool[128];

static char mb_str1[] = "I'm a mail!";
static char mb_str2[] = "this is another mail!";
static char mb_str3[] = "over";

ALIGN(RT_ALIGN_SIZE)
static char thread1_stack[1024];
static struct rt_thread thread1;

/* thread #1 entry function */
static void thread1_entry(void *parameter)
{
    char *str;

    while (1)
    {
        rt_kprintf("thread1: try to recv a mail\n");

        /* pend and receive mail(s) from mailbox */
        if (rt_mb_recv(&mb, (rt_ubase_t *)&str, RT_WAITING_FOREVER) == RT_EOK)
        {
            rt_kprintf("thread1: get a mail from mailbox, the content:%s\n", str);
            if (str == mb_str3)
                break;

            /* delay for 100ms */
            rt_thread_mdelay(100);
        }
    }
    /* detach mailbox */
    rt_mb_detach(&mb);
}

ALIGN(RT_ALIGN_SIZE)
static char thread2_stack[1024];
static struct rt_thread thread2;

/* thread #2 entry function */
static void thread2_entry(void *parameter)
{
    rt_uint8_t count;

    count = 0;
    while (count < 10)
    {
        count ++;
        if (count & 0x1)
        {
            /* send the 'mb_str1' variable's address to the mailbox */
            rt_mb_send(&mb, (rt_uint32_t)&mb_str1);
        }
        else
        {
            /* send the 'mb_str2' variable's address to the mailbox */
            rt_mb_send(&mb, (rt_uint32_t)&mb_str2);
        }

        /* delay for 200ms */
        rt_thread_mdelay(200);
    }

    /* send the 'mb_str3' variable's address to the mailbox */
    /* to inform thread #1 that thread #2 has finished running */
    rt_mb_send(&mb, (rt_uint32_t)&mb_str3);
}

/* mailbox(s) demo */
int mailbox_sample(void)
{
    rt_err_t result;

    /* initiate a mailbox */
    result = rt_mb_init(&mb,
                        "mbt",
                        &mb_pool[0],
                        sizeof(mb_pool) / sizeof(rt_ubase_t), /* size of mails */
                        RT_IPC_FLAG_PRIO);
    if (result != RT_EOK)
    {
        rt_kprintf("init mailbox failed.\n");
        return -1;
    }

    /* initiate thread #1 */
    rt_thread_init(&thread1,
                   "thread1",
                   thread1_entry,
                   RT_NULL,
                   &thread1_stack[0],
                   sizeof(thread1_stack),
                   THREAD_PRIORITY, THREAD_TIMESLICE);
    rt_thread_startup(&thread1); /* start thread #1 */

    /*initiate thread #2 */
    rt_thread_init(&thread2,
                   "thread2",
                   thread2_entry,
                   RT_NULL,
                   &thread2_stack[0],
                   sizeof(thread2_stack),
                   THREAD_PRIORITY, THREAD_TIMESLICE);
    rt_thread_startup(&thread2); /* start thread #2 */

    return 0;
}

/* export the msh command */
MSH_CMD_EXPORT(mailbox_sample, mailbox sample);
