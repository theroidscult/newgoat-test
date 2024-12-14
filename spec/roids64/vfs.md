# The Roids64 VFS
The Roids64 VFS doesn't only manage files and directories but also all resources accessable via a URL.

URL format: `scheme:[//]url`
the scheme can be a mounted disk by name(like `A` or `DRIVE`), an unmounted partition by id(like `sda1` or a GPT GUID), a network protocol(like `https` or `gopher`), or a resource(like `proc` or `zero`).

special characters:
* `/` - reserved for directory separator
* `\` - lets you escape any special character except `/`
* `"` - can be wrapped around a string like a path to make it able to contain spaces
* `?` - matches 1 character
* `*` - matches 0 or more characters
* `space` - cannot be used in a URL, unless its wrapped in quotes

reserved names:
* `~` - home directory
* `%` - root of the current drive

## Mounting
you can mount a disk to:
* the `/` directory
* any letter in the `A`-`Z` range
* any all-caps string less than `8` characters
* any static link link(by linking to the device by UUID)

## Partial URLs
if a URL starts with any of the reserved names and a slash, it is considered a partial URL

partial URLs are relative to the current directory and user