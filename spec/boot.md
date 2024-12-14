# The codename "Asriel" Boot protocol
The kernel must be an ELF executable, and it must have a valid ELF header.

The kernel only recieves the following `boot_info` structure:
```c
struct boot_info {
    const uint16_t memmap_entry_count;
    const uintptr_t memmap;

    const uint16_t driver_ramfs_size;
    const uintptr_t driver_ramfs;

    const uintptr_t rsdt;
    const uintprint_t kernel_addr;

    const char boot_tag[4];

    const framebuffer fb;
} __attribute__((packed));
```

the type `memmap_entry` is defined as
```c
struct memmap_entry {
    uint8_t type;
    uintptr_t start;
    uintptr_t size;
};
```

and `framebuffer` is defined as:
```c
struct framebuffer {
    uint32_t phys_addr;
    uint16_t width;
    uint16_t height;
    uint16_t pitch;
    uint8_t  bpp;
    uint8_t  red_bits;
    uint8_t  green_bits;
    uint8_t  blue_bits;
    uint8_t  channel_order;
} __attribute__((packed));
```

there are `4` `type`s:
* `0` - R_RECLAIMABLE
* `1` - R_RESERVED
* `2` - R_USABLE_MEM
* `3` - R_BAD_MEM

`start` is the starting *physical* adress of the entry.</br>
`size` is the size of the entry in bytes.

the executable must start at `0xfffffff800000000` or later, A.K.A in the higher half of the address space.