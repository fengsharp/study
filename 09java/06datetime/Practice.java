import java.time.Instant;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.ZonedDateTime;
import java.time.format.DateTimeFormatter;

public class Practice {
    public static void main(String[] args) {
        try {
            long nowEpochSecond = Instant.now().getEpochSecond();
            System.out.println(nowEpochSecond);

            ZonedDateTime zdt = Instant.ofEpochSecond(nowEpochSecond).atZone(ZoneId.systemDefault());
            LocalDateTime dt = zdt.toLocalDateTime();

            DateTimeFormatter format = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
            System.out.println(format.format(dt));

            System.out.println("=====================================");

            zdt = ZonedDateTime.now();
            long currSeconds = zdt.toEpochSecond();
            System.out.println(currSeconds);

        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
}