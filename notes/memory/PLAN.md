# Milestone 4: Memory (theory first)

**Goal:** understand every memory idea used in microcontroller programming,
well enough to **teach it to someone else**.

**Phase 1 is theory only.** There's no simulation and no project code. Short C
lines appear only to show what a term looks like. The practical part (code,
GDB, Solo 3) comes after the theory, as Phase 2.

## Rules that keep it from getting confusing
1. **Every term is defined before it is used.** Each lesson only uses words
   from earlier lessons.
2. **One lesson = one idea.** Each lesson is taught one small step at a time.
3. **Words with two meanings are flagged** in the lesson where they first appear:

| Word | Meaning 1 | Meaning 2 |
|---|---|---|
| volatile | hardware: memory that forgets when power is off (RAM) | C keyword: "don't optimise reads/writes of this" |
| register | CPU register: a tiny storage slot inside the CPU | peripheral register: a control address of a peripheral (GPIO, UART) |
| static | on a local: "keep the value between calls" | on a global/function: "private to this file" |
| word | general: a CPU's natural data size | on Cortex-M: exactly 32 bits |
| declaration / definition | declaration: "this exists" (a promise) | definition: "here it is" (the memory or the body) |
| section / segment | section: a named part of a program file (`.data`) | segment: a group of sections loaded together |

## How each lesson works
1. **Why it matters on an MCU:** a real bug or a real use
2. **The picture:** a box diagram
3. **The terms,** in order. **Every term gets:**
   - **Definition:** one or two sentences in plain words
   - **Example:** a real one, from an MCU or from C code
   - **Watch out:** only where there's a common mistake or a word with two meanings
4. **Check questions:** short
5. **Teach-back:** you explain it in your own words, as if to a junior
   engineer. Once it's right, it goes into your own handbook.

## What "mastered" means for a term
You can (1) define it, (2) give an example, (3) say why it matters on an
MCU, and (4) explain it to someone else without notes.

---

## Part A: Memory as hardware (what memory is, and where it is)

### 4.1 Bits, bytes and addresses
Terms: bit, byte, bit numbering (bit 0 = LSB, bit 7 = MSB), nibble, word, half-word, double-word, address, contents
(the address vs the value at that address), address space, hexadecimal
notation (`0x`), KB/MB (and KiB), memory size vs address width (32-bit
addresses = 4 GB address space).

### 4.2 Kinds of memory
Terms: volatile vs non-volatile memory, RAM, SRAM vs DRAM, ROM, mask ROM,
OTP, Flash (NOR vs NAND), EEPROM, read speed vs write speed, erase, page and
sector, endurance (write cycles), data retention, CPU registers, the memory
hierarchy (registers → cache → RAM → Flash).

### 4.3 How the CPU reaches memory
Terms: bus, address bus, data bus, read and write cycles, access width
(8/16/32-bit), memory-mapped I/O, peripheral register, von Neumann vs Harvard
architecture (Cortex-M0 vs Cortex-M3/M4), wait states (why Flash can be
slower than the CPU).

### 4.4 The memory map
Terms: memory map, region, reserved region, the Cortex-M memory map (Code,
SRAM, Peripheral, External, System/PPB), Flash base address (0x0000_0000),
SRAM base (0x2000_0000), peripheral base (0x4000_0000), vector table
location, aliasing (one memory visible at two addresses), bit-banding
(M3/M4 only). Example: your Nuvoton M031. Contrast: how ESP32 differs (IRAM,
DRAM, RTC memory, Flash through a cache).

---

## Part B: How a C program uses memory

### 4.5 Variables in C: scope, lifetime, linkage
Terms: object (a piece of memory holding a value), declaration vs
definition, scope (block, file), lifetime / storage duration (automatic,
static, allocated), linkage (external, internal, none), `static` (both
meanings), `extern`, `auto`, `register` (the keyword), initial values (static
→ zero, local → garbage), global variable, local variable.

### 4.6 From source code to a memory image
Terms: compiler, object file (`.o`), symbol, symbol table, linker, section,
linker script (GCC `.ld`) = scatter file (Keil `.sct`), relocation, ELF
file, `.bin` and `.hex` files, image, load address vs run address (LMA vs
VMA), memory regions in the linker script (FLASH, RAM).

### 4.7 The sections: where each variable lives
Terms: `.text` (code), `.rodata` (`const` data), `.data` (globals/statics
with a starting value), `.bss` (globals/statics that start at zero), heap,
stack, the "which variable goes where" table, Flash cost vs RAM cost (why
`.data` costs both), string literals, `const` tables in Flash.

### 4.8 Before `main()`: start-up
Terms: reset, power-on reset, vector table, initial stack pointer value,
reset vector, reset handler, start-up code (`startup_xxx.s`), copying
`.data` from Flash to RAM, zeroing `.bss`, `SystemInit`, C runtime
start-up (crt0), map file (reading sizes and addresses).

### 4.9 The stack
Terms: stack, stack pointer (SP; MSP and PSP on Cortex-M), push and pop,
LIFO, "full descending" (grows downward), stack frame, return address, link
register (LR), calling convention (AAPCS: arguments in r0–r3, then the
stack), local variables on the stack, stack size, stack overflow, recursion
risk, large local arrays, dangling pointer to a local, stack painting /
high-water mark, stack canary.

### 4.10 The heap
Terms: dynamic allocation, heap, `malloc`, `calloc`, `realloc`, `free`, heap
size, memory leak, double free, use-after-free, fragmentation (internal vs
external), determinism, stack–heap collision, static allocation, memory pool /
fixed-block allocator, the "no malloc after start-up" rule, MISRA C.

---

## Part C: Pointers (reaching memory from C)

### 4.11 Pointer basics
Terms: pointer, address-of `&`, dereference `*`, pointer type, pointer size
(4 bytes on a 32-bit MCU, 8 on a 64-bit PC), `NULL`, wild (uninitialised)
pointer, dangling pointer, pointer to a local vs to a static.

### 4.12 Pointers and arrays
Terms: array layout in memory, element, index, pointer arithmetic (it steps
by the type size), array decay, `sizeof` array vs `sizeof` pointer (the
function-parameter trap), out of bounds, buffer overflow, C strings and the
null terminator `'\0'`, 2-D arrays in memory.

### 4.13 Structs in memory
Terms: struct layout, member offset, `offsetof`, alignment, natural
alignment, padding, `sizeof` a struct, packed struct, unaligned access (a
HardFault on Cortex-M0), pass by value vs pass by pointer, `->`, returning a
struct.

### 4.14 Qualifiers
Terms: `const`, pointer to const vs const pointer vs both (reading right to
left), `volatile` (the C keyword, and why the optimiser needs it), `const
volatile` (a read-only register), `restrict` (term only).

### 4.15 Advanced pointers
Terms: pointer to pointer, `void *` (a generic pointer), casting pointers,
function pointer, callback, table of function pointers (jump table; the
vector table is one), `union`, type punning, strict aliasing (term only).

---

## Part D: Memory meets hardware

### 4.16 Peripheral registers in C
Terms: an address cast to a `volatile` pointer
(`*(volatile uint32_t *)0x40004000`), register map, register struct overlay
(CMSIS style, `GPIOA->DOUT`), reserved fields, read-modify-write, set/clear
registers, write-1-to-clear (W1C), read-only and write-only registers,
bit-banding in practice.

### 4.17 How data is stored
Terms: endianness (little vs big; Cortex-M is little-endian), byte order
on a wire (network byte order), fixed-width types (`uint8_t` … `uint32_t`),
bit fields (and why they're risky for registers), integer size per platform.

### 4.18 Non-volatile storage in practice
Terms: Flash erase/program cycle, erase-before-write, page/sector erase,
wear, wear levelling, EEPROM emulation, storing settings, execute in place
(XIP), bootloader and application partitions (terms only).

### 4.19 Memory shared with other hardware and code
Terms: DMA (Direct Memory Access), DMA buffer lifetime, cache and cache
coherence (term; Cortex-M7), memory shared with an ISR, atomic access, torn
read, critical section (a preview; full detail comes in the concurrency
milestone).

---

## Part E: When memory goes wrong

### 4.20 Faults and protection
Terms: fault, HardFault (the only fault on M0), MemManage / BusFault /
UsageFault (M3/M4), a NULL pointer on an MCU (address 0 is Flash: reads
often *don't* fault!), MPU (Memory Protection Unit), stack overflow
detection, tools to find memory bugs (map file, GDB memory view `x`,
watchpoints, static analysis).

### 4.21 The rulebook, and the final teach-back
A memory checklist for firmware (the MISRA-style rules collected from
every lesson), then a teach-back test: I play a junior engineer and ask
you questions.

---

## Phase 2 (after the theory): practice
Code labs for the key lessons (lifetime break-it, stack overflow, pointer
to struct, register overlay, memory pool, buffer overflow), GDB to look at
real addresses and bytes, and **Solo 3** (45 minutes, a written
prediction).

## Later milestones (not in this one)
Writing a linker script yourself and the start-up code in detail (plan
item 8: MCU architecture), full concurrency (plan item 9), RTOS memory such
as task stacks (plan item 10).
