# tcp-server
Short and sweet tcp-server written in C. Minimal dependencies, compiles in many environments. Echos bytes received.

# compile
```console
  gcc server.c -o tcp-server
```
# run
```console
  ./tcp-server
  socket successfully created.
  port bound!
  attempting to listen for connection attempts...
  server listening...
  client connection request accepted!
  from client: fuck yes!
  
  echoing 10 bytes... [0]
```

# network / port forwarding
If it says it's listening, but requests are not getting through
you may have to forward / open the ports from the device you're
trying to run a server on as well as whatever router that potential server
is running traffic through. I will never be your system administrator, 
if you don't know how to work your router or your system that's your problem.
