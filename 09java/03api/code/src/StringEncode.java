package demo;

import java.util.StringJoiner;

public class StringEncode {
    public static void main(String[] args) {
        String s1 = "hello";
        String s2 = "HELLO".toLowerCase();
        System.out.println(s1 == s2);
        System.out.println(s1.equals(s2));
        System.out.println("====================");

        try {
        byte[] barr = s1.getBytes("utf-8");
        System.out.println(barr);

        String s3 = new String(barr, "utf-8");
        System.out.println(s3);

        System.out.println("==================");
        } catch (Exception ex) {
            System.out.println(ex);
        }
        System.out.println("============");

        StringBuilder builder = new StringBuilder();
        for (int i=0; i<100; ++i) {
            builder.append(i);
            builder.append(',');
        }
        System.out.println(builder.substring(0, builder.length()-1));
        System.out.println("==================");

        String[] names = {"Bob", "Alice", "Grace"};
        StringJoiner joiner = new StringJoiner(", ", "Hello ", "!");
        for (String item : names) {
            joiner.add(item);
        }
        System.out.println(joiner);
    }
}