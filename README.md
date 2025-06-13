# Client application for chat server

![language](https://img.shields.io/badge/language-C++-3A86FF)
![misc](https://img.shields.io/badge/tool-Wireshark-FF006E)

## Description
Client application for communicating with a chat server using a custom `IPK25-CHAT` protocol. The project implements the TCP variant of the protocol, handling connection setup, authentication, joining channels, sending and receiving messages, and graceful termination. The design anticipates extension to the UDP variant. The implementation uses object-oriented design and state/strategy design patterns.

---

## Usage

### Dependencies and prerequisites

- Linux (or WSL/Unix-like environment)
- g++ (with C++20 support)
- make

### Setup

1. Clone the repository
```bash
git clone git@github.com:petr-plihal/chat-client-ipk25.git
cd chat-client-ipk25
```

2. Build the project
```bash
make
```

NOTE: The `Makefile` is set to compile the program without `-DDEBUG_PRINT` flag. The flag can be added, in case you want to know how the program roughly operates. To turn this on, simply add the `-DDEBUG_PRINT` to the flags in [`Makefile` on line 5](./Makefile).

### Executing program with local example server

If no server is setup for connection, you can run simple dummy python server script, or use the `nc` command to simulate chat server. All the Python server does is respond to your messages and commands with acknowledgements. Plus it repeats any messages sent.

#### Example usage with Python dummy server

First run the server in one terminal:
```bash
python3 ./test/simple_server.py
```

In another terminal, run application with:
```bash
./ipk25chat-client -t tcp -s 127.0.0.1
```
NOTE: To see what the arguments mean, you can see help:
```bash
./ipk25chat-client -h
```

In the client application, to see what commands are available for interaction with the program, type `/help`.

The general user-flow might be as follows:

1. Authenticate with the server using username, secret, display name.
```bash
/auth username secret display_name
```

2. Join different channels (by default you are joined into general channel).
```bash
/join different_channel
```

3. Send message into current channel.
```bash
Simply write the message without any "\", the server will just send it back.
```

4. To correctly terminate the connection, use `CTRL+C`, the server will shut down too.

Alternatively, you can use `nc` command to receive client messages as text, and respond by manually typing the responses. To start listening:
```bash
nc -4 -C -l -p 4567
```

See the [assignment](./doc/assignment.md) or [documentation](./doc/documentation.md) for more usage examples.

---

## License
This project is licensed under the GNU General Public License v3.0. See the [LICENSE](LICENSE) file for details.