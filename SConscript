
from building import *
Import('rtconfig')

src   = []
cwd   = GetCurrentDir()
group = []
CPPPATH = []

# add kernel samples.
if GetDepend('KERNEL_SAMPLES_USING_THREAD'):
    src += ['thread/thread.c']
    src += Glob('thread/*/*.c')
    CPPPATH += [cwd + '/thread']

if GetDepend('KERNEL_SAMPLES_USING_SEMAPHORE'):
    src += ['semaphore/semaphore.c']
    CPPPATH += [cwd + '/semaphore']

if GetDepend('KERNEL_SAMPLES_USING_MUTEX'):
    src += ['mutex/mutex.c']
    CPPPATH += [cwd + '/mutex']

if GetDepend('KERNEL_SAMPLES_USING_MAILBOX'):
    src += ['mailbox/mailbox.c']
    CPPPATH += [cwd + '/mailbox']

if GetDepend('KERNEL_SAMPLES_USING_EVENT'):
    src += ['event/event.c']
    CPPPATH += [cwd + '/event']

if GetDepend('KERNEL_SAMPLES_USING_MESSAGEQUEUE'):
    src += ['msg/msg.c']
    CPPPATH += [cwd + '/msg']

if GetDepend('KERNEL_SAMPLES_USING_TIMER'):
    src += ['timer/timer.c']
    CPPPATH += [cwd + '/timer']

if GetDepend('KERNEL_SAMPLES_USING_HEAP'):
    src += ['heap/heap.c']
    CPPPATH += [cwd + '/heap']

if GetDepend('KERNEL_SAMPLES_USING_MEMPOOL'):
    src += ['mempool/memp.c']
    CPPPATH += [cwd + '/mempool']

if GetDepend('KERNEL_SAMPLES_USING_IDLEHOOK'):
    src += ['idlehook/idlehook.c']
    CPPPATH += [cwd + '/idlehook']

if GetDepend('KERNEL_SAMPLES_USING_PRODUCER_CONSUMER'):
    src += ['producer_consumer/producer_consumer.c']
    CPPPATH += [cwd + '/producer_consumer']

if GetDepend('KERNEL_SAMPLES_USING_SIGNAL'):
    src += ['signal/signal.c']
    CPPPATH += [cwd + '/signal']

group = DefineGroup('kernel-samples', src, depend = ['PKG_USING_KERNEL_SAMPLES'], CPPPATH = CPPPATH)

Return('group')
