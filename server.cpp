#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h> 
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/epoll.h>

#include"CLIENT.h"
#include"THREADPOOL.h"


int sock_init(char** argv){
	int listenfd;
	struct sockaddr_in server_addr;
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr(argv[1]);
	server_addr.sin_port=htons(atoi(argv[2]));
	socklen_t server_addr_len=sizeof(server_addr);
	
	if((listenfd=socket(AF_INET,SOCK_STREAM,0))<0)
		perror("socket error"),exit(-1);
	
	if(bind(listenfd,(struct sockaddr*)&server_addr,server_addr_len)<0)
		perror("bind error"),exit(-1);
	
	if(listen(listenfd,CLIENT_SIZE)<0)
		perror("listen error"),exit(-1);
	
	return listenfd;
}

int main(int argc,char** argv){
	if(argc<3)
		printf("%s ip port\n",argv[0]),exit(-1);
	
	int listenfd=sock_init(argv);
	
	epoll_event events[EVENTS_SIZE];
	int epollfd=epoll_create(1);
	addfd(epollfd,listenfd);
	
	CLIENT::m_epollfd=epollfd;//static变量
	
	THREADPOOL<CLIENT> pool;
	
	printf("server is running on %s:%s\n",argv[1],argv[2]);
	
	while(1){
		int num=epoll_wait(epollfd,events,EVENTS_SIZE,-1);
		for(int i=0;i<num;i++){
			int sockfd=events[i].data.fd;
			if(sockfd==listenfd){//新连接
				int connectfd;
				struct sockaddr_in client_addr;
				memset(&client_addr,0,sizeof(client_addr));
				socklen_t client_addr_len=sizeof(client_addr);
				
				if((connectfd=accept(sockfd,(struct sockaddr*)&client_addr,&client_addr_len))<0)
					perror("accept error"),exit(-1);
				
				client[connectfd].init(connectfd,client_addr);
			}
			else if(events[i].events&(EPOLLERR|EPOLLHUP)){//socket关闭或错误
				client[sockfd].close_conn();
			}
			else if(events[i].events&EPOLLIN){//数据可读
				pool.append(client+sockfd);
			}
		}
	}
	removefd(epollfd,listenfd);
	return 0;
}