
unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__ volatile("inb %w1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void outb(unsigned short port, unsigned char data) {
    __asm__ volatile("outb %0, %w1" : : "a"(data), "Nd"(port));
}
