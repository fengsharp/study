# 新风格
```js
class Student {
    constructor(name) {
        this.name = name;
    }

    hello() {
        console.log("hello", this.name);
    }
}

var s = new Student('eric');
s.hello();
```
对于继承
```js
class Shap {
    constructor(type) {
        this.type = type;
    }
    draw () {
        console.log("draw", this.type);
    }
}

class Rect extends Shap {
    constructor(type, width, height) {
        super(type);
        this.width = width;
        this.height = height;
    }
    draw() {
        super.draw();
        console.log(`draw width:${this.width} height:${this.height}`);
    }
}

var rectObj = new Rect("rect", 10, 20);
rectObj.draw();

/*
draw rect
draw width:10 height:20
*/
```

# 旧风格
```js
function Student(name) {
    this.name = name;
}

Student.prototype.hello = function () {
    console.log('Hello, ' + this.name + '!');
};

var s = new Student("abc");
s.hello();
```
