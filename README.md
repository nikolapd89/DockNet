---

# DockNet TCP Server

DockNet is a lightweight, easy-to-use TCP server, designed to provide a robust framework for handling TCP connections. It's built with simplicity and efficiency in mind, making it ideal for those looking to understand or implement fundamental TCP server operations. To further support this goal, the source code of DockNet is enriched with tutorial-like explanatory comments.

## Features

- Supports both IPv4 and IPv6 connections.
- Graceful handling of SIGINT for server shutdown.
- Basic client connection management implemented, with advanced features such as polling and forking planned for future updates.
- Basic piping to standard output of server operations and connections.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

What you need to install the software:

- A C compiler (like gcc)
- Basic understanding of network programming in C

### Installing

A step-by-step series of examples that tell you how to get a development environment running:

1. Clone the repository to your local machine:
   ```
   git clone https://github.com/roundfly/DockNet
   ```

2. Navigate to the project directory:
   ```
   cd DockNet
   ```

3. Compile the server:
   ```
   gcc -o docknet src/main.c src/server.c src/client_info.c -Iinclude
   ```

4. Run the server:
   ```
   ./docknet
   ```

## Usage

After starting the server, it will listen for incoming TCP connections. Clients can connect to the server using tools like netcat or other TCP clients.

Example using `nc`:

```
nc localhost 9090
```

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

---
