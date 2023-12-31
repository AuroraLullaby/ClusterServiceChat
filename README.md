# 一.项目依赖  
1.Json三方库 (https://github.com/nlohmann/json)  
2.muduo网络库 (https://github.com/chenshuo/muduo)  
3.负载均衡nginx (https://github.com/nginx/nginx)  
4.MySQL数据库: sudo apt instll mysql-server    
5.Redis :sudo apt-get install redis-server  
# 二.项目框架  
业务模块设计：从网络模块接收数据，根据 messageID 定位到注册模块。客户端传递过来的 json 对象反序列化获取用户 ID 和用户密码。并生成 User 对象，调用 model 层方法将新生成的 User 插入到数据库中。 
  
登录模块设计：获取用户ID和密码，并在数据库中查询获取用户信息是否匹配。如果用户已经登录过，状态信息state == "online"，则返回错误信息。登录成功后需要在服务端的用户表中记录登录用户数量，并显示该用户的好友列表和收到的离线消息。  

异常退出模块设计：如果客户端异常退出，会从服务端记录用户连接的表中找到该用户，如果断连就从表中删除，并设置其状态为 offline。如果服务端异常退出，信号处理函数，向信号注册回调函数，然后在函数内将所有用户置为下线状态。  

点对点聊天模块设计：通过传递的 json 查找对话用户 ID。用户处于登录状态：直接向该用户发送信息；用户处于离线状态：需存储离线消息。  

添加好友模块设计：从 json 对象中获取添加登录用户 ID 和其想添加的好友的 ID，调用 model 层代码在 friend 表中插入好友信息。  

群组模块设计：  
 1.创建群组需要描述群组名称，群组的描述，然后调用 model 层方法在数据库中记录新群组信息。    
 2.加入群组需要给出用户 ID 和想要加入群组的 ID，其中会显示该用户是群组的普通成员还是创建者。  
 3.群组聊天给出群组 ID 和聊天信息，群内成员在线会直接接收到  
# 三.Nginx负载均衡模块使用  
  安装好Nginx后，在cong文件中添加TCP模块信息，使负载均衡可以收到TCP流消息(可以使用netstat查看是否开启，nginx默认工作在80端口)  
  作用：  
 1.把client的请求按照负载算法分发到具体的业务服务器ChatServer上  
 2.能够ChantServer保持心跳机制，检测ChatServer故障  
 3.能够发现新添加的ChatServer设备，方便扩展服务器数量  
# 四.Redis发布订阅消息的使用  
  引入中间件消息队列，解耦各个服务器，使整个系统松耦合，提高服务器的响应能力，节省服务器的带宽资源。发布订阅功能要开两条redis连接即两个线程,一条用于订阅和接收消息，一条用于发布消息，因为订阅消息会进入阻塞状态，而发布消息不会。
# 五.线程池的使用(小技巧)  
  使用了一个while循环，在线程池处于工作时循环从任务队列中提取任务。并利用条件变量，在任务队列为空时阻塞当前线程，等待上文中的提交函数添加任务后发出的通知。在任务队列不为空时，将任务队列中的任务取出，并放在事先声明的基础函数类func中。成功取出后便立即执行该任务。
# 六.编译
  提供了两种编译方式
1.compile.sh的shell文件全局编译  
2.build文件中有compile.txt，也就是cmake.. 和make 可以完成编译，在bin目录中运行。
# 七.总结  
1.基于muduo网络库实现，将网络模块代码和通信业务模块代码解耦，可以实现业务和通信的代码分离    
2.实现Json实现数据的序列化与反序列化作为私有通信协议  
3.通过线性池实现主线程读操作，其他线程写操作，实现高并发    
4.nginx反向代理实现负载均衡，实现聊天服务器的集群功能，提高后端服务的并发能力  
5.通过mysql数据库实现在线信息的实时收发和离线信息的缓存功能，提高项目数据的落盘存储(include/server/db)  
6.通过redis的发布订阅模式工作在nginx /tcp 负载均衡模式下，实现跨服务器的信息通信(include/server/redis)  


