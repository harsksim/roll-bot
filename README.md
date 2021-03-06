# roll-bot
基于 __[酷Q（CQP）](https://cqp.cc/)__ 与 __[C++版SDK](https://github.com/CoolQ/cqsdk-vc)__ 的骰子机器人。  

只支持私聊和群，不支持讨论组

# 用法
注册一个专用的QQ号 - [http://im.qq.com/](http://im.qq.com/)  

下载酷Q - [https://cqp.cc/](https://cqp.cc/)  

安装酷Q

将本源中的app目录下的文件拷贝到 _<酷Q安装目录>/app/_ 下

启动酷Q并登录QQ号

在任务栏的酷Q图标上右键选择_应用_ - _应用管理_，启用骰子机器人

# 命令示例
※全部使用半角字符和空格，不能有其他字符
- .r
- .r 100
- .r 100 明天下雨
- .r 2d6
- .r 2d6+4
- .r 2d6+4 明天下雨

# 原理
在群消息和私聊中用对收到的消息进行正则匹配，命中后按照规则进行解析并计算骰子点数。  

### 正则字符串
> ``^[\\./][rR]( ([1-9])[dD]([1-9][0-9]{0,2})(([\\+-])([1-9][0-9]{0,3}))?)?( [\\D]+)?``  
> ``^[\\./][rR] ([1-9][0-9]{0,2})( [\\D]+)?``

# 关联地址
酷Q：https://cqp.cc/ 
 
酷Q C++ SDK：https://github.com/CoolQ/cqsdk-vc  

QQ：http://im.qq.com/