import socket
import time
import random
import select

server_address = '127.0.0.1'
server_port = 4567

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:

    server_socket.bind((server_address, server_port))
    server_socket.listen(1)
    print(f"Listening on {server_address}:{server_port}...")

    connection, client_address = server_socket.accept()

    with connection:

        print(f"Connected by {client_address}")

        while True:
            data = connection.recv(1024)
            if not data:
                break

            message = data.decode(errors="replace").strip()
            print("Received:", message)

            # Responses to incoming messages
            if message.startswith("AUTH"):
                response = "REPLY OK IS Auth success.\r\n"
                connection.sendall(response.encode())

            elif message.startswith("JOIN"):
                response = "REPLY OK IS Join success.\r\n"
                connection.sendall(response.encode())

            elif message.startswith("BYE"):
                response = "BYE FROM server\r\n"
                connection.sendall(response.encode())
                print(f"Sent:", response)
                break

            elif message.startswith("MSG FROM"):
                response = message + "\r\n"
                connection.sendall(response.encode())

            elif message.startswith("ERR"):
                break

            else:
                # Default response
                response = "ERR FROM server IS Unknown command\r\n"
                connection.sendall(response.encode())

            print(f"Sent:", response)

