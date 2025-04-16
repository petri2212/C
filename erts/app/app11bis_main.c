/*
 * Main program
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <stdint.h>  	
#include <time.h>       
#include <math.h>
#include <stdbool.h>

#include "../lib/time_library.h"
#include "../lib/activity_library.h"



/*
 * Function that implements the activity
 * the input parameter "cost" is used to change the computational cost
 */
void Activity1(void *parameter) {
    //printf("Activity1 parla\n");
	
	float pointer ;
	pointer= *((float*)parameter);
	//int a=5;
	
	
	
	printf("Activity1 parameter: %f\n", pointer);
	//loat p =  (*parameter);
	int p=10;
	double result = 0;
    for (long i=0; i<pointer*1000*1000; i++) {
		result = result + sin(i) + cos(i) + i *tan(i);
    }
}

void Activity2(void *parameter) {
	double pointer ;
	pointer= *((double*)parameter);
	//float p = (*parameter);
	printf("Activity2 parameter: %f\n", pointer);
    //printf("Activity2 parla\n");
	double result = 0;
    for (long i=0; i<pointer*1000*1000; i++) {
		result = result + sin(i) + cos(i) + i *tan(i);
    }
}




int main(int argc, char* argv[]) {
    pthread_t thread1, thread2;
    int ret_err;  
	
	float first = 1.23;
	double secondo =2.444;
	
	
	
	/* Create the first periodic thread */
	t_activity_par activity_1;
	sprintf(activity_1.name,	"Activity_1");
	activity_1.function = Activity1;
	activity_1.period = 800;
	activity_1.parameter = &first;
    ret_err = pthread_create( &thread1, NULL, PeriodicTask, (void*) &activity_1);
    handle_error(ret_err, "Error in creating PeriodicTask 1");

	/* Create the second periodic thread */
	t_activity_par activity_2;
	sprintf(activity_2.name,	"Activity_2");
	activity_2.function = Activity2;
	activity_2.period = 800;
	activity_2.parameter = &secondo;
    ret_err = pthread_create( &thread2, NULL, PeriodicTask, (void*) &activity_2);
    handle_error(ret_err, "Error in creating PeriodicTask 2");

    /* Wait till threads are complete before main continues.  */
    pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);
       	   	
    exit(0);
}


