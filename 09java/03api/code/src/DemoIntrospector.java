package demo;

import java.beans.*;

public class DemoIntrospector {
    public static void main(String[] args) throws Exception {
        BeanInfo info = Introspector.getBeanInfo(Person.class);
        for (PropertyDescriptor item : info.getPropertyDescriptors()) {
            System.out.println(item.getName());
            System.out.println("\t" + item.getReadMethod());
            System.out.println("\t" + item.getWriteMethod());
        }  
    }
}

class Person {
    private String name;
    private int age;

    public Person() {

    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }
}