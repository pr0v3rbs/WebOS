#ifndef _LINUX_LGEMMC_H
#define _LINUX_LGEMMC_H

#ifdef CONFIG_LGEMMC_PARTITION
extern int lgemmc_get_partnum(const char *name);
extern char *lgemmc_get_partname(int partnum);
extern unsigned int lgemmc_get_partsize(int partnum);
extern unsigned int lgemmc_get_filesize(int partnum);
extern unsigned int lgemmc_get_sw_version(int partnum);
extern unsigned long long lgemmc_get_partoffset(int partnum);
#else
inline static int lgemmc_get_partnum(const char *name)
{
	return -ENODEV;
};
inline static char *lgemmc_get_partname(int partnum)
{
	return NULL;
};
inline static unsigned int lgemmc_get_partsize(int partnum)
{
	return 0;
};
inline static unsigned int lgemmc_get_filesize(int partnum)
{
	return 0;
};
inline static unsigned int lgemmc_get_sw_version(int partnum)
{
	return 0;
};
inline unsigned long long lgemmc_get_partoffset(int partnum)
{
	return 0;
};
#endif

#endif
