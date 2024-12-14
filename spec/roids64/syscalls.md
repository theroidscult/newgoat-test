# Roids64 syscalls
## Definitions
`int` is a number that can be stored in a single register on the target architecture</br>

`ptr` is a pointer to some memory</br>

there can be `6` arguments maximum
## syscall list
0. exit(`int` status) </br>
stops the process with status code `status`

1. open(`ptr` URI, `int` flags, `int` mode) -> res(`int`) </br>
opens an object at location `URI` with flags `flags` and mode `mode`, returns an resource descriptor.
2. close(`int` res) </br>
closes the resource `res`
3. newobj(`int` type, `ptr` URI, `int` specific 1, `int` specific 2) -> res(`int`) </br>
creates a new object of type  `type` at location `URI`
4. act(`int` res, `int` action,`ptr` buffer, `int` size, *action specific 1*,*action specific 2*) -> status(`int`) </br>
does an action on an resource `res` and returns a status code
