# BrightSky 操作系统github站点
## 从code.google.com/p/brightsky 上镜像到 @github.com 上来

![alt tag](https://raw.githubusercontent.com/mzeric/tinyos/master/demo_qemu.png)

## A simple OS just for fun!
## 最近更新
* Change Build platform to Linux （ubuntu 16.04） gcc 5.4.0
* 演示视频：http://v.youku.com/v_show/id_XMzAwNzU4NDg4OA==.html?spm=a2h3j.8428770.3416059.1

## 不同的版本，可以用github切换不同的版本的tag，如何切换详见github文档

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

# 如何编译 & 运行

需要：

djgpp(gcc 的dos移植版本)
brightsky 0.04可以使用mingw-gcc 来编译了

## 开始编译：

  1. 设置djgpp的环境变量，命令行下gcc可用
  2. 进入源码目录，直接make即可

## 编译生成的文件：

  1. BSKY.FLP 可用于虚拟机(bochs)运行的软盘镜像，
   虚拟机脚本为bochrc003.txt,bochs启动命令参考003.cmd

  2. bsboot ,512字节的启动代码，支持FAT32文件系统
   bsldr  ,真正的内核，由bsboot自动加载，放在C:\盘根目录下

## 运行：

  1. 如果在虚拟机中运行：
    1. 只需要BSKY.FLP一个文件，虚拟机(bochs)的配置文件
   可以参考源码目录下的bochrc003.txt，配置正确后直接启动bochs即可。
    1. qemu -fda 从floppy 启动
      ` qemu-system-i386 -fda BSKY.flp`

  2. 安装到真实机器：
   0.03版本支持从FAT32文件系统引导，所以可以简单的安装到真实的电脑里；
    1. 方法A(借用NTLDR)：
      1. 将BSBOOT和BSLDR复制到C:\根目录下（C盘必须是FAT32文件系统）
      2. 在C:\boot.ini中加入一行：C:\bsboot="BrightSky"
      3. 重启，在启动菜单中选择"BrightSky"
    2. 方法B(使用Grub)：
      1. 使用grub内嵌命令chainloader加载bsboot，关于chainloader参加
         grub使用手册

   note:  键盘解码只实现了PC上的101键盘及其扩展，但不支持笔记本的键盘解码，所以笔记本上的控制台输入无法使用!!!
          
