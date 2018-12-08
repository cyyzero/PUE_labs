[Quine](https://en.wikipedia.org/wiki/Quine_(computing))。不读取输入而打印自身的程序。

- 1.c 输出自身到文件。
- 2.c 输出自身到文件，编译，fork，执行。
- 3.c 接收某个信号之后将自己输出到文件并编译fork执行，父进程退出。
- helper.cpp 读取文件然后输出一些转义后的字符串以及format字符串
- rm.rb 删除生成的文件
