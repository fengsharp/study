const fs = require("fs");
const stream = fs.createReadStream('./02fsreadstream.js', {autoClose: false});
stream.on("data", (chunk) => {
    console.log("### receive " + chunk.length + ". ");
    console.log(chunk.toString());
});
stream.on("end", () => {
    console.log("### end");
    stream.close();
});
stream.on("close", () => {
    console.log("### close stream");
});