async function fn () {
    return 123;
}

// async function返回的结果是promise

fn().then((value) => {
    console.log(value);
});