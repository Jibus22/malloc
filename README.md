What is malloc, from where does this memory comes, how does it work ?
This question is greatly solved by doing this 42 project.

### malloc & mmap

`malloc(3)` isn't a syscall but a wrapper of `mmap(2)` syscall, and so a memory managment utility, keeping tracks of allocation and avoiding to do too much expensive syscalls.
`mmap(2)` - memory mapping - maps files or devices into memory. It is a method of memory-mapped file I/O. It implements demand paging because file contents are not read from disk directly and initially do not use physical RAM at all. The actual reads from disk are performed in a "lazy" manner, after a specific location is accessed. After the memory is no longer needed, it is important to `munmap(2)` the pointers to it.

File-backed mapping maps an area of the process's virtual memory to files; i.e. reading those areas of memory causes the file to be read. It is the default mapping type.
Anonymous mapping maps an area of the process's virtual memory not backed by any file. The contents are initialized to zero. In this respect an anonymous mapping is similar to malloc, and is used in some `malloc(3)` implementations for certain allocations. However, anonymous mappings are not part of the POSIX standard, though implemented by almost all operating systems by the MAP_ANONYMOUS and MAP_ANON flags.

### virtual memory

virtual memory, or virtual storage is a memory management technique that provides an "idealized abstraction of the storage resources that are actually available on a given machine" which "creates the illusion to users of a very large (main) memory".

The computer's operating system, using a combination of hardware and software, maps memory addresses used by a program, called virtual addresses, into physical addresses in computer memory. Main storage, as seen by a process or task, appears as a contiguous address space or collection of contiguous segments. The operating system manages virtual address spaces and the assignment of real memory to virtual memory. Address translation hardware in the CPU, often referred to as a memory management unit (MMU), automatically translates virtual addresses to physical addresses. Software within the operating system may extend these capabilities, utilizing, e.g., disk storage, to provide a virtual address space that can exceed the capacity of real memory and thus reference more memory than is physically present in the computer.

### subject

There is some guidelines about the design:

- use `mmap()` to fetch virtual memory to the system
- split the allocations in 3 blocks which host allocations of different size categories: TINY, SMALL and LARGE. TINY & SMALL must be able to hold at least 100 allocations of (tiny/small)\_max_size. LARGE blocks hold one big allocation. These sizes must be choosen while keeping in mind the goal of performance (not calling too much `mmap()`. These blocks are multiple of pagesize (commonly 4096, fetch it with `getpagesize()`)
- implementation of `free()`, `realloc()` and a utility `show_alloc_memory()` which prints out all allocations and total allocated size per blocks.

### design

I choosed to make each new block having a dope vector (metadata header) which points to the next, the prev, holding type of current block (which allocation size it accepts), the maximum contiguous space remaining (is there enough space for this newcoming allocation here?) , the adress of the first allocation.
So each demanded page are filled by these header data so they are linked between themselves.
Each allocation have also there own header, which holds the size of this allocation, the address of next allocation (so we can roam all allocations of a block) and the address of the payload (which is always after the header but it is kind of security: payload address match with the header address + sizeof(header)).

```
#   |[B]->[Axx]              [Axxxxxxxx][Axx][Axxx] [Ax]  [Axxxxxx]     |
#     |    |                 ^|         ^|   ^|     ^|    ^
#     |    |-----------------.|---------.|---.|-----.|----.
#     v
#   |[B]->[Axxxxxxxxx]           [Axxxxx][Axxxxxxx][Axxx]      [Axxx]  [Axxxxxx]       |
#     |    |                     ^|      ^|        ^|          ^|      ^
#     |    |---------------------.|------.|--------.|----------.|------.
#     v
#   (...)
#
#   B: Block header
#   A: Allocation header
#   x: data
# ' ': unusued & available memory
```

I made bonuses with env variables:

- if FtLeaks is set, malloc prints out at the end of pgm unfreed memory
- if FtMallocErrorAbort is set, abort pgm if `free()`is used on wrong memory address.
- if FtMallocScribble is set, fill memory that has been allocated with 0xaa bytes. This increases the likelihood that a program making assumptions about the contents of freshly allocated memory will fail. Also if set, fill memory that has been deallocated with 0x55 bytes.This increases the likelihood that a program will fail due to accessing memory that is no longer allocated.

