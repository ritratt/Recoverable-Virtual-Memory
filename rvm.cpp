using namespace std;
#ifndef RVM_H
#define RVM_H
#include "rvm.h"
#endif
#include <iostream>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <list>
#include <fstream>
#include <sys/mman.h>

const char * slash = "/";
const char * ext = ".seg";

typedef struct segment_t
{
	char* segname;
	char* backingFile;
	int size;
	void *segbase;
}segment_t;

list<segment_t> seglist;

/*
Helper functions start here.
*/
bool doesFileExist(char * filename)
{
	bool exists = false;
	struct stat st;
	if(stat(filename, &st) == 0)
		exists = true;
	return exists;
}

bool doesDirectoryExist(const char* directory)
{
	bool dirExists = false;
	/*Checking if Directory Exists*/
	struct stat st;
	if(stat(directory, &st) == 0)
		if((st.st_mode) & (S_IFDIR != 0))
			dirExists = true;
	return dirExists;
}

char* constructSegmentFileName(const char* segname, char* dir)
{	
	char*filename = (char*) malloc(sizeof(dir) + sizeof(slash) + sizeof(segname) + sizeof(ext) + 1);
	strcpy(filename, dir);
	strcat(filename, slash);
	strcat(filename, segname);
	strcat(filename, ext);
	return filename;
}

bool doesSegmentExist(const char* segname)
{
	bool segmentExists = false;
	for(list<segment_t>::iterator it = seglist.begin(); it != seglist.end(); it++)
	{
		segment_t seg = *it;
		if(strcmp(seg.segname, segname) == 0)
		{	
			segmentExists = true;	
			return segmentExists;
		}
	}	
	return segmentExists;
}

/*segment_t searchSegment(void* segbase)
{
	segment_t seg;
	for(list<segment_t>::iterator it = seglist.begin(); it != seglist.end(); it++)
	{
		seg = *it;
		if(seg.segbase == segbase)
		{	
			return seg;
		}
	}	
	return seg;
}
*/
/*
Helper functions end here.
*/

/*
1. Initialize the library with the particular directory as backing store.
*/
rvm_t rvm_init(const char*directory)
{
	rvm_t rvm;
	/*If directory does not exist then we create one*/
	if(!doesDirectoryExist(directory))
		mkdir(directory, 0700);
	/*Load the rvm object with the corresponding directory name*/		
	rvm.directory = (char*)malloc(sizeof(directory));
	strcpy(rvm.directory, directory);
	return rvm;
}

/*
1. Destrop a segment completely - Erasing its backing store.
2. This function should not be called on a segment that is currently mapped.
*/
void rvm_destroy(rvm_t rvm, const char *segname)
{
	/*Check if this segment is mapped
	if(mapped)
		error
	else
		delete the corresponding backing file plus log file.
	*/
}

/*
1. Map a segment from disc to memory.
2. If the segment does not already exist then create it and give it size size_to_create.
3. If the segment exists but is shorter than size_to_create, then extend it until it is long enough.
4. It is an error to map a same segment twice.
*/
void *rvm_map(rvm_t rvm, const char *segname, int size_to_create)
{
	void *segbase;
	/*Check if the segment exists in list
		So we error out*/
	if(doesSegmentExist(segname))
	{
		cout << "ERROR: Segment " << segname << "is already mapped and cannot be mapped again." << endl;
		exit(1);	
	}		
	/*So, if the segment does not exist.*/
	else
	{
		/*Malloc memory*/
		segbase = malloc(size_to_create);
		/*Check if the segment backing file exists*/
		char * filename = constructSegmentFileName(segname, rvm.directory);
		if(doesFileExist(filename))
		{
			cout << "Backing file exists" << endl;
			FILE * bfd;
			bfd = fopen(filename, "rb+");
			fread(segbase, 1, size_to_create, bfd);	
			/*TODO: Then apply logs if any log file exists*/
			cout << segbase << endl;
			printf("From rvm_map : %s\n", (char*)segbase);
			fclose(bfd);
		}
		else
		{
			cout << "Backing file does not exist" << endl;
			/*Create the seg file*/
			ofstream o(filename);
			o.close();			
		}

		segment_t seg;
		seg.size = size_to_create;
		seg.backingFile = (char*)malloc(sizeof(filename) + 1);
		strcpy(seg.backingFile, filename);
		seg.segname = (char*)malloc(sizeof(segname) + 1);
		strcpy(seg.segname, segname);
		seg.segbase = segbase;
		seglist.push_back(seg);
		cout << "Returning" << endl;
		return segbase;
	}
}
/*
const char* getSegname(void * segbase)
{
	const char *seg_name;
	map<const char*, segment_t>::iterator iterator = seg_map.begin();
//	cout << "Iterator first" << iterator->first << endl;
	cout << "MAp size is "<< seg_map.size() << endl;

	for( ;iterator != seg_map.end(); iterator++)
	{
		cout << "In for " << endl;
		segment_t seg = iterator->second;
		cout << "seg.seg_addr is " << seg.seg_addr << endl;
		cout << "segbase is " << segbase << endl;
		if(seg.seg_addr == segbase)
		{
			cout << "The corresponding segname is " << iterator->first << endl; 
			//segname = (char * )malloc(sizeof(iterator->first) + 1);
			seg_name = iterator->first;
			return seg_name;	
		}
	}
	
	cout << "ERROR: The segbase was not found" << endl;
	exit(1);
}
*/
/*
1. Unmap a segment from memory
*/

void rvm_unmap(rvm_t rvm, void * segbase)
{	
	segment_t seg;
	for(list<segment_t>::iterator it = seglist.begin(); it != seglist.end(); it++)
	{
		seg = *it;
		if(seg.segbase == segbase)
		{	
			seglist.erase(it);
		}
	}	
	free(seg.segbase);
}

/*

*/
/*trans_t rvm_begin_trans(rvm_t rvm, int numsegs, void **segbases)
{
		
}
*/
