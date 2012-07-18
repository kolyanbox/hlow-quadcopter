#ifndef __SDCARD_H
#define __SDCARD_H

#include <lpc17xx_spi.h>

void sdcard_init(void);
void sdcard_write(uint8_t *writebuf, int location);
void sdcard_read(uint8_t *readbuf, int location);



#endif
