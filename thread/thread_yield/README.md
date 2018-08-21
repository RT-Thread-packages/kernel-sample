# 线程让出处理器 #

## 介绍 ##

在这个例子中，将创建两个相同优先级的线程， 它们会通过rt_thread_yield
接口把处理器相互让给对方进行执行。

## 程序清单 ##

```{.c}
/*
 * 程序清单：线程让出处理器
 * 在这个例子中，将创建两个相同优先级的线程， 它们会通过rt_thread_yield
 * 接口把处理器相互让给对方进行执行。
 */
#include <rtthread.h>

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

/* 指向线程控制块的指针 */
static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2 = RT_NULL;
/* 线程1入口 */
static void thread1_entry(void* parameter)
{
    rt_uint32_t count = 0;

    while (1)
    {
        /* 打印线程1的输出 */
        rt_kprintf("thread1: count = %d\n", count ++);

        /* 执行yield后应该切换到thread2执行 */
        rt_thread_yield();
    }
}

/* 线程2入口 */
static void thread2_entry(void* parameter)
{
    rt_uint32_t count = 0;

    while (1)
    {
        /* 打印线程2的输出 */
        rt_kprintf("thread2: count = %d\n", count ++);

        /* 执行yield后应该切换到thread1执行 */
        rt_thread_yield();
    }
}

/* 线程让出处理器示例的初始化 */
int thread_yield_init(void)
{
    /* 创建线程1 */
    tid1 = rt_thread_create("thread",
                            thread1_entry,      /* 线程入口是thread1_entry */
                            RT_NULL,            /* 入口参数是RT_NULL */
                            THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    /* 创建线程2 */
    tid2 = rt_thread_create("thread",
                            thread2_entry,      /* 线程入口是thread2_entry */
                            RT_NULL,            /* 入口参数是RT_NULL */
                            THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid2 != RT_NULL)
        rt_thread_startup(tid2);

    return 0;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(thread_yield_init, thread yield);
```

## 运行结果 ##

```
 \ | /
- RT -     Thread Operating System
 / | \     3.0.4 build Jul 17 2018
 2006 - 2018 Copyright by rt-thread team
msh >th
thread_yield_init
msh >thread_yield_init
msh >thread1: count = 0
thread2: count = 0
thread1: count = 1
thread2: count = 1
thread1: count = 2
thread2: count = 2
...
thread2: count = 23
thread1: count = 24
thread2: count =thread1: count = 25
 25
thread1: count = 26
thread2: count = 25
thread1: count = 27
thread2: count = 26
threathread2: count = 27
d2: count = 27
thread2: count = 28
thread1: count = 29
thread2: count = 29
...
thread1: count = 51
thread2: count = 51
thread1: cthread2: count = 52
ount = 52
thread2: count = 53
thread1: count = 53
thread2: count = 54
thread1: count = 54
...
```