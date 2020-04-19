const fs = require("fs");
var data = "hello, test fs.writeFile";

var stream = fs.createWriteStream("./del.txt");
stream.on("finish", () => {
    console.log("### finish");
});
stream.on("close", () => {
    console.log("### close");
})
stream.write(data, (err) => {
    if (err) {
        console.log("### write fail.");
    } else {
        console.log("### write ok.");
    }
});
stream.end();