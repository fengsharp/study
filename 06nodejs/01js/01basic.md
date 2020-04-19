# 基本语法
1. ；结束
2. {...}语句块
3. var声明变量，如果不用var，则作为全局变量
4. var声明的变量只能声明一次
5. 注释 // /* */
6. 严格区分大小写

# 变量定义
1. var声明变量，如果不用var，则作为全局变量
2. var声明的变量只能声明一次
3. 变量的数据类型可以变更
```js
var a = 123;
a = "abc";
```

# 基本数据类型
1. 整数和浮点数统称为 number。number的值有 undefine NaN Infinity
2. 字符串 '' ""
3. boolean true|false
4. null undefined. undefined仅作为参数是否赋值使用
5. Array []
6. object 
---
使用 typeof [var] 返回的值
"number" "string" "boolean" "object" "null" "array"

# 运算
1. number进行四则运算，%运算
2. 逻辑运算 && || !
3. 比较运算 > < >= <=。特别的 ==（自动数据类型转换判断）， ===(两边数据类型如果不一致，则false)。  
另外NaN != NaN。如果要判断NaN问题，isNaN(NaN) -> true  
浮点数比较 Math.abs(a-b) < 0.00001  

# 条件语句
同c语言

# 循环
- for 
- while
- do while

# 字符串
1. ``可以实现多行字符串
2. 模板字符串 
```js
var name = "Jone";
var age = 12;
console.log(`name:${name}, age:${age}`);
```
3. 字符串接口 特别注意，字符串内容不可变，接口都是新的字符串
- lenth
```js
var name = "Jone";
console.log(name.length);
```
- [index]
```js
var name = "Jone";
console.log(name[0]);
console.log(name[name.length]); // 特别的，返回 undefined

name[0] = 'A';
console.log(name[0]); // 字符串内容不可变，依然是J
```
- 大小写
```js
var name = "Jone";
console.log(name.toUpperCase()); // JONE
console.log(name.toLowerCase()); // jone
```
- indexOf
```js
var name = "Jone";
console.log(name.indexOf("Jo")); // 0
console.log(name.indexOf("jo")); // -1
```
- substring
```js
var name = "Jone";
console.log(name.substring(1, 2)); // o
console.log(name.substring(1)); // one
```


# 对象
```js
var person = {
    name: "eric",
    age: 10
}

// get property
console.log(person.name); // eric
// property key是字符串
console.log(person["name"]); // eric
// add property
person.sex = "boy";
console.log(person); // { name: 'eric', age: 10, sex: 'boy' }
// delete property
delete person.sex;
console.log(person); // { name: 'eric', age: 10 }
console.log("name" in person); // true
console.log(person.hasOwnProperty("name")); // true
```

# 数据结构
## 数组
- get length
```js
var colors = ["red", "blue", "black", "yellow"];
console.log(colors.length); // 4
```
- set length 通过set length或者直接访问赋值索引外数据，
```js
var colors = ["red", "blue", "black", "yellow"];
console.log(colors.length);
colors.length = 6;
console.log(colors[4]); // undefined
```

```js
var colors = ["red", "blue", "black", "yellow"];
console.log(colors.length);
colors.length = 6;
console.log(colors[4]); // undefined
console.log(colors[8]); // undefined
console.log(colors.length); // 6 没有设置值，仅访问了8，超出索引，认为是没有定义
colors[8]="GREEN";
console.log(colors.length); // 9
```

- indexOf
```js
var colors = ["red", "blue", "black", "yellow"];
console.log(colors.indexOf("red")); // 0
console.log(colors.indexOf("black")); // 2
console.log(colors.indexOf("Red")); // -1
```

- slice
```js
var colors = ["red", "blue", "black", "yellow"];
console.log(colors); // [ 'red', 'blue', 'black', 'yellow' ]
console.log(colors.slice(1, 2)); // [ 'blue' ]
```

- push | pop 栈
```js
var colors = ["red", "blue", "black", "yellow"];
console.log(colors); // ["red", "blue", "black", "yellow"]
colors.push("a", "b", "c"); 
console.log(colors); // [ 'red', 'blue', 'black', 'yellow', 'a', 'b', 'c' ]
for (var i = 0; i < 5; i++) {
    colors.pop();
}
console.log(colors); // [ 'red', 'blue' ]
```

- unshift | shift
```js
var colors = ["red", "blue", "black", "yellow"];
console.log(colors); // [ 'red', 'blue', 'black', 'yellow' ]
colors.unshift("a", "b");
colors.unshift("c");
console.log(colors); // [ 'c', 'a', 'b', 'red', 'blue', 'black', 'yellow' ]
for (var i = 0; i < 5; i++) {
    colors.shift();
}
console.log(colors); // [ 'black', 'yellow' ]
```

-- 队列
```js
var colors = ["red", "blue", "black", "yellow"];
console.log(colors); // [ 'red', 'blue', 'black', 'yellow' ]
colors.push("a", "b", "c"); // [ 'red', 'blue', 'black', 'yellow', 'a', 'b', 'c' ]
console.log(colors);
for (var i = 0; i < 2; i++) {
    colors.shift();
}
console.log(colors); // [ 'black', 'yellow', 'a', 'b', 'c' ]
```

- sort
```js
var colors = ["red", "blue", "black", "yellow"];
colors.sort();
console.log(colors); // [ 'black', 'blue', 'red', 'yellow' ]
```

- reverse
```js
var colors = ["red", "blue", "black", "yellow"];
colors.reverse();
console.log(colors); // [ 'yellow', 'black', 'blue', 'red' ]
```

- splice 剪接
``` js
var colors = ["red", "blue", "black", "yellow"];
colors.splice(1, 2, "a", "b", "c"); // notice 2 is length
console.log(colors); // [ 'red', 'a', 'b', 'c', 'yellow' ]
``` 

- concat 返回新的数组
```js
var colors = ["red", "blue", "black", "yellow"];
var arr = new Array();
arr.push("a", "b", "c");
console.log(arr); // [ 'a', 'b', 'c' ]
var newArr = colors.concat(arr); 
console.log(colors); // [ 'red', 'blue', 'black', 'yellow' ]
console.log(newArr); // [ 'red', 'blue', 'black', 'yellow', 'a', 'b', 'c' ]
```
- join
```js
var colors = ["red", "blue", "black", "yellow"];
var str = colors.join("-")
console.log(str); // red-blue-black-yellow
```
## map
```js
var m = new Map();
m.set(1, "eric");
m.set(2, "feng");
console.log(m.get(1)); // eric
console.log(m.get(3)); // undefined
// delete
m.delete(1);
console.log(m); // Map(1) { 2 => 'feng' }
```

## set
```js
var s = new Set();
s.add(1);
s.add(2);
s.add(2);
console.log(s); // Set(2) { 1, 2 }
s.delete(2);
console.log(s); // Set(1) { 1 }
console.log(s.has(1)); // true
```

## 遍历
1. for遍历数组
```js
var colors = ["red", "blue"];
for (var i=0; i<colors.length; i++) {
    console.log(colors[i]);
}
```

2. for in 遍历
```js
var colors = ["red", "blue"];
for (var k in colors) {
    console.log(`${k} - ${colors[k]}`);
}
/*
0 - red
1 - blue
*/
```
3. for of 遍历array
``` js
var arr = ["a", "b", "c"];
for (var v of arr) {
    console.log(v);
}
```

4. for of map
```js
var m = new Map();
m.set(1, "eric");
m.set(2, "feng");
for (var v of m) {
    console.log(`${v[0]} - ${v[1]}`);
}
```

5. forEach
```js
var m = new Map();
m.set(1, "eric");
m.set(2, "feng");
m.forEach(function (value, key, map) {
    console.log(`${key} - ${value}`);
});
```