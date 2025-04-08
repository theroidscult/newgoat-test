# The kernel: Newgoat
<img src="../../ass-ets/kernel-map.png">

### HAL: Hardware Abstraction Layer
The hardware abraction layer should be the only component that is hardware dependent; it should provide interrupt, timer and PCI functions and the architecture parts of the scheduler and syscall handler

### Object and memory manager
The object and memory manager has a few functions: 
* allocating physical pages
* allocating virtual pages for processes
* managing all kinds of kernel objects
* freeing all the initialisation code when the initialisation finishes

### Scheduler
The scheduler manages processes

### ADI and Drivers
The ADI system loads ADI drivers and provides metalanguages for said drivers.