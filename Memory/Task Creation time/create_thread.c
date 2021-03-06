#include <time.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>

#define iter 1
struct timespec time_start,time_end,time_taken;
struct timespec diff(struct timespec start, struct timespec end);
unsigned long  *times;

void *increment_x(void *x_void_ptr) {
    int *x_ptr = (int *)x_void_ptr;
    while(++(*x_ptr) < 100);
 return NULL;
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
void inline GetElapsedTime() {
    	int i,x=0;
	pthread_t inc_x_thread;


    	for (i = 0; i < iter; i++) {
		clock_gettime(CLOCK_REALTIME,&time_start);
if(pthread_create(&inc_x_thread, NULL, increment_x,&x)) {
        fprintf(stderr, "Error\n");
        
    	}
	clock_gettime(CLOCK_REALTIME,&time_end);
		 
		time_taken = diff(time_start,time_end);
		times[i] = time_taken.tv_nsec;
    	}
    
    	return;
}

int main( int argc, const char* argv[] ){

	int i;
	unsigned long average_time = 0;

	times = malloc(iter*sizeof(unsigned long*));

	if (!times) {
		printf("less memory\n");
		return 0;
	}	

	GetElapsedTime(times);

	for (i = 0; i < iter; i++) {
		average_time += times[i];
	}

	average_time /= iter;
	printf("%lu\n", average_time);
	return 0;
}


