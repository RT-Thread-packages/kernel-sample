/*
 * 程序清单：空闲任务钩子例程
 *
 * 这个例程设置了一个空闲任务钩子用于计算CPU使用率，并创建一个线程循环打印CPU使用率
 * 通过修改CPU使用率打印线程中的休眠tick时间可以看到不同的CPU使用率
 */

#include <rtthread.h>
#include <rthw.h>

#define THREAD_PRIORITY      25
#define THREAD_STACK_SIZE    512
#define THREAD_TIMESLICE     5

/* 指向线程控制块的指针 */
static rt_thread_t tid = RT_NULL;

#define CPU_USAGE_CALC_TICK    10
#define CPU_USAGE_LOOP        100

static rt_uint8_t  cpu_usage_major = 0, cpu_usage_minor = 0;

/* 记录CPU使用率为0时的总count数 */
static rt_uint32_t total_count = 0;

/* 空闲任务钩子函数 */
static void cpu_usage_idle_hook()
{
    rt_tick_t tick;
    rt_uint32_t count;
    volatile rt_uint32_t loop;

    if (total_count == 0)
    {
        /* 获取 total_count */
        rt_enter_critical();
        tick = rt_tick_get();
        while (rt_tick_get() - tick < CPU_USAGE_CALC_TICK)
        {
            total_count ++;
            loop = 0;
            while (loop < CPU_USAGE_LOOP) loop ++;
        }
        rt_exit_critical();
    }

    count = 0;
    /* 计算CPU使用率 */
    tick = rt_tick_get();
    while (rt_tick_get() - tick < CPU_USAGE_CALC_TICK)
    {
        count ++;
        loop  = 0;
        while (loop < CPU_USAGE_LOOP) loop ++;
    }

    /* 计算整数百分比整数部分和小数部分 */
    if (count < total_count)
    {
        count = total_count - count;
        cpu_usage_major = (count * 100) / total_count;
        cpu_usage_minor = ((count * 100) % total_count) * 100 / total_count;
    }
    else
    {
        total_count = count;

        /* CPU使用率为0 */
        cpu_usage_major = 0;
        cpu_usage_minor = 0;
    }
}

void cpu_usage_get(rt_uint8_t *major, rt_uint8_t *minor)
{
    RT_ASSERT(major != RT_NULL);
    RT_ASSERT(minor != RT_NULL);

    *major = cpu_usage_major;
    *minor = cpu_usage_minor;
}

/* CPU使用率打印线程入口 */
static void thread_entry(void *parameter)
{
    rt_uint8_t major, minor;

    while (1)
    {
        cpu_usage_get(&major, &minor);
        rt_kprintf("cpu usage: %d.%d%\n", major, minor);

        /* 休眠50个OS Tick */
        /* 手动修改此处休眠 tick 时间，可以模拟实现不同的CPU使用率 */
        rt_thread_delay(50);
    }
}

int cpu_usage_init()
{
    /* 设置空闲线程钩子 */
    rt_thread_idle_sethook(cpu_usage_idle_hook);

    /* 创建线程 */
    tid = rt_thread_create("thread",
                           thread_entry, RT_NULL, /* 线程入口是thread_entry, 入口参数是RT_NULL */
                           THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    return 0;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(cpu_usage_init, idle hook sample);
