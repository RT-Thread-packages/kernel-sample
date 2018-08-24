from building import *
Import('rtconfig')

src   = []
cwd   = GetCurrentDir()
group = []
CPPPATH = []

# add kernel samples.
if GetDepend('RT_USING_SAMPLE_THREAD'):
    src += ['thread/thread_sample.c']
    CPPPATH += [cwd + '/thread']

if GetDepend('RT_USING_SAMPLE_SEMAPHORE'):
    src += ['semaphore/semaphore_sample.c']
	src += ['semaphore/producer_consumer.c']
    CPPPATH += [cwd + '/semaphore']

if GetDepend('RT_USING_SAMPLE_MUTEX'):
    src += ['mutex/mutex_sample.c']
	src += ['mutex/pri_inversion.c']
    CPPPATH += [cwd + '/mutex']

if GetDepend('RT_USING_SAMPLE_MAILBOX'):
    src += ['mailbox/mailbox_sample.c']
    CPPPATH += [cwd + '/mailbox']

if GetDepend('RT_USING_SAMPLE_EVENT'):
    src += ['event/event_sample.c']
    CPPPATH += [cwd + '/event']

if GetDepend('RT_USING_SAMPLE_MESSAGEQUEUE'):
    src += ['msgq/msgq_sample.c']
    CPPPATH += [cwd + '/msgq']

if GetDepend('RT_USING_SAMPLE_TIMER'):
    src += ['timer/timer_sample.c']
    CPPPATH += [cwd + '/timer']

if GetDepend('RT_USING_SAMPLE_HEAP'):
    src += ['dynmem/dynmem_sample.c']
    CPPPATH += [cwd + '/dynmem']

if GetDepend('RT_USING_SAMPLE_MEMPOOL'):
    src += ['mempool/memp_sample.c']
    CPPPATH += [cwd + '/mempool']

if GetDepend('RT_USING_SAMPLE_IDLEHOOK'):
    src += ['idlehook/idlehook_sample.c']
    CPPPATH += [cwd + '/idlehook']

if GetDepend('RT_USING_SAMPLE_SIGNAL'):
    src += ['signal/signal_sample.c']
    CPPPATH += [cwd + '/signal']
	
group = DefineGroup('samples', src, depend = [''], CPPPATH = CPPPATH)

Return('group')
