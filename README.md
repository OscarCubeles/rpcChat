# rpcChat
This project implements a client-server RPC chat using rpcgen and ncruses.
This is a low-level code implemented in C language. The project includes an RPC server and RPC clients, which can be used to communicate with each other in real-time.

## Requirements

To run the chat application, you will need to have the following software installed:

- C compiler (e.g. GCC)
- RPCGEN

## Usage

To use the chat application, follow these steps:

1. Clone the repository to your local machine.
2. Open a terminal window and navigate to the project directory.
3. Run the command `make` to compile the code.
4. Start the RPC server by running the command `./P3_server`.
5. Start the RPC client by running the command `./P3_client localhost [username]`, where `[username]` is the username you want to use for the chat.

Once the client is connected to the server, you can start sending messages to other clients connected to the server. The chat application supports real-time communication between multiple clients, allowing you to chat with other users in real-time.
