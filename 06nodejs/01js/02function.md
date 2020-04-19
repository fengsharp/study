# 定义
```js
function add (x, y) {
    return x + y;
}
console.log(add(1, 3));
```
或者
```js
var add = function (x, y) {
    return x + y;
}; // 赋值语义
```
# 参数
1. typeof 判断参数
```js
var add = function (x, y) {
    if (typeof x !== 'number' || typeof y !== 'number') {
        console.log(`input err. ${x}, ${y}`);
    }
    return x + y;
}
console.log(add(1, 3));
console.log(add(1));
console.log(add(1, "a"));
```

2. arguments 类似Array，但不是Array
```js
function foo(x) {
    console.log('x = ' + x); // 10
    for (var i=0; i<arguments.length; i++) {
        console.log('arg ' + i + ' = ' + arguments[i]); // 10, 20, 30
    }
}
foo(10, 20, 30);
```

3. rest
```js
function add (...rest) {
    var ret = 0;
    for (let i = 0; i < rest.length; i++) {
        ret += rest[i];
    }
    return ret;
}
console.log(add(1, 2, 3, 4, 5));
```

# 解构赋值
```js
var array = ['hello', 'JavaScript', 'ES6'];
var [a, b, c] = array;
console.log(a, b, c); // hello JavaScript ES6
```

# 方法
```js
var cal = {
    a: 1,
    b: 2,
    add: function () {
        return this.a + this.b;
    }
};
console.log(cal.add());
```

# 高阶函数
高阶函数，就是让函数的参数能够接收别的函数.

## map
```js
var arr = [1, 2, 3, 4, 5, 6, 7, 8, 9];
arr = arr.map((value, index, array) => {
    return value * 2;
});
console.log(arr);
```
## reduce
```js
var arr = [1, 2, 3, 4, 5, 6, 7, 8, 9];
var ret = arr.reduce((prevValue, currentValue, currIndex, array) => {
    return prevValue * 10 + currentValue;
});
console.log(ret); // 123456789
```
## filter
```js
var arr = ['A', '', 'B', null, undefined, 'C', '  '];
arr = arr.filter((value, index, array) => {
    return value && value.trim();
});
console.log(arr); // [ 'A', 'B', 'C' ]
```

## sort
Array默认sort将值全部转为字符串排序
```js
var arr = [10, 20, 1, 2];
arr.sort();
console.log(arr); // [ 1, 10, 2, 20 ]
```
正确的数值排序
```js
var arr = [10, 20, 1, 2];
arr.sort((a, b) => {
    if (a < b) {
        return -1;
    } else if (a > b) {
        return 1;
    } else {
        return 0;
    }
});
console.log(arr); // [ 1, 2, 10, 20 ]
```

## every
```js
var arr = ['Apple', 'pear', 'orange'];
var ret = arr.every((value, index, array) => {
    return value && value.length > 0;
});
console.log(ret); // true
```

## find | findIndex
```js
var arr = [1, 5, 10, 7];
var ret = arr.find((value, index, obj) => {
    return value % 2 == 0;
});
console.log(ret); // 10. 如果没有则返回undefined
```

## foreach
往往用于遍历
```js
var arr = [1, 5, 10, 7];
arr.forEach((value, index, array) => {
    array[index] = value * 2;
});
console.log(arr); // [ 2, 10, 20, 14 ]
```

# generator
```js
function* foo (x) {
    x = x || 0;
    yield x + 1;
    yield x + 2;
    x += 1;
    yield x * 10;
    return x;
}

var f = foo();
console.log(f.next()); // { value: 1, done: false }
console.log(f.next()); // { value: 2, done: false }
console.log(f.next()); // { value: 10, done: false }
console.log(f.next()); // { value: 1, done: true }
console.log(f.next()); // { value: undefined, done: true }
console.log(f.next()); // { value: undefined, done: true }
```
另一种遍历方式，不处理最后return的值
```js
function* foo (x) {
    x = x || 0;
    yield x + 1;
    yield x + 2;
    x += 1;
    yield x * 10;
    return x;
}

for (var n of foo()) {
    console.log(n);
}
/*
1
2
10
*/
```