# KAOS
![KAOS Banner](assets/kaos_banner.png)

KAOS is yet another 32-bit microkernel Operating System written from scratch. It is currently in it's early stages.

## Progress

Since KAOS is still in an early alpha phase, there are lots of things on the todo list to help lift it off the ground, here are (some) major examples:
- [x] Bootloader
- [x] Early Initialization
- [ ] IDT setup (including PIC remapping)
- [ ] Setup paging & make a PMM 
- [ ] Scheduler
- [ ] Setup IPC
- [ ] Video driver (efficient management of the LFB)
- [ ] Input driver (keyboard, mouse, other devices like that)
- [ ] Chrono driver (clock)
> _These are broken down into small tasks_

## Build & Run

To build or run kaos, you need to follow a few steps described below. **However**, please check the [requirements](#requirements) section first.

### Requirements

The following tools are **__required__**
- **Make** (for the makefile)
- **NASM** (all assembly scripts are written with the NASM syntax)
- **GCC** (unless modified in the [makefile](./makefile))

As for the emulator, it's highly recommended to use **QEMU** (more specifically, tested under qemu-system-i386)


### Steps

1. Before trying to build or run, make sure you clone the repository. You can do so by inputting the following in your terminal:
```bash
git clone https://github.com/kashiexe/kaos.git
cd kaos
```
2. After that, build the OS image file by simply executing, on the terminal:
```bash
make
```
3. Now, to run KAOS, you can execute
```bash
make run
```
4. When you're done, you can cleanup the build directory by executing:
```bash
make clean
```

#### Bonus

The makefile also features a `debug` command, although this is not very useful for now, this could become very valuable in the future.

## Project Structure

> ![WARNING]
> Please keep in mind this section is edited __MANUALLY__, so it might not be fully up-to-date.

Quick overview (with comments) of the project's folders:
```bash
kaos/
├── assets/             # assets used throughout the project
├── src/
    ├── boot/           # contains the assembly code for the bootloader
    ├── core/           # generally contains the most important setup for the kernel and utilities
        ├── include/    # header files
        ├── isrs/       # contains the wrappers and handlers for all interrupts in the IDT
        ├── utils/
    ├── libc            # essential files for C and eventually C++
        ├── include/ 
# root files are mostly either config files, github specific repo files or files for building KAOS
```

## Contributions

As KAOS is mostly a personal project, contributions will largely not be accepted. The respository is public purely for show :)

## License

KAOS is licensed under the [Apache 2.0 license](LICENSE).

Copyright © 2026 KAOS
