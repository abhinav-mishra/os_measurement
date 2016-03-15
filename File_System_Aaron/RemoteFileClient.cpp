#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <cstring>
#include <vector>
#include <array>
#include <algorithm>
#include <iostream>
#include <string>
#include <stdio.h>
#include <python2.7/Python.h>
#define ITERATIONS 50
#define KB(x)   ((x) << 10)
#define MB(x)   ((x) << 20)


using namespace std;

void GetElapsedTime(uint64_t *);
int inline GetOverhead();
int numreads=5;
void Loop(void);
char* mode;

float cycles_per_second = 2.4*1000*1000*1000;  //2.5*10^9 = 2.5GHZ
int numlines = 10000000;

int main(int argc,char *argv[])
{
    mode = argv[1]; // r or s
    int i;
    uint64_t *times;
    vector<unsigned long long> results;
    
    times = (uint64_t *)malloc(ITERATIONS*sizeof(unsigned long long*));
        
    system("python maketestfile.py 10000000");


    if (!times) {
        printf("[!] Failed to allocate times memory\n");
        return 0;
    }
 
    int overhead = GetOverhead();    

    for (int a=0; a<1; ++a){
       GetElapsedTime(times);
       for (i = 0; i < ITERATIONS; i++) {
           if (*mode=='r' | *mode == 's'){          
              results.push_back(times[i]-overhead);
           }
           else { results.push_back(times[i]);}
           printf("Iteration %d: time = %" PRIu64 " cycles\n", i, times[i]);
       }
      
       std::sort(results.begin(), results.end());
       unsigned long long fastest = results[ITERATIONS-1];
       unsigned long long slowest = results[0];
       printf("The median time to read across all iterations is: %f seconds\n", (float) (results[ITERATIONS/2])/(cycles_per_second*numreads));
       printf("The maximum time to read accross all iterations is: %1.13lf seconds\n", ((float) fastest)/(cycles_per_second*numreads)); 
       printf("The minimum time to read accross all iterations is: %1.13lf seconds\n", ((float) slowest)/(cycles_per_second*numreads));       
    }


    return 0;
}

void inline GetElapsedTime(uint64_t *times) {
    unsigned long flags;uint64_t start, end;
    int i; unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
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
    
    for (int i = 0; i < ITERATIONS; i++) {
    
          asm volatile (
                     "CPUID\n\t"
                     "RDTSC\n\t"
                     "mov %%edx, %0\n\t"
                     "mov %%eax, %1\n\t"
                     : "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx"
                     );

          if (*mode == 'r'){
             system("python client.py 10000000 r 5");
          }
          if (*mode =='s'){
             system("python client.py 10000000 s 5");
          }
 
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

int inline GetOverhead() {
    unsigned long flags;uint64_t start, end;
    int i; unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
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

          asm volatile (
                     "CPUID\n\t"
                     "RDTSC\n\t"
                     "mov %%edx, %0\n\t"
                     "mov %%eax, %1\n\t"
                     : "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx"
                     );

    
          system("python client.py 10000000 r 0");

          asm volatile (
                     "RDTSCP\n\t"
                     "mov %%edx, %0\n\t"
                     "mov %%eax, %1\n\t"
                     "CPUID\n\t"
                     : "=r" (cycles_high1), "=r" (cycles_low1)::"%rax", "%rbx", "%rcx", "%rdx"
                     );

          start = (((uint64_t) cycles_high << 32) | cycles_low);
          end = (((uint64_t) cycles_high1 << 32) | cycles_low1);
          return (end - start);
}

