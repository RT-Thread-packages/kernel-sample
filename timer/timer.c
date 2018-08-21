/*
* 程序清单：定时器例程
*
* 这个例程会创建两个动态定时器对象，一个是单次定时，一个是周期性的定时
* */
#include <rtthread.h>

/* 定时器的控制块 */
static rt_timer_t timer1;
static rt_timer_t timer2;
static int cnt = 0;

/* 定时器1超时函数 */
static void timeout1(void *parameter)
{
    rt_tick_t timeout = 300;

    rt_kprintf("periodic timer is timeout\n");

    /* 运行10次 */
    if (cnt++ >= 10)
    {
        rt_timer_control(timer1, RT_TIMER_CTRL_SET_ONESHOT, (void *)&timeout);
    }
}

/* 定时器2超时函数 */
static void timeout2(void *parameter)
{
    rt_kprintf("one shot timer is timeout\n");
}

int timer_sample_init(void)
{
    /* 创建定时器1 */
    timer1 = rt_timer_create("timer1", /* 定时器名字是 timer1 */
                             timeout1, /* 超时时回调的处理函数 */
                             RT_NULL, /* 超时函数的入口参数 */
                             10, /* 定时长度，以OS Tick为单位，即10个OS Tick */
                             RT_TIMER_FLAG_PERIODIC); /* 周期性定时器 */

    /* 启动定时器 */
    if (timer1 != RT_NULL) rt_timer_start(timer1);

    /* 创建定时器2 */
    timer2 = rt_timer_create("timer2", /* 定时器名字是 timer2 */
                             timeout2, /* 超时时回调的处理函数 */
                             RT_NULL, /* 超时函数的入口参数 */
                             30, /* 定时长度为30个OS Tick */
                             RT_TIMER_FLAG_ONE_SHOT); /* 单次定时器 */

    /* 启动定时器 */
    if (timer2 != RT_NULL) rt_timer_start(timer2);
    return 0;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(timer_sample_init, timer sample);
