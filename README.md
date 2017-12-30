# digitalRecognition
基于opencv3.2.0的手写数字识别代码，此工程是基于树莓派系统的交叉编译工程
#如果你想使用本工程你需要完成如下配置
> * 在树莓派配置opencv环境，最好是opencv3以上的
> * 树莓派端配置好环境后，将usr/local文件夹下的lib和include两个文件夹拷贝到windows端，存放位置任意
> * windows端安装最新的visualGDB并完成破解
> * 按照[教程](http://etrd.org/2017/01/25/Windows%E4%B8%8B%E5%BB%BA%E7%AB%8B%E7%AC%AC%E4%B8%80%E4%B8%AA%E6%A0%91%E8%8E%93%E6%B4%BE%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F/)建立一个基础的教程编译工程，教程中介绍到的编译器要根据自己树莓派的版本选择下载相对应的编译器
> * 在工程的visualGDB中的对应属性中把需要用到的头文件库，链接库位置，即第二步中拷过来的include和lib两个文件夹存放的位置，以及要用到的链接库的名字，即lib文件夹下那些链接库的名字，添加进去即可。注意链接库的名字要去掉开头的lib和结尾的.so。
