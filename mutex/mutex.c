/*
 * 程序清单：互斥锁例程
 *
 * 互斥锁是一种保护共享资源的方法。当一个线程拥有互斥锁的时候，另一个线程若是等待锁，
 * 则其就会被挂起，从而保证只有一个线程会操作共享数据。
 *
 */
#include <rtthread.h>

/* 互斥量控制块 */
static struct rt_mutex static_mutex;
/* 指向互斥量的指针 */
static rt_mutex_t dynamic_mutex = RT_NULL;

ALIGN(RT_ALIGN_SIZE)
static char thread1_stack[1024];
static struct rt_thread thread1;
static void rt_thread_entry1(void *parameter)
{
    rt_err_t result;
    rt_tick_t tick;

    /* 1. staic mutex demo */

    /* 试图持有互斥量，最大等待10个OS Tick后返回 */
    rt_kprintf("thread1 try to get static mutex, wait 10 ticks.\n");

    /* 获得当前的OS Tick */
    tick = rt_tick_get();
    result = rt_mutex_take(&static_mutex, 10);

    if (result == -RT_ETIMEOUT)
    {
        /* 超时后判断是否刚好是10个OS Tick */
        if (rt_tick_get() - tick != 10)
        {
            rt_mutex_detach(&static_mutex);
            return;
        }
        rt_kprintf("thread1 take static mutex timeout\n");
    }
    else
    {
        /* 线程2持有互斥量，且在相当长的时间后才会释放互斥量，
         * 因此10个tick后线程1不可能获得 */
        rt_kprintf("thread1 take a static mutex, failed.\n");
        rt_mutex_detach(&static_mutex);
        return;
    }

    /* 永久等待方式持有互斥量 */
    rt_kprintf("thread1 try to get static mutex, wait forever.\n");
    result = rt_mutex_take(&static_mutex, RT_WAITING_FOREVER);
    if (result != RT_EOK)
    {
        /* 不成功则测试失败 */
        rt_kprintf("thread1 take a static mutex, failed.\n");
        rt_mutex_detach(&static_mutex);
        return;
    }

    rt_kprintf("thread1 take a staic mutex, done.\n");

    /* 脱离互斥量对象 */
    rt_mutex_detach(&static_mutex);

    /* 2. dynamic mutex test */

    /* 试图持有互斥量，最大等待10个OS Tick后返回 */
    rt_kprintf("thread1 try to get dynamic mutex, wait 10 ticks.\n");

    tick = rt_tick_get();
    result = rt_mutex_take(dynamic_mutex, 10);
    if (result == -RT_ETIMEOUT)
    {
        /* 超时后判断是否刚好是10个OS Tick */
        if (rt_tick_get() - tick != 10)
        {
            rt_mutex_delete(dynamic_mutex);
            return;
        }
        rt_kprintf("thread1 take dynamic mutex timeout\n");
    }
    else
    {
        /* 线程2持有互斥量，且在相当长的时间后才会释放互斥量，
         * 因此10个tick后线程1不可能获得 */
        rt_kprintf("thread1 take a dynamic mutex, failed.\n");
        rt_mutex_delete(dynamic_mutex);
        return;
    }

    /* 永久等待方式持有互斥量 */
    rt_kprintf("thread1 try to get dynamic mutex, wait forever.\n");
    result = rt_mutex_take(dynamic_mutex, RT_WAITING_FOREVER);
    if (result != RT_EOK)
    {
        /* 不成功则测试失败 */
        rt_kprintf("thread1 take a dynamic mutex, failed.\n");
        rt_mutex_delete(dynamic_mutex);
        return;
    }

    rt_kprintf("thread1 take a dynamic mutex, done.\n");
    /* 删除互斥量对象 */
    rt_mutex_delete(dynamic_mutex);
}


ALIGN(RT_ALIGN_SIZE)
static char thread2_stack[1024];
static struct rt_thread thread2;
static void rt_thread_entry2(void *parameter)
{
    /* 1. static mutex test */
    rt_kprintf("thread2 try to get static mutex\n");
    rt_mutex_take(&static_mutex, 10);
    rt_kprintf("thread2 got static mutex\n");
    rt_thread_delay(RT_TICK_PER_SECOND);
    rt_kprintf("thread2 release static mutex\n");
    rt_mutex_release(&static_mutex);

    /* 2. dynamic mutex test */
    rt_kprintf("thread2 try to get dynamic mutex\n");
    rt_mutex_take(dynamic_mutex, 10);
    rt_kprintf("thread2 got dynamic mutex\n");
    rt_thread_delay(RT_TICK_PER_SECOND);
    rt_kprintf("thread2 release dynamic mutex\n");
    rt_mutex_release(dynamic_mutex);
}

/* 互斥量示例的初始化 */
int mutex_sample_init()
{
    rt_err_t result;

    /* 初始化静态互斥量 */
    result = rt_mutex_init(&static_mutex, "smutex", RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("init static mutex failed.\n");
        return -1;
    }

    /* 创建一个动态互斥量 */
    dynamic_mutex = rt_mutex_create("dmutex", RT_IPC_FLAG_FIFO);
    if (dynamic_mutex == RT_NULL)
    {
        rt_kprintf("create dynamic mutex failed.\n");
        return -1;
    }

    rt_thread_init(&thread1,
                   "thread1",
                   rt_thread_entry1,
                   RT_NULL,
                   &thread1_stack[0],
                   sizeof(thread1_stack), 11, 5);
    rt_thread_startup(&thread1);


    rt_thread_init(&thread2,
                   "thread2",
                   rt_thread_entry2,
                   RT_NULL,
                   &thread2_stack[0],
                   sizeof(thread2_stack), 10, 5);
    rt_thread_startup(&thread2);
    return 0;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(mutex_sample_init, mutex sample);
