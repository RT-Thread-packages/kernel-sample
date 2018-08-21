/*
 * 程序清单：信号例程
 *
 * 这个例子会创建两个线程线程1和线程2，每个线程会分别安装信号，然后给这两个线程发送信号。
 *
 */
#include <rtthread.h>

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

/*
 * 为了在一个线程中访问另一个线程的控制块，所以把线程块指针中访问
 * 另一个线程的控制块，所以把线程块指针声明成全局类型以供全局访问
 */
static rt_thread_t tid1 = RT_NULL, tid2 = RT_NULL;

/* 线程1的信号处理函数 */
void thread1_signal_handler(int sig)
{
    rt_kprintf("thread1 received signal %d\n", sig);
}

/* 线程2的信号处理函数 */
void thread2_signal_handler(int sig)
{
    rt_kprintf("thread2 received signal %d\n", sig);
}

/* 线程1的入口函数 */
static void thread1_entry(void *parameter)
{
    int cnt = 0;

    /* 安装信号 */
    rt_signal_install(SIGUSR1, thread1_signal_handler);
    rt_signal_unmask(SIGUSR1);

    /* 运行10次 */
    while (cnt < 10)
    {
        /* 线程1采用低优先级运行，一直打印计数值 */
        rt_kprintf("thread1 count : %d\n", cnt);

        cnt++;
        rt_thread_delay(10);
    }
}

/* 线程2的入口函数 */
static void thread2_entry(void *parameter)
{
    int cnt = 0;

    /* 安装信号 */
    rt_signal_install(SIGUSR2, thread2_signal_handler);
    rt_signal_unmask(SIGUSR2);

    /* 运行10次 */
    while (cnt < 10)
    {
        rt_kprintf("thread2 count : %d\n", cnt);

        cnt++;
        rt_thread_delay(20);
    }
}

/* 信号示例的初始化 */
int signal_sample_init()
{
    /* 创建线程1 */
    tid1 = rt_thread_create("t1", /* 线程1的名称是t1 */
                            thread1_entry, RT_NULL,   /* 入口是thread1_entry，参数是RT_NULL */
                            THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);

    if (tid1 != RT_NULL) /* 如果获得线程控制块，启动这个线程 */
        rt_thread_startup(tid1);

    /* 创建线程2 */
    tid2 = rt_thread_create("t2", /* 线程2的名称是t2 */
                            thread2_entry, RT_NULL,   /* 入口是thread2_entry，参数是RT_NULL */
                            THREAD_STACK_SIZE, THREAD_PRIORITY - 1, THREAD_TIMESLICE);

    if (tid2 != RT_NULL) /* 如果获得线程控制块，启动这个线程 */
        rt_thread_startup(tid2);

    rt_thread_delay(50);

    /* 发送信号 SIGUSR1 给线程1 */
    rt_thread_kill(tid1, SIGUSR1);
    /* 发送信号 SIGUSR2 给线程2 */
    rt_thread_kill(tid2, SIGUSR2);

    return 0;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(signal_sample_init, signal sample);
