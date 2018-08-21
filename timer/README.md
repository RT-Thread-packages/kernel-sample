# 定时器的使用 #

## 介绍 ##

这个例程会创建两个动态定时器对象，一个是单次定时，一个是周期性的定时。

## 程序清单 ##

```{.c}
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
/* 如果设置了RT_SAMPLES_AUTORUN，则加入到初始化线程中自动运行 */
#if defined (RT_SAMPLES_AUTORUN) && defined(RT_USING_COMPONENTS_INIT)
    INIT_APP_EXPORT(timer_sample_init);
#endif
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(timer_sample_init, timer sample);
```

## 运行结果 ##

```
 \ | /
- RT -     Thread Operating System
 / | \     3.0.4 build Jun 14 2018
 2006 - 2018 Copyright by rt-thread team
msh >timer
timer_static_sample_init
msh >timer_static_sample_init
msh >periodic timer is timeout
periodic timer is timeout
one shot timer is timeout
periodic timer is timeout
periodic timer is timeout
periodic timer is timeout
periodic timer is timeout
periodic timer is timeout
periodic timer is timeout
periodic timer is timeout
periodic timer is timeout
periodic timer is timeout
```