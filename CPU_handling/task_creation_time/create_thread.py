from subprocess import Popen,PIPE
avg_times = 0
for i in range(1000):
	avg_times += int(Popen(['./create_thread'],stdout=PIPE).communicate()[0][:-1])
print (str(avg_times/1000))




