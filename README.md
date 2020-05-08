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

[]([https://github.com/gwtak/chat_room_pro/blob/master/image/@%25U4T)OP4XUGEXGG3LX%7DC3U.png?raw=true](https://github.com/gwtak/chat_room_pro/blob/master/image/@%U4T)OP4XUGEXGG3LX}C3U.png?raw=true))

[]([https://raw.githubusercontent.com/gwtak/chat_room_pro/master/image/46EZ89XD_Q)%40%600HGWL7DBXW.png]

[]()

