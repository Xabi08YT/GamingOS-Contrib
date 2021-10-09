void outb(unsigned short port, unsigned char val){
  asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
} 
void reboot(){

    outb(0x64, 0xfe);

    asm("hlt");
}