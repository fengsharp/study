function getMsg(timeCallback) {
    setTimeout(() => {
        timeCallback("fire");
    }, 1000);
}

getMsg((data) => {
    console.log("receive data:", data);
});