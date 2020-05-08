# Linux环境下聊天室

## server

1、初始化线程池、socket、epoll

2、使用epoll监听socket数据传输

3、触发EPOLLIN后，将client实例推入工作队列，此时互斥锁（mutex）锁住，待推入后解锁

4、信号量（sem）等待工作队列

5、sem触发后，mutex锁住，等待工作完成后解锁

## client

1、初始化socket、epoll

2、开启监听线程，监听键盘输入

3、主线程使用epoll监听socket



## 实验结果

![1](https://raw.githubusercontent.com/gwtak/chat_room_pro/master/image/1.png)

![2](https://raw.githubusercontent.com/gwtak/chat_room_pro/master/image/2.png)

![3](https://raw.githubusercontent.com/gwtak/chat_room_pro/master/image/3.png)