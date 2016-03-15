#include <stdio.h>
#include <iostream>
#include "cycle.h"
#include <fstream>
#include <string.h>


#define no_Of_Iterations 10
using namespace std;

int return_time_cache(string &filename) {

	long long int total_time_elapsed;
	long long int overhead;
	
	ifstream inStreamFile;
	int j = 0,i=0;
	ticks tick1, tick2, tick1overhead, tick2overhead;

	inStreamFile.open(filename.c_str(),ios::in);

	if (inStreamFile.is_open()) {
		string line;
		tick1 = getticks();
		
	while(getline(inStreamFile, line))
		i++;
	cout<<"size is"<< i<< "             ";

/*
	for(j = 0; j<no_Of_Iterations; j++)
		{
			getline(inStreamFile, line);
		}*/
		tick2 = getticks();
		inStreamFile.close();
	}
	total_time_elapsed = elapsed(tick2,tick1);

	tick1overhead = getticks();
	for(j = 0; j<i; j+=0)
	{
		j++;
	}	

	/*for(j = 0; j<no_Of_Iterations; j+=0)
	{
		j++;
	}*/
	tick2overhead = getticks();
	overhead = elapsed(tick2overhead,tick1overhead);
	
	//return ((total_time_elapsed-overhead));

	return ((total_time_elapsed-overhead)/1000);
}



void create(long long int size, string filename)
{
	char* array = new char[size];

	// char array[size];

	int x;
	for(x = 0; x<size; x++)
	{
		array[x] = 'a';
	}

	ofstream outstream_File;
	outstream_File.open(filename.c_str());

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


int main(int argc, const char* argv[])
{
	int MBtoKB = 1024;
	string created_file = "testFile.txt";
	
	int sizeOfFile;
	//1.5GB file cache = 1500MB cache = 1536000 KB
	//start from 50KB
	cout << "size (MB)  " << "Time in ms"<< endl;
	for(sizeOfFile = 1; sizeOfFile < 2500000; sizeOfFile=sizeOfFile*2) //this is all in KB
	{
		long long int converted_size = sizeOfFile*MBtoKB;
		create(converted_size, created_file);
		long long int answer = return_time_cache(created_file);
		cout << sizeOfFile << "	aaaaa		" << answer << endl; 
	}
	
}
