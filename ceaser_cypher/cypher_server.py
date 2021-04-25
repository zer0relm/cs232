import socket

class server:
    def __init__(self):
        self._my_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._my_socket.bind((socket.gethostname(), 9876))

    def run(self):
        self._my_socket.listen(5)
        while True:
            self._client_socket, self._client_address = self._my_socket.accept()
            self._msg = self._client_socket.recv(1024)
            number = self._msg.decode()
            print(f"connection from {self._client_address} has been established")
            print(f"message is {number}")
            #self._client_socket.send(bytes("Welcome to the server!", "utf-8"))
            self._client_socket.send(bytes(self._msg))

cServer = server()
cServer.run()