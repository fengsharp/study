package demo;

public class DemoEnum {
    public static void main(String[] args) {
        System.out.println(Color.RED.name() + " -> " + Color.RED.ordinal());
        System.out.println(Color.GREEN.name() + " -> " + Color.GREEN.ordinal());
        System.out.println(Color.BLUE.name() + " -> " + Color.BLUE.ordinal());

        String configStr = "GREEN";
        Color type = Color.valueOf(Color.class, configStr);
        System.out.println(type.name() + " - " + type.ordinal());
    }
}

enum Color {
    RED, GREEN, BLUE
}