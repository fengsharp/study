# object -> json
```js
var person = {
    name: 'eric',
    age: 4,
    colors: ['red', 'blue']
};
var jsonStr = JSON.stringify(person, null, '');
console.log(jsonStr); // {"name":"eric","age":4,"colors":["red","blue"]}
```
要格式化，就 JSON.stringify(person, null, '    ');  
第二个参数用于过滤输出

# string -> object
```js
var obj = JSON.parse('{"name":"小明","age":14}');
console.log(obj.name, obj.age); // 小明 14
```