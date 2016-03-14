#include <errno.h>
#include <cstdlib> 
#include <ctime> 
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "cycle.h"
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <sys/stat.h>


using namespace std;

#define ITERATIONS 10
#define BLOCK_SIZE 4096

const int MAX_THREAD = 10;
const char *FNAME_PREFIX= "f";
const int FILE_SIZE = 1024*1024*8; //1MB file
char* filename_array[MAX_THREAD];

int file_name_counter = 0;
double total_time_thread = 0.0;
pthread_mutex_t thread_mutex = PTHREAD_MUTEX_INITIALIZER;


int file_exist(char *filename){
  struct stat buffer;
  return (stat(filename, &buffer) == 0);
}

void create(long long int size, char* filename)
{
  if(file_exist(filename)) return;
  char* array = new char[size];


  int x;
  for(x = 0; x<size; x++)
  {
    array[x] = 'a';
  }

  ofstream outstream_File;
  outstream_File.open(filename);

  if(outstream_File.is_open())
  {
    for(x=0; x<size; x++)
    {
      outstream_File << array[x] << endl;
    }
    outstream_File.close();
  }

  delete[] array;
}

int seq_read(long long int size, char* filename) {

  ticks tick1, tick2;
  long long int totalTimeElapsed;
  int fd;
  char* buffer; //= new char[size];
  int page = getpagesize();

  posix_memalign((void**) &buffer, page, size);
  fd = open(filename, O_RDONLY | O_DIRECT);
  if(fd != -1)
  {
    tick1 = getticks();
    lseek(fd, 0, SEEK_CUR);
    read(fd, buffer, size);
    tick2 = getticks();

    if(errno != 0) // to check if their are no c error codes in linux, used header file errno.h
    {
      cout << "errno: " << errno << endl;
      return -1;
    }

    //depending on the size of the file, you may need to change this
    //so that it doesn't give stack overflow
    totalTimeElapsed = ((elapsed(tick2,tick1) - 45)*0.417)/ITERATIONS; 
  }
  else
  {
    cout << "file not opened" << endl;
  }

  close(fd);
  delete[] buffer;

  return totalTimeElapsed;
}



void *run_readfile(void *ptr)
{
  //sequential access
  long long int stotal;

  //cout << "size (KB)  " << "Time/block (ns) S " << "Time/byte (ns) R "<< endl;
  //change the numbers here to get more size stuff

  int i;
  stotal = 0;
  long long int converted_size = FILE_SIZE;
  for(i = 0; i < ITERATIONS; i++)
  {
    long long int sAnswer = seq_read(converted_size, (char*)ptr);
    stotal += sAnswer;
  }
  //long long int bytePerBlock = ((long) BLOCK_SIZE)/((long) sizeof(char));
  long long int sFinal = (stotal)/converted_size*((long) BLOCK_SIZE)/((long) sizeof(char));

  pthread_mutex_lock(&thread_mutex);
  total_time_thread += sFinal;
  pthread_mutex_unlock(&thread_mutex);
}

double create_process(int no_threads){
  pthread_t thread_id[MAX_THREAD];
  total_time_thread = 0.0;
  int i,j;

  for(i = 0 ; i < no_threads; i ++){
    pthread_create( &thread_id[i] , NULL, run_readfile, filename_array[i]);
  }
  for(j = 0 ; j < no_threads; j ++){
    pthread_join(thread_id[j], NULL);
  }
  return total_time_thread / no_threads;
}

int main(int argc, const char* argv[]){
  int k;
  for(k = 0 ; k < MAX_THREAD; k ++){
    char buffer[10];
    sprintf(buffer, "%d", file_name_counter);
    filename_array[k] = new char[50];
    strcpy(filename_array[k], FNAME_PREFIX);
    strcat(filename_array[k], buffer);
    file_name_counter ++;
    create(FILE_SIZE, filename_array[k]);
  }
cout << create_process(0) << endl;
cout << create_process(1) << endl;
  cout << create_process(2) << endl;
  cout << create_process(3) << endl;
  cout << create_process(4) << endl;
  cout << create_process(5) << endl;
  cout << create_process(6) << endl;
  cout << create_process(7) << endl;
  cout << create_process(8) << endl;
  cout << create_process(9) << endl;
  cout << create_process(10) << endl;

  for(k = 0 ; k < MAX_THREAD; k ++){
    delete[] filename_array[k];
  }
  exit(0);
}
