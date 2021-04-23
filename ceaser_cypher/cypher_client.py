import socket

class cypher_client:
    def __init__(self, serverName, port, rotation):
        self._server_name = serverName
        self.ip = socket.gethostbyname(self._server_name)
        self._port = port
        self._rotation = rotation
        socket.setdefaulttimeout(30)
        self._timeout = socket.getdefaulttimeout()
        try:
           
            self._server_connection = socket.create_connection((self._server_name, self._port))
            #self._packet = socket.PF_PACKET(self._rotation)
        except:
            print("server: {} and port: is not connecting", format(self._server_name))

    def run(self):
        self._server_connection.send(self._packet)


# host = input("Hello, please input host: ")
# port = int(input("please input port: "))
# rotation = input("Input rotation : ")
host = "brooks.cs.calvin.edu"
port = 9876
rotation = 2
cypher = cypher_client(host, port, rotation)
cypher.run()
    