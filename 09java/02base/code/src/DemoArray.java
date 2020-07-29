package demo;

import java.util.Arrays;

public class DemoArray {
    public static void main(String[] args) {
        int[] arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        int sum = 0;
        for (int i=0; i<arr.length; ++i) {
            sum += arr[i];
        }
        System.out.println("fori result: " + sum);

        sum = 0;
        for(int item : arr) {
            sum += item;
        }
        System.out.println("foreach result: " + sum);

        System.out.println(Arrays.toString(arr));
        System.out.println("======================");

        int[] arr2 = {3, 9, 7, 5};
        System.out.println("before sort: " + Arrays.toString(arr2));
        Arrays.sort(arr2);
        System.out.println("After sort: " + Arrays.toString(arr2));
    }
}