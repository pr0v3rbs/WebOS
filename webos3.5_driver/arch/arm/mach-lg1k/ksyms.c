#include <linux/module.h>
#include <linux/init.h>

/* Exporting a symbol from /init/main.c */
EXPORT_SYMBOL(saved_command_line);
/* Exporting a symbol from /arch/arm/mm/cache-v7.S */
EXPORT_SYMBOL(v7_dma_map_area);
EXPORT_SYMBOL(v7_dma_unmap_area);
EXPORT_SYMBOL(v7_dma_flush_range);
