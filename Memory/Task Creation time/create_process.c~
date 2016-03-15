#include <time.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>

#define ITERATIONS 1
#define STACK_SIZE (1024 * 1024)

struct timespec time_start,time_end,time_taken;
struct timespec diff(struct timespec start, struct timespec end);
unsigned long  *times;

void inline GetElapsedTime() {
    	int i;
	int child_pid=0;


    	for (i = 0; i < ITERATIONS; i++) {
		clock_gettime(CLOCK_REALTIME,&time_start);
		child_pid=fork();
    		clock_gettime(CLOCK_REALTIME,&time_end);
		 if (child_pid > 0 ){
      			  kill(child_pid,SIGKILL);
  		  }
		time_taken = diff(time_start,time_end);
		times[i] = time_taken.tv_nsec;
    	}
    
    	return;
}

int main( int argc, const char* argv[] ){

	int i;
	unsigned long average_time = 0;

	times = malloc(ITERATIONS*sizeof(unsigned long*));

	if (!times) {
		printf("Failed to allocate memory\n");
		return 0;
	}	

	GetElapsedTime(times);

	for (i = 0; i < ITERATIONS; i++) {
		average_time += times[i];
	}

	average_time /= ITERATIONS;
	printf("%lu\n", average_time);
	return 0;
}

struct timespec diff(struct timespec start, struct timespec end)
{
	struct timespec t;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		t.tv_sec = end.tv_sec-start.tv_sec-1;
		t.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		t.tv_sec = end.tv_sec-start.tv_sec;
		t.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return t;
}
