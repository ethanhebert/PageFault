//Ethan Hebert - CSC345

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int fifo(int frames, int* parr);
int lru(int frames, int* parr);
int opt(int frames, int* parr);


int main(int argc, char* argv[])
{
	//input handling
	if (argc >= 2) //inputted frames
	{
		int frames = atoi(argv[1]);
		int seed = -1; //default seed
		if (argc >= 3) //inputted seed
		{
			seed = atoi(argv[2]);
		}
		
		
		
		//create pages
		int parr[100] = {2, 8, 7, 2, 0, 3, 1, 7, 3, 1, 9, 3, 6, 1, 8, 4, 5, 1, 8, 8, 3, 4, 4, 3, 4, 2, 0, 6, 6, 7, 0, 1, 0, 9, 3, 6, 4, 6, 8, 0, 4, 6, 2, 3, 5, 7, 8, 0, 3, 2, 0, 0, 0, 4, 6, 9, 1, 4, 3, 8, 8, 0, 0, 9, 7, 0, 7, 0, 9, 7, 7, 3, 8, 8, 9, 2, 7, 2, 1, 2, 0, 9, 1, 1, 1, 5, 0, 7, 1, 4, 9, 1, 9, 5, 8, 4, 4, 7, 9, 6}; //default seed
		if (seed != -1) //inputted seed
		{
			srand(seed);
			for (int i=0; i<100; i++)
				parr[i] = rand() % 10;
		}
		
		//test parr and frames
		/*
		printf("#frames: %d\n", frames);
		printf("parr: ");
		for (int j=0; j<100; j++)
			printf("%d ", parr[j]);
		printf("\n");
		*/
		
		//function calls
		int fifo_pf = fifo(frames, parr);
		int lru_pf = lru(frames, parr);
		int opt_pf = opt(frames, parr);
		
		//print results
		printf("Algorithm\t# Page Faults\n");
		printf("FIFO\t\t%d\n", fifo_pf);
		printf("LRU\t\t%d\n", lru_pf);
		printf("OPT\t\t%d\n", opt_pf);
	}
	
	else //error message
	{
		printf("Usage: ./a.out f s\nf is the number of frames, s is the seed\n");
		exit(1);
	}
	
	return 0;
}




//FUNCTIONS
int fifo(int frames, int* parr)
{
	int fifo_pf = 0;
	int farr[frames];
	for (int k=0; k<frames; k++) //fill frame array with -1 to get rid of 0
		farr[k] = -1;
	int isThere = 0;
	int replace = 0;
	
	for (int i=0; i<100; i++) //iterate through parr
	{
		for (int j=0; j<frames; j++) //iterate through farr
		{
			if (parr[i] == farr[j]) //if the page is already in the frame array, mark it as there
				isThere = 1;
		}
		
		if (isThere == 0) //if the page was not in the frame array, insert it in, increment page faults
		{
			farr[replace] = parr[i];
			replace = (replace + 1) % frames;
			fifo_pf += 1;
		}
		
		isThere = 0;
	}
	
	return fifo_pf;
}




int lru(int frames, int* parr)
{
	int lru_pf = 0;
	int isThere = 0;
	int farr[frames];
	for (int k=0; k<frames; k++) //fill frame array with -1 to get rid of 0
		farr[k] = -1;
	int age[frames]; //tracks how long each frame has been held by a page
	for (int k=0; k<frames; k++) //start all empty frames at age 100 so they get replaced for being so old
		age[k] = 100;
		
	
	for (int i=0; i<100; i++) //iterate through parr
	{
		for (int j=0; j<frames; j++) //iterate through farr
		{
			if (parr[i] == farr[j]) //if the page is already in the frame array, mark it as there and reset age to 0
			{
				isThere = 1;
				age[j] = 0;
			}
			
			age[j] += 1; //increment every frame's age
		}
		
		if (isThere == 0) //page not there, replace the oldest 
		{
			int oldest = 0;
			for (int j=0; j<frames; j++) //iterate through farr to find oldest frame
			{
				if (age[j] > age[oldest])
					oldest = j;
			}
			
			farr[oldest] = parr[i]; //replace oldest frame and reset its age
			age[oldest] = 1;
			
			lru_pf += 1; //increment page faults
		}
			
		isThere = 0;	
	}
	
	return lru_pf;
}




int opt(int frames, int* parr)
{
	int opt_pf = 0;
	int curr;
	int maxIndex;
	int isThere = 0;
	int farr[frames];
	for (int k=0; k<frames; k++) //fill frame array with -1 to get rid of 0
		farr[k] = -1;
	int distance[frames];
	for (int k=0; k<frames; k++) //fill distance array with 101 - max distance (item not found)
		distance[k] = 101;


	for (int i=0; i<100; i++) //iterate through parr
	{
		for (int j=0; j<frames; j++) //iterate through farr
		{	
			if (parr[i] == farr[j]) //if the page is already in the frame array, mark it as there and break loop
			{
				isThere = 1;
				break;
			}
			else if (farr[j] == -1) //empty frame - automatically fill it and break loop
			{
				farr[j] = parr[i];
				isThere = 1;
				opt_pf += 1;
				break;
			}
			
		}
		
		if (isThere == 0) //if the page was not in the frame array, find a frame to replace
		{
			//fill the distance array
			for (int j=0; j<frames; j++) //iterate through farr again
			{
				curr = i;
				for (curr; curr<100; curr++) //iterate through parr again, starting at curr
				{
					if (farr[j] == parr[curr]) //when you find the distance to the next frame item, record it
					{
						distance[j] = curr;
						break;
					}
				}
			}
			
			//find largest value in distance array and replace this value
			maxIndex = 0;
			for (int j=0; j<frames; j++)
			{
				if (distance[j] > distance[maxIndex])
					maxIndex = j;
			}
			
			farr[maxIndex] = parr[i]; //replace the correct frame with the page value
			opt_pf += 1;
			
			//reset variables
			for (int k=0; k<frames; k++) //reset distance array to 101
				distance[k] = 101;
		}
		
		isThere = 0;
	}
	
	return opt_pf;
}


