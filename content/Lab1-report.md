# Lab1

[TOC]

## 简介

分为三部分

1. 熟悉x86汇编语言、QEMU x86仿真器、PC 电源开启时的引导程序。
2. 验证在lab/boot中的引导装载程序
3. 深入研究在kernel目录下的6.828内核

## 软件安装

1. GIt
2. 按照[tools page](https://pdos.csail.mit.edu/6.828/2018/tools.html)安装环境

## qemu使用

`cd lab`

`make qemu`

`make qemu-dgb`



## Part1: PC Booststrap

### PC机的物理地址空间

<img src="../pictures/p31.png" style="zoom:90%;" />

这张图仅仅展示了内存空间的一部分。

- 第一代PC处理器是16位字长的Intel 8088处理器，这类处理器只能访问1MB的地址空间，即0x00000000 ~ 0x000FFFFF。但是这1MB也不是用户都能利用到的，只有低640KB(0x00000000 ~ 0x000A0000)的地址空间是用户程序可以使用的。

- 而剩下的384KB的高地址空间则被保留用作其他的目的，比如(0x000A0000 ~ 0x000C0000)被用作屏幕显示内容缓冲区，其他的则被非易失性存储器(ROM)所使用，里面会存放一些固件，其中最重要的一部分就是BIOS，占据了0x000F0000~0x00100000的地址空间。BIOS负责进行一些基本的系统初始化任务，比如开启显卡，检测该系统的内存大小等等工作。在初始化完成后，BIOS就会从某个合适的地方加载操作系统。

- 虽然Intel处理器突破了1MB内存空间，在80286和80386上已经实现了16MB，4GB的地址空间，但是PC的架构必须仍旧把原来的1MB的地址空间的结构保留下来，这样才能实现向后兼容性。所以现代计算机的地址 0x000A0000 ~ 0x00100000区间是一个空洞，不会被使用。因此这个空洞就把地址空间划分成了两个部分，第一部分就是从0x00000000~0x000A0000，叫做传统内存。剩下的不包括空洞的其他部分叫做扩展内存。而对于这种32位字长处理器通常把BIOS存放到整个存储空间的顶端处。

- 由于xv6操作系统设计的一些限制，它只利用256MB的物理地址空间，即它假设用户的主机只有256MB的内存。

### The ROM BIOS

#### make qemu-gdb

<img src="/Users/asmqsaccount/Library/Application Support/typora-user-images/image-20200509172014295.png" alt="image-20200509172014295" style="zoom:33%;" />

新建terminal，cd pwd，输入`gdb`

<img src="/Users/asmqsaccount/Library/Application Support/typora-user-images/image-20200509172107791.png" alt="image-20200509172107791" style="zoom:33%;" />

此时可以开始调试

`[f000:fff0] 0xffff0:ljmp $0x3630,$0xf000e05b`是启动PC后，执行BIOS的第一条指令。

### Exercise 2: 使用`si`来追踪ROM BIOS的几条指令

```assembly
[f000:e05b]    0xfe05b:	cmpw   $0xffb8,%cs:(%esi)
[f000:e062]    0xfe062:	jne    0xd241d121
[f000:e066]    0xfe066:	xor    %edx,%edx
[f000:e068]    0xfe068:	mov    %edx,%ss
[f000:e06a]    0xfe06a:	mov    $0x7000,%sp
[f000:e070]    0xfe070:	mov    $0x18d6,%dx
[f000:e076]    0xfe076:	jmp    0x5576cf9b
[f000:cf99]    0xfcf99:	cli    
[f000:cf9a]    0xfcf9a:	cld    
[f000:cf9b]    0xfcf9b:	mov    %ax,%cx
[f000:cf9e]    0xfcf9e:	mov    $0x8f,%ax
[f000:cfa4]    0xfcfa4:	out    %al,$0x70
[f000:cfa6]    0xfcfa6:	in     $0x71,%al
[f000:cfa8]    0xfcfa8:	in     $0x92,%al
[f000:cfaa]    0xfcfaa:	or     $0x2,%al
[f000:cfac]    0xfcfac:	out    %al,$0x92
[f000:cfae]    0xfcfae:	mov    %cx,%ax
[f000:cfb1]    0xfcfb1:	lidtl  %cs:(%esi)
[f000:cfb7]    0xfcfb7:	lgdtl  %cs:(%esi)
[f000:cfbd]    0xfcfbd:	mov    %cr0,%ecx
[f000:cfc0]    0xfcfc0:	and    $0xffff,%cx
[f000:cfc7]    0xfcfc7:	or     $0x1,%cx
[f000:cfcb]    0xfcfcb:	mov    %ecx,%cr0
[f000:cfce]    0xfcfce:	ljmpw  $0xf,$0xcfd6
```

- `[f000:fff0] 0xffff0: ljmp $0x3630,$0xf000e05b`是启动PC后，执行BIOS的第一条指令。
  - CS（CodeSegment）和IP（Instruction Pointer）寄存器一起用于确定下一条指令的地址。计算公式： physical address = 16 * segment + offset.
  - PC开始运行时，CS = 0x3630，IP = 0xf000e05b，第一条指令做了jmp操作，跳到物理地址为16 * segment + offset的位置。
  
- `[f000:e05b]    0xfe05b:	cmpw   $0xffb8,%cs:(%esi)` 
  
  - 把0xffb8立即数和%cs:(%esi)所代表的内存地址处的值比较，<font color = 0x0000af>至于为什么这样比较，现在还不是很清楚。</font>
  
- `[f000:e062]    0xfe062:	jne    0xd241d121`
  -  jne指令：如果ZF标志位为0的时候跳转，即上一条指令cmpl的结果不是0时跳转。
  - 由下一条指令看出来没有跳转
  
- `[f000:e066]    0xfe066:	xor    %edx,%edx`
  
  - 把edx寄存器清零
  
- ```assembly
  [f000:e068]    0xfe068:	mov    %edx,%ss
  [f000:e06a]    0xfe06a:	mov    $0x7000,%sp
[f000:e070]    0xfe070:	mov    $0x18d6,%dx
  [f000:e076]    0xfe076:	jmp    0x5576cf9b
  ```
  
  - 接下来的这些指令就是设置一些寄存器的值

- `[f000:cf99]    0xfcf99:	cli`

  - clear interrupt
  -  关闭中断指令。启动时的操作是比较关键的，所以肯定是不能被中断的。这个关中断指令用于关闭那些可以屏蔽的中断。比如大部分硬件中断。

- `[f000:cf9a]    0xfcf9a:	cld`

  - 设置方向标识位为0，表示后续的串操作比如MOVS操作，内存地址的变化方向，如果为0代表从低地址值变为高地址。

- ```assembly
  [f000:cf9b]    0xfcf9b:	mov    %ax,%cx
  [f000:cf9e]    0xfcf9e:	mov    $0x8f,%ax
  [f000:cfa4]    0xfcfa4:	out    %al,$0x70
  [f000:cfa6]    0xfcfa6:	in     $0x71,%al
  ```

  - 80x86CPU采用IO端口单独编址的方式，即所有设备的端口都被命名到一个IO端口地址空间中。这个空间是独立于内存地址空间的。所以必须采用和访问内存的指令不一样的指令来访问端口。标准规定端口操作必须要用al寄存器作为缓冲。

  -  **in %al, PortAddress   向端口地址为PortAddress的端口写入值，值为al寄存器中的值**

  - **out PortAddres,%al   把端口地址为PortAddress的端口中的值读入寄存器al中**

  - http://bochs.sourceforge.net/techspec/PORTS.LST可以查看端口操作

  - 0x70和0x71对应CMOS，这个CMOS中可以控制跟PC相关的多个功能，其中最重要的就是**时钟设备（Real Time Clock）**的 ，它还可以控制是否**响应不可屏蔽中断NMI(Non-Maskable Interrupt)**。

  - 操作CMOS存储器（包括一个实时钟和128个RAM存储单元）中的内容需要两个端口，一个是0x70另一个就是0x71。其中0x70可以叫做<font color = #af0000>索引寄存器（或者叫地址端口）</font>，**这个8位寄存器的最高位是不可屏蔽中断(NMI)使能位。如果你把这个位置1，则NMI不会被响应。低7位用于指定CMOS存储器中的存储单元地址**，所以如果你想访问第1号存储单元，并且在访问时，我要使能NMI，那么你就应该向端口0x70里面送入0b10000001 = 0x81。

    - E.g. `mov $0x81, %al`

         	`out %al, 0x70`

  - 然后对于这个地址单元的操作，比如读或者写就可以由0x71端口完成，比如你现在想从1号存储单元里面读出它的值，在完成上面的两条指令后，就可以输入`in $0x71, %al`

  - 再回到我们的系统，这三条指令可以看出，它首先关闭了NMI中断，并且要访问存储单元0xF的值，并且把值读到al中，但是在后面我们发现这个值并没有被利用，所以可以认为**这些指令是用来关闭NMI中断的。**

- ```assembly
  [f000:cfa8]    0xfcfa8:	in     $0x92,%al
  [f000:cfaa]    0xfcfaa:	or     $0x2,%al
  [f000:cfac]    0xfcfac:	out    %al,$0x92
  ```

  - [端口查看](http://bochs.sourceforge.net/techspec/PORTS.LST )
  - 0x92端口控制的是PS/2系统控制端口A。2,3步是在把这个端口的1号bit置为1。
  - bit 1= 1 indicates A20 active
  -  即A20位，即第21个地址线被使能，了解实模式和保护模式的同学肯定清楚，如果A20地址线被激活，那么系统工作在保护模式下。但是在之后的boot loader程序中，计算机首先要工作在实模式下啊。所以这里的这个操作，根据网上 http://kernelx.weebly.com/a20-address-line.html 所说应该是去测试可用内存空间。在boot loader之前，它肯定还会转换回实模式。

- `[f000:cfb1]    0xfcfb1:	lidtl  %cs:(%esi)`

  - 加载中断向量表寄存器(IDTR)。
  - 这个指令会把从地址%cs:(%esi)起始的后面6个字节的数据读入到中断向量表寄存器(IDTR)中。中断是操作系统中非常重要的一部分，有了中断操作系统才能真正实现进程。每一种中断都有自己对应的中断处理程序，那么这个中断的处理程序的首地址就叫做这个中断的中断向量。中断向量表自然是存放所有中断向量的表了。关于中断向量表的介绍，大家可以戳这个链接 http://wiki.osdev.org/Interrupt_Descriptor_Table 。王爽汇编P236

- `[f000:cfb7]    0xfcfb7:	lgdtl  %cs:(%esi)`

  - 把从%cs:(%esi)为起始地址处的6个字节的值加载到全局描述符表格寄存器中GDTR中。这个表实现保护模式非常重要的一部分，我们在介绍boot loader时会具体介绍它。

- ```assembly
  [f000:cfbd]    0xfcfbd:	mov    %cr0,%ecx
  [f000:cfc0]    0xfcfc0:	and    $0xffff,%cx
  [f000:cfc7]    0xfcfc7:	or     $0x1,%cx
  [f000:cfcb]    0xfcfcb:	mov    %ecx,%cr0
  ```

  - 计算机中包含CR0~CR3四个控制寄存器，用来控制和确定处理器的操作模式。其中这三个语句的操作明显是要把CR0寄存器的最低位(0bit)置1。CR0寄存器的0bit是PE位，启动保护位，当该位被置1，代表开启了保护模式。但是这里出现了问题，我们刚刚说过BIOS是工作在实模式之下，后面的boot loader开始的时候也是工作在实模式下，所以这里把它切换为保护模式，显然是自相矛盾。所以只能推测它在检测是否机器能工作在保护模式下。

- ```assembly
  [f000:cfce]    0xfcfce:	ljmpw  $0xf,$0xcfd6
  The target architecture is assumed to be i386
  => 0xfcfd6:	mov    $0x10,%ecx
  => 0xfcfdb:	mov    %ecx,%ds
  => 0xfcfdd:	mov    %ecx,%es
  => 0xfcfdf:	mov    %ecx,%ss
  => 0xfcfe1:	mov    %ecx,%fs
  => 0xfcfe3:	mov    %ecx,%gs
  ```

  - 这里的几步之所以这么做是按照规定来的，https://en.wikibooks.org/wiki/X86_Assembly/Global_Descriptor_Table链接中指出，如果刚刚加载完GDTR寄存器我们必须要重新加载所有的段寄存器的值，而其中CS段寄存器必须通过长跳转指令，即23号指令来进行加载。所以这些步骤是在第19步完成后必须要做的。这样才能是GDTR的值生效。

- 我们可以看到BIOS的操作就是在控制，初始化，检测各种底层的设备，比如时钟，GDTR寄存器。以及设置中断向量表。
- 作为PC启动后运行的第一段程序，**它最重要的功能是把操作系统从磁盘中导入内存，然后再把控制权转交给操作系统。**所以BIOS在运行的最后会去检测可以从当前系统的哪个设备中找到操作系统，通常来说是我们的磁盘。也有可能是U盘等等。当BIOS确定了，操作系统位于磁 盘中，那么它就会把这个磁盘的第一个扇区，通常把它叫做启动区（boot sector）先加载到内存中，这个启动区中包括一个非常重要的程序--**boot loader，**它会负责完成整个操作系统从磁盘导入内存的工作，以及一些其他的非常重要的配置工作。最后操作系统才会开始运行。
- PC启动后的运行顺序为 BIOS --> boot loader --> 操作系统内核



## Part 2: The Boot Loader

### 启动扇区

对于PC来说，软盘，硬盘都可以被划分为一个个大小为512字节的区域，叫做扇区。一个扇区是一次磁盘操作的最小粒度。每一次读取或者写入操作都必须是一个或多个扇区。如果一个磁盘是可以被用来启动操作系统的，就把这个磁盘的第一个扇区叫做启动扇区。这一部分介绍的boot loader程序就位于这个启动扇区之中。当BIOS找到一个可以启动的软盘或硬盘后，它就会把这512字节的启动扇区加载到内存地址`0x7c00~0x7dff`这个区域内。

对于6.828，我们将采用传统的硬盘启动机制，这就意味着我们的<font color = af0000>boot loader程序的大小必须小于512字节</font>。整个boot loader是由一个汇编文件，boot/boot.S，以及一个C语言文件，boot/main.c。Boot loader必须完成两个主要的功能。

1. 首先，boot loader要把处理器从实模式转换为32bit的保护模式，因为只有在这种模式下软件可以访问超过1MB空间的内容。

2. 然后，boot loader可以通过使用x86的特定的IO指令，直接访问IDE磁盘设备寄存器，从磁盘中读取内核。

对于boot loader来说，有一个文件很重要，obj/boot/boot.asm。这个文件是我们真实运行的boot loader程序的反汇编版本。所以我们可以把它和它的源代码即boot.S以及main.c比较一下。



### 查看/boot/boot.S和/boot/main.c文件

当BIOS运行完成之后，CPU的控制权就会转移到boot.S文件上

```assembly
# Start the CPU: switch to 32-bit protected mode, jump into C.
# The BIOS loads this code from the first sector of the hard disk into
# memory at physical address 0x7c00 and starts executing in real mode
# with %cs=0 %ip=7c00.
```

- boot.S

  ```assembly
  .globl start
  start:
    .code16                     # Assemble for 16-bit mode
    cli                         # Disable interrupts
    cld                         # String operations increment
  ```

  - 这几条指令就是boot.S最开始的几句，其中cli是boot.S，也是boot loader的第一条指令。这条指令用于把所有的中断都关闭。因为在BIOS运行期间有可能打开了中断。此时CPU工作在实模式下。

  ```assembly
    # Set up the important data segment registers (DS, ES, SS).
    xorw    %ax,%ax             # Segment number zero
    movw    %ax,%ds             # -> Data Segment
    movw    %ax,%es             # -> Extra Segment
    movw    %ax,%ss             # -> Stack Segment
  ```
  - 这几条命令主要是在把三个段寄存器，ds，es，ss全部清零，因为经历了BIOS，操作系统不能保证这三个寄存器中存放的是什么数。所以这也是为后面进入保护模式做准备。

  ```assembly
    # Enable A20:
    #   For backwards compatibility with the earliest PCs, physical
    #   address line 20 is tied low, so that addresses higher than
    #   1MB wrap around to zero by default.  This code undoes this.
  seta20.1:
    inb     $0x64,%al               # Wait for not busy
    testb   $0x2,%al
    jnz     seta20.1
  
    movb    $0xd1,%al               # 0xd1 -> port 0x64
    outb    %al,$0x64
  
  seta20.2:
    inb     $0x64,%al               # Wait for not busy
    testb   $0x2,%al
    jnz     seta20.2
  
    movb    $0xdf,%al               # 0xdf -> port 0x60
    outb    %al,$0x60
  ```

  - 这部分指令就是在准备把CPU的工作模式从实模式转换为保护模式。我们可以看到其中的指令包括inb，outb这样的IO端口命令。所以这些指令都是在对外部设备进行操作。根据下面的链接：http://bochs.sourceforge.net/techspec/PORTS.LST我们可以看到

  - ```assembly
    0064	r	KB controller read status (MCA)
    		 bit 7 = 1 parity error on transmission from keyboard
    		 bit 6 = 1 general timeout
    		 bit 5 = 1 mouse output buffer full
    		 bit 4 = 0 keyboard inhibit
    		 bit 3 = 1 data in input register is command
    			 0 data in input register is data
    		 bit 2	 system flag status: 0=power up or reset  1=selftest OK
    		 bit 1 = 1 input buffer full (input 60/64 has data for 804x)
    		 bit 0 = 1 output buffer full (output 60 has data for system)
    ```

  - 所以16~18号指令是在不断的检测bit1。bit1的值代表输入缓冲区是否满了，也就是说CPU传送给控制器的数据，控制器是否已经取走了，如果CPU想向控制器传送新的数据的话，必须先保证这一位为0。所以这三条指令会一直等待这一位变为0，才能继续向后运行。

    当0x64端口准备好读入数据后，现在就可以写入数据了，所以19~20这两条指令是把0xd1这条数据写入到0x64端口中。当向0x64端口写入数据时，则代表向键盘控制器804x发送指令。这个指令将会被送给0x60端口。

    ```
     D1	dbl   write output port. next byte written  to 0060
            will be written to the 804x output port; the
            original IBM AT and many compatibles use bit 1 of
            the output port to control the A20 gate.
    ```

  - D1指令代表下一次写入0x60端口的数据将被写入给804x控制器的输出端口。可以理解为下一个写入0x60端口的数据是一个控制指令。

  - 14-16又开始再次等待，等待刚刚写入的指令D1，是否已经被读取了。

  - 如果指令被读取了，18-19行指令会向控制器输入新的指令，0xdf。通过查询我们看到0xDF指令的含义`DF	sngl  enable address line A20 (HP Vectra only???)`，这个指令的含义可以看到，使能A20线，代表可以进入保护模式了。

  ```assembly
    # Switch from real to protected mode, using a bootstrap GDT
    # and segment translation that makes virtual addresses 
    # identical to their physical addresses, so that the 
    # effective memory map does not change during the switch.
    lgdt    gdtdesc
    movl    %cr0, %eax
    orl     $CR0_PE_ON, %eax
    movl    %eax, %cr0
  ```

  - 首先31号指令 lgdt gdtdesc，是把gdtdesc这个标识符的值送入全局映射描述符表寄存器GDTR中。这个GDT表是处理器工作于保护模式下一个非常重要的表。具体可以参照关于实模式和保护模式的介绍。至于这条指令的功能就是把关于GDT表的一些重要信息存放到CPU的GDTR寄存器中，其中包括GDT表的内存起始地址，以及GDT表的长度。这个寄存器由48位组成，其中低16位表示该表长度，高32位表该表在内存中的起始地址。所以gdtdesc是一个标识符，标识着一个内存地址。从这个内存地址开始之后的6个字节中存放着GDT表的长度和起始地址。我们可以在这个文件的末尾看到gdtdesc，如下：

    ```assembly
    # Bootstrap GDT
    .p2align 2                                # force 4 byte alignment
    gdt:
      SEG_NULL				# null seg
      SEG(STA_X|STA_R, 0x0, 0xffffffff)	# code seg
      SEG(STA_W, 0x0, 0xffffffff)	        # data seg
    
    gdtdesc:
      .word   0x17                            # sizeof(gdt) - 1
      .long   gdt                             # address gdt
    ```

  - 其中第3行的gdt是一个标识符，标识从这里开始就是GDT表了。可见这个GDT表中包括三个表项(4,5,6行)，分别代表三个段，null seg，code seg，data seg。由于xv6其实并没有使用分段机制，也就是说数据和代码都是写在一起的，所以数据段和代码段的起始地址都是0x0，大小都是0xffffffff=4GB。

    在第4~6行是调用SEG()子程序来构造GDT表项的。这个子函数定义在mmu.h中，形式如下:

    ```c
    　#define SEG(type,base,lim)                    \
            .word (((lim) >> 12) & 0xffff), ((base) & 0xffff);    \
            .byte (((base) >> 16) & 0xff), (0x90 | (type)),        \
            (0xC0 | (((lim) >> 28) & 0xf)), (((base) >> 24) & 0xff)
    ```

    可见函数需要3个参数，一是type即这个段的访问权限，二是base，这个段的起始地址，三是lim，即这个段的大小界限。gdt表中的每一个表项的结构如图所示：

    <img src="/Users/asmqsaccount/Library/Application Support/typora-user-images/image-20200509200804212.png" alt="image-20200509200804212" style="zoom:50%;" />

    每个表项一共8字节，其中limit_low就是limit的低16位。base_low就是base的低16位，依次类推，所以我们就可以理解SEG函数为什么要那么写（其实还是有很多不理解的。。）。

    然后在gdtdesc处就要存放这个GDT表的信息了，其中0x17是这个表的大小-1 = 0x17 = 23，至于为什么不直接存表的大小24，根据查询是官方规定的。紧接着就是这个表的起始地址gdt。

    再回到刚才那里，当加载完GDT表的信息到GDTR寄存器之后。紧跟着3个操作，6-8指令。 这几步操作明显是在修改CR0寄存器的内容。CR0寄存器还有CR1~CR3寄存器都是80x86的控制寄存器。其中$CR0_PE的值定义于"mmu.h"文件中，为0x00000001。可见上面的操作是把CR0寄存器的bit0置1，CR0寄存器的bit0是保护模式启动位，把这一位值1代表保护模式启动。

  - `ljmp    $PROT_MODE_CSEG, $protcseg`，这只是一个简单的跳转指令，这条指令的目的在于把当前的运行模式切换成32位地址模式

  - ```assembly
      .code32                     # Assemble for 32-bit mode
    protcseg:
      # Set up the protected-mode data segment registers
      movw    $PROT_MODE_DSEG, %ax    # Our data segment selector
      movw    %ax, %ds                # -> DS: Data Segment
      movw    %ax, %es                # -> ES: Extra Segment
      movw    %ax, %fs                # -> FS
      movw    %ax, %gs                # -> GS
      movw    %ax, %ss                # -> SS: Stack Segment
    ```

    修改这些寄存器的值。这些寄存器都是段寄存器。大家可以戳这个链接看一下具体介绍 http://www.eecg.toronto.edu/~amza/[www.mindsec.com/files/x86regs.html](http://www.mindsec.com/files/x86regs.html)

    这里的4-9步之所以这么做是按照规定来的，https://en.wikibooks.org/wiki/X86_Assembly/Global_Descriptor_Table链接中指出，如果刚刚加载完GDTR寄存器我们必须要重新加载所有的段寄存器的值，而其中CS段寄存器必须通过长跳转指令，即23号指令来进行加载。所以这些步骤是在第19步完成后必须要做的。这样才能是GDTR的值生效。

  - ```assembly
      # Set up the stack pointer and call into C.
      movl    $start, %esp
      call bootmain
    ```

  

  

### main.c

- `readseg`

  ```c
   // Read 'count' bytes at 'offset' from kernel into physical address 'pa'.
  // Might copy more than asked
  void
  readseg(uint32_t pa, uint32_t count, uint32_t offset)
  ```

  - 它的功能从注释上来理解应该是，把距离内核起始地址offset个偏移量存储单元作为起始，将它和它之后的count字节的数据读出送入以pa为起始地址的内存物理地址处。



- ```c
  	// read 1st page off disk
  	readseg((uint32_t) ELFHDR, SECTSIZE*8, 0);
  ```

  - 这条指令是把内核的第一个页(4MB = 4096 = SECTSIZE*8 = 512*8)的内容读取的内存地址ELFHDR(0x10000)处。其实完成这些后相当于把操作系统映像文件的elf头部读取出来放入内存中。



- ```c
  	// is this a valid ELF?
  	if (ELFHDR->e_magic != ELF_MAGIC)
  		goto bad;
  ```

  - 读取完这个内核的elf头部信息后，需要对这个elf头部信息进行验证，并且也需要通过它获取一些重要信息。所以有必要了解下elf头部。

  - 注： http://wiki.osdev.org/ELF

  - elf文件：elf(executable and linkable format)是一种文件格式，主要被用来把程序存放到磁盘上。是在程序被编译和链接后被创建出来的。一个elf文件包括多个段。对于一个可执行程序，通常包含存放代码的文本段(text section)，存放全局变量的data段，存放字符串常量的rodata段。elf文件的头部就是用来描述这个elf文件如何在存储器中存储。

    需要注意的是，你的文件是可链接文件还是可执行文件，会有不同的elf头部格式。

  -  elf头部信息的magic字段是整个头部信息的开端。并且如果这个文件是格式是ELF格式的话，文件的elf->magic域应该是=ELF_MAGIC的，所以这条语句就是判断这个输入文件是否是合法的elf可执行文件。

- ```c
  // load each program segment (ignores ph flags)
  ph = (struct Proghdr *) ((uint8_t *) ELFHDR + ELFHDR->e_phoff);
  eph = ph + ELFHDR->e_phnum;
  for (; ph < eph; ph++)
    // p_pa is the load address of this segment (as well
    // as the physical address)
    readseg(ph->p_pa, ph->p_memsz, ph->p_offset);
  ```

  - 我们知道头部中一定包含Program Header Table。这个表格存放着程序中所有段的信息。通过这个表我们才能找到要执行的代码段，数据段等等。所以我们要先获得这个表。

  - 这条指令就可以完成这一点，首先elf是表头起址，而phoff字段代表Program Header Table距离表头的偏移量。所以ph可以被指定为Program Header Table表头。

  - 3: 由于phnum中存放的是Program Header Table表中表项的个数，即段的个数。所以这步操作是吧eph指向该表末尾。

  - 4-7: 　 这个for循环就是在加载所有的段到内存中。

    - ph->paddr根据参考文献中的说法指的是这个段在内存中的物理地址。
    - ph->off字段指的是这一段的开头相对于这个elf文件的开头的偏移量。
    - ph->filesz字段指的是这个段在elf文件中的大小。
    - ph->memsz则指的是这个段被实际装入内存后的大小。通常来说memsz一定大于等于filesz，因为段在文件中时许多未定义的变量并没有分配空间给它们。

    所以这个循环就是在把操作系统内核的各个段从外存读入内存中。

- ```c
  	// call the entry point from the ELF header
  	// note: does not return!
  	((void (*)(void)) (ELFHDR->e_entry))();
  ```

  - e_entry字段指向的是这个文件的执行入口地址。所以这里相当于开始运行这个文件。也就是内核文件。 自此就把控制权从boot loader转交给了操作系统的内核。

### Exercise 3:

设置一个断点在地址0x7c00处，这是boot sector被加载的位置。然后让程序继续运行直到这个断点。跟踪/boot/boot.S文件的每一条指令，同时使用boot.S文件和系统为你反汇编出来的文件obj/boot/boot.asm。你也可以使用GDB的x/i指令来获取去任意一个机器指令的反汇编指令，把源文件boot.S文件和boot.asm文件以及在GDB反汇编出来的指令进行比较。

追踪到bootmain函数中，而且还要具体追踪到readsect()子函数里面。找出和readsect()c语言程序的每一条语句所对应的汇编指令，回到bootmain()，然后找出把内核文件从磁盘读取到内存的那个for循环所对应的汇编语句。找出当循环结束后会执行哪条语句，在那里设置断点，继续运行到断点，然后运行完所有的剩下的语句。







## Reference

1. [MIT 6.828 JOS学习笔记1. Lab 1 Part 1: PC Bootstrap](https://www.cnblogs.com/fatsheep9146/p/5068353.html)
2. [《MIT 6.828 Lab1: Booting a PC》实验报告](https://www.cnblogs.com/wuhualong/p/mit_6-828_lab1.html)
3. [MIT 操作系统实验 MIT JOS lab1](https://blog.csdn.net/cinmyheart/article/details/39754269)
4. http://leenjewel.github.io/blog/2014/07/29/[(xue-xi-xv6)]-cong-shi-mo-shi-dao-bao-hu-mo-shi/