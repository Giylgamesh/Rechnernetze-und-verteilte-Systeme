import socket
import struct

# Nachrichtendaten
message_type = 1          # 1 Byte
hash_id = 42              # 2 Bytes
node_id = 5               # 2 Bytes
node_ip = "127.0.0.1"     # 4 Bytes
node_port = 1234          # 2 Bytes

# IP-Adresse in Network Byte Order umwandeln
node_ip_packed = socket.inet_aton(node_ip)

# Nachricht serialisieren
message = struct.pack("!BHH4sH", message_type, hash_id, node_id, node_ip_packed, node_port)

# Zieladresse und Port
server_address = ("127.0.0.1", 1234)

# Erstelle ein UDP-Socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

print(udp_socket)

# Nachricht senden
udp_socket.sendto(message, server_address)
udp_socket.close()
