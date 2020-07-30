# JavaBean
1. 字段private
2. 最好提供构造方法
3. getXyz setXyz
4. boolean 字段 isXyz
5. 主要作用在于传递数据
6. 遍历属性 Introspector.getBeanInfo


# 枚举
1. 每一类型就是一个class的具体实例，且只有一个实例，所以可以通过 == 比较引用来进行判断
2. 编译后类似类+实例 不能自己new实例
```java
enum Color { RED, WHITE, BLACK};

// ->

finnal class Color extends java.lang.Enum {
    public static final Color RED = new Color();
    public static final Color WHITE = new Color();
    public static final Color BLACK = new Color();
    private Color() {}
}
```
3. 从Enum继承的接口 name ordinal valueOf(string -> enum)
4. Math.abs Math.max Math.min Math.pow Math.sqrt Math.random
5. random
- 伪随机，给定一个种子，随机结果总是一致
```java
Random rd = Random(seed);
// 如果赋值，默认使用系统时间  Math.random 内部调用的就是Random
/*
rd.nextInt();
rd.nextInt(10);  [0, 10)
rd.nextLong();
rd.nextFloat(); [0, 1)
rd.nextDouble(); [0, 1)
*/
```
6. SecureRandom  真随机数

