# Simple TCP/UDP client and server

[![Build Status](https://travis-ci.com/mkvdv/protei_test.svg?branch=master)](https://travis-ci.com/mkvdv/protei_test)

## Requirements
Programs require this libraries:
  * [Boost](https://www.boost.org/) library for dealing with **program options**

## Notes
* TCP server use epoll and nonblocking sockets for dealing with multiple connections.
* There is NO state (no session) at every server ~~(rest)~~
* Server think, that all messages ascii sequences, their length is no more than 2014 bytes
and new line symbol '\n' is a ender  (ending marker of message). If there is no ender --
server interpret the whole (no more than 1024) sequence as full message.
* Server extract all unsigned numbers from message,  print all numbers in sorted 
(ascending) order, and then print their sum.
* Example:
```txt
> 20 apples, 30 bananas, 15 peaches and 1 watermelon
< 1 15 20 30
< 66
```

## Usage
```bash
# start server
build/bin/server --port=8000 --proto=tcp

# start any clients
build/bin/client --addr=127.0.0.1 --port=8000 --proto=tcp

```

