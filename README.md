# SimpHTTP - A Simple HTTP server
SimpHTTP is a **Simp**le HTTP server implementing HTTP/1.x written in C++ without any external dependencies.

> This project is still a WIP and as such does not have a lot of features implemented

It currently works on only Windows but Linux support will be added in the future.

## TODO
* UNIX socket code
* Make Socket abstractions for actual sockets and not connections
* Use separate implementation files for platform specific code instead of `#ifdef`s
* Compile as a library and add a Sandbox subproject