# 信号量的使用 #

## 介绍 ##

这个例程展示了如何在RT-Thread里使用信号量。

## 程序清单 ##

```{.c}
/*
 * 程序清单：信号量例程
 *
 * 该例程创建了一个动态信号量和静态信号量，分别展示了获取信号量超时和获取成功的情况
 *
 */
#include <rtthread.h>

/* 信号量控制块 */
static struct rt_semaphore static_sem;
/* 指向信号量的指针 */
static rt_sem_t dynamic_sem = RT_NULL;

ALIGN(RT_ALIGN_SIZE)
static char thread1_stack[1024];
static struct rt_thread thread1;
static void rt_thread_entry1(void *parameter)
{
    rt_err_t result;
    rt_tick_t tick;

    /* 1. staic semaphore demo */

    /* 获得当前的OS Tick */
    tick = rt_tick_get();

    /* 试图持有信号量，最大等待10个OS Tick后返回 */
    result = rt_sem_take(&static_sem, 10);
    if (result == -RT_ETIMEOUT)
    {
        /* 超时后判断是否刚好是10个OS Tick */
        if (rt_tick_get() - tick != 10)
        {
            rt_sem_detach(&static_sem);
            return;
        }
        rt_kprintf("take semaphore timeout\n");
    }
    else
    {
        /* 因为没有其他地方释放信号量，所以不应该成功持有信号量，否则测试失败 */
        rt_kprintf("take a static semaphore, failed.\n");
        rt_sem_detach(&static_sem);
        return;
    }

    /* 释放一次信号量 */
    rt_sem_release(&static_sem);

    /* 永久等待方式持有信号量 */
    result = rt_sem_take(&static_sem, RT_WAITING_FOREVER);
    if (result != RT_EOK)
    {
        /* 不成功则测试失败 */
        rt_kprintf("take a static semaphore, failed.\n");
        rt_sem_detach(&static_sem);
        return;
    }

    rt_kprintf("take a staic semaphore, done.\n");

    /* 脱离信号量对象 */
    rt_sem_detach(&static_sem);

    /* 2. dynamic semaphore test */

    tick = rt_tick_get();

    /* 试图持有信号量，最大等待10个OS Tick后返回 */
    result = rt_sem_take(dynamic_sem, 10);
    if (result == -RT_ETIMEOUT)
    {
        /* 超时后判断是否刚好是10个OS Tick */
        if (rt_tick_get() - tick != 10)
        {
            rt_sem_delete(dynamic_sem);
            return;
        }
        rt_kprintf("take semaphore timeout\n");
    }
    else
    {
        /* 因为没有其他地方释放信号量，所以不应该成功持有信号量，否则测试失败 */
        rt_kprintf("take a dynamic semaphore, failed.\n");
        rt_sem_delete(dynamic_sem);
        return;
    }

    /* 释放一次信号量 */
    rt_sem_release(dynamic_sem);

    /* 永久等待方式持有信号量 */
    result = rt_sem_take(dynamic_sem, RT_WAITING_FOREVER);
    if (result != RT_EOK)
    {
        /* 不成功则测试失败 */
        rt_kprintf("take a dynamic semaphore, failed.\n");
        rt_sem_delete(dynamic_sem);
        return;
    }

    rt_kprintf("take a dynamic semaphore, done.\n");
    /* 删除信号量对象 */
    rt_sem_delete(dynamic_sem);
}

/* 信号量示例的初始化 */
int semaphore_sample_init()
{
    rt_err_t result;

    /* 初始化静态信号量，初始值是0 */
    result = rt_sem_init(&static_sem, "ssem", 0, RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("init dynamic semaphore failed.\n");
        return -1;
    }

    /* 创建一个动态信号量，初始值是0 */
    dynamic_sem = rt_sem_create("dsem", 0, RT_IPC_FLAG_FIFO);
    if (dynamic_sem == RT_NULL)
    {
        rt_kprintf("create dynamic semaphore failed.\n");
        return -1;
    }

    rt_thread_init(&thread1,
                   "thread1",
                   rt_thread_entry1,
                   RT_NULL,
                   &thread1_stack[0],
                   sizeof(thread1_stack), 11, 5);
    rt_thread_startup(&thread1);

    return 0;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(semaphore_sample_init, semaphore sample);
```

## 运行结果 ##

```
 \ | /
- RT -     Thread Operating System
 / | \     3.0.4 build Jul 17 2018
 2006 - 2018 Copyright by rt-thread team
msh >sem
semaphore_sample_init
msh >semaphore_sample_init
msh >take semaphore timeout
take a staic semaphore, done.
take semaphore timeout
take a dynamic semaphore, done.
```


