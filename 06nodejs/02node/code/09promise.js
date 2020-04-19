const fs = require("fs");

function task1() {
    return new Promise((resolve, reject) => {
        fs.readFile("./08asycallback.js", (err, data1) => {
            if (err) {
                reject(err);
            } else {
                resolve("read1");
            }
        });
    });
}


function task2() {
    return new Promise((resolve, reject) => {
        fs.readFile("./08asycallback.js", (err, data1) => {
            if (err) {
                reject(err);
            } else {
                resolve("read2");
            }
        });
    });
}

function task3() {
    return new Promise((resolve, reject) => {
        fs.readFile("./08asycallback.js", (err, data1) => {
            if (err) {
                reject(err);
            } else {
                resolve("read3");
            }
        });
    });
}

task1().then((value) => {
    console.log(value);
    return task2();
}).then((value) => {
    console.log(value);
    return task3();
}).then((value) => {
    console.log(value);
}).catch( (reason) => {
    console.log(reason);
}).finally(() => {
    console.log("all over");
});