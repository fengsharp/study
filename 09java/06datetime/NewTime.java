import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public class NewTime {
    public static void main(String[] args) {
        LocalDateTime dt = LocalDateTime.now();
        System.out.println(dt);
        System.out.println("=================");

        dt = LocalDateTime.parse("2020-07-30T16:10:30");
        System.out.println(dt);
        System.out.println("=================");

        String str1 = "2020/07/30 16:41:00";
        DateTimeFormatter formater = DateTimeFormatter.ofPattern("yyyy/MM/dd HH:mm:ss");
        dt = LocalDateTime.parse(str1, formater);
        System.out.println(dt);
        System.out.println(formater.format(dt));
        System.out.println("=================");
    }
}