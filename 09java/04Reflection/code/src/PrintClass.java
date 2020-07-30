package demo;

public class PrintClass {
    public static void main(String[] args) {
        print("".getClass());
        print(Runnable.class);
        print(java.time.Month.class);
        print(String[].class);
        print(int.class);
    }

    private static void print(Class cls) {
        System.out.println(cls.getName());
        System.out.println("\t" + cls.getSimpleName());
        System.out.println("\t" + cls.getPackageName());
        System.out.println("\tis interface: " + cls.isInterface());
        System.out.println("\tis enum: " + cls.isEnum());
        System.out.println("\tis array: " + cls.isArray());
        System.out.println("\tis primitive: " + cls.isPrimitive());
    }
}