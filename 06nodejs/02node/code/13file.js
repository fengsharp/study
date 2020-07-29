const path = require('path');
const fs = require('fs');

let initData = {
    projectName : 'mydemo',
    data : [
        {
            name : 'img',
            type : 'dir'
        },
        {
            name : 'css',
            type : 'dir'
        },
        {
            name : 'js',
            type : 'dir'
        },
        {
            name : 'index.html',
            type : 'file'
        }
    ]
}

let rootDir = '/home/feng/share/study/06nodejs/02node/code';
let fileContent = `
<html>
    <head><title>test</title></htead>
    <body>
        <h1> this is test page.</h1>
    </body>
</html>
`;

fs.mkdir(path.join(rootDir, initData.projectName), (err) => {
    if (err) 
        return;
    
    initData.data.forEach((item) => {
        if (item.type === 'file') {
            fs.writeFileSync(path.join(rootDir, initData.projectName, item.name), fileContent);
        } else if (item.type === 'dir') {
            fs.mkdirSync(path.join(rootDir, initData.projectName, item.name));
        }
    });
});
