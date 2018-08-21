/*
 * 程序清单：内存池例程
 *
 * 这个程序会创建一个静态的内存池对象，2个动态线程。两个线程会试图分别从内存池中获得
 * 内存块
 */
#include <rtthread.h>

static rt_uint8_t *ptr[48];
static rt_uint8_t mempool[4096];
static struct rt_mempool mp;

#define THREAD_PRIORITY      25
#define THREAD_STACK_SIZE    512
#define THREAD_TIMESLICE     5

/* 指向线程控制块的指针 */
static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2 = RT_NULL;

/* 线程1入口 */
static void thread1_entry(void *parameter)
{
    int i, count = 0;
    char *block;

    while (1)
    {
        for (i = 0; i < 48; i++)
        {
            /* 申请内存块 */
            if (ptr[i] == RT_NULL)
            {
                ptr[i] = rt_mp_alloc(&mp, RT_WAITING_FOREVER);
                rt_kprintf("allocate No.%d\n", i);
            }
        }

        /* 继续申请一个内存块，因为已经没有内存块，线程应该被挂起 */
        block = rt_mp_alloc(&mp, RT_WAITING_FOREVER);
        rt_kprintf("allocate the block mem\n");
        /* 释放这个内存块 */
        rt_mp_free(block);
        block = RT_NULL;

        /* 休眠10个OS Tick */
        rt_thread_delay(10);

        /* 循环10次后，退出线程1 */
        if (count++ >= 10) break;
    }
}

/* 线程2入口，线程2的优先级比线程1低，应该线程1先获得执行。*/
static void thread2_entry(void *parameter)
{
    int i, count = 0;

    while (1)
    {
        rt_kprintf("try to release block\n");

        for (i = 0 ; i < 48; i ++)
        {
            /* 释放所有分配成功的内存块 */
            if (ptr[i] != RT_NULL)
            {
                rt_kprintf("release block %d\n", i);

                rt_mp_free(ptr[i]);
                ptr[i] = RT_NULL;
            }
        }

        /* 休眠10个OS Tick */
        rt_thread_delay(10);

        /* 循环10次后，退出线程2 */
        if (count++ >= 10) break;
    }
}

int mempool_sample_init()
{
    int i;
    for (i = 0; i < 48; i ++) ptr[i] = RT_NULL;

    /* 初始化内存池对象 */
    rt_mp_init(&mp, "mp1", &mempool[0], sizeof(mempool), 80);

    /* 创建线程1 */
    tid1 = rt_thread_create("t1",
                            thread1_entry, RT_NULL, /* 线程入口是thread1_entry, 入口参数是RT_NULL */
                            THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    /* 创建线程2 */
    tid2 = rt_thread_create("t2",
                            thread2_entry, RT_NULL, /* 线程入口是thread2_entry, 入口参数是RT_NULL */
                            THREAD_STACK_SIZE, THREAD_PRIORITY + 1, THREAD_TIMESLICE);
    if (tid2 != RT_NULL)
        rt_thread_startup(tid2);

    return 0;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(mempool_sample_init, mempool sample);
