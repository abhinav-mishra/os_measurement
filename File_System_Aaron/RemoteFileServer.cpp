#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <cstring>
#include <vector>
#include <array>
#include <iostream>
#include <string>
#include <stdio.h>
#include <python2.7/Python.h>
#define ITERATIONS 1000
#define KB(x)   ((x) << 10)
#define MB(x)   ((x) << 20)


using namespace std;

int compare(const void *, const void *);
void GetElapsedTime(uint64_t *, double size_increment);
void Loop(void);

const int numMB=50;
unsigned char first_array[MB(numMB)];
unsigned char second_array[MB(numMB)];
float cycles_per_second = 2.5*1000*1000*1000;  //2.5*10^9 = 2.5GHZ

int main(void)
{
    int i;
    uint64_t *times;
    double results[ITERATIONS];

    times = (uint64_t *)malloc(ITERATIONS*sizeof(uint64_t*));

    if (!times) {
        printf("[!] Failed to allocate times memory\n");
        return 0;
    }
    vector<float> divisions;
    for (int i=0; i<=50; i++){
       divisions.push_back(.02*i);
    }
 
    
    for (int a=0; a<1; ++a){
       double mem_amount = divisions[a]*sizeof(first_array);
       GetElapsedTime(times,mem_amount);
       for (i = 0; i < ITERATIONS; i++) {
           results[i] = times[i];
           //printf("Iteration %d: timing overhead = %" PRIu64 " cycles\n", i, times[i]);
       }
       qsort (results, ITERATIONS, sizeof(uint64_t), compare);
       double fastest = results[0];
       float num_MBs_copied = numMB*divisions[a];
       printf("Size of array slice: %f --- MB/Second coppied  %6.10lf MB/s",num_MBs_copied, (num_MBs_copied)/((float) fastest/cycles_per_second));      
       cout<<" "<<endl;
 //      printf("The median time to copy across all iterations is: %f nanoseconds\n", results[ITERATIONS/2]*cycle_time);
 //      printf("The minimum time to copy accross all iterations is: %1.13lf nanoseconds\n", ((float) fastest)*cycle_time); 
       
    }
    return 0;
}

int compare (const void * a, const void * b) 
{
    return ( *(int*)a - *(int*)b );
}

void inline GetElapsedTime(uint64_t *times, double size) {
    unsigned long flags;
    uint64_t start, end;
    int i;
    unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
    volatile int variable = 0;
        


    asm volatile (
                 "CPUID\n\t"
                 "RDTSC\n\t"
                 "mov %%edx, %0\n\t"
                 "mov %%eax, %1\n\t"
                 : "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx"
                 );


    asm volatile (
                 "RDTSCP\n\t"
                 "mov %%edx, %0\n\t"
                 "mov %%eax, %1\n\t"
                 "CPUID\n\t"
                 : "=r" (cycles_high1), "=r" (cycles_low1)::"%rax", "%rbx", "%rcx", "%rdx"
                 );
    
    for (int i = 0; i < 1; i++) {

    
          asm volatile (
                     "CPUID\n\t"
                     "RDTSC\n\t"
                     "mov %%edx, %0\n\t"
                     "mov %%eax, %1\n\t"
                     : "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx"
                     );

          system("python server.py 2 2");
         

          asm volatile (
                     "RDTSCP\n\t"
                     "mov %%edx, %0\n\t"
                     "mov %%eax, %1\n\t"
                     "CPUID\n\t"
                     : "=r" (cycles_high1), "=r" (cycles_low1)::"%rax", "%rbx", "%rcx", "%rdx"
                     );

          start = (((uint64_t) cycles_high << 32) | cycles_low);
          end = (((uint64_t) cycles_high1 << 32) | cycles_low1);
          times[i] = end - start;
    }
    
    return;

   

}
