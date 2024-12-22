# The newgoat VFS

## The tree and caching
The first layer of the tree has `5` directories:
* `/` - the root of the filesystem
* `dev:` - devices
* `proc:` - process information
* `tmp:` - temporary files
The tree may be implemented as any balanced binary search tree. The BST uses the `full path` of each node as the key.

## Nodes
Each node has a the following properties:
* `path` - full path
* `size` - the size of the file if its a file
* `num_children` - the number of children if its a directory
* `flags` - there are `2` flags: `DIR` and `FILE`, if it has both it is a `duality` that can be **opened both as a directory and a file**
* `linkage type` - `0` for no linkage, `1` for static link, `2` for symbolic link
* `linkage target` - the target of the link
* `createion time` - the time the node was created
* `last modified time` - the time the node was last modified
* `last opened time` - the time the node was last opened

Nodes can also have attachments that are defined by the executive layers.

## Mounting
you can mount a disk to:
* the root directory
* any static link