'''
Author: AJ Vrieland (ajv234)
Date: 04/29/2021
'''
import socket



class cypher_client:
    def __init__(self):
        self._my_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        socket.setdefaulttimeout(30)
        self._timeout = socket.getdefaulttimeout()
        self.debug = 1
        while True:
            self._server_name = input("Welcome please input Server name or q to quit: ")
            if (self._server_name == "q"):
                    quit(0)
            try: 
                if(self._server_name == "localhost" or self._server_name == ""):
                    self._port = int(input("please input the Port: "))
                    self._my_socket.connect((socket.gethostname(), self._port))
                    welcome = self._my_socket.recv(1024)
                    print(welcome.decode())
                    self.run("local")
                    self._my_socket.close()
                else:
                    self._port = int(input("please input the Port: "))
                    self._my_socket.connect((self._server_name, self._port))
                    self.run("brooks")
                    self._my_socket.close()

            except:
                print(f"server: {self._server_name} and port: {self._port} is not connecting")

    '''
    Messages sent between Python and Java, the python string must include \r\n because Java is expecting it but python does not
    automatically add those to the end of a string like java
    '''

    def run(self, server):
        choice = "c"
        #Rotation Group
        ######
        rotation = input("Input rotation amount: ")
        self._my_socket.send(bytes(rotation + "\n", "utf-8"))
        EncodedMessage = self._my_socket.recv(1024)
        if self.debug == 1:
            print(f"Initial message from server: {EncodedMessage}")
        #######
        while (choice != "q"):
            try:
                unEncodedMessage = input("Input message to encode: ")
                
                self._my_socket.sendall(bytes(unEncodedMessage + "\n", "utf-8"))
                
                EncodedMessage = self._my_socket.recv(1024)
                if server == "brooks":
                    EncodedMessage = self._my_socket.recv(4096)
                if self.debug == 1:
                    print(f"encoded message directly is: {EncodedMessage}")
                print(EncodedMessage.decode())
                choice = input("Type q to quit\nType c to continue: ")
                if(choice == "q"):
                    continue
                
            except:
                print(f"Error: Input must be between 1 and 25")
                choice = input("Type q to quit\nType c to continue: ")
                if(choice == "q"):
                    continue

        

cypher = cypher_client()
    