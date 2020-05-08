#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/epoll.h>

#define EVENTS_SIZE 1000

//添加epoll事件
void addfd(int epollfd,int fd){
	epoll_event event;
	event.data.fd=fd;
	event.events=(EPOLLIN|EPOLLHUP|EPOLLERR|EPOLLET);
	epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
}

//删除epoll事件
void removefd(int epollfd,int fd){
	epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
	close(fd);
}

//socket初始化
int sock_init(char** argv){
	int sockfd;
	struct sockaddr_in server_addr;
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr(argv[1]);
	server_addr.sin_port=htons(atoi(argv[2]));
	
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
		perror("socket error"),exit(-1);
	
	if(connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
		perror("connect error"),exit(-1);
	
	return sockfd;
}

//监听键盘输入
void* pthread_send(void* arg){
	int sockfd=*(int*)arg;
	char buf[1000];
	while(1){
		memset(buf,0,sizeof(buf));
		fgets(buf,1000,stdin);
		if(send(sockfd,buf,sizeof(buf),0)<0)
			perror("send failed"),exit(0);
	}
}

int main(int argc,char** argv){
	if(argc<3)
		printf("%s ip port\n",argv[0]),exit(-1);
	
	int sockfd=sock_init(argv);
	
	int epollfd=epoll_create(1);
	addfd(epollfd,sockfd);
	epoll_event events[EVENTS_SIZE];
	
	pthread_t pid;
	pthread_create(&pid,0,pthread_send,&sockfd);//创建线程监听键盘输入
	
	while(1){
		int num=epoll_wait(epollfd,events,EVENTS_SIZE,-1);
		for(int i=0;i<EVENTS_SIZE;i++){
			if(events[i].events&EPOLLIN){//数据可读
				char msg[1024];
				if(recv(sockfd,msg,sizeof(msg),0)<0)
					perror("recv error\n"),exit(-1);
				printf("%s\n",msg);
			}
		}
	}
	
	return 0;
}