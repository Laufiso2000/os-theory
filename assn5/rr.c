//Jared Laufiso
//Assignment 5: CPU Scheduler Simulator
//Round Robin

/* Promise of Originality
I promise that this source code file has, in its entirety, been
written by myself or a member of my group and by no other person or
persons. If at any time an exact copy of this source code is found to
be used by another person outside my group in this term, I understand
that all members of my group and the members of the group that
submitted the copy will receive a zero on this assignment.
*/


#include<stdio.h>
#include<stdlib.h>
#define MAX 100

void roundRobin(int, int*, int*);

int main()
{
int count = 0;
int sub[MAX],run[MAX];


FILE *fp = fopen("process-list.txt","r");

while (fscanf(fp,"%d %d", &sub[count], &run[count])==2) count++;

roundRobin(count, sub,run);

}


double Averager(int size, double* numbers)
{
	int i;
	double sum = 0;
	
	for(i = 0;i<size;i++)
			sum += numbers[i];
	
	sum = sum/size;
	
	
	return sum;
	
}

void roundRobin (int count, int *sub, int *run)
{
	
	int quantum = 100;
	
	//readyQ variables. 
	int ready[count];
	int index = 0;				//Head eadyQ possition
	int readyQ_size = 0; 		//size of readyQ
	int end = 0;				//end ofreadyQ
	
	
	//clock acounting variables
	double rem_time[count];
	double clock = 0;
	double next_clock = 0;
//	int current_clock[100];
//	int clocker = 0;


	//statistics
	double turnaround[count], wait[count], response[count];
	double average_ta = 0, average_wait = 0, average_resp = 0;
	
	//response[] initialized to -1 to protect data. 
	int q;
	for(q=0;q<count;q++)response[q] = -1;
	
	
	
	int job = 0;
	int completed_jobs = 0;
//	int context_switch = 10;
	

	while (completed_jobs < count )
	{
		/* 
		this first section handles the case that the waiting q is empty and the job
		is first to be seeen. 
		*/
		
		//readyQ is empty and the sub time is greater than current time
		//update the clock to start time of submision. 
		if ( readyQ_size == 0 && sub[job] > clock )
		{
			
			clock = sub[job]; 				//set clock to the start time of first submission. 
			
			if(response[job]<0)
					response[job] = 0;
			if(run[job] <= quantum) 
			{
				next_clock = clock + run[job];
				wait[job] = 0;
				turnaround[job] = run[job];
				completed_jobs++;
            	job++;
				
			
				//look at the next jobs to determin if they need to be added to the readyQ.
				while (sub[job] <= next_clock && job < count) 
	        	{

					ready[end] = job;			//add process to end of q
					rem_time[job] = run[job];	//remaining time 
					readyQ_size++; 
					end = (end + 1) % count;
            		job++;
        	
				}
			}
			else if (run[job] > quantum)
			{
				next_clock = clock+quantum;
				rem_time[job] = run[job]-quantum;
				int current_job = job;
				job++;
				while (sub[job] <= next_clock && job < count) 
	        	{

					ready[end] = job;			//add process to end of q
					rem_time[job] = run[job];	//remaining time 
					readyQ_size++; 
					end = (end + 1) % count;
            		job++;
        	
				}
					ready[end] = current_job;			//add process to end of q
					readyQ_size++; 
					end = (end + 1) % count;
				
			}
			
				


		}//<<<<<<end of if ( readyQ_size == 0 && sub[job] > clock )
		//////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////
		else 
		{
			//if the remaining time = run time 
			//then this is the first time this job is getting cpu time
			//and needs to have the respose time set. 
			if (rem_time[ready[index]] == run[ready[index]]) 
			{
				if(response[ready[index]] < 0) //it will be -1 if it hasn't already been set. 
					response[ready[index]] = (clock - sub[ready[index]]);
				
			}
		
			
			if (rem_time[ready[index]] <= quantum )
			{
				next_clock = clock + rem_time[ready[index]];
				
				turnaround[ready[index]] = (next_clock - sub[ready[index]]);
				wait[ready[index]] = (turnaround[ready[index]] - run[ready[index]]);
				
				completed_jobs++;
				readyQ_size--;
				index = (index+1) % count;
				
				while (sub[job] <= next_clock && job < count) 
	        	{

					ready[end] = job;			//add process to end of q
					rem_time[job] = run[job];	//remaining time 
					readyQ_size++; 
					end = (end + 1) % count;
            		job++;
        	
				}
			}
		
		
			//If the process has remaining time, put it back in the ready queue. 
			//Rem time decrements by quantum.
			else if(rem_time[ready[index]] > quantum) 
			{
				next_clock = clock + quantum;
				
				rem_time[ready[index]] = rem_time[ready[index]] - quantum;

				while (sub[job] <= next_clock && job < count) 
	        	{

					ready[end] = job;			//add process to end of q
					rem_time[job] = run[job];	//remaining time 
					readyQ_size++; 
					end = (end + 1) % count;
            		job++;
        	
				}
				ready[end] = ready[index];
				end = (end + 1) % count;
				index = (index + 1) % count;
			}

		}//<<<<<end of intitial else <<<<<<<<<
		
		//update clock.
		clock = next_clock;

	}//<<<< end of while loop<<<<<

		printf("job    resp   wait   ta\n");
	int p;	
	for(p=0;p<count;p++)
	{
		printf(" %d     %.2f     %.2f    %.2f\n",p,response[p],wait[p],turnaround[p]);
	}


	average_wait = Averager(count,wait);
	average_resp = Averager(count,response);
	average_ta   = Averager(count,turnaround);
	
	printf("Round Robin with Time Quantum of %d\n", quantum);
	printf ("Avg. Resp.:%.2f, Avg. T.A:%.2f, Avg. Wait:%.2f\n", average_resp, average_ta, average_wait);
}


