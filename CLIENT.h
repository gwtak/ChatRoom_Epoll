#ifndef CLIENT_H
#define CLIENT_H

#include<sys/epoll.h>
#include<sys/types.h>
#include<sys/socket.h>


#define EVENTS_SIZE 1000
#define CLIENT_SIZE 65535
#define READ_BUF_SIZE 1000
#define WRITE_BUF_SIZE 1024

void addfd(int epollfd,int fd){
	epoll_event event;
	event.data.fd=fd;
	event.events=(EPOLLIN|EPOLLHUP|EPOLLERR|EPOLLET);
	epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
}

void removefd(int epollfd,int fd){
	epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
	close(fd);
}

class CLIENT{
	private:
		int CLIENT_SOCKFD=-1;
		struct sockaddr_in CLIENT_ADDR;
		
	public:
		static int m_epollfd;//epoll事件表描述符，所有client共用
		char read_buf[READ_BUF_SIZE];
		char write_buf[WRITE_BUF_SIZE];
		void init(int connectfd,struct sockaddr_in client_addr);
		void close_conn();
		void recv_msg();
		void send_msg();
};

int CLIENT::m_epollfd=-1;
CLIENT* client=new CLIENT[CLIENT_SIZE];

//client初始化，保存connectfd和client地址
void CLIENT::init(int connectfd,struct sockaddr_in client_addr){
	CLIENT_SOCKFD=connectfd;
	CLIENT_ADDR=client_addr;
	addfd(m_epollfd,connectfd);
	printf("%s:%d接入\n",inet_ntoa(CLIENT_ADDR.sin_addr),ntohs(CLIENT_ADDR.sin_port));
}

//client链接关闭
void CLIENT::close_conn(){
	removefd(m_epollfd,CLIENT_SOCKFD);
	CLIENT_SOCKFD=-1;
}

//收取信息，放入read_buf，添加头部后放入write_buf
void CLIENT::recv_msg(){
	if(recv(CLIENT_SOCKFD,read_buf,sizeof(read_buf),0)<0)
		perror("recv error"),exit(-1);
	
	sprintf(write_buf,"%s:%d:%s\n",inet_ntoa(CLIENT_ADDR.sin_addr),ntohs(CLIENT_ADDR.sin_port),read_buf);
	printf("%s\n",write_buf);
}

//发送write_buf内容
void CLIENT::send_msg(){
	for(int i=0;i<CLIENT_SIZE;i++){
		if(client[i].CLIENT_SOCKFD!=-1){
			send(client[i].CLIENT_SOCKFD,write_buf,sizeof(write_buf),0);
		}
	}
}

#endif