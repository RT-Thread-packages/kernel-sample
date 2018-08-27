# kernel samples

## 1、介绍

这个软件包包含了内核相关的使用示例代码。

### 1.1 目录结构

| 名称 | 说明 |
| ---- | ---- |
| dynmem | 动态堆内存的使用 |
| event  | 事件的使用 |
| idlehook  | 空闲任务钩子的使用 |
| interrupt | 使用开关中断进行线程间同步 |
| mailbox | 邮箱的使用 |
| mempool  | 内存池的使用 |
| msgq | 消息队列的使用 |
| mutex  | 互斥量的使用、防止优先级翻转特性 |
| semaphore | 信号量的使用、生产者消费者模型 |
| signal  | 信号的使用 |
| thread | 线程的使用、线程优先级、线程时间片、调度器钩子 |
| timer  | 定时器的使用 |

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
