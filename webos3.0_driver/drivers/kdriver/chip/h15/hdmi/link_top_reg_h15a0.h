#ifndef _HDMI_RX_LINK_REGISTER_MAP_DWS_LINK_TOP_H_
#define _HDMI_RX_LINK_REGISTER_MAP_DWS_LINK_TOP_H_
/*-----------------------------------------------------------------------------
		0xc00 link_top_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 link_sw_rstn_cbus_prt0             : 1 ;  //     0     
	UINT32 link_sw_rstn_hdcp_prt0             : 1 ;  //     1     
	UINT32 link_sw_rstn_tmds_prt0             : 1 ;  //     2     
	UINT32 link_sw_rstn_edid_prt0             : 1 ;  //     3     
	UINT32 link_sw_rstn_aud_prt0              : 1 ;  //     4     
	UINT32 link_sw_rstn_pix_prt0              : 1 ;  //     5     
	UINT32 link_sw_rstn_sys_prt0              : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 link_sw_rstn_cbus_prt1             : 1 ;  //     8     
	UINT32 link_sw_rstn_tmds_prt1             : 1 ;  //     9     
	UINT32 link_sw_rstn_hdcp_prt1             : 1 ;  //    10     
	UINT32 link_sw_rstn_edid_prt1             : 1 ;  //    11     
	UINT32 link_sw_rstn_aud_prt1              : 1 ;  //    12     
	UINT32 link_sw_rstn_pix_prt1              : 1 ;  //    13     
	UINT32 link_sw_rstn_sys_prt1              : 1 ;  //    14     
	UINT32                                    : 9 ;  // 23:15     reserved
	UINT32 reg_rev_num                        : 8 ;  // 31:24     
} H15A0_LINK_TOP_01_T;

/*-----------------------------------------------------------------------------
		0xc04 link_top_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 link_sw_rstn_vfifo_wn_prt0         : 1 ;  //     0     
	UINT32 link_sw_rstn_afifo_wn_prt0         : 1 ;  //     1     
	UINT32 link_sw_rstn_vfifo_rn_prt0         : 1 ;  //     2     
	UINT32 link_sw_rstn_afifo_rn_prt0         : 1 ;  //     3     
	UINT32 link_sw_rstn_vfifo_wn_prt1         : 1 ;  //     4     
	UINT32 link_sw_rstn_afifo_wn_prt1         : 1 ;  //     5     
	UINT32 link_sw_rstn_vfifo_rn_prt1         : 1 ;  //     6     
	UINT32 link_sw_rstn_afifo_rn_prt1         : 1 ;  //     7     
	UINT32                                    : 2 ;  //  9: 8     reserved
	UINT32 link_sw_rstn_vfifo_rn_prt3         : 1 ;  //    10     
	UINT32 link_sw_rstn_afifo_rn_prt3         : 1 ;  //    11     
} H15A0_LINK_TOP_02_T;

/*-----------------------------------------------------------------------------
		0xc08 link_top_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 link_display_on_prt0               : 1 ;  //     0     
	UINT32 link_display_on_prt1               : 1 ;  //     1     
} H15A0_LINK_TOP_03_T;

/*-----------------------------------------------------------------------------
		0xc0c link_top_intr_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 link_apb_intr_prt0                 : 1 ;  //     0     
	UINT32 link_apb_intr_prt1                 : 1 ;  //     1     
} H15A0_LINK_TOP_INTR_01_T;

/*-----------------------------------------------------------------------------
		0xc10 hdcp_key_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_0             : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_1             : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_2             : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_3             : 8 ;  // 31:24     
} H15A0_HDCP_KEY_00_T;

/*-----------------------------------------------------------------------------
		0xc14 hdcp_key_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_4             : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_5             : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_6             : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_7             : 8 ;  // 31:24     
} H15A0_HDCP_KEY_01_T;

/*-----------------------------------------------------------------------------
		0xc18 hdcp_key_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_8             : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_9             : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_10            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_11            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_02_T;

/*-----------------------------------------------------------------------------
		0xc1c hdcp_key_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_12            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_13            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_14            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_15            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_03_T;

/*-----------------------------------------------------------------------------
		0xc20 hdcp_key_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_16            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_17            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_18            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_19            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_04_T;

/*-----------------------------------------------------------------------------
		0xc24 hdcp_key_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_20            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_21            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_22            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_23            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_05_T;

/*-----------------------------------------------------------------------------
		0xc28 hdcp_key_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_24            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_25            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_26            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_27            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_06_T;

/*-----------------------------------------------------------------------------
		0xc2c hdcp_key_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_28            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_29            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_30            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_31            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_07_T;

/*-----------------------------------------------------------------------------
		0xc30 hdcp_key_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_32            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_33            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_34            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_35            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_08_T;

/*-----------------------------------------------------------------------------
		0xc34 hdcp_key_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_36            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_37            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_38            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_39            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_09_T;

/*-----------------------------------------------------------------------------
		0xc38 hdcp_key_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_40            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_41            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_42            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_43            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_10_T;

/*-----------------------------------------------------------------------------
		0xc3c hdcp_key_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_44            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_45            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_46            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_47            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_11_T;

/*-----------------------------------------------------------------------------
		0xc40 hdcp_key_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_48            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_49            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_50            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_51            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_12_T;

/*-----------------------------------------------------------------------------
		0xc44 hdcp_key_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_52            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_53            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_54            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_55            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_13_T;

/*-----------------------------------------------------------------------------
		0xc48 hdcp_key_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_56            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_57            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_58            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_59            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_14_T;

/*-----------------------------------------------------------------------------
		0xc4c hdcp_key_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_60            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_61            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_62            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_63            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_15_T;

/*-----------------------------------------------------------------------------
		0xc50 hdcp_key_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_64            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_65            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_66            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_67            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_16_T;

/*-----------------------------------------------------------------------------
		0xc54 hdcp_key_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_68            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_69            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_70            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_71            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_17_T;

/*-----------------------------------------------------------------------------
		0xc58 hdcp_key_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_72            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_73            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_74            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_75            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_18_T;

/*-----------------------------------------------------------------------------
		0xc5c hdcp_key_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_76            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_77            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_78            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_79            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_19_T;

/*-----------------------------------------------------------------------------
		0xc60 hdcp_key_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_80            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_81            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_82            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_83            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_20_T;

/*-----------------------------------------------------------------------------
		0xc64 hdcp_key_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_84            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_85            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_86            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_87            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_21_T;

/*-----------------------------------------------------------------------------
		0xc68 hdcp_key_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_88            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_89            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_90            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_91            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_22_T;

/*-----------------------------------------------------------------------------
		0xc6c hdcp_key_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_92            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_93            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_94            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_95            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_23_T;

/*-----------------------------------------------------------------------------
		0xc70 hdcp_key_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_96            : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_97            : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_98            : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_99            : 8 ;  // 31:24     
} H15A0_HDCP_KEY_24_T;

/*-----------------------------------------------------------------------------
		0xc74 hdcp_key_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_100           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_101           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_102           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_103           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_25_T;

/*-----------------------------------------------------------------------------
		0xc78 hdcp_key_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_104           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_105           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_106           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_107           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_26_T;

/*-----------------------------------------------------------------------------
		0xc7c hdcp_key_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_108           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_109           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_110           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_111           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_27_T;

/*-----------------------------------------------------------------------------
		0xc80 hdcp_key_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_112           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_113           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_114           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_115           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_28_T;

/*-----------------------------------------------------------------------------
		0xc84 hdcp_key_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_116           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_117           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_118           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_119           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_29_T;

/*-----------------------------------------------------------------------------
		0xc88 hdcp_key_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_120           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_121           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_122           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_123           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_30_T;

/*-----------------------------------------------------------------------------
		0xc8c hdcp_key_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_124           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_125           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_126           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_127           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_31_T;

/*-----------------------------------------------------------------------------
		0xc90 hdcp_key_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_128           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_129           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_130           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_131           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_32_T;

/*-----------------------------------------------------------------------------
		0xc94 hdcp_key_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_132           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_133           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_134           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_135           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_33_T;

/*-----------------------------------------------------------------------------
		0xc98 hdcp_key_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_136           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_137           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_138           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_139           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_34_T;

/*-----------------------------------------------------------------------------
		0xc9c hdcp_key_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_140           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_141           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_142           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_143           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_35_T;

/*-----------------------------------------------------------------------------
		0xca0 hdcp_key_36 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_144           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_145           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_146           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_147           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_36_T;

/*-----------------------------------------------------------------------------
		0xca4 hdcp_key_37 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_148           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_149           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_150           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_151           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_37_T;

/*-----------------------------------------------------------------------------
		0xca8 hdcp_key_38 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_152           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_153           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_154           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_155           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_38_T;

/*-----------------------------------------------------------------------------
		0xcac hdcp_key_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_156           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_157           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_158           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_159           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_39_T;

/*-----------------------------------------------------------------------------
		0xcb0 hdcp_key_40 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_160           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_161           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_162           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_163           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_40_T;

/*-----------------------------------------------------------------------------
		0xcb4 hdcp_key_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_164           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_165           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_166           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_167           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_41_T;

/*-----------------------------------------------------------------------------
		0xcb8 hdcp_key_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_168           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_169           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_170           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_171           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_42_T;

/*-----------------------------------------------------------------------------
		0xcbc hdcp_key_43 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_172           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_173           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_174           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_175           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_43_T;

/*-----------------------------------------------------------------------------
		0xcc0 hdcp_key_44 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_176           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_177           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_178           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_179           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_44_T;

/*-----------------------------------------------------------------------------
		0xcc4 hdcp_key_45 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_180           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_181           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_182           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_183           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_45_T;

/*-----------------------------------------------------------------------------
		0xcc8 hdcp_key_46 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_184           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_185           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_186           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_187           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_46_T;

/*-----------------------------------------------------------------------------
		0xccc hdcp_key_47 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_188           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_189           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_190           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_191           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_47_T;

/*-----------------------------------------------------------------------------
		0xcd0 hdcp_key_48 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_192           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_193           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_194           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_195           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_48_T;

/*-----------------------------------------------------------------------------
		0xcd4 hdcp_key_49 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_196           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_197           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_198           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_199           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_49_T;

/*-----------------------------------------------------------------------------
		0xcd8 hdcp_key_50 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_200           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_201           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_202           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_203           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_50_T;

/*-----------------------------------------------------------------------------
		0xcdc hdcp_key_51 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_204           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_205           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_206           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_207           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_51_T;

/*-----------------------------------------------------------------------------
		0xce0 hdcp_key_52 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_208           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_209           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_210           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_211           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_52_T;

/*-----------------------------------------------------------------------------
		0xce4 hdcp_key_53 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_212           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_213           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_214           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_215           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_53_T;

/*-----------------------------------------------------------------------------
		0xce8 hdcp_key_54 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_216           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_217           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_218           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_219           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_54_T;

/*-----------------------------------------------------------------------------
		0xcec hdcp_key_55 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_220           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_221           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_222           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_223           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_55_T;

/*-----------------------------------------------------------------------------
		0xcf0 hdcp_key_56 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_224           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_225           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_226           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_227           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_56_T;

/*-----------------------------------------------------------------------------
		0xcf4 hdcp_key_57 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_228           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_229           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_230           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_231           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_57_T;

/*-----------------------------------------------------------------------------
		0xcf8 hdcp_key_58 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_232           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_233           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_234           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_235           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_58_T;

/*-----------------------------------------------------------------------------
		0xcfc hdcp_key_59 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_236           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_237           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_238           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_239           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_59_T;

/*-----------------------------------------------------------------------------
		0xd00 hdcp_key_60 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_240           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_241           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_242           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_243           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_60_T;

/*-----------------------------------------------------------------------------
		0xd04 hdcp_key_61 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_244           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_245           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_246           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_247           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_61_T;

/*-----------------------------------------------------------------------------
		0xd08 hdcp_key_62 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_248           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_249           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_250           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_251           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_62_T;

/*-----------------------------------------------------------------------------
		0xd0c hdcp_key_63 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_252           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_253           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_254           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_255           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_63_T;

/*-----------------------------------------------------------------------------
		0xd10 hdcp_key_64 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_256           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_257           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_258           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_259           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_64_T;

/*-----------------------------------------------------------------------------
		0xd14 hdcp_key_65 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_260           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_261           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_262           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_263           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_65_T;

/*-----------------------------------------------------------------------------
		0xd18 hdcp_key_66 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_264           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_265           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_266           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_267           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_66_T;

/*-----------------------------------------------------------------------------
		0xd1c hdcp_key_67 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_268           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_269           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_270           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_271           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_67_T;

/*-----------------------------------------------------------------------------
		0xd20 hdcp_key_68 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_272           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_273           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_274           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_275           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_68_T;

/*-----------------------------------------------------------------------------
		0xd24 hdcp_key_69 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_276           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_277           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_278           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_279           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_69_T;

/*-----------------------------------------------------------------------------
		0xd28 hdcp_key_70 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_280           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_281           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_282           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_283           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_70_T;

/*-----------------------------------------------------------------------------
		0xd2c hdcp_key_71 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_hdcp_key_bak_284           : 8 ;  //  7: 0     
	UINT32 reg_dbg_hdcp_key_bak_285           : 8 ;  // 15: 8     
	UINT32 reg_dbg_hdcp_key_bak_286           : 8 ;  // 23:16     
	UINT32 reg_dbg_hdcp_key_bak_287           : 8 ;  // 31:24     
} H15A0_HDCP_KEY_71_T;
typedef struct {
	H15A0_LINK_TOP_01_T                	link_top_01;                  //0xc00
	H15A0_LINK_TOP_02_T                	link_top_02;                  //0xc04
	H15A0_LINK_TOP_03_T                	link_top_03;                  //0xc08
	H15A0_LINK_TOP_INTR_01_T           	link_top_intr_01;             //0xc0c
	H15A0_HDCP_KEY_00_T                	hdcp_key_00;                  //0xc10
	H15A0_HDCP_KEY_01_T                	hdcp_key_01;                  //0xc14
	H15A0_HDCP_KEY_02_T                	hdcp_key_02;                  //0xc18
	H15A0_HDCP_KEY_03_T                	hdcp_key_03;                  //0xc1c
	H15A0_HDCP_KEY_04_T                	hdcp_key_04;                  //0xc20
	H15A0_HDCP_KEY_05_T                	hdcp_key_05;                  //0xc24
	H15A0_HDCP_KEY_06_T                	hdcp_key_06;                  //0xc28
	H15A0_HDCP_KEY_07_T                	hdcp_key_07;                  //0xc2c
	H15A0_HDCP_KEY_08_T                	hdcp_key_08;                  //0xc30
	H15A0_HDCP_KEY_09_T                	hdcp_key_09;                  //0xc34
	H15A0_HDCP_KEY_10_T                	hdcp_key_10;                  //0xc38
	H15A0_HDCP_KEY_11_T                	hdcp_key_11;                  //0xc3c
	H15A0_HDCP_KEY_12_T                	hdcp_key_12;                  //0xc40
	H15A0_HDCP_KEY_13_T                	hdcp_key_13;                  //0xc44
	H15A0_HDCP_KEY_14_T                	hdcp_key_14;                  //0xc48
	H15A0_HDCP_KEY_15_T                	hdcp_key_15;                  //0xc4c
	H15A0_HDCP_KEY_16_T                	hdcp_key_16;                  //0xc50
	H15A0_HDCP_KEY_17_T                	hdcp_key_17;                  //0xc54
	H15A0_HDCP_KEY_18_T                	hdcp_key_18;                  //0xc58
	H15A0_HDCP_KEY_19_T                	hdcp_key_19;                  //0xc5c
	H15A0_HDCP_KEY_20_T                	hdcp_key_20;                  //0xc60
	H15A0_HDCP_KEY_21_T                	hdcp_key_21;                  //0xc64
	H15A0_HDCP_KEY_22_T                	hdcp_key_22;                  //0xc68
	H15A0_HDCP_KEY_23_T                	hdcp_key_23;                  //0xc6c
	H15A0_HDCP_KEY_24_T                	hdcp_key_24;                  //0xc70
	H15A0_HDCP_KEY_25_T                	hdcp_key_25;                  //0xc74
	H15A0_HDCP_KEY_26_T                	hdcp_key_26;                  //0xc78
	H15A0_HDCP_KEY_27_T                	hdcp_key_27;                  //0xc7c
	H15A0_HDCP_KEY_28_T                	hdcp_key_28;                  //0xc80
	H15A0_HDCP_KEY_29_T                	hdcp_key_29;                  //0xc84
	H15A0_HDCP_KEY_30_T                	hdcp_key_30;                  //0xc88
	H15A0_HDCP_KEY_31_T                	hdcp_key_31;                  //0xc8c
	H15A0_HDCP_KEY_32_T                	hdcp_key_32;                  //0xc90
	H15A0_HDCP_KEY_33_T                	hdcp_key_33;                  //0xc94
	H15A0_HDCP_KEY_34_T                	hdcp_key_34;                  //0xc98
	H15A0_HDCP_KEY_35_T                	hdcp_key_35;                  //0xc9c
	H15A0_HDCP_KEY_36_T                	hdcp_key_36;                  //0xca0
	H15A0_HDCP_KEY_37_T                	hdcp_key_37;                  //0xca4
	H15A0_HDCP_KEY_38_T                	hdcp_key_38;                  //0xca8
	H15A0_HDCP_KEY_39_T                	hdcp_key_39;                  //0xcac
	H15A0_HDCP_KEY_40_T                	hdcp_key_40;                  //0xcb0
	H15A0_HDCP_KEY_41_T                	hdcp_key_41;                  //0xcb4
	H15A0_HDCP_KEY_42_T                	hdcp_key_42;                  //0xcb8
	H15A0_HDCP_KEY_43_T                	hdcp_key_43;                  //0xcbc
	H15A0_HDCP_KEY_44_T                	hdcp_key_44;                  //0xcc0
	H15A0_HDCP_KEY_45_T                	hdcp_key_45;                  //0xcc4
	H15A0_HDCP_KEY_46_T                	hdcp_key_46;                  //0xcc8
	H15A0_HDCP_KEY_47_T                	hdcp_key_47;                  //0xccc
	H15A0_HDCP_KEY_48_T                	hdcp_key_48;                  //0xcd0
	H15A0_HDCP_KEY_49_T                	hdcp_key_49;                  //0xcd4
	H15A0_HDCP_KEY_50_T                	hdcp_key_50;                  //0xcd8
	H15A0_HDCP_KEY_51_T                	hdcp_key_51;                  //0xcdc
	H15A0_HDCP_KEY_52_T                	hdcp_key_52;                  //0xce0
	H15A0_HDCP_KEY_53_T                	hdcp_key_53;                  //0xce4
	H15A0_HDCP_KEY_54_T                	hdcp_key_54;                  //0xce8
	H15A0_HDCP_KEY_55_T                	hdcp_key_55;                  //0xcec
	H15A0_HDCP_KEY_56_T                	hdcp_key_56;                  //0xcf0
	H15A0_HDCP_KEY_57_T                	hdcp_key_57;                  //0xcf4
	H15A0_HDCP_KEY_58_T                	hdcp_key_58;                  //0xcf8
	H15A0_HDCP_KEY_59_T                	hdcp_key_59;                  //0xcfc
	H15A0_HDCP_KEY_60_T                	hdcp_key_60;                  //0xd00
	H15A0_HDCP_KEY_61_T                	hdcp_key_61;                  //0xd04
	H15A0_HDCP_KEY_62_T                	hdcp_key_62;                  //0xd08
	H15A0_HDCP_KEY_63_T                	hdcp_key_63;                  //0xd0c
	H15A0_HDCP_KEY_64_T                	hdcp_key_64;                  //0xd10
	H15A0_HDCP_KEY_65_T                	hdcp_key_65;                  //0xd14
	H15A0_HDCP_KEY_66_T                	hdcp_key_66;                  //0xd18
	H15A0_HDCP_KEY_67_T                	hdcp_key_67;                  //0xd1c
	H15A0_HDCP_KEY_68_T                	hdcp_key_68;                  //0xd20
	H15A0_HDCP_KEY_69_T                	hdcp_key_69;                  //0xd24
	H15A0_HDCP_KEY_70_T                	hdcp_key_70;                  //0xd28
	H15A0_HDCP_KEY_71_T                	hdcp_key_71;                  //0xd2c
}HDMI_LINK_TOP_REG_H15A0_T;
#endif
