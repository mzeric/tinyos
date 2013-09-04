# BrightSky 操作系统github站点
## 从code.google.com/p/brightsky 上镜像到 @github.com 上来


## A simple OS just for fun!


众人拾柴火焰高!
版本说明

【brightsky0.02】于0.03版本合并挂出

新功能：

一个控制台
测试任务切换

##

BSky之前的版本是挂在学校的内网上的，07.3日搬到clinux.org上， 为了保持版本的连续性，把之前的版本（0.02省略掉了）也一并挂上来； 日志保持到最新进展，只列出升级增加部分

##

2006-5-23 完成:

【 brightsky0.01 】

---boot:

软盘引导，硬件初始化，Setup.asm中进入保护模式，转入C代码
---kernel:

i386.c,kbd.c,mouse.c,video.c,kernel.c string.c i386.c : x86的初始化,包括中断门,调用门,任务门,

基本的输入输出设备驱动

message.c :简易的消息机制 video.c :图形模式的驱动，含基本的字符/串/点输出函数 console.c :控制台驱动，内嵌有几个演示命令 head.asm :汇编初始化部分、中断入口等

##【brightsky0.02】
与0.03版本合并挂出


##【brightsky0.03】
---boot:

bsldr.asm : 增加了硬盘引导功能,会加载根目录下的bsldr文件

---kernel:

i386.c,kbd.c,mouse.c,video.c,kernel.c string.c i386.c : x86的初始化,包括中断门,调用门,任务门,

video.c :增加了几个图形函数如：画线、矩形； console.c :控制台驱动，内嵌有几个演示命令； string.c :优化了kpt算法； memory.c :增加内存管理算法，不支持虚拟内存；
