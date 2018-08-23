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
void interrupt_sample_init()
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
MSH_CMD_EXPORT(interrupt_sample_init, interrupt sample);