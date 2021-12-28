from time import sleep
import socket

FUSEE_UDP_IP = "192.168.0.50"
UDP_PORT = 23900
MESSAGE = b"Hello, World!"

print("UDP target IP: %s" % FUSEE_UDP_IP)
print("UDP target port: %s" % UDP_PORT)
print("message: %s" % MESSAGE)

# Envoi d'une première commande, pour que la fusée connaisse l'IP de l'émetteur
sock_send = socket.socket(socket.AF_INET, # Internet
    socket.SOCK_DGRAM) # UDP
sock_send.sendto(MESSAGE, (FUSEE_UDP_IP, UDP_PORT))



LOCAL_IP = ""
sock_rec = socket.socket(socket.AF_INET, # Internet
    socket.SOCK_DGRAM) # UDP
sock_rec.bind((LOCAL_IP, UDP_PORT))

while True:
    data, addr = sock_rec.recvfrom(255) # taille du buffer
    print("received message: %s" % data)