1. string cmp  
use equals

2. string search
- indexOf
- lastIndexOf
- startWith
- endWith

3. substring
- substring(startIndex, endIndex)  [s, e)

4. trim
- trim  \t \r \n space
- strip  去掉中文的空格

5. isEmpty isBlank \t \r \n space

6. replace replaceAll

7. splite

8. Strings.join

9. other to String and String to other
```java
String str = String.valueOf(true);
str = String.valueOf(123);

int a = Integer.parseInt("123");
boolean b = Boolean.parseBoolean("true");
```

10. string to char[]
```java
char[] arr = "hello".toCharArray();
```

11. encoding
- string to byte[]
```java
byte[] b1 = "hello".getBytes();
byte[] b2 = "hello".getBytes("utf-8");
byte[] b3 = "hello".getBytes(StandardCharsets.UTF_8);
```

- byte[] to String
```java
byte[] b = ...;
String str1 = new String(b);
str1 = new String(b, "utf-8");
```

12. StringJoiner
```java
StringJoiner(CharSequence delimiter) 
StringJoiner(CharSequence delimiter, CharSequence prefix, CharSequence suffix) 

StringJoiner add(CharSequence newElement)  
```