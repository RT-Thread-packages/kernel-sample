# 使用开关中断进行线程间同步

## 介绍

这个例程展示了使用开关中断进行线程间同步。

# 程序清单

```{.c}
/* 代码清单：关闭中断进行全局变量的访问 */
#include <rthw.h>
#include <rtthread.h>

/* 同时访问的全局变量 */
static rt_uint32_t cnt;
void thread_entry(void* parameter)
{
    rt_uint32_t no;
    rt_uint32_t level;

    no = (rt_uint32_t) parameter;
    while(1)
    {
        /* 关闭中断 */
        level = rt_hw_interrupt_disable();
        cnt += no;
        /* 恢复中断 */
        rt_hw_interrupt_enable(level);

        rt_kprintf("thread[%d]'s counter is %d\n", no, cnt);
        rt_thread_delay(no);
    }
}

/* 用户应用程序入口 */
void interrupt_sample()
{
    rt_thread_t thread;

    /* 创建t1线程 */
    thread = rt_thread_create("t1", thread_entry, (void*)10,
        512, 10, 5);
    if (thread != RT_NULL) rt_thread_startup(thread);

    /* 创建t2线程 */
    thread = rt_thread_create("t2", thread_entry, (void*)20,
        512, 20, 5);
    if (thread != RT_NULL) rt_thread_startup(thread);
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(interrupt_sample, interrupt sample);
```
# 运行结果

 ```{.c}
 \ | /
- RT -     Thread Operating System
 / | \     3.0.4 build Jun 14 2018
 2006 - 2018 Copyright by rt-thread team
msh >inter
interrupt_sample
msh >interrupt_sample
thread[10]'s counter is 10
msh >thread[20]'s counter is 30
thread[10]'s counter is 40
thread[10]'s counter is 50
thread[20]'s counter is 70
thread[10]'s counter is 80
thread[10]'s counter is 90
thread[20]'s counter is 110
thread[10]'s counter is 120
thread[10]'s counter is 130
thread[20]'s counter is 150
thread[10]'s counter is 160
thread[10]'s counter is 170
thread[20]'s counter is 190
thread[10]'s counter is 200
thread[10]'s counter is 210
thread[20]'s counter is 230
thread[10]'s counter is 240
thread[10]'s counter is 250
thread[20]'s counter is 270
...
 ```