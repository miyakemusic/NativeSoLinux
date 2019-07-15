#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

pthread_t thread;
pthread_mutex_t testFuncMutex, waitInterruptMutex;
pthread_cond_t testFuncCond, waitInterruptCond;

void * testFunc( void * arg ) {
	while(true) {
		pthread_mutex_lock(&testFuncMutex);
		pthread_cond_wait(&testFuncCond, &testFuncMutex);
		pthread_mutex_unlock(&testFuncMutex);
		for (int i = 0; i < 5; i++){
			sleep(1);
			printf("%d\n", i);
		}
	    pthread_mutex_lock(&waitInterruptMutex);
	    pthread_cond_signal(&waitInterruptCond);
	    pthread_mutex_unlock(&waitInterruptMutex);
	}
}

extern "C"
{
bool init() {
	 pthread_mutex_init(&testFuncMutex, NULL);
	 pthread_mutex_init(&waitInterruptMutex, NULL);
	 pthread_cond_init(&testFuncCond, NULL);
	 pthread_cond_init(&waitInterruptCond, NULL);
	 pthread_create(&thread, NULL, testFunc, NULL);

	 return true;
}

bool writeBlock(unsigned long addr, unsigned char* data, int size) {

	return true;
}
bool writeReg32(unsigned long addr, unsigned int data)
{
	if ((addr == 0x04) && ((data & 0x01) == 0x01)) {
	    pthread_mutex_lock(&testFuncMutex);
	    pthread_cond_signal(&testFuncCond);
	    pthread_mutex_unlock(&testFuncMutex);
	}

	return true;
}

unsigned int readReg32(unsigned long addr) {
	return true;
}

bool readBlock(unsigned long addr, unsigned char* data, int size) {
	return true;
}

int waitInterrupt()
{
	pthread_mutex_lock(&waitInterruptMutex);
	pthread_cond_wait(&waitInterruptCond, &waitInterruptMutex);
	pthread_mutex_unlock(&waitInterruptMutex);

	return true;
}

}
