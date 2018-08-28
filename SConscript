
from building import *
Import('rtconfig')

src   = []
cwd   = GetCurrentDir()
group = []
CPPPATH = []

# add kernel samples.
if GetDepend('KERNEL_SAMPLES_USING_THREAD'):
    src += Glob('thread/*.c')
    CPPPATH += [cwd + '/thread']

if GetDepend('KERNEL_SAMPLES_USING_SEMAPHORE'):
    src += Glob('semaphore/*.c')
    CPPPATH += [cwd + '/semaphore']

if GetDepend('KERNEL_SAMPLES_USING_MUTEX'):
    src += Glob('mutex/*.c')
    CPPPATH += [cwd + '/mutex']

if GetDepend('KERNEL_SAMPLES_USING_MAILBOX'):
    src += Glob('mailbox/*.c')
    CPPPATH += [cwd + '/mailbox']

if GetDepend('KERNEL_SAMPLES_USING_EVENT'):
    src += Glob('event/*.c')
    CPPPATH += [cwd + '/event']

if GetDepend('KERNEL_SAMPLES_USING_MESSAGEQUEUE'):
    src += Glob('msgq/*.c')
    CPPPATH += [cwd + '/msgq']

if GetDepend('KERNEL_SAMPLES_USING_TIMER'):
    src += Glob('timer/*.c')
    CPPPATH += [cwd + '/timer']

if GetDepend('KERNEL_SAMPLES_USING_HEAP'):
    src += Glob('dynmem/*.c')
    CPPPATH += [cwd + '/dynmem']

if GetDepend('KERNEL_SAMPLES_USING_MEMPOOL'):
    src += Glob('mempool/*.c')
    CPPPATH += [cwd + '/mempool']

if GetDepend('KERNEL_SAMPLES_USING_IDLEHOOK'):
    src += Glob('idlehook/*.c')
    CPPPATH += [cwd + '/idlehook']

if GetDepend('KERNEL_SAMPLES_USING_SIGNAL'):
    src += Glob('signal/*.c')
    CPPPATH += [cwd + '/signal']

if GetDepend('KERNEL_SAMPLES_USING_INTERRUPT'):
    src += Glob('interrupt/*.c')
    CPPPATH += [cwd + '/interrupt']
	
group = DefineGroup('kernel-samples', src, depend = ['PKG_USING_KERNEL_SAMPLES'], CPPPATH = CPPPATH)

Return('group')
