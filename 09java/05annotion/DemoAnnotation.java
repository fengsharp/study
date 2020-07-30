import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
import java.lang.reflect.Field;

public class DemoAnnotation {
    public static void main(String[] args) {
        try {
            Person p = new Person(100, "my name is long");
            check(p);

            p = new Person(999, "feng");
            check(p);
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    private static void check(Person p) throws Exception {
        for (Field item : p.getClass().getFields()) {
            Range range = item.getAnnotation(Range.class);
            if (range == null) {
                continue;
            }

            Object fieldValue = item.get(p);
            if (fieldValue instanceof String) {
                String s = (String) fieldValue;
                if (s.length() < range.min() || s.length() > range.max()) {
                    System.out.println(item.getName() + " length Invalid.");
                }
            } else if (fieldValue instanceof Integer) {
                Integer i = (Integer) fieldValue;
                if (i < range.min() || i > range.max()) {
                    System.out.println(item.getName() + " value Invalid.");
                }
            }
        }
    }
}

class Person {
    public Person(Integer age, String name) {
        this.age = age;
        this.name = name;
    }

    @Range(min = 1, max = 150)
    public Integer age;

    @Range(min = 0, max = 5)
    public String name;
}