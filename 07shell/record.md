1. 如何查看本机支持的shell？
```sh
vi /etc/shells
```

2. 多命令执行方式
```sh
; && ||
```

3. 获取命令的值 
```sh
a=$(date)
a=`date`
```

4. {} ()的区别
{} 是在本shell中执行命令，且命令和符号间必须有空格
() 是在子shell中执行命令

5. 参数处理
```sh
# $# 参数个数
# $* 所有参数作为一个整体
# $@ 所有参数作为一个数组
# $n 第几个参数的值
```

6. 预定义变量
```sh
# $? 上一条命令返回状态
# $$ 当前进程ID
```

7. 数值运算
```sh
# $(( ))
# + - * / % 
# ~ << >>  & | 位运算
# ! && || 逻辑运算符
# = += -= /= *= %= <<= >>=
```

8. 测试符
```sh
# [ ... ]
```

9. 环境变量配置文件
- source命令

10. 修改配置生效
```sh
# source [file]
# . /.../.../.../[file]
```

11. 文本处理
```text
grep 搜索筛选行
cut 提取列： cut -f 2,3 [filename]
awk 
```

12. awk
```sh
awk 'cond1{done} cond2{done}'

awk '{printf $1 "\t" $3 "\t" $6 "\n"}' [filename]
```

13. condition
```sh
if [condition] 
    then
        ...
fi


if [condition]
    then
        ...
    else
        ...
fi

if [condition]
    then
        ...
elif [cond2]
    then
        ...
else
    ...
fi


case $变量名 in
    "值1")
        ...
        ;;
    "值2")
        ...
        ;;
    *)
        ...
        ;;
esac
```

14. for
```sh

for [变量] in v1 v2 v3
    do
        ...
    done


for (( 初始值; 循环控制条件; 变量变化))
    do
        ...
    done

while [condtion]
    do
        ...
    done

```