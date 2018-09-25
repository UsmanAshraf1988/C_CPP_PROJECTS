#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

using namespace std;

pthread_cond_t p_cond_t  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t p_mtx_sum_t = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t p_mtx_cond_t = PTHREAD_MUTEX_INITIALIZER;

int g_num=0, g_sum=0;

static void exit_handler_mem (void * );
void * threadFuncOddSum(void *);
class threadClass;
typedef void * (*THREADFUNCPTR)(void *);

void exit_handler_mem (void * arg) {
   cout<< "called: Exit handler...\n";
   if(arg) delete arg;
   cout<<"Exit clean up done...\n";
}

void * threadFuncOddSum(void * arg){

	cout<<"Thread func with id: "<<pthread_self()<<"\n";

	pthread_mutex_lock(&p_mtx_cond_t);
	while((int)(*(int *)arg)%2==0) pthread_cond_wait(&p_cond_t, &p_mtx_cond_t);
	cout<<"thread func *arg: "<< (int)(*(int *)arg)<< "\n";
	pthread_mutex_unlock(&p_mtx_cond_t);



	pthread_mutex_lock(&p_mtx_sum_t);

	//To mimic some resources which require cleanup
	char *pChar = new char();
	pthread_cleanup_push(exit_handler_mem, (void *)pChar);
	g_sum= g_sum+(int)(*(int *)arg);
	*(int *)arg = (int)(*(int *)arg) + 1;
	cout<<"From thread func g_sum: "<<g_sum<<" and Now (int)(*(int *)arg): "<<(int)(*(int *)arg)<<"\n";

	cout <<"From thread function calling for pthread_cleanup_pop(1)...\n";
	pthread_cleanup_pop(1);

	pthread_mutex_unlock(&p_mtx_sum_t);	
	pthread_exit( (void *)pthread_self() );
	return((void*)NULL);
}

class threadClass{
public:
	void * threadClassFuncEvenSum( void * arg){
		cout<<"Thread class func with id: "<<pthread_self()<<"\n";
		int l_num = (int)(*(int *)arg);

		pthread_mutex_lock(&p_mtx_cond_t);
		while( g_num%2!=0) pthread_cond_wait(&p_cond_t, &p_mtx_cond_t);
		cout<<"thread class func g_num: "<< g_num << "\n";
		pthread_mutex_unlock(&p_mtx_cond_t);


		pthread_mutex_lock(&p_mtx_sum_t);
		//To mimic some resources which require cleanup
		char *pChar = new char();
		pthread_cleanup_push(exit_handler_mem, (void *)pChar);

		g_sum= g_sum+g_num;
		g_num = g_num + 1;
		cout <<"From thread class func g_sum: "<<g_sum<<" and Now g_num: "<< g_num <<"\n";
		
		cout <<"From thread class func calling for pthread_cleanup_pop(1)...\n";
		pthread_cleanup_pop(1);

		pthread_mutex_unlock(&p_mtx_sum_t);

		pthread_exit( (void *)pthread_self() );
		return((void*)NULL);
	}
};



int main(int argc, char * argv []) {

	cout<<"Main thread with id: "<<pthread_self()<<"\n";
	pthread_t p_th1_t, p_th2_t;
	threadClass tc;

	pthread_create( &p_th1_t, NULL, threadFuncOddSum, &g_num);
	// Thats not possible due to number of args to pthread_create.
	//pthread_create( &p_th2_t, NULL, (THREADFUNCPTR)&threadClass::threadClassFuncEvenSum, &tc, &g_num);
	pthread_create( &p_th2_t, NULL, (THREADFUNCPTR)&threadClass::threadClassFuncEvenSum, &tc);




        pthread_join(p_th1_t, NULL);

	return 0;
}


