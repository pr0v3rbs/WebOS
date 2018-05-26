
Change List by LG (raxis.lim@lge.com 2010/03/03)
------------------------------------------------

1. Add #ifdef __KERNEL__ #endif to tlsf.h and tlsf.c

2. change USE_MMAP  to TLSF_USE_MMAP
   change USE_SBRK  to TLSF_USE_SBRK
   change set_bit   to tlsf_set_bit
   change clear_bit to tlsf_clear_bit
   change ls_bit    to tlsf_ls_bit
   change ms_bit    to tlsf_ms_bit

warning 
-------
Do not use Makefile in src nor example directroy
src/tlsf.c is build as a sub object of base module of kdriver.
Refer to Makefile in base module.
