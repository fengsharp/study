const fs = require("fs");

fs.readFile("./08asycallback.js", (err, data1) => {
    if (err) { throw err; }
    console.log("read 1");

    fs.readFile("./08asycallback.js", (err, data2) => {
        if (err) { throw err; }
        console.log("read 2");

        fs.readFile("./08asycallback.js", (err, data3) => {
            if (err) { throw err; }
            console.log("read 3");
        });
    });
});