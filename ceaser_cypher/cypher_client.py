import socket

class cypher_client:
    def __init__(self, serverName, port, rotation):
        self._my_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        self._server_name = serverName
        self._port = port
        self._rotation = rotation
        socket.setdefaulttimeout(30)
        self._timeout = socket.getdefaulttimeout()
        try:  
            #self._my_socket.connect((self._server_name, self._port))
            self._my_socket.connect((socket.gethostname(), self._port))
            #mssg = self._my_socket.recvmsg(16)
            
        except:
            print("server: {self._server_name} and port: {self._port} is not connecting")

    def run(self):
        try:
            self._my_socket.send(bytes(self._rotation))
            print("socket sent, now recieving")
            message = self._my_socket.recv(1024)
        except:
            print(f"send did not work")
        # self._mssg = self._my_socket.recv(2)
        print(message.decode())


# host = input("Hello, please input host: ")
# port = int(input("please input port: "))
# rotation = input("Input rotation : ")
host = "brooks.cs.calvin.edu"
port = 9876
rotation = 2
cypher = cypher_client(host, port, rotation)
cypher.run()
    