#ifndef _HMA_DRV_H
#define _HMA_DRV_H

#include <linux/ioctl.h>

#include "hma_alloc.h"

/**
 * HMA_IOCTL_ALLOC ioctl system call 에 사용되는 구조체이다. 할당하고자 하는
 * 메모리 pool의 이름, 메모리 크기, 메모리 시작주소의 alignment를 나타낸다.
 */
struct hma_ioctl_alloc
{
	char pool[HMA_POOL_LEN];/**< 메모리를 할당받을 때 사용할 memory pool 이름 */
	int size;		/**< 할당 받을 메모리 크기 */
	int align;		/**< 할당 받을 메모리 시작주소의 aligment */

	unsigned long paddr;	/**< 할당 받은 메모리의 시작주소, 물리 주소 */
};

/**
 * HMA_IOCTL_CACHE_CTL ioctl system call 에 사용되는 구조체이다. 
 * L1, L2 cache를 invalidate, clean 하기 위해서 필요한, 유저영역 가상주소, 물리 주소, 크기를 나타낸다
 * L1은 가상 주소를 사용하며, L2는 물리 주소를 사용한다. 
 * cache 의 용어가 혼용되기 때문에 hma에서 제공하는 cache operation을 정확히 설명한다.
 * FIXME
 * CACHE_INV(cache invalidate) : cache line의 속성이 valid, dirty인 것을 unvaild로 변경
 * CACHE_CLEAN(cache clean) : cache line의 속성이 dirty인 것을 main memory에 write하고 unvaild로 변경 
 * CACHE_FLUSH(cache flush) : x
 */
struct hma_ioctl_cache_ctl
{
	unsigned long	paddr;    //L2 cache control을 위한 물리 주소
	void		*vaddr;    //L1 cache control을 위한 가상 주소
	size_t		size;    //control을 위한 크기
	int		operation;  //contorl 종류, CACHE_INV(invalidate), CACHE_CLEAN(clean)
#define HMA_CACHE_INV       0
#define HMA_CACHE_CLEAN     1
};


#define HMA_IOCTL_TYPE	'h'

#define HMA_IOCTL_ALLOC			_IOW (HMA_IOCTL_TYPE,1, struct hma_ioctl_alloc)
#define HMA_IOCTL_CACHE_CTL		_IOW (HMA_IOCTL_TYPE,2, struct hma_ioctl_cache_ctl)
#define HMA_IOCTL_GET_UMP_SECURE_ID	_IOR (HMA_IOCTL_TYPE,3, unsigned int)
#define HMA_IOCTL_CACHED		_IOW (HMA_IOCTL_TYPE,4, int)

#endif
