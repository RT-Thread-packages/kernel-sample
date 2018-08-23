/*
 * 程序清单：静态线程脱离
 *
 * 这个例子会初始化两个静态线程(t1和t2)，在t2中会对t1进行脱离操作；
 * t1脱离后将不再运行，状态也更改为初始状态
 */
#include <rtthread.h>

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

/* 线程1控制块 */
static struct rt_thread thread1;
/* 线程1栈 */
static rt_uint8_t thread1_stack[THREAD_STACK_SIZE];
/* 线程2控制块 */
static struct rt_thread thread2;
/* 线程2栈 */
static rt_uint8_t thread2_stack[THREAD_STACK_SIZE];

/* 线程1入口 */
static void thread1_entry(void* parameter)
{
    rt_uint32_t count = 0;

    while (1)
    {
        /* 线程1采用低优先级运行，一直打印计数值 */
        rt_kprintf("thread count: %d\n", count ++);
    }
}

/* 线程2入口 */
static void thread2_entry(void* parameter)
{
    /* 线程2拥有较高的优先级，以抢占线程1而获得执行 */

    /* 线程2启动后先睡眠10个OS Tick */
    rt_thread_delay(10);

    /*
     * 线程2唤醒后直接执行线程1脱离，线程1将从就绪线程队列中删除
     */
    rt_thread_detach(&thread1);

    /*
     * 线程2继续休眠10个OS Tick然后退出
     */
    rt_thread_delay(10);

    /*
     * 线程2运行结束后也将自动被从就绪队列中删除，并脱离线程队列
     */
}

/* 线程脱离示例的初始化 */
int thread_detach_init(void)
{
    rt_err_t result;

    /* 初始化线程1 */
    result = rt_thread_init(&thread1, "t1",     /* 线程名：t1 */
                            thread1_entry,      /* 线程的入口是thread1_entr */
                            RT_NULL,            /* 入口参数是RT_NULL*/
                            &thread1_stack[0],  /* 线程栈是thread1_stack */
                            sizeof(thread1_stack),
                            THREAD_PRIORITY, 10);
    if (result == RT_EOK)                       /* 如果返回正确，启动线程1 */
        rt_thread_startup(&thread1);

    /* 初始化线程2 */
    result = rt_thread_init(&thread2, "t2",     /* 线程名：t2 */
                            thread2_entry,      /* 线程的入口是thread2_entry */
                            RT_NULL,            /* 入口参数是RT_NULL*/
                            &thread2_stack[0],  /* 线程栈是thread2_stack */
                            sizeof(thread2_stack),
                            THREAD_PRIORITY - 1, 10);
    if (result == RT_EOK)                       /* 如果返回正确，启动线程2 */
        rt_thread_startup(&thread2);

    return 0;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(thread_detach_init, thread detach);
