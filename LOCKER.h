#ifndef LOCKER_H
#define LOCKER_H

#include<exception>
#include<semaphore.h>
#include<pthread.h>

//封装信号量的类，记录工作队列内请求数量
class SEM{
	private:
		sem_t m_sem;
	public:
		SEM(){
			if(sem_init(&m_sem,0,0)!=0){
				throw std::exception();
			}
		}
		~SEM(){
			sem_destroy(&m_sem);
		}
		bool wait(){
			return sem_wait(&m_sem)==0;
		}
		bool post(){
			return sem_post(&m_sem)==0;
		}
};

//封装互斥锁的类，保护工作队列
class LOCKER{
	private:
		pthread_mutex_t m_mutex;
	public:
		LOCKER(){
			if(pthread_mutex_init(&m_mutex,NULL)!=0){
				throw std::exception();
			}
		}
		~LOCKER(){
			pthread_mutex_destroy(&m_mutex);
		}
		bool lock(){
			return pthread_mutex_lock(&m_mutex) == 0;
		}
		bool unlock(){
			return pthread_mutex_unlock(&m_mutex) == 0;
    }
};

#endif