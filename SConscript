from building import *

src   = []
cwd   = GetCurrentDir()
include_path = [cwd]

# add kernel samples.
if GetDepend('KERNEL_SAMPLES_USING_THREAD'):
    src += ['thread_sample.c']

if GetDepend('KERNEL_SAMPLES_USING_SEMAPHORE'):
    src += ['semaphore_sample.c']

if GetDepend('KERNEL_SAMPLES_USING_MUTEX'):
    src += ['mutex_sample.c']

if GetDepend('KERNEL_SAMPLES_USING_MAILBOX'):
    src += ['mailbox_sample.c']

if GetDepend('KERNEL_SAMPLES_USING_EVENT'):
    src += ['event_sample.c']

if GetDepend('KERNEL_SAMPLES_USING_MESSAGEQUEUE'):
    src += ['msgq_sample.c']

if GetDepend('KERNEL_SAMPLES_USING_TIMER'):
    src += ['timer_sample.c']

if GetDepend('KERNEL_SAMPLES_USING_HEAP'):
    src += ['dynmem_sample.c']

if GetDepend('KERNEL_SAMPLES_USING_MEMPOOL'):
    src += ['memp_sample.c']

if GetDepend('KERNEL_SAMPLES_USING_IDLEHOOK'):
    src += ['idlehook_sample.c']

if GetDepend('KERNEL_SAMPLES_USING_SIGNAL'):
    src += ['signal_sample.c']

if GetDepend('KERNEL_SAMPLES_USING_INTERRUPT'):
    src += ['interrupt_sample.c']

if GetDepend('KERNEL_SAMPLES_USING_PRI_INVERSION'):
    src += ['priority_inversion.c']

if GetDepend('KERNEL_SAMPLES_USING_TIME_SLICE'):
    src += ['timeslice_sample.c']

if GetDepend('KERNEL_SAMPLES_USING_SCHEDULER_HOOK'):
    src += ['scheduler_hook.c']

group = DefineGroup('kernel-samples', src, depend = ['PKG_USING_KERNEL_SAMPLES'], CPPPATH = include_path)

Return('group')
