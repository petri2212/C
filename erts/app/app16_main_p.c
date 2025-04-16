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



// create and init a mutex
 pthread_mutex_t  mutexOnSharedRes  = PTHREAD_MUTEX_INITIALIZER;

//Create a condition variable
pthread_cond_t condOnSharedRes =  PTHREAD_COND_INITIALIZER; 

//Create a shared variable macchine
int shared_variable_NS = 0;
int shared_variable_EO = 0;
//Create a shared variable semaforo
bool shared_NS = true;
bool shared_EO = false;


int flag = 0;
bool pablo = true;

bool Ambulanza_sh = false;
int cicliAmbulanzaflag = 0;

/*
 * Function that implements the activity
 * the input parameter "cost" is used to change the computational cost
 */
void Semaforo(int delta) {
	
    pthread_mutex_lock( &mutexOnSharedRes );

    cicliAmbulanzaflag++;

    if(  Ambulanza_sh  ){
        shared_NS = false;
        shared_EO = false;
        printf("   Stato AMBULANZA  \n\n");

         pthread_cond_wait(&condOnSharedRes, &mutexOnSharedRes);
        cicliAmbulanzaflag = 0;
        	 
    }else{

    pthread_mutex_unlock( &mutexOnSharedRes );

    flag++;

    if (flag==3){
        pthread_mutex_lock( &mutexOnSharedRes );
    if(shared_NS == true && shared_EO ==false ){

        shared_NS = false;
        shared_EO = true;

    }else{
        shared_NS = true;
        shared_EO = false;
    }

	pthread_mutex_unlock( &mutexOnSharedRes ); 


        flag=0;
    } 
    printf("   *** Stato Semaforo NS: = %d\n   *** Stato Semaforo EO: =   %d\n\n", shared_NS ,shared_EO );

    pthread_mutex_lock (&mutexOnSharedRes); //ENTER CRIT. SEC.
	if(shared_NS == true && shared_EO ==false ){
        if(shared_variable_NS == 0){
             pthread_cond_wait(&condOnSharedRes, &mutexOnSharedRes);
        }
        shared_variable_NS -= delta ;
    }else{
        if(shared_variable_EO == 0){
             pthread_cond_wait(&condOnSharedRes, &mutexOnSharedRes);
        }
        shared_variable_EO -= delta ;
    }
    
	pthread_mutex_unlock (&mutexOnSharedRes); //EXIT CRIT. SEC.
    
    
	printf("   --- Stato Decremento NS: = %d\n   --- Stato Decremento EO: =   %d\n\n", shared_variable_NS , shared_variable_EO);	
}
	   	
}



void GestioneIncrement(int delta) {
	activity_load(2);

    

	pthread_mutex_lock (&mutexOnSharedRes); //ENTER CRIT. SEC.
	
    if (pablo == true){
        shared_variable_EO += delta ;
        pablo = false;
    }else{
        shared_variable_NS += delta ;
        pablo = true;
    }

    if(shared_variable_EO >= 2 || shared_variable_NS >= 2 )
        pthread_cond_signal(&condOnSharedRes);


	printf("   +++ Stato Incremento NS: = %d\n   +++ Stato Incremento EO: =   %d\n\n", shared_variable_NS , shared_variable_EO);	
	pthread_mutex_unlock (&mutexOnSharedRes); //EXIT CRIT. SEC.
}


void Ambulanza(int delta) {
    
	
    
    pthread_mutex_lock (&mutexOnSharedRes); //ENTER CRIT. SEC.

        if (cicliAmbulanzaflag > 20)
    {
         Ambulanza_sh = true;

         pthread_cond_signal(&condOnSharedRes);
        
    }
       
    
    pthread_mutex_unlock (&mutexOnSharedRes); //EXIT CRIT. SEC.

	
}







int main(int argc, char* argv[]) {
    pthread_t thread1, thread2, thread3;
    int ret_err;

	/* Create the first periodic thread */
	t_activity_par activity_1;
	sprintf(activity_1.name,	"Semaforo");
	activity_1.function = Semaforo;
	activity_1.period = 1200;
	activity_1.parameter = 1;
    ret_err = pthread_create( &thread1, NULL, PeriodicTask, (void*) &activity_1);
    handle_error(ret_err, "Error in creating PeriodicTask 1");

	/* Create the second periodic thread */
	t_activity_par activity_2;
	sprintf(activity_2.name,	"GestioneIncrement");
	activity_2.function = GestioneIncrement;
	activity_2.period = 800;
	activity_2.parameter = 1;
    ret_err = pthread_create( &thread2, NULL, PeriodicTask, (void*) &activity_2);
    handle_error(ret_err, "Error in creating PeriodicTask 2");

    t_activity_par activity_3;
	sprintf(activity_3.name,	"Ambulanza");
	activity_3.function = Ambulanza;
	activity_3.period = 800;
	activity_3.parameter = 1;
    ret_err = pthread_create( &thread3, NULL, PeriodicTask, (void*) &activity_3);
    handle_error(ret_err, "Error in creating PeriodicTask 3");


    /* Wait till threads are complete before main continues.  */
    pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);
    pthread_join( thread3, NULL);
    
       	   	
    exit(0);
}


