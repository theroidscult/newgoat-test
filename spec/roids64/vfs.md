# The Roids64 VFS
The Roids64 VFS doesn't only manage files and directories but also all resources accessable via a URL, because I stole the "Everything is a URL" philosophy from RedoxOS.

URL format: `[<scheme>:/][/]<the rest>`
the scheme can be an unmounted partition by id(like `sda1` or a GPT GUID), a network protocol(like `https` or `gopher`), or a resource(like `proc` or `zero`).

special characters:
* `/` - reserved for directory separator
* `\` - lets you escape any special character except `/`
* `"` - can be wrapped around a string like a path to make it able to contain spaces
* `?` - matches 1 character
* `*` - matches 0 or more characters
* `~` - home directory

## Relative paths
If a path starts without a `/` it is relative to the current working directory.

## permissions
Permissions are added to a node in form of an attachment, which has the following fields:
* `owner` - the owner of the node
* `group` - the owner group of the node
* `group read` - whether the group can read the node
* `group write` - whether the group can write to the node
* `group execute` - whether the group can execute the node
* `group stat` - whether the group can get the properties of the node
* `other read` - whether the other can read the node
* `other write` - whether the other can write to the node
* `other execute` - whether the other can execute the node
* `other stat` - whether the other can get the properties of the node