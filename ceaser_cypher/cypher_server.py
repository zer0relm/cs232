import socket
from string import ascii_letters as letters

class server:
    def __init__(self):
        self._my_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._my_socket.bind((socket.gethostname(), 9876))
        self._alphabet = "abcdefghijklmnop"

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

           
            number.split("\r\n")
            rotation = number[0]
            my_message = number[2]
            print(f"non-decoded message is {number}")
            print(f"rotation is {rotation}")
            print(f"message is {my_message}")
            rotated_message = self.rotate(rotation, my_message)
            print(f"rotated message is {rotated_message}")
            
            self._client_socket.send(bytes(number + "\r\n", "utf-8"))
            #self._client_socket.send(bytes(rotated_message + "\r\n", "utf-8"))
        self._my_socket.close()

    def rotate(self, rotation, message):
        encrypted_message = ""
        rotate = int(rotation)
        for char in message:
            index = (letters.find(char) + rotation) % len(letters)
            encrypted_message += letters[index]
        print(encrypted_message)
        return encrypted_message


cServer = server()
cServer.run()