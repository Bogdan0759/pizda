/* hardware.h - Low-level hardware I/O functions header
 * 
 * This header provides declarations for assembly-implemented
 * hardware I/O functions (inb/outb).
 * 
 * Implementation: hardware.asm (NASM)
 */

#ifndef HARDWARE_H
#define HARDWARE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Read a byte from an I/O port
 * 
 * @param port - 16-bit port number
 * @return byte read from the port
 */
unsigned char inb(unsigned short port);

/* Write a byte to an I/O port
 * 
 * @param port - 16-bit port number
 * @param data - byte to write
 */
void outb(unsigned short port, unsigned char data);

#ifdef __cplusplus
}
#endif

#endif /* HARDWARE_H */
