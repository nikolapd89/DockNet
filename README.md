---

# DockNet TCP Server

DockNet is a lightweight, easy-to-use TCP server, designed to provide a robust framework for handling TCP connections. It's built with simplicity and efficiency in mind, making it ideal for those looking to understand or implement fundamental TCP server operations. To further support this goal, the source code of DockNet is enriched with tutorial-like explanatory comments.

## Features

- Supports both IPv4 and IPv6 connections.
- Graceful handling of SIGINT for server shutdown.
- Basic client connection management implemented, with advanced features such as polling and forking planned for future updates.
- Basic piping to standard output of server operations and connections.

## Getting Started

These instructions will guide you through setting up the project on your local machine for development and testing purposes.

### Prerequisites

Before you begin, ensure you have the following installed:

- A compatible C compiler, such as `clang` or `gcc`
- `make` utility for automating the build process
- Basic understanding of network programming in C

### Installation

Follow these steps to get your development environment running:

1. Clone the repository to your local machine:
   ```sh
   git clone https://github.com/nikolapd89/DockNet.git
   ```

2. Navigate to the project directory:
   ```sh
   cd DockNet/DockNet
   ```

3. Compile the server using `make`:
   ```sh
   make
   ```
This command invokes the Makefile, which automatically compiles the source files and generates the executable in the `bin` directory.

### Running the Server

After compiling the server, you can start it with the following command:

```sh
./bin/docknet
```

This will launch the server executable.

### Cleaning Up

To clean up the build artifacts and start fresh, you can use the `clean` target in the Makefile:

```sh
make clean
```
This command removes the `bin` and `build` directories, ensuring that the next build starts from scratch.

## Usage

After starting the server, it will listen for incoming TCP connections. Clients can connect to the server using tools like netcat or other TCP clients.

Example using `nc`:

```sh
nc localhost 9090
```

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

---
