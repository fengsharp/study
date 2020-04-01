# 数据库访问
mysql -h 192.168.154.128 -P 3306 -u root -p
love_123

## 列出数据库
show databases;

## 打开指定数据库
use test;

## 查看所有的表
show tables;

## 查看表结构
desc user;

## 查看数据库版本
- mysql --version
- select version();

## sql分类
- DQL data query language(select)
- DML data manipulate language(insert update delete)
- DDL data define language(create drop alter)
- TCL transaction control language(commit rollback)