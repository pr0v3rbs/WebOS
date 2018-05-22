#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#include <asm/setup.h>

extern struct tag_custom customTAG;

#define procfsdir "mmapger"
#define procfsbin "mmap.bin"
#define procfsini "mmap.ini"
#define procfsinfo "info"
#define procfsinfobin "info.bin"

static struct proc_dir_entry *root;
static struct proc_dir_entry *bin;
static struct proc_dir_entry *ini;
static struct proc_dir_entry *info;
static struct proc_dir_entry *infobin;

//MMAPGer[96][5];
#define MAX_MIU_SIZE (7)
#define MAX_MIU_DATA_SIZE (MAX_MIU_SIZE * sizeof(unsigned))
#ifdef MAX_MMAP_DATA_SIZE
#undef MAX_MMAP_DATA_SIZE
#endif
#define MAX_MMAP_DATA_SIZE (MAX_MMAP_SIZE * 5 * sizeof(unsigned))

char *MMAPGerTag[96] = {
    "DUMMY_MEM",
    "LINUX_MEM",
    "EMAC_MEM",
    "XIP_MEM",
    "NFDRM_NUTTX_MEM",
    "NFDRM_HW_AES_BUF",
    "HW_SECURE_BUFFER",
    "SECURE_SHM",
    "SECURE_UPDATE_AREA",
    "SECURE_TSP",
    "BIN_MEM",
    "MAD_BASE_BUFFER",
    "VD_3DCOMB",
    "G3D_TEXTURE0_BUF",
    "LINUX_LOW_MEM_RESERVE",
    "VE_FRAMEBUFFER",
    "TTX_BUF",
    "PVR_DOWNLOAD_BUFFER",
    "PVR_UPLOAD_BUFFER",
    "TSP_BUF",
    "VQ_BUF",
    "MAILBOX",
    "DIP",
    "MPOOL",
    "PNG_OUTPUT_BUF",
    "PNG_INPUT_BUF",
    "VDEC_GN_IAP",
    "GOP_FRAMEBUFFER",
    "GOP_REGDMA",
    "GFX_VQ_BUFFER",
    "VBI_1_BUFFER",
    "VBI_2_BUFFER",
    "LD_BUFFER",
    "PIU_BUF",
    "DUMMY",
    "DUAL_STREAM_DUMY",
    "VDEC_CPU",
    "SVD_CPU",
    "VDEC_BITSTREAM",
    "VDEC_SUB_BITSTREAM",
    "VDEC_N1_BITSTREAM",
    "VDEC_N2_BITSTREAM",
    "VDEC_FRAMEBUFFER",
    "VDEC_SUB_FRAMEBUFFER",
    "NFDRM_VDEC_SHARE_MEM",
    "JPD_THUMBNAIL",
    "POSD0",
    "POSD1",
    "AUDIO_CLIP",
    "PVR_THUMBNAIL_DECODE_BUFFER",
    "LD_BUF",
    "BB",
    "DEBUG",
    "BT_POOL",
    "APVR_BUF",
    "VIDEO_CONF",
    "G3D_CMD_Q_BUF",
    "G3D_VERTEX_BUF",
    "G3D_COLOR_BUF",
    "G3D_TEXTURE1_BUF",
    "GPD_ES_BUF",
    "GPD_OUTPUT_BUF",
    "ISDBT_TDI",
    "VE_DIPW_BUF",
    "MFE_BUF",
    "MFE_OUT_BUF",
    "VIDEO_IN_CAPTURE_TMP",
    "LX_MEM1",
    "DTMB_TDI",
    "SCALER_DNR_BUF",
    "SCALER_DNR_W_BARRIER",
    "SCALER_PIP_BUF",
    "SCALER_MAIN_FRCM_BUFFER",
    "SC_SUB_FRCM",
    "SCALER_MCDI_ME1_BUFFER",
    "SCALER_MCDI_ME2_BUFFER",
    "SC_OD",
    "SC_BUF",
    "SCALER_MLOAD_BUFFER",
    "SCALER_DYNAMIC_XC_BUFFER",
    "TWOTO3D_DEPTH_DETECT_BUF",
    "TWOTO3D_DEPTH_RENDER_BUF",
    "T3D_DMA_CLIENT9_BUF",
    "SC2_MAIN_FB",
    "VDEC_DUMMY",
    "JPD_FRAMEBUFFER",
    "VDEC_SW_DETILE_BUF",
    "JPG_OUTPUT_BUF",
    "HWI2C_DMA",
    0, 0, 0, 0, 0, 0, 0
}; //MAPPGerTag[]


static int info_dump(struct seq_file *m, void *v) {
    seq_printf(m, "0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X\n\n",
        customTAG.miu_dram_len,
        customTAG.miu_dram_len0,
        customTAG.miu_dram_len1,
        customTAG.miu_dram_len2,
        customTAG.enable_miu,
        customTAG.miu_interval,
        customTAG.miu2_interval);
    return 0;
}

static int ini_dump(struct seq_file *m, void *v) {
#ifdef CONFIG_MSTAR_PORTING_KERNEL_4_4_3
    /* unused variable */
#else
    int ret;
#endif /* CONFIG_MSTAR_PORTING_KERNEL_4_4_3 */
    u8 i = 0;
    unsigned int *info;
    char hex[11];

    seq_printf(m, "#define MIU0                    (0x000000)\n");
    seq_printf(m, "#define MIU1                    (0x000001)\n");
    seq_printf(m, "#define TYPE_NONE               (0x000000)\n");
    seq_printf(m, "#define UNCACHE                 (0x000002)\n");
    seq_printf(m, "#define REMAPPING_TO_USER_SPACE (0x000004)\n");
    seq_printf(m, "#define CACHE                   (0x000008)\n");
    seq_printf(m, "#define NONCACHE_BUFFERABLE     (0x000010)\n");
    seq_printf(m, "#define MIU2                    (0x000020)\n");
    seq_printf(m, "\n");

    for (i = 0; i < customTAG.mmap_count; i++)
    {
        if(MMAPGerTag[i] == 0)
            break;
        info = customTAG.mmapger[i];
        sprintf(hex, "0x%08X", info[0]);
#ifdef CONFIG_MSTAR_PORTING_KERNEL_4_4_3
        seq_printf(m, "#define %s_AVAILABLE", MMAPGerTag[i]);
#else
        ret = seq_printf(m, "#define %s_AVAILABLE", MMAPGerTag[i]);
#endif /* CONFIG_MSTAR_PORTING_KERNEL_4_4_3 */
        seq_printf(m, " %s\n", hex);

        sprintf(hex, "0x%08X", info[1]);
#ifdef CONFIG_MSTAR_PORTING_KERNEL_4_4_3
        seq_printf(m, "#define %s_ADR", MMAPGerTag[i]);
#else
        ret = seq_printf(m, "#define %s_ADR", MMAPGerTag[i]);
#endif /* CONFIG_MSTAR_PORTING_KERNEL_4_4_3 */
        seq_printf(m, " %s\n", hex);

        sprintf(hex, "0x%08X", info[2]);
#ifdef CONFIG_MSTAR_PORTING_KERNEL_4_4_3
        seq_printf(m, "#define %s_GAP_CHK", MMAPGerTag[i]);
#else
        ret = seq_printf(m, "#define %s_GAP_CHK", MMAPGerTag[i]);
#endif /* CONFIG_MSTAR_PORTING_KERNEL_4_4_3 */
        seq_printf(m, " %s\n", hex);

        sprintf(hex, "0x%08X", info[3]);
#ifdef CONFIG_MSTAR_PORTING_KERNEL_4_4_3
        seq_printf(m, "#define %s_LEN", MMAPGerTag[i]);
#else
        ret = seq_printf(m, "#define %s_LEN", MMAPGerTag[i]);
#endif /* CONFIG_MSTAR_PORTING_KERNEL_4_4_3 */
        seq_printf(m, " %s\n", hex);

        sprintf(hex, "0x%08X", info[4]);
#ifdef CONFIG_MSTAR_PORTING_KERNEL_4_4_3
        seq_printf(m, "#define %s_MEMORY_TYPE ", MMAPGerTag[i]);
#else
        ret = seq_printf(m, "#define %s_MEMORY_TYPE ", MMAPGerTag[i]);
#endif /* CONFIG_MSTAR_PORTING_KERNEL_4_4_3 */
        if(info[4]&0x00000020) {
            seq_printf(m, "%s", "(MIU2 | TYPE_NONE");
        } else {
            if(info[4]&0x00000001)
                seq_printf(m, "%s", "(MIU1 | TYPE_NONE");
            else
                seq_printf(m, "%s", "(MIU0 | TYPE_NONE");
        }

        if(info[4]&0x00000002) seq_printf(m, " | UNCACHE");
        if(info[4]&0x00000004) seq_printf(m, " | REMAPPING_TO_USER_SPACE");
        if(info[4]&0x00000008) seq_printf(m, " | CACHE");
        if(info[4]&0x00000010) seq_printf(m, " | NONCACHE_BUFFERABLE");
        seq_printf(m, ")");

        seq_printf(m, "\n\n");
    }
    return 0;
}

static int infobin_open(struct inode *in, struct file *f) {
    try_module_get(THIS_MODULE);
    return 0;
}

static ssize_t infobin_read(struct file *f, char __user *out, size_t s, loff_t *ppos) {
    size_t size;
    char *data;
#if 0   // use local var to insure array order or by setup.h's customTAG order
    static unsigned miu_info[MAX_MIU_SIZE];
    miu_info[0] = customTAG.miu_dram_len;
    miu_info[1] = customTAG.miu_dram_len0;
    miu_info[2] = customTAG.miu_dram_len1;
    miu_info[3] = customTAG.miu_dram_len2;
    miu_info[4] = customTAG.enable_miu;
    miu_info[5] = customTAG.miu_interval;
    miu_info[6] = customTAG.miu2_interval;*/
    if(f->f_pos >= MAX_MIU_DATA_SIZE) return 0;
    size = ((f->f_pos + s) > MAX_MIU_DATA_SIZE) ? (MAX_MIU_DATA_SIZE - f->f_pos) : s;
    data = (char *)(miu_info) + f->f_pos;
#else
    if(f->f_pos >= MAX_MIU_DATA_SIZE) return 0;
    size = ((f->f_pos + s) > MAX_MIU_DATA_SIZE) ? (MAX_MIU_DATA_SIZE - f->f_pos) : s;
    data = (char *)(&customTAG.miu_dram_len) + f->f_pos;
#endif
    copy_to_user(out, data, size);
    f->f_pos += size;
    *ppos = f->f_pos;
    return size;
}
static int infobin_release (struct inode *in, struct file *f) {
    module_put(THIS_MODULE);
    return 0;
}

static loff_t infobin_llseek (struct file *f, loff_t offset, int origin) {
    switch(origin) {
    case 0: break;
    case 1: offset += f->f_pos; break;
    default: return -EINVAL;
    }

    if(offset > MAX_MIU_DATA_SIZE) return -EINVAL;
    if(offset < 0) return -EINVAL;
    f->f_pos = offset;
    return offset;
}


static int info_open(struct inode *in, struct file *f) {
    return single_open(f, info_dump, NULL);
}

static int ini_open(struct inode *in, struct file *f) {
    return single_open(f, ini_dump, NULL);
}

static int bin_open(struct inode *in, struct file *f) {
    try_module_get(THIS_MODULE);
    return 0;
}

static ssize_t bin_read(struct file *f, char __user *out, size_t s, loff_t *ppos) {
    size_t size;
    char *data;
    if(f->f_pos >= MAX_MMAP_DATA_SIZE) return 0;
    size = ((f->f_pos + s) > MAX_MMAP_DATA_SIZE) ? (MAX_MMAP_DATA_SIZE - f->f_pos) : s;
    data = (char *)(customTAG.mmapger) + f->f_pos;
    copy_to_user(out, data, size);
    f->f_pos += size;
    *ppos = f->f_pos;
    return size;
}

static int bin_release (struct inode *in, struct file *f) {
    module_put(THIS_MODULE);
    return 0;
}

static loff_t bin_llseek (struct file *f, loff_t offset, int origin) {
    switch(origin) {
    case 0: break;
    case 1: offset += f->f_pos; break;
    default: return -EINVAL;
    }

    if(offset > MAX_MMAP_DATA_SIZE) return -EINVAL;
    if(offset < 0) return -EINVAL;
    f->f_pos = offset;
    return offset;
}

static const struct file_operations infofops = {
    .owner   = THIS_MODULE,
    .open    = info_open,
    .read    = seq_read,
    .release = single_release,
};

static const struct file_operations infobinfops = {
    .owner   = THIS_MODULE,
    .open    = infobin_open,
    .read    = infobin_read,
    .llseek  = infobin_llseek,
    .release = infobin_release,
};

static const struct file_operations inifops = {
    .owner   = THIS_MODULE,
    .open    = ini_open,
    .read    = seq_read,
    .release = single_release,
};

static const struct file_operations binfops = {
    .owner   = THIS_MODULE,
    .open    = bin_open,
    .read    = bin_read,
    .llseek  = bin_llseek,
    .release = bin_release,
};

int init_MMAPGERmodule(void) {
    root = proc_mkdir(procfsdir, NULL);
    if (!root) {
        printk(KERN_WARNING "mmapger: fail to load. #0\n");
        return -ENOMEM;
    }

    bin = proc_create(procfsbin, 0666, root, &binfops);
    if (!bin) {
        proc_remove(root);
        printk(KERN_WARNING "mmapger: fail to load. #1\n");
        return -ENOMEM;
    }

    ini = proc_create(procfsini, 0666, root, &inifops);
    if (!ini) {
        proc_remove(bin);
        proc_remove(root);
        printk(KERN_WARNING "mmapger: fail to load. #2\n");
        return -ENOMEM;
    }

    info = proc_create(procfsinfo, 0666, root, &infofops);
    if (!info) {
        proc_remove(ini);
        proc_remove(bin);
        proc_remove(root);
        printk(KERN_WARNING "mmapger: fail to load. #3\n");
        return -ENOMEM;
    }

    infobin = proc_create(procfsinfobin, 0666, root, &infobinfops);
    if (!infobin) {
        proc_remove(info);
        proc_remove(ini);
        proc_remove(bin);
        proc_remove(root);
        printk(KERN_WARNING "mmapger: fail to load. #4\n");
        return -ENOMEM;
    }

    printk(KERN_INFO "mmapger: one mmap for all, quick and clean. ~oger&ennon : succeed.\n");
    return 0;
}

void cleanup_MMAPGERmodule(void) {
    proc_remove(infobin);
    proc_remove(info);
    proc_remove(bin);
    proc_remove(ini);
    proc_remove(root);
    printk(KERN_INFO "mmapger: unloaded.\n");
}
