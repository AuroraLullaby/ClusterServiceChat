#ifndef REDIS_H
#define REDIS_H

#include <hiredis/hiredis.h>
#include <thread>
#include <functional>
using namespace std;

/*
redis作为集群服务器通信的基于发布-订阅消息队列时，会遇到两个难搞的bug问题，参考我的博客详细描述：
https://blog.csdn.net/QIANGWEIYUAN/article/details/97895611
*/
class Redis
{
public:
    Redis();
    ~Redis();

    // 连接redis服务器 
    bool connect();

    // 向redis指定的通道channel发布消息
    bool publish(int channel, string message);

    // 向redis指定的通道subscribe订阅消息
    bool subscribe(int channel);

    // 向redis指定的通道unsubscribe取消订阅消息,离线之后直接存到数据库中了，如果不取消订阅，redis中数据会越来越多
    bool unsubscribe(int channel);

    // 在独立线程中接收订阅通道中的消息
    void observer_channel_message();

    // 初始化向业务层上报通道消息的回调对象
    void init_notify_handler(function<void(int, string)> fn);

private:
    // hiredis同步上下文对象，负责publish消息
    redisContext *_publish_context;

    // hiredis同步上下文对象，负责subscribe消息
    redisContext *_subcribe_context;   // 发布消息后，就阻塞了，所以发布和订阅需要两个客户端

    // 回调操作，收到订阅的消息，给service层上报
    function<void(int, string)> _notify_message_handler; // 相当于通知消息notify
};

#endif
