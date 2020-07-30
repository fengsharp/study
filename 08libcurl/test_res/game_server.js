var http = require('http');

http.createServer(function (request, response) {
    let data = ""
    request.on('data', chunk => {
        data += chunk;
    });

    request.on('end', () => {
        console.log('headers', request.headers); // object
        console.log('httpVersion', request.httpVersion); // string
        console.log('method', request.method); // string
        console.log('url', request.url); // string

        console.log('body', data); // string
        // 发送 HTTP 头部 
        // HTTP 状态值: 200 : OK
        // 内容类型: text/plain
        response.writeHead(200, {
            'Content-Type': 'text/plain',
            'Set-Cookie': ['foo=bar', 'bar=baz']
        });

        // 发送响应数据 "Hello World"
        let sendData = 'echo[' + data + ']\n';
        response.end(sendData);
    });
}).listen(8888);