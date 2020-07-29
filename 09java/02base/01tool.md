# 手动运行代码
    -- root
        -- target
        -- src
            -- Hello.java
```java
package demo

public class ...
...
```
cd root/src
javac -d ../target Hello.java
cd ../target
java -cp . demo.Hello 