# kernel samples

## 1、介绍

这个软件包包含了内核相关的使用示例代码。

### 1.1 例程说明

| 文件 | 说明 |
| ---- | ---- |
| dynmem_sample.c | 动态堆内存的使用 |
| event_sample.c  | 事件的使用 |
| idlehook_sample.c  | 空闲任务钩子的使用 |
| interrupt_sample.c | 使用开关中断进行线程间同步 |
| mailbox_sample.c | 邮箱的使用 |
| memp_sample.c  | 内存池的使用 |
| msgq_sample.c | 消息队列的使用 |
| mutex_sample.c  | 互斥量的使用 |
| priority_inversion.c | 优先级翻转特性 |
| producer_consumer.c | 生产者消费者模型 |
| scheduler_hook.c | 调度器钩子的使用 |
| semaphore_sample.c | 信号量的使用|
| signal_sample.c  | 信号的使用 |
| thread_sample.c | 线程的使用 |
| timer_sample.c  | 定时器的使用 |
| timeslice_sample.c  | 线程时间片 |

### 1.2 许可证

kernel samples package 遵循 Apache license v2.0 许可，详见 `LICENSE` 文件。

### 1.3 依赖

依赖系统内核对应的模块。

## 2、如何打开 kernel samples

使用 kernel samples package 需要在 RT-Thread 的 menuconfig 配置菜单中选择它，具体路径如下：

```
RT-Thread online packages
    miscellaneous packages --->
        samples: kernel and components samples --->
            a kernel_samples package for rt-thread --->

```

然后让 RT-Thread 的包管理器自动更新，或者使用 `pkgs --update` 命令更新包到 BSP 中。

## 3、使用 kernel samples

在打开 kernel samples package 后，当进行 BSP 编译时，选择的软件包相关源代码会被加入到 BSP 工程中进行编译。

## 4、注意事项

暂无。

## 5、联系方式 & 感谢

* 维护：name
* 主页：https://github.com/RT-Thread-packages/kernel-sample.git
