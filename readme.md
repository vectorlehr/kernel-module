# 知识点梳理
## 设备、mmap、内存、用户空间、内核空间的关系梳理

0. 设备的驱动程序会在**内核空间**中分配内存，用于和硬件进行数据交互，注意这个内存是物理内存
1. 正常情况下，用户访问设备需要通过read、write等系统调用，这些系统调用会在驱动程序注册阶段与一些内核函数进行挂钩
2. mmap本质上也是一种系统调用，但是它是一种特殊的系统调用，它可以将设备的内存映射到用户空间，这样用户空间就可以通过访问内存的方式操作设备
3. 通过驱动程序实现的mmap方法，内核讲用户空间的虚拟地址关联到设备的物理地址



# 问题
还有几个小问题没有解决
1. makfile中的-objs和-y有什么区别。还有编译的时候的链接方式
2. ioctl定义的命令解析，使用switch经常会编号2无法使用的问题

# 注意点
1. 首先需要明确多文件编译的时候有很多小细节，在源文件中有标注
2. 其次设备的主从编号和创建问题，在从设备号可以是0-255的任意指定的值，但是删除的时候一整个设备簇都会被删除，不管有多少个从设备
3. 当用户空间程序和内核空间程序共享的头文件（协议）的时候，要注意很多头文件只能用在某些场景下。比如编译内核模块的时候会指定源码位置。但是用户空间程序往往只是用gcc编译和链接一下，所以如果随便共享头文件，会经常导致某些头文件找不到的问题，这个时候看一下是不是用户空间引用了什么不该引用的东西。最好是协议头文件不带其他任何的头文件。