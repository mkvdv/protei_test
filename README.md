# Simple TCP/UDP client and server

## Requirements
Programs require this libraries:
   * [Boost](https://www.boost.org/) library for dealing with **program options**

## Notes
* TCP server use epoll and nonblocking sockets for dealing with multiple connections.
* There is NO state (no session) at every server ~~(rest)~~

## Usage
```bash
# start server
build/bin/server --port=8000 --proto=tcp

# start any clients
build/bin/client --addr=127.0.0.1 --port=8000 --proto=tcp

```
