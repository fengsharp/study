1. %eax 保存系统调用号
2. %ebx 保存返回状态

3. 通用寄存器  
%eax %ebx %ecx %edx %edi %esi  
4. 准用寄存器
%ebp %esp %eip %eflags  
5. $1 表示使用立即寻址方式  
6. 常用寄存器用途
```shell
%edi 数据项索引
%ebx 最终的结果
%eax cpu临时遍历

```

7. 常用指令
```shell

incl %edi 增加
movl data_items(, %edi, 4), %eax   取内存地址内容加载到cpu
cmpl %eax, %ebx
jle ... 如果 left <= right
je  ... 如果相等跳转          
jmp ...

```

```shell
跳转指令
je  =
jg  <
jge <=
jl  >
jle >=
jmp 

```

8. 数据类型
```shell

.byte 一个字节
.int  两个字节
.long 4个字节
.ascii 

```

9. 寻址方式

```shell
1. 立即方式
    movl $123, %eax
2. 直接寻址
    movl address, %eax
3. 索引寻址
    movl string_start(, %eax, 1), %eax
4. 间接寻址
    movl (%eax), %ebx  %eax存储的是内存地址
5. 基址寻址
    movl 4(%eax), %ebx  (%eax)+4的地址

movb  一次移动一个字节
movl 一次移动4个字节

%eax 4个字节
%ax  2个字节
%ah %al 1个字节
```

10. function
1. 组成部分
    - 函数名
    - 函数参数
    - 局部变量
    - 静态变量
    - 全部变量
    - 返回地址
        call指令传入function要返回的地址
        ret指令会返回传入的的返回地址
    - 返回值

2. call调用后
```shell

参数n
...
参数2
参数1
返回地址 <--------- (%esp)

```

3. 进入fuction内部
    - pushl %ebp 保存当前的基址指针  
此时看起来
```shell

参数n          n*4+4(%ebp)
...
参数2          12(%ebp)
参数1           8(%ebp)
返回地址         4(%ebp)
旧%ebp           (%esp)  (%ebp)

```

4. 如果有两个局部变量

```shell

参数n          n*4+4(%ebp)
...
参数2          12(%ebp)
参数1           8(%ebp)
返回地址         4(%ebp)
旧%ebp    -----------------------  (%ebp)
局部变量1         -4(%ebp)
局部变量2         -8(%ebp)  %esp

```

5. 返回时
    - 将返回值放入 %eax
    - 将栈恢复到调用函数时的状态
    - 将控制权交还给调用它的程序
```shell

movl %ebp, %esp
popl %ebp
ret

```


