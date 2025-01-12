import socket

# Lade die Binärnachricht aus der Datei
with open("udp_message.bin", "rb") as f:
    message = f.read()

# Zieladresse und Port
server_address = ("127.0.0.1", 1234)

# Erstelle ein UDP-Socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Nachricht senden
udp_socket.sendto(message, server_address)
udp_socket.close()