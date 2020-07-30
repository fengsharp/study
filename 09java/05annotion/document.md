# 定义注解
1. 用@interface 定义注解
2. 添加参数+默认值
3. 元注解(目标和生命周期)
4. 解析注解接口
```java
Class.isAnnotationPresent(Class);
Field.isAnnotationPresent(Class);
Method.isAnnotationPresent(Class);
Constructor.isAnnotationPresent(Class);

Class.getAnnotation(Class);
Field.getAnnotation(Class);
Method.getAnnotation(Class);
Constructor.getAnnotation(Class);
```