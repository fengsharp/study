const fs = require('fs');
fs.readFile("./01fsread.js", (err, data) => {
    if (err) { throw err; }
    console.log(String(data));
});