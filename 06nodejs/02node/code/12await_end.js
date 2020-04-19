const assert = require('assert');
const fs = require("fs");
const promisefy = require("util").promisify;
const readFilePromise = promisefy(fs.readFile);

async function run () {
    let ret1 = await readFilePromise("./10async.js");
    console.log(ret1.toString());
    let ret2 = await readFilePromise("./11await.js");
    console.log(ret2.toString());
    let ret3 = await readFilePromise("./12await_end.js");
    console.log(ret3.toString());
}

run();