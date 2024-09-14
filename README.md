# FsuOS SDK 2

## 文档请参见 https://www.fsuos.com/docs/

代码生成器是使用 https://github.com/kolypto/j2cli 工具，大家克隆SDK后，需要自己安装j2工具，相关文档可以自己查看 https://jinja.palletsprojects.com/en/3.1.x/。  
# pip install j2cli[yaml]
根据自己使用的Linux发行版本的不同，命令可以会稍有区别，比如Opensuse下，需要使用  
# pipx install j2cli[yaml]  
不需要root用户，普通用户即可。


需要安装ninja
## 各个不同厂家型号的编译器，请联系相关人员


2024-09-03 支持生成CPP的数据解析代码，支持803/801命令，使SMDDevice可以实现B接口的数据转换。  CPP的数据解析代码不支持，函数，Array等复杂场景，基础的单级配置都支持，如果遇到的时候再做。

2024-09-14 支持生成CPP的代码支持BlockTemplate，需要更多协议进行验证