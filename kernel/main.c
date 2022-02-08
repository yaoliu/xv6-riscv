#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

volatile static int started = 0;

// start() jumps here in supervisor mode on all CPUs.
void
main()
{
  if(cpuid() == 0){
    consoleinit();
    printfinit();
    printf("\n");
    printf("xv6 kernel is booting\n");
    printf("\n");
    // 分配页
    kinit();         // physical page allocator
    // 创建内核页表 和把对应IO设备 内核代码等映射到页表中
    kvminit();       // create kernel page table
    // 开启虚拟内存 把页表地址映射到satp寄存器中 开启MMU
    kvminithart();   // turn on paging
    procinit();      // process table
    // 陷入相关处理设置
    trapinit();      // trap vectors
    trapinithart();  // install kernel trap vector
    // 外部设备中断设置
    plicinit();      // set up interrupt controller
    plicinithart();  // ask PLIC for device interrupts
    // 块缓存
    binit();         // buffer cache
    iinit();         // inode table
    fileinit();      // file table
    virtio_disk_init(); // emulated hard disk
    userinit();      // first user process
    __sync_synchronize();
    started = 1;
  } else {
    while(started == 0)
      ;
    __sync_synchronize();
    printf("hart %d starting\n", cpuid());
    kvminithart();    // turn on paging
    trapinithart();   // install kernel trap vector
    plicinithart();   // ask PLIC for device interrupts
  }

  scheduler();        
}
