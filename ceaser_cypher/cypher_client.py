import socket

class cypher_client:
    def __init__(self, serverName, port):
        self._my_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        self._server_name = serverName
        self._port = port
        socket.setdefaulttimeout(30)
        self._timeout = socket.getdefaulttimeout()
        try: 
            if(self._server_name == "localhost" or self._server_name == ""):
                self._my_socket.connect((socket.gethostname(), self._port))
                welcome = self._my_socket.recv(1024)
                print(welcome.decode())
            else:
                self._my_socket.connect((self._server_name, self._port))
                welcome = self._my_socket.recv(1024)
                print(welcome.decode())
        except:
            print(f"server: {self._server_name} and port: {self._port} is not connecting")

    '''
    Messages sent between Python and Java, the python string must include \r\n because Java is expecting it but python does not
    automatically add those to the end of a string like java
    '''
    # def run(self):  #This was a hardcode test to make sure it could connect to the server
    #     try:
    #         print(bytes("3\r\n", "utf-8"))
    #         self._my_socket.send(bytes("10\r\n", "utf-8"))
    #         self._my_socket.send(bytes("Rotate\r\n", "utf-8"))
    #         # self._my_socket.send(bytes("2\r\n"))
    #         print("socket sent, now recieving")
    #         message = self._my_socket.recv(1024)
    #         print(message.decode())
    #         rotated = self._my_socket.recv(1024)
    #         print(rotated.decode())
    #     except:
    #         print(f"send did not work")
    #     # self._mssg = self._my_socket.recv(2)
    #     self._my_socket.close()

    def run(self):
        choice = "c"
        
        
        while (choice != "q"):
            try:
                #Rotation Group
                ######
                rotation = input("Input rotation amount: ")
                self._my_socket.send(bytes(rotation + "\r\n", "utf-8"))
                EncodedMessage = self._my_socket.recv(1024)
                #######

                unEncodedMessage = input("Input message to encode: ")
                
                self._my_socket.send(bytes(unEncodedMessage + "\r\n", "utf-8"))
                
                
                EncodedMessage = self._my_socket.recv(1024)
                print(EncodedMessage.decode())
                choice = input("Type q to quit\nType c to continue: ")
                if(choice == "q"):
                    continue
                
            except:
                print(f"Error: Input must be between 1 and 25")
                choice = input("Type q to quit\nType c to continue: ")
                if(choice == "q"):
                    quit

        


# host = input("Hello, please input host: ")
# port = int(input("please input port: "))
host = "brooks.cs.calvin.edu"
# host = "localhost"
port = 9876

cypher = cypher_client(host, port)
cypher.run()
    