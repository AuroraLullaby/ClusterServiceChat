# ClusterServiceChat
1.基于muduo网络库实现，将网络模块代码和通信业务模块代码解耦，可以实现业务和通信的代码分离    
2.实现Json实现数据的序列化与反序列化作为私有通信协议  
3.通过线性池实现主线程读操作，其他线程写操作，实现高并发    
4.nginx反向代理实现负载均衡，实现聊天服务器的集群功能，提高后端服务的并发能力  
5.通过mysql数据库实现在线信息的实时收发和离线信息的缓存功能，提高项目数据的落盘存储(include/server/db)  
6.通过redis的发布订阅模式工作在nginx /tcp 负载均衡模式下，实现跨服务器的信息通信(include/server/redis)  
7.compile.sh的shell文件全局编译

