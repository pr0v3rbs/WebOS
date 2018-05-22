
#undef TRACE_SYSTEM
#define TRACE_SYSTEM	dmflash

#if !defined(_TRACE_DMFLASH_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_DMFLASH_H

#include <linux/kernel.h>
#include <linux/bio.h>
#include <linux/blktrace_api.h>
#include <linux/tracepoint.h>

#define RWBS_LEN	8

DECLARE_EVENT_CLASS(dmflash_bio,

	TP_PROTO(struct bio *bio),

	TP_ARGS(bio),

	TP_STRUCT__entry(
		__field(dev_t,		dev			)
		__field(sector_t,	sector			)
		__field(unsigned int,	byte_size		)
		__array(char,		rwbs,	RWBS_LEN	)
	),

	TP_fast_assign(
		__entry->dev		= bio->bi_bdev->bd_dev;
		__entry->sector		= bio->bi_iter.bi_sector;
		__entry->byte_size	= bio->bi_iter.bi_size;
		blk_fill_rwbs(__entry->rwbs, bio->bi_rw, bio->bi_iter.bi_size);
	),

	TP_printk("%d,%d %s %llu + %u",
		MAJOR(__entry->dev), MINOR(__entry->dev),
		__entry->rwbs,
		(unsigned long long)__entry->sector,
		DIV_ROUND_UP(__entry->byte_size, 512))
);

DEFINE_EVENT(dmflash_bio, dmflash_flash_map_enter,

	TP_PROTO(struct bio *bio),

	TP_ARGS(bio)
);

DEFINE_EVENT(dmflash_bio, dmflash_digest_enter,

	TP_PROTO(struct bio *bio),

	TP_ARGS(bio)
);

DEFINE_EVENT(dmflash_bio, dmflash_remap_write_enter_hme_insert,

	TP_PROTO(struct bio *bio),

	TP_ARGS(bio)
);

DEFINE_EVENT(dmflash_bio, dmflash_hme_insert_exit,

	TP_PROTO(struct bio *bio),

	TP_ARGS(bio)
);

DEFINE_EVENT(dmflash_bio, dmflash_comp_enter,

	TP_PROTO(struct bio *bio),

	TP_ARGS(bio)
);

DEFINE_EVENT(dmflash_bio, dmflash_link_lbn_enter,

	TP_PROTO(struct bio *bio),

	TP_ARGS(bio)
);

/*
 * Logic of the parsing tool has dependency on the following tracepoint.
 * Modification to the tracepoint requires an corresponding update to the
 * parsing tool.
 */
DEFINE_EVENT(dmflash_bio, dmflash_wb_enter,

	TP_PROTO(struct bio *bio),

	TP_ARGS(bio)
);

/*
 * Logic of the parsing tool has dependency on the following tracepoint.
 * Modification to the tracepoint requires an corresponding update to the
 * parsing tool.
 */
DEFINE_EVENT(dmflash_bio, dmflash_write_make_request,

	TP_PROTO(struct bio *bio),

	TP_ARGS(bio)
);

/*
 * Logic of the parsing tool has dependency on the following tracepoint.
 * Modification to the tracepoint requires an corresponding update to the
 * parsing tool.
 */
DEFINE_EVENT(dmflash_bio, dmflash_comp_end_bio,

	TP_PROTO(struct bio *bio),

	TP_ARGS(bio)
);

DEFINE_EVENT(dmflash_bio, dmflash_remap_read_enter,

	TP_PROTO(struct bio *bio),

	TP_ARGS(bio)
);

/*
 * Logic of the parsing tool has dependency on the following tracepoint.
 * Modification to the tracepoint requires an corresponding update to the
 * parsing tool.
 */
DEFINE_EVENT(dmflash_bio, dmflash_read_make_request,

	TP_PROTO(struct bio *bio),

	TP_ARGS(bio)
);

DEFINE_EVENT(dmflash_bio, dmflash_decomp_enter,

	TP_PROTO(struct bio *bio),

	TP_ARGS(bio)
);

/* FIXME: HASH_LEN is defined temporarily for sha256 only */
#define HASH_LEN	32

DECLARE_EVENT_CLASS(dmflash_bio_hash_digest_exit,

	TP_PROTO(struct bio *bio, unsigned char *digest, unsigned int hashlen),

	TP_ARGS(bio, digest, hashlen),

	TP_STRUCT__entry(
		__field(dev_t,		dev			)
		__field(sector_t,	sector			)
		__array(unsigned char,	hashval, HASH_LEN	)
		__field(unsigned int,	hashlen			)
		__array(char,		rwbs,	RWBS_LEN	)
	),

	TP_fast_assign(
		__entry->dev		= bio->bi_bdev->bd_dev;
		__entry->sector		= bio->bi_iter.bi_sector;
		memcpy(__entry->hashval, digest, hashlen);
		__entry->hashlen	= hashlen;
		blk_fill_rwbs(__entry->rwbs, bio->bi_rw, bio->bi_iter.bi_size);
	),

	TP_printk("%d,%d %s %llu: %s",
		MAJOR(__entry->dev), MINOR(__entry->dev),
		__entry->rwbs, (u64)__entry->sector,
		__print_hex(__entry->hashval, __entry->hashlen))
);

DEFINE_EVENT(dmflash_bio_hash_digest_exit, dmflash_digest_exit,

	TP_PROTO(struct bio *bio, unsigned char *digest, unsigned int hashlen),

	TP_ARGS(bio, digest, hashlen)
);

DECLARE_EVENT_CLASS(dmflash_bio_compress_exit,

	TP_PROTO(struct bio *bio, unsigned int new_byte_size),

	TP_ARGS(bio, new_byte_size),

	TP_STRUCT__entry(
		__field(dev_t,		dev			)
		__field(sector_t,	sector			)
		__field(unsigned int,	old_byte_size		)
		__field(unsigned int,	new_byte_size		)
		__array(char,		rwbs,	RWBS_LEN	)
	),

	TP_fast_assign(
		__entry->dev		= bio->bi_bdev->bd_dev;
		__entry->sector		= bio->bi_iter.bi_sector;
		__entry->old_byte_size	= bio->bi_iter.bi_size;
		__entry->new_byte_size	= new_byte_size;
		blk_fill_rwbs(__entry->rwbs, bio->bi_rw, bio->bi_iter.bi_size);
	),

	TP_printk("%d,%d %s %llu + %u(%u) <- %llu + %u",
		MAJOR(__entry->dev), MINOR(__entry->dev),
		__entry->rwbs,
		(unsigned long long)__entry->sector,
		DIV_ROUND_UP(__entry->new_byte_size, 512),
		__entry->new_byte_size,
		(unsigned long long)__entry->sector,
		DIV_ROUND_UP(__entry->old_byte_size, 512))
);


DEFINE_EVENT(dmflash_bio_compress_exit, dmflash_comp_exit,

	TP_PROTO(struct bio *bio, unsigned int new_byte_size),

	TP_ARGS(bio, new_byte_size)
);

DEFINE_EVENT(dmflash_bio_compress_exit, dmflash_decomp_exit,

	TP_PROTO(struct bio *bio, unsigned int new_byte_size),

	TP_ARGS(bio, new_byte_size)
);

DECLARE_EVENT_CLASS(dmflash_bio_remap,

	TP_PROTO(struct bio *bio, unsigned int map_status,
		sector_t old_sector, unsigned int old_nr_sector,
		sector_t new_sector, unsigned int new_nr_sector),

	TP_ARGS(bio, map_status, old_sector, old_nr_sector,
		new_sector, new_nr_sector),

	TP_STRUCT__entry(
		__field(dev_t,		dev			)
		__field(sector_t,	old_sector		)
		__field(unsigned int,	old_nr_sector		)
		__field(sector_t,	new_sector		)
		__field(unsigned int,	new_nr_sector		)
		__field(unsigned int,	map_status		)
		__array(char,		rwbs,	RWBS_LEN	)
	),

	TP_fast_assign(
		__entry->dev		= bio->bi_bdev->bd_dev;
		__entry->old_sector	= old_sector,
		__entry->old_nr_sector	= old_nr_sector,
		__entry->new_sector	= new_sector;
		__entry->new_nr_sector	= new_nr_sector,
		__entry->map_status	= map_status;
		blk_fill_rwbs(__entry->rwbs, bio->bi_rw, bio->bi_iter.bi_size);
	),

	TP_printk("%d,%d %s %u %llu + %u <- %llu + %u",
		MAJOR(__entry->dev), MINOR(__entry->dev),
		__entry->rwbs, __entry->map_status,
		(unsigned long long)__entry->new_sector,
		__entry->new_nr_sector,
		(unsigned long long)__entry->old_sector,
		__entry->old_nr_sector)
);

/*
 * For the tool to be able to parse tracepoints based on their physical sector
 * numbers(in case of no dedup hit), the following dmflash_remap_write_exit
 * tracepoint which is showing logical to physical sector number remapping
 * information is required.
 */
DEFINE_EVENT(dmflash_bio_remap, dmflash_remap_write_exit,

	TP_PROTO(struct bio *bio, unsigned int map_status,
		sector_t old_sector, unsigned int old_nr_sector,
		sector_t new_sector, unsigned int new_nr_sector),

	TP_ARGS(bio, map_status, old_sector, old_nr_sector,
		new_sector, new_nr_sector)
);

/*
 * For the tool to be able to parse tracepoints based on their physical sector
 * numbers(in case of submitting io for read), the following
 * dmflash_remap_read_exit tracepoint which is showing logical to physical
 * sector number remapping information is required.
 */
DEFINE_EVENT(dmflash_bio_remap, dmflash_remap_read_exit,

	TP_PROTO(struct bio *bio, unsigned int map_status,
		sector_t old_sector, unsigned int old_nr_sector,
		sector_t new_sector, unsigned int new_nr_sector),

	TP_ARGS(bio, map_status, old_sector, old_nr_sector,
		new_sector, new_nr_sector)
);

DECLARE_EVENT_CLASS(dmflash_bio_plugging,

	TP_PROTO(unsigned int req_type, unsigned int nr_sector),

	TP_ARGS(req_type, nr_sector),

	TP_STRUCT__entry(
		__field(unsigned int,	req_type		)
		__field(unsigned int,	nr_sector		)
	),

	TP_fast_assign(
		__entry->req_type	= req_type;
		__entry->nr_sector	= nr_sector;
	),

	TP_printk("%u %u", __entry->req_type, __entry->nr_sector)
);

DEFINE_EVENT(dmflash_bio_plugging, dmflash_plug,

	TP_PROTO(unsigned int req_type, unsigned int nr_pending_sector),

	TP_ARGS(req_type, nr_pending_sector)
);

DEFINE_EVENT(dmflash_bio_plugging, dmflash_unplug,

	TP_PROTO(unsigned int req_type, unsigned int nr_sector),

	TP_ARGS(req_type, nr_sector)
);

#endif

#include <trace/define_trace.h>
