#ifndef _D14_API_H
#define _D14_API_H

int reset_mcu (void);
int set_firmware (unsigned char *data, int size);
int check_available (void);

int mem_read (unsigned int addr, __user unsigned char *buf, int len);
int mem_write (unsigned int addr, __user unsigned char *buf, int len);
int reg_write (unsigned int addr, unsigned int reg);
int reg_read (int reg, unsigned int *data);
int ipc_read (int reg, unsigned int *data);
int _ipc_write (int reg, unsigned int data);
int ipc_write (int reg, unsigned int data);

#endif
