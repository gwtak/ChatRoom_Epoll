#ifndef THREADPOOL_H
#define THREADPOOL_H

#include<pthread.h>
#include<list>

#include"LOCKER.h"

#define thread_num 8

template<typename T>
class THREADPOOL{
	private:
		std::list<T*> work_queue;
		pthread_t pid[thread_num];
	public:
		THREADPOOL();
		void append(T* client);
		static void* worker(void* arg);
		void run();
		SEM sem;
		LOCKER locker;
};

template<typename T>
THREADPOOL<T>::THREADPOOL(){
	for(int i=0;i<thread_num;i++){
		if(pthread_create(pid+i,NULL,worker,this)<0)
			perror("thread_create error"),exit(-1);
	}
	printf("threadpool create successfully\n");
}

template<typename T>
void THREADPOOL<T>::append(T* client){
	locker.lock();
	work_queue.push_back(client);
	sem.post();
	locker.unlock();
}

template<typename T>
void* THREADPOOL<T>::worker(void *arg){
    THREADPOOL *pool = (THREADPOOL *)arg;
    pool->run();
    return pool;
}

template<typename T>
void THREADPOOL<T>::run(){
	sem.wait();
	locker.lock();
	T* client=work_queue.front();
	work_queue.pop_front();
	locker.unlock();
	client->recv_msg();
	client->send_msg();	
}

#endif