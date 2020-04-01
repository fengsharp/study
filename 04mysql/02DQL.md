## 1.基础查询
select [字段 常量 表达式 方法]
注意：  
- select 的结果看作虚拟的表
- distinct
- sql中+的作用只能作为运算符，不能作为字符串的连接符

#案例：查询员工名和姓连接成一个字段，并显示为 姓名
```sql
SELECT e.`last_name`, e.`first_name`, CONCAT(e.`last_name`, e.`first_name`) AS 'name'
FROM employees e;
```

## 2.条件查询
```sql
select [list]  
from [table]  
where [condition]
```
1. 条件表达式 > < = != <> >= <=
2. 逻辑表达式 and or not. && || !
3. 模糊表达式
- like 和通配符搭配使用 %任意多个字符，包括0. _ 任意单个字符
- between ... and ...  包含临界值
- in
- is null

### 条件表达式案例
1. 案例1：查询工资>12000的员工信息
```sql
SELECT *
FROM employees e
WHERE e.`salary` > 1200;
```

2. 案例2：查询部门编号不等于90号的员工名和部门编号
```sql
SELECT e.`last_name`, e.`department_id`
FROM employees e
WHERE e.`department_id` != 90;
```

### 逻辑表达式案例
1. 查询工资在10000到20000之间的员工名、工资以及奖金
```sql
SELECT
	last_name,
	salary,
	commission_pct
FROM
	employees
WHERE
	salary>=10000 AND salary<=20000;
```

### 模糊查询
1. 查询员工名中包含字符a的员工信息
name like '%a%'
2. 查询员工名中第三个字符为e，第五个字符为a的员工信息
name like '__e_a%'
3. 查询没有奖金的员工名和奖金率
```sql
SELECT e.`last_name`, e.`commission_pct`
FROM employees e
WHERE e.`commission_pct` IS NULL;
```

## 3.排序
select [list]
from [table]
where[condition]
order by [field] asc[1 2 3] | desc[3 2 1]

注意：
where条件筛查后的结果是虚拟表，order by是针对虚拟表的字段进行排序

1. 查询部门编号>=90的员工信息，并按员工编号降序
```sql
SELECT *
FROM employees e
WHERE e.`department_id` >= 90
ORDER BY e.`department_id` DESC;
```

2. 查询员工名，并且按名字的长度降序
```sql
SELECT e.`last_name`, LENGTH(e.`last_name`) AS name_len
FROM employees e
ORDER BY name_len DESC;
```

## 4.常见函数
分类：
1. 单行函数
- 字符串
length(汉字三个字节) concat substr instr trim upper lower lpad rpad replace
- 数学
round ceil floor truncate mod
- 日期函数
now curdate curtime year month day hour minute second str_to_date date_format
- 流程控制函数
    - if函数 if else的效果  
        select if(10<5, 'big', 'smal')
    - case函数 switch case的效果
    ```sql
    case [condition]
        when 常量1 then ...
        when 常量2 then ...
        else ...
    end
    或者类似多重if elseif elseif else
    case
        when [condition] then ...
        when [condition] then ...
    end
    ```

```sql
/*案例：查询员工的工资，要求
部门号=30，显示的工资为1.1倍
部门号=40，显示的工资为1.2倍
部门号=50，显示的工资为1.3倍
其他部门，显示的工资为原工资*/

SELECT e. employee_id, e.`salary`, 
CASE e.`employee_id`
	WHEN 30 THEN e.`salary` * 1.1
	WHEN 40 THEN e.`salary` * 1.2
	WHEN 50 THEN e.`salary` * 1.3
	ELSE e.`salary`
END AS new_salary
FROM employees e
```

```sql
/*
案例：查询员工的工资的情况
如果工资>20000,显示A级别
如果工资>15000,显示B级别
如果工资>10000，显示C级别
否则，显示D级别
*/

SELECT e.`salary`, 
CASE
	WHEN e.salary > 20000 THEN 'A'
	WHEN e.salary > 15000 THEN 'B'
	WHEN e.salary > 10000 THEN 'C'
	ELSE 'D'
END AS LEVEL
FROM employees e

```

2. 分组函数
sum avg max min count


## 5.分组查询

order by
{
    group by
    {
        select [list]
        from [table]
        where [condition]
    }
    having
}

1. 查询每个部门的员工个数
```sql
SELECT e.`department_id`, COUNT(e.`department_id`)
FROM employees e
WHERE e.`department_id` IS NOT NULL 
GROUP BY e.`department_id`
```

2. 查询有奖金的每个领导手下员工的平均工资
```sql
SELECT e.`manager_id`, AVG(e.`salary`)
FROM employees e
WHERE e.`commission_pct` IS NOT NULL
GROUP BY e.`manager_id`
```

3. 查询哪个部门的员工个数>5
```sql
SELECT e.`department_id`
FROM employees e
GROUP BY e.`department_id`
HAVING COUNT(e.`department_id`) > 5;
```

4. 领导编号>102的每个领导手下的最低工资大于5000的领导编号和最低工资
```sql
SELECT e.`manager_id`, MIN(e.`salary`) AS min_salary
FROM employees e
WHERE e.`manager_id` > 102
GROUP BY e.`manager_id`
HAVING MIN(e.`salary`) > 5000
```


## 6.连接查询
``` sql
select [list]
{
    group by
    {
        {
            from t1 [join type] t2
                on [join condition]
        }
        where [condition]
    }having
}order by
```
1. 内连接: inner join. 等值，非等值， 自连接  
2. 外连接：left right full
3. 交叉连接: cross 笛卡尔积
---
1. 查询员工名、部门名
```sql
SELECT e.`last_name`, d.`department_name`
FROM employees e INNER JOIN departments d
ON e.`department_id` = d.`department_id`
```

2. 查询哪个部门的员工个数>3的部门名和员工个数
```sql
SELECT d.`department_name`, COUNT(d.`department_id`) AS department_cnt
FROM employees e INNER JOIN departments d
ON e.`department_id` = d.`department_id`
GROUP BY d.`department_id`
HAVING COUNT(d.`department_id`) > 3
```

3. 查询员工的名字、上级的名字
```sql
SELECT e.`last_name`, e.`employee_id`, e.`manager_id`, m.`employee_id`, m.`last_name`
FROM employees e INNER JOIN employees m
ON e.`manager_id` = m.`employee_id`
```

## 7.子查询
```sql
/*
按照子查询出现的位置分类：
1. select 后，只能是标量
2. from后，虚拟表
3. where | having 后，可以是标量，列，行
4. exists后面 虚拟表
*/
```

```sql
-- 谁的工资比 Abel 高?
SELECT *
FROM employees
WHERE salary>(

	SELECT salary
	FROM employees
	WHERE last_name = 'Abel'

);
```

## 8.分页查询
limit [offset,] size;  
offset要显示条目的起始索引（起始索引从0开始）  
size 要显示的条目个数  
```sql
要显示的页数 page，每页的条目数size
	select 查询列表
	from 表
	limit (page-1)*size,size;
	
	size=10
	page  
	1	0
	2  	10
	3	20
``









