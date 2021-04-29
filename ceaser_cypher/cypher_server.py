'''
Author: AJ Vrieland (ajv234)
Date: 04/29/2021
'''
import socket
import time
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
        while True:
            self._my_socket.listen(5)
            try:
                self._client_socket, self._client_address = self._my_socket.accept()
                self._client_socket.send(bytes("Welcome to the server!", "utf-8"))
                print(f"connection from {self._client_address} has been established")
                #This section of code is blocked of because I can move this with it's equivilant in the client, into the below while loop
                #and it allows the change of the rotation amount for each message without having to reconnect to the server
                #however since that is not how brooks.cs.calvin.edu works, it is out side of the loop so that the client can connect to the other server
                ##############
                self._msg = self._client_socket.recv(1024)
                rotation_amount = self._msg.decode()
                self._client_socket.send(bytes(rotation_amount + "\r\n", "utf-8"))
                rotation_amount = rotation_amount.split("\r\n")
                rotation = int(rotation_amount[0])
                ##############
                while True:
                    self._msg2 = self._client_socket.recv(1024)
                    message_to_encode = self._msg2.decode()
                    message_to_encode = message_to_encode.split("\r\n")
                    message_to_encode_list = message_to_encode[0].split()
                    unRotatedMessage = ""
                    rotated_message = ""
                    for i in message_to_encode_list:
                        unRotatedMessage += i + " "
                        rotated_message += self.rotate(rotation, i)
                        rotated_message += " "
                    print(f"rotation is {rotation}")
                    print(f"unrotated message is {unRotatedMessage}")
                    print(f"rotated message is {rotated_message}\n\n")
                    self._client_socket.send(bytes(rotated_message + "\r\n", "utf-8"))
                self._my_socket.close()
            except:
                print(f"Exception thrown")
                time.sleep(2)
                continue

    def rotate(self, rotation, message):
        encrypted_message = ""
        rotate = int(rotation)
        for char in message:
            index = (letters.find(char) + rotate) % 26
            encrypted_message += letters[index]
        return encrypted_message


cServer = server()
cServer.run()