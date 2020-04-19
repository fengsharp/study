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

async function run() {
    let ret = await task1();
    console.log(ret);
    ret = await task2();
    console.log(ret);
    ret = await task3();
    console.log(ret);
}

run();