#ifndef _DDR_CONFIG_H_
#define _DDR_CONFIG_H_

/************************************************
*  ½?«O«ù Utopia ©M DSP ¸?ªº³o­???®?¤@­P
*
*  1. µù¸?¤£­n¥? !
*  2. «?§i¤£­n¥? .const xxxx = ????;
************************************************/

/************************************************
*   DSP DDR memory layout
*************************************************/
    /* DSP DDR size */
        #define ADEC__R2_DDR_SIZE       0x4A0000             //4.625 MB
        #define ASND__R2_DDR_SIZE       0x2E0000             //2.875 MB
        #define ASND_DSP_DDR_SIZE       0x300000             //3.00 MB
        #define COMMON_DDR_SIZE         0x580000             //5.50 MB
                                                             // Total = 16 MB
        /* ASND-DSP DDR */
            /* SPDIF delay (GP C Bffer) */
            #define OFFSET_SPDIF_DLY_DRAM_BASE             0x0000000
            #define SPDIF_DLY_DRAM_SIZE                    0x18000

            #define OFFSET_SE_MAIN_IN_DRAM_ADDR            0x003C000
            #define SE_MAIN_IN_DRAM_SIZE                   0x2D000      //180KB, (108ms) multiple of (SE_BUFF2_DMA_24BIT_LINE_SIZE*BYTES_IN_MIU_LINE)

            #define OFFSET_SE_MAIN_OUT_DRAM_ADDR           0x0069000
            #define SE_MAIN_OUT_DRAM_SIZE                  0x72C00      //459KB, (544ms) multiple of (SE_BUFF1_DMA_24BIT_LINE_SIZE*BYTES_IN_MIU_LINE)

            /* multi-channel input delay */
            #define OFFSET_MULTI_CH_INPUT_DLY_DRAM_BASE    0x00DBC00
            #define MULTI_CH_INPUT_DLY_DRAM_SIZE           0x6C000      // 6ch (512 ms), can be adjusted by request    //432KB, multiple of (MULTI_CH_INPUT_DELAY_DMA_LINESIZE*BYTES_IN_MIU_LINE)

            #define OFFSET_DDENC_METADATA_DRAM_ADDR        0x014F000
            #define DDENC_METADATA_DRAM_SIZE               0x660       //48 * (PCM1_DRAM_SIZE/2/10/1536) if PCM1_DRAM_SIZE == PCM2_DRAM_SIZE

            #define OFFSET_DM_PREFETCH_DRAM_ADDR           0x0150000
            #define DM_PREFETCH_DRAM_SIZE                  0x10000                                                     //64KB

            #define OFFSET_ES1_DRAM_ADDR                   0x0160000
            #define ES1_DRAM_SIZE                          0x20000                                                     //128KB

            #define OFFSET_ES2_DRAM_ADDR                   0x0180000
            #define ES2_DRAM_SIZE                          0x20000                                                     //128KB

            #define OFFSET_ES3_DRAM_ADDR                   0x01A0000
            #define ES3_DRAM_SIZE                          0x20000                                                     //128KB

            #define OFFSET_ES4_DRAM_ADDR                   0x01C0000
            #define ES4_DRAM_SIZE                          0x20000                                                     //128KB

            #define OFFSET_SIF1_DRAM_ADDR                  0x01E0000
            #define SIF1_DRAM_SIZE                         0x8000                                                      // 32KB
            #define OFFSET_SIF2_DRAM_ADDR                  0x01E8000
            #define SIF2_DRAM_SIZE                         0x8000                                                      // 32KB

            #define OFFSET_SER2_DDENC_MCHOUT_DRAM_ADDR     0x1F0000
            #define SER2_DDENC_MCHOUT_DRAM_SIZE            0xD800           //256 sample * 2byte * 6ch * 18

            #define OFFSET_SER2_OUTPCM_DMX_DRAM_ADDR       0x1FD800
            #define SER2_OUTPCM_DMX_DRAM_SIZE              0x4800           //256 sample * 2byte * 2ch * 18

            #define OFFSET_DEC1_METADATA1_DRAM_ADDR        0x20C200
            #define OFFSET_DEC1_METADATA2_DRAM_ADDR        0x20C300
            #define OFFSET_DEC1_METADATA3_DRAM_ADDR        0x20E940

            #define OFFSET_DEC2_METADATA1_DRAM_ADDR        0x228100
            #define OFFSET_DEC2_METADATA2_DRAM_ADDR        0x228200
            #define OFFSET_DEC2_METADATA3_DRAM_ADDR        0x22A840

            #define DEC_METADATA1_DRAM_SIZE                0x100
            #define DEC_METADATA2_DRAM_SIZE                0x2640   //288 * 34=0x2640, 288 * 34 * 6 = 0xE580
            #define DEC_METADATA3_DRAM_SIZE                0x19778

            /* HEAD PHONE delay */
            #define OFFSET_HEAD_PHONE_DLY_DRAM_BASE        0x0244000
            #define HEAD_PHONE_DLY_DRAM_SIZE               0x24000                                                     //144KB

            /* CH5 input delay */
            #define OFFSET_CH5_INPUT_DLY_DRAM_BASE         0x0268000
            #define CH5_INPUT_DLY_DRAM_SIZE                0x24000                                                     //144KB

            /* CH6 input delay */
            #define OFFSET_CH6_INPUT_DLY_DRAM_BASE         0x028C000
            #define CH6_INPUT_DLY_DRAM_SIZE                0x24000                                                     //144KB

            #define OFFSET_SIF_OUTPCM_ADDR                 0x02B0000
            #define SIF_OUTPCM_DRAM_SIZE                   0x2000                                                      //8KB

        /* Common DDR */
            #define OFFSET_DMA_READER_DRAM_BASE            0x0000000
            #define DMA_READER_DRAM_SIZE                   0x20000                                                     //128KB

            //DMR doesn't use H/W DMA Reader2 and S/W DMA, just make compiler happy
            #define OFFSET_HW_DMA_READER2_DRAM_BASE        0x020000
            #define HW_DMA_READER2_DRAM_SIZE               0x10000

            #define OFFSET_SW_DMA_READER_DRAM_BASE         0x020000
            #define SW_DMA_READER_DRAM_SIZE                0x10000                                                     //64KB

            #define OFFSET_PCM_CAPTURE1_BUFFER_DRAM_BASE   0x0030000
            #define OFFSET_PCM_CAPTURE2_BUFFER_DRAM_BASE   0x003C000
            #define PCM_CAPTURE_BUFFER_DRAM_SIZE           0xC000                                                      //48KB

            #define OFFSET_PCM1_DRAM_ADDR                  0x0048000
            #define PCM1_DRAM_SIZE                         0xFF000                                                     //1020KB                                               //64KB

            #define OFFSET_PCM2_DRAM_ADDR                  0x0147000
            #define PCM2_DRAM_SIZE                         0xFF000                                                     //1020KB

            #define OFFSET_SPDIF_NONPCM_DRAM_BASE          0x0246000                                                   //216KB
            #define SPDIF_NONPCM_DRAM_SIZE                 0x9F000

            #define OFFSET_MP3_ENC_DRAM_BASE               0x2E5000                                                   //12KB
            #define MP3_ENC_DRAM_SIZE                      0x3000

            #define OFFSET_OUTPUT_TEMP_DRAM_BASE           0x02E8000                                                  //64KB
            #define OUTPUT_TEMP_DRAM_SIZE                  0x10000

            #define OFFSET_HDMI_NONPCM_DRAM_BASE           0x02F8000
            #define HDMI_NONPCM_DRAM_SIZE                  0xD8000

            #define OFFSET_PCM_MIXER_ELEMENT_INFO_BASE     0x03D0000
            #define PCM_MIXER_ELEMENT_INFO_SIZE            0x1000

            #define OFFSET_PCM_MIXER_ELEMENT_BUFFER_BASE   0x03D1000
            #define PCM_MIXER_ELEMENT_BUFFER_SIZE          0x78000

            #define OFFSET_PCM_MIXER1_MIXED_BUFFER_BASE    0x449000
            #define OFFSET_PCM_MIXER2_MIXED_BUFFER_BASE    0x44D000
            #define PCM_MIXER_MIXED_BUFFER_SIZE            0x4000

            #define OFFSET_MCU_R2_DMA1_READER_DRAM_BASE    0x451000
            #define MCU_R2_DMA1_READER_DRAM_SIZE           0x50000                      //320KB

            #define OFFSET_MCU_R2_DMA2_READER_DRAM_BASE    0x4A1000
            #define MCU_R2_DMA2_READER_DRAM_SIZE           0x50000                      //320KB
#endif  //_DDR_CONFIG_H_
