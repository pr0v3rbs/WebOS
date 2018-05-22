#ifndef _ASMARM_GPUINFO_H
#define _ASMARM_GPUINFO_H

struct gpu_info_driver {
	int	(*getTotal)(void);		/* return total pages count*/
	int	(*getCached)(void);		/* return internally cached pages count */
	int	(*getReclaimable)(void);	/* return reclaimable pages count that can be shrinked by shrink_slab */
};

int gpu_info_register(struct gpu_info_driver *driver);
int gpu_info_unregister(struct gpu_info_driver *driver);

#endif
