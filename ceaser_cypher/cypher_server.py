import socket

class server:
    def __init__(self):
        self._my_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._my_socket.bind((socket.gethostname(), 9876))

    def run(self):
        self._my_socket.listen(5)
        self._client_socket, self._client_address = self._my_socket.accept()
        self._client_socket.send(bytes("Welcome to the server!", "utf-8"))
        print(f"connection from {self._client_address} has been established")
        while True:
            
            
            self._msg = self._client_socket.recv(1024)
            self._msg2 = self._client_socket.recv(1024)
            number = self._msg.decode()
            number2 = self._msg2.decode()
            print(f"rotation is {number}")
            print(f"message is {number2}")
            
            self._client_socket.send(bytes(number + "\r\n", "utf-8"))
            self._client_socket.send(bytes(number2 + "\r\n", "utf-8"))
        self._my_socket.close()

cServer = server()
cServer.run()