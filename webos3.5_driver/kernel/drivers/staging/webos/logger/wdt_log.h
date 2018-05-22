#ifndef _LINUX_WDT_LOG_H
#define _LINUX_WDT_LOG_H

extern void wdt_log_save(void) __attribute__((noreturn));
extern bool wdt_log_ready(void);

#endif
