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
 * Demo: event(s)
 *
 * This demo creates two threads and one statical event:
 *    1) thread #1: pend and receive events
 *    2) thread #2: sent events (Event3 and Event5)
 *
 * read more:
 *    https://www.rt-thread.io/document/site/thread-sync/thread-sync/#event
 */

#include <rtthread.h>

#define THREAD_PRIORITY      9
#define THREAD_TIMESLICE     5

#define EVENT_FLAG3 (1 << 3)
#define EVENT_FLAG5 (1 << 5)

/* ECB (Event Control Block) */
static struct rt_event event;

ALIGN(RT_ALIGN_SIZE)
static char thread1_stack[1024]; /* thread stack 1024 Byte*/
static struct rt_thread thread1; /* TCB (Thread Control Block) */

/* thread #1 entry function */
static void thread1_recv_event(void *param)
{
    rt_uint32_t e;

    /*
        first time to receive event(s):
        EITHER Event3 OR Event5 happened can resume thread1
        and then clear conrresponding event(s)' flag
    */
    if (rt_event_recv(&event, (EVENT_FLAG3 | EVENT_FLAG5),
                      RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                      RT_WAITING_FOREVER, &e) == RT_EOK)
    {
        rt_kprintf("thread1: OR recv event 0x%x\n", e);
    }

    rt_kprintf("thread1: delay 1s to prepare the second event\n");
    rt_thread_mdelay(1000);

    /*
        second time to receive event(s):
        BOTH Event3 AND Event5 happened can resume thread1
        and then clear conrresponding event(s)' flag
    */
    if (rt_event_recv(&event, (EVENT_FLAG3 | EVENT_FLAG5),
                      RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                      RT_WAITING_FOREVER, &e) == RT_EOK)
    {
        rt_kprintf("thread1: AND recv event 0x%x\n", e);
    }
    rt_kprintf("thread1 leave.\n");
}

ALIGN(RT_ALIGN_SIZE)
static char thread2_stack[1024]; /* thread stack 1024 Byte*/
static struct rt_thread thread2; /* TCB (Thread Control Block) */

/* thread #2 entry function */
static void thread2_send_event(void *param)
{
    rt_kprintf("thread2: send event3\n");
    rt_event_send(&event, EVENT_FLAG3);
    rt_thread_mdelay(200);

    rt_kprintf("thread2: send event5\n");
    rt_event_send(&event, EVENT_FLAG5);
    rt_thread_mdelay(200);

    rt_kprintf("thread2: send event3\n");
    rt_event_send(&event, EVENT_FLAG3);
    rt_kprintf("thread2 leave.\n");
}

int event_sample(void)
{
    rt_err_t result;

    /* initiate the event (statically) */
    result = rt_event_init(&event, "event", RT_IPC_FLAG_PRIO);
    if (result != RT_EOK)
    {
        rt_kprintf("init event failed.\n");
        return -1;
    }

    /* initiate the thread #1 (statically) */
    rt_thread_init(&thread1,
                   "thread1",
                   thread1_recv_event,
                   RT_NULL,
                   &thread1_stack[0],
                   sizeof(thread1_stack),
                   THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    rt_thread_startup(&thread1); /* start thread #1 */

    /* initiate the thread #2 (statically) */
    rt_thread_init(&thread2,
                   "thread2",
                   thread2_send_event,
                   RT_NULL,
                   &thread2_stack[0],
                   sizeof(thread2_stack),
                   THREAD_PRIORITY, THREAD_TIMESLICE);
    rt_thread_startup(&thread2); /* start thread #2 */

    return 0;
}

/* export the msh command */
MSH_CMD_EXPORT(event_sample, event sample);
