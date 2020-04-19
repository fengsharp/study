const fs = require("fs");

var data = "hello, test fs.writeFile";
fs.writeFile("./del.txt", data, "utf8", (err) => {
    if (err) throw err;
    console.log('文件已被保存');
});