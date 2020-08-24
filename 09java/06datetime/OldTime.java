import java.text.SimpleDateFormat;
import java.util.Date;

public class OldTime {
    public static void main(String[] args) {
        Date date = new Date();
        System.out.println(date.toString());
        System.out.println(date.getYear() + 1900);
        System.out.println(date.getMonth() + 1);
        System.out.println(date.getDate());
       
        System.out.println(date.toGMTString());
        System.out.println(date.toLocaleString());
        System.out.println("================");

        SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        System.out.println(format.format(date));
    }
}