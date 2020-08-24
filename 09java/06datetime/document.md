# 概念区分
1. 2020-07-30  日期
2. 15:59:00    时间
3. 2020-07-30 15:59:00 时间
4. 时间可以带日期，也可以不带日期
5. java System.currentTimeMillis() 获取当前时间戳
```java
static long currentTimeMillis()  
```
6. 有两个库api
- java.util     Date Calendar TimeZone
- java.time     LocalDateTime ZonedDatetime ZoneId

7. java.util.Date Calendar
- Date date = new Date();  内部long的毫秒
- SimpleDateFormat yyyy-MM-dd HH:mm:ss
- Calendar 相对于date可以set
- Calendar -> .getTime() -> Date
- Calendar SimpleDateFormat可以设置TimeZone对象，进行时区转换

8. java8后的时间api  ISO 8601格式
- 本地时间：LocalDateTime LocalDate LocalTime
- 带时区的时间：ZonedDateTime
- 时刻：Instant
- 时区：ZoneId ZoneOffset
- 间隔：Duration
- 格式化：DateTimeFormatter

9. nwe api
- 获取当前时间 LocalDateTime.now()
- 指定时间 LocalDateTime.of(2020, 7, 30, 16, 37, 0);
- 字符串指定时间 LocalDateTime.parse("2020-07-30T16:37:00");
- LocalDateTime -> DateTimeFormatter -> string
- string -> DateTimeFormatter -> LocalDateTime
- Instant.now()  获取当前时间戳 getEpochSecond() toEpochMilli

10. 转换关系
- LocalDateTime + ZoneId -> ZonedDateTime -> Instant(long)
- Instant(long) -> ZonedDateTime -> LocalDateTime + ZoneId

11. 最佳实践
- 记录utc时间戳