
This simple tutorial is for the kernel driver developer.

-------------------------------------------------------------------------------
	HOW TO BUILD KDRIVER
-------------------------------------------------------------------------------


1. 	You may need to extract Linux kernel.
	go to ../../kernel and 'tar xvzf linux-2.6.28.tgz'

2. 	Constomize modules.mk
	You may need to change the option of INCLUDE_KDRV_XXXX.	

3. 	Go to 'build' directory and 'make'
	You may need to change the option of OUT_DIR or TFTP_DIR in Makefile.

-------------------------------------------------------------------------------
	HOW TO TEST KDRIVER
-------------------------------------------------------------------------------

1.	Go to z.ext/kdriver_test 

2.	Make your own test code

3.	Go to 'build' directory and 'make'	
