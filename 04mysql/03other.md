# 1.DML
## 1.insert
insert into tb(f1, f2, ...)
values(v1, v, ...),
(v2, v, ...)

## 2.update
update tb
where [conditon]
set f1=v1, f2=v2

## 3.delete
delete from tb
where [condition]

# 2.DDL
1. database
    create database if not exists books;
    rename database books to books2;
    alter database books character set gbk;
    drop database if exists books;
2. datatable
    create table book (
        id int,
        bName varchar(20),
        price double,
        authorID int,
        publishDate datetime
    )
    desc book;
    alter table book [add|drop|modify|change] column [field] [type 约束]
    drop table if exists book;

    create table copy like author; // 仅仅复制表结构
    create table copy2 select * from author; // 表结构和数据
    create table copy3 select id, au_name from author // 只复制数据
    create table copy4 select id, au_name where 0; // 仅仅复制几个字段
3. datatype
1.数值
- 整形 tinyint:1 smallint:2 mediumint:3 integer:4 bigint:8
- 小数 float(M,D) double(M,D) dec(M,D) decimal(M,D)  
    M：整数部位+小数部位. D：小数部位  
2.字符
    较短的文本：char varchar  
        binary和varbinary用于保存较短的二进制  
    较长的文本：text blob(较大的二进制)  
```sql
/*
	    写法		 M的意思					        特点			空间的耗费	效率
char	char(M)		最大的字符数，可以省略，默认为1		 固定长度的字符		比较耗费	高
varchar varchar(M)	最大的字符数，不可以省略		     可变长度的字符		比较节省	低
*/
```
3.日期
4. 约束
```sql
/*
六大约束
1. not null
2. default
3. primary key
4. unique
5. check
6. foreign key
*/
```
5.自增长
AUTO_INCREMENT