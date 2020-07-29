package demo;

public class DemoTypeSize {
    public static void main(String[] args) {
        // num: byte short int long float double
        System.out.println("byte: " + Byte.SIZE / 8);
        System.out.println("short: " + Short.SIZE / 8);
        System.out.println("int: " + Integer.SIZE / 8);
        System.out.println("long: " + Long.SIZE / 8);
        System.out.println("float: " + Float.SIZE / 8);
        System.out.println("double: " + Double.SIZE / 8);
        // char
        System.out.println("char: " + Character.SIZE / 8);
        // boolean no size
    }
}