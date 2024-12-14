# The Roids64 event system

The Roids64 event system is very simple and let's processes create, subscribe to, fire, unsubscribe from and delete events.
An event holds the following information:
* the event type ID
* the event priority
* the process which owns the event
* the relevant process group ID for the event
* the permissions* of the event
* the list of processes subscribed and the address of the handler function
* attached data

*: permissions are: Subscribe,Fire,Recieve and Delete for owner process, specified process group and everyone else
