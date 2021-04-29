import socket
from string import ascii_letters as letters

class server:
    def __init__(self):
        self._my_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            self._my_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 9876)
            self._my_socket.bind((socket.gethostname(), 9876))
        except:
            self._my_socket.detach()
            exit(-1)
        self._alphabet = "abcdefghijklmnop"

    def run(self):
        try:
            self._my_socket.listen(5)
            self._client_socket, self._client_address = self._my_socket.accept()
            self._client_socket.send(bytes("Welcome to the server!", "utf-8"))
            print(f"connection from {self._client_address} has been established")


            while True:

                #Rotation group
                ##############
                self._msg = self._client_socket.recv(1024)
                print(f"1st sent is: {self._msg}")
                number = self._msg.decode()
                self._client_socket.send(bytes(number + "\r\n", "utf-8"))
                number = number.split("\r\n")
                self._msg2 = self._client_socket.recv(1024)
                ##############

                print(f"2nd sent is: {self._msg2}")

                number2 = self._msg2.decode()

                number2 = number2.split("\r\n")
                number2 = number2[0].split()

                print(f"Split string is: {number2} ")
                print(f"rotation is {rotation}")
                # print(f"Length of number2 is: {number2.__len__()}")
                for i in number2:
                    print(i)
                    rotated_message += self.rotate(rotation, number2[i])
                print(f"rotated message is {rotated_message}")


                self._client_socket.send(bytes(rotated_message + "\r\n", "utf-8"))
            self._my_socket.close()
        except:
            print(f"Exception thrown")
            self._my_socket.close()
            self._client_socket.close()

    def rotate(self, rotation, message):
        encrypted_message = ""
        rotate = int(rotation)
        for char in message:
            print(letters.find(char), rotate)
            index = (letters.find(char) + rotate) % 26
            encrypted_message += letters[index]
        print(encrypted_message)
        return encrypted_message


cServer = server()
cServer.run()