# IPK project 2 - Client for a chat server using the `IPK25-CHAT` protocol

## Executive Summary
The application is a client for communicating with server through `IPK25-CHAT` protocol. It supports the TCP protocol variant and is designed to allow later implementation of the UDP variant. The application manages to successfully communicate according to the assignment requirements. That includes authentization of client, joining different chat rooms, sending and receiving messages, and correctly responding to termination inputs from both the server and user.

## Table of Contents
1. [Executive Summary](#executive-summary)
2. [Theory](#theory)
3. [Implementation Details](#implementation-details)
   - [Interesting Code Sections](#interesting-code-sections)
   - [Extra Features](#extra-features)
4. [Testing](#testing)
   - [Testing Overview](#testing-overview)
   - [Testing Environment](#testing-environment)
   - [Test Cases](#test-cases)
   - [Validation and Verification](#validation-and-verification)
   - [Comparison with Similar Tools](#comparison-with-similar-tools)
5. [Bibliography](#bibliography)
6. [Conclusion](#conclusion)

## Theory
TCP variant of `IPK25-CHAT` protocol is text-based, connection-oriented, reliable and stream-oriented.

 - **Text-based** - Individual messages are sent in human readable format. This allows better testing and readibility especiall during developement.
 - **Connection-oriented** - "Session" is established on both the server (who allocates a socket for client) and on the client, who communicates through allocated socekt with one server.
 - **Reliable** - The TCP protocol handles establishing connection (3 way handshake), reliable delivery of messages (ACK packets) and terminating connection successfuly for both sides (4 way handshake).
 - **Stream oriented** - At any one moment, on a TCP socket, there may be available multiple messages, or only partial message. This means that the application needs to keep track of uncomplete messages and store them in buffer. And it also means that more than one message may need to be read and parsed from single receive.

## Implementation Details
The implementation utilizes the *State* design pattern for client's state (`src\States`), *Strategy* design pattern (`src\Transport`,`src\Message`) for encapsulating the transport layer logic and command processing.

### UML Class diagram
Below is the UML class diagram for the project:
![UML Class Diagram](diagrams-class_diagram.drawio.svg)

### Diagram of structures
Below is a diagram of structures, useful for interpreting types in the class diagram.
![Structures Diagram](diagrams-structures.drawio.svg)

### Extra Features
 - Usage of the assignment provided logging messages through out the program.

## Testing
### Testing Overview
- **What was tested**: 
  - Communication with server (happy path) - sending correct messages according to grammar, reacting to server sent messages.
  - Communication termination from the server side.
  - Communication termination from the client side.
  - Correct state transitions according to the state diagram (excluding End state).
- **How it was tested**: 
  - Netcat - Used to simulate server responses and check validity of outgoing messages.
  - Wireshark - Verification (mostly in the early stages) of successful connection establishment, termination, and message contents.
  - Reference server - Authentication, joining channels, sending messages.

### Testing Environment
- **Hardware**: Standard Windows PC with x86 processor and access to internet.
- **Software**: Ubuntu 24 (WSL), reference enviroment provided in assignment.

### Validation and Verification
During the testing the clients outputs were compared to the expected results and program was modified accordingly.

## Bibliography and sources
 - [Project assignment](https://git.fit.vutbr.cz/NESFIT/IPK-Projects/src/branch/master/Project_2)
 - Testing resources:
   * Reference Server from assignment: `anton5.fit.vutbr.cz:4567`
   * [Netcat](https://cs.wikipedia.org/wiki/Netcat)
   * [WireShark](https://www.wireshark.org/)
 - Various online resources and documentations: 
   * [C++ Socket Programming](https://www.tutorialspoint.com/cplusplus/cpp_socket_programming.htm)
   * [Sockets Tutorial](https://www.linuxhowtos.org/C_C++/socket.htm)
   * [Unique pointers](https://stackoverflow.com/questions/53870522/why-use-stdmake-unique-in-c17)
   * [Basic poll usage](https://www.reddit.com/r/C_Programming/comments/eeqi8r/understanding_how_to_use_poll_in_socket/)
   * [Non blocking sockets and polls](https://stackoverflow.com/questions/37011195/how-to-poll-simultaneously-from-stdin-and-from-sockets)
   * [Poll manpage](https://man7.org/linux/man-pages/man2/poll.2.html)
   * [Regex](https://en.cppreference.com/w/cpp/regex)
   * [IPK lecture on 6.3.2025](https://www.youtube.com/watch?v=6AcrgC4Dcgo&list=PL_eb8wrKJwYvanC5wqUUdG9_BRk8GxrNU&index=17)

 - Aritficial intelligence assistance:
   * ChatGPT-4o - Consulting possible approaches to program structure and desing. Selecting and understanding basic object oriented patterns.
   * Gemini-Google - Generating boilerplate code. Discussing issues of my original code. Suggesting libraries and best practices.
   
   All AI generated code was reviewed, tested and verified for suitability and corectness.


## Conclusion
The project implement client for a chat server using the **TCP variant** of the `IPK25-CHAT` protocol.

