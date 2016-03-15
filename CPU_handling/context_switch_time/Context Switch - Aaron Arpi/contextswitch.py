import Queue

def foo():
    print('Constructing foo')
    yield
    print('Running in foo')
    yield
    print('Explicit context switch to foo again')

def bar():
    print('Constructing bar')
    yield
    print('Explicit context to bar')
    yield
    print('Implicit context switch back to bar')

def trampoline(taskq):
    while not taskq.empty():
        task = taskq.get()
        try:
            task.next()
            taskq.put(task)
        except StopIteration:
            pass

tasks = Queue.Queue()
for i in range(0,100):  
    tasks.put(foo())
    tasks.put(bar())
    trampoline(tasks)

print('Finished')
