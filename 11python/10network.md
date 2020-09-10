# 1. 环境说明
    不要在windows环境，在linux环境

# 2. udp
```python
import socket

udpSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
bindAdr = ('127.0.0.1', 8889)
udpSocket.bind(bindAdr)
recvMsg = udpSocket.recvfrom(1024)
print("recv request: ", bytes.decode(recvMsg[0]))
udpSocket.close()


import socket

udpSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sendAdr = ('127.0.0.1', 8889)
sendData = 'hello, world'
udpSocket.sendto(sendData.encode(), sendAdr)
udpSocket.close()

# echo
#!/usr/bin/python3

import socket

udpServer = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
udpServer.bind(('127.0.0.1', 8889))

num = 1
while True:
    recvMsg = udpServer.recvfrom(1024)
    udpServer.sendto(recvMsg[0], recvMsg[1])
    print('%d recv data:%s'%(num, bytes.decode(recvMsg[0])))
    num += 1

udpServer.close()

```
recvMsg返回值类似于：(b'hello', ('127.0.0.1', 35206))

# 3. tcp

```python
import socket

tcpServSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
tcpServSocket.bind(('127.0.0.1', 8889))
tcpServSocket.listen(5)
clientSocket, remoteAddr = tcpServSocket.accept()
recvData = clientSocket.recv(1024)
print('recv data:', recvData)
clientSocket.send("thank you!".encode())
clientSocket.close()
tcpServSocket.close()


import socket

tcpClientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serAddr = ('127.0.0.1', 8889)
tcpClientSocket.connect(serAddr)

sendData = 'hello'
tcpClientSocket.send(sendData.encode())

recvData = tcpClientSocket.recv(1024)
print("recv data:", recvData)

tcpClientSocket.close()
```
