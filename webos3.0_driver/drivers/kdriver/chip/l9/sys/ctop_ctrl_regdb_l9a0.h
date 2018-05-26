#ifndef _DBG_CTOP_CTRL_h
#define _DBG_CTOP_CTRL_h

/*------------------------
	0xfd300010 CTR00
------------------------*/
const DBG_REG_FIELD_T dbg_CTR00[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,1  ,"gfx_aclk_sel"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,5  ,"gfx_pclk_sel"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,9  ,"de_de_clk_sel"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,13 ,"cvda_clk_sel"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,17 ,"dea_clk_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,21 ,"de_apb_clk_sel"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"swrst_usb_bt_init"           },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"swrst_usb_init"              },
};

/*------------------------
	0xfd300014 CTR01
------------------------*/
const DBG_REG_FIELD_T dbg_CTR01[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,1  ,"te_ahclk_sel"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,5  ,"te_teclk_sel"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,10 ,8  ,"ve_aclk_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,13 ,11 ,"ve270clk_sel"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,16 ,14 ,"veclk_sel"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,17 ,"sive_apbclk_sel"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,20 ,20 ,"gpuclk_off_sel"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,21 ,"gpuclk_sel"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,25 ,"gpu_apbclk_sel"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,29 ,"gfx_clk_sel"                 },
};

/*------------------------
	0xfd300018 CTR02
------------------------*/
const DBG_REG_FIELD_T dbg_CTR02[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,1  ,"vd_declk_sel"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,5  ,"vd_teclk_sel"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,9  ,"vd_aclk_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,13 ,"vdclk_sel"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,17 ,"vd_apbclk_sel"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,21 ,"icod_aclk_sel"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,25 ,"icodclk_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,29 ,"te_aclk_sel"                 },
};

/*------------------------
	0xfd30001c CTR03
------------------------*/
const DBG_REG_FIELD_T dbg_CTR03[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,1  ,"flexnoc1_aclk_sel"           },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,5  ,"flexnoc0_aclk_sel"           },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,9  ,"aud_veclk_sel"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,13 ,"aud_teclk_sel"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,17 ,"aud_apbclk_sel"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,21 ,"aud_aclk_sel"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,25 ,"aud_dsp1aclk_sel"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,29 ,"aud_dsp0aclk_sel"            },
};

/*------------------------
	0xfd300020 CTR04
------------------------*/
const DBG_REG_FIELD_T dbg_CTR04[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,2  ,0  ,"at2_apbclk_sel"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,6  ,4  ,"at1_apbclk_sel"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,10 ,8  ,"at0_apbclk_sel"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,13 ,11 ,"s2_aclk_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,16 ,14 ,"s1_aclk_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,17 ,"s0_aclk_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,22 ,20 ,"f2_aclk_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,25 ,23 ,"f1_aclk_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,28 ,26 ,"f0_aclk_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,29 ,"flexnoc2_aclk_sel"           },
};

/*------------------------
	0xfd300024 CTR05
------------------------*/
const DBG_REG_FIELD_T dbg_CTR05[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,29 ,"slt_apbclk_sel"              },
};

/*------------------------
	0xfd300028 CTR06
------------------------*/
const DBG_REG_FIELD_T dbg_CTR06[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,2  ,2  ,"swrst_noc2"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,3  ,"swrst_noc1"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,4  ,4  ,"swrst_noc0"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,5  ,5  ,"swrst_de_vd"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,6  ,6  ,"swrst_te_vd"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,7  ,"swrst_vda"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,8  ,8  ,"swrst_vd"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,9  ,9  ,"swrst_vd_apb"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,10 ,10 ,"swrst_icoda"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,11 ,"swrst_icod"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,12 ,12 ,"swrst_tea"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,13 ,13 ,"swrst_teah"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,14 ,14 ,"swrst_te_te"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"swrst_te_ve"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,16 ,16 ,"swrst_vea"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,17 ,17 ,"swrst_ve"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,18 ,18 ,"swrst_icod_p"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"swrst_te_apb"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,20 ,20 ,"swrst_ve_apb"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,21 ,21 ,"swrst_gpu"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,22 ,22 ,"swrst_gpu_p"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"swrst_gfx"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,24 ,24 ,"swrst_gfx_a"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,25 ,25 ,"swrst_gfx_p"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,26 ,26 ,"swrst_de_dp"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"swrst_de_de"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,28 ,28 ,"swrst_cvda"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,29 ,29 ,"swrst_dea_dp"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,30 ,30 ,"swrst_dea"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"swrst_de_apb"                },
};

/*------------------------
	0xfd30002c CTR07
------------------------*/
const DBG_REG_FIELD_T dbg_CTR07[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,24 ,24 ,"swrst_f2_a"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,25 ,25 ,"swrst_f1_a"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,26 ,26 ,"swrst_f0_a"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"swrst_slt_p"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,29 ,29 ,"swrst_at2_p"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,30 ,30 ,"swrst_at1_p"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"swrst_at0_p"                 },
};

/*------------------------
	0xfd300030 CTR08
------------------------*/
const DBG_REG_FIELD_T dbg_CTR08[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,17 ,"trace_sel"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,21 ,"dbgvsoc_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,25 ,"atvsoc_sel"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,29 ,"atclk_sel"                   },
};

/*------------------------
	0xfd300034 CTR09
------------------------*/
const DBG_REG_FIELD_T dbg_CTR09[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,5  ,"pclken_sel"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,9  ,"ahbclken_sel"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"swrst_trace"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"swrst_dbgv"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"swrst_at"                    },
};

/*------------------------
	0xfd300038 CTR10
------------------------*/
const DBG_REG_FIELD_T dbg_CTR10[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,0  ,"ahb7_sel"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,5  ,"ahb6_sel"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,9  ,"ahb5_sel"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,13 ,"ahb4_sel"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,17 ,"ahb3_sel"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,21 ,"ahb2_sel"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,25 ,"ahb1_sel"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,29 ,"ahb0_sel"                    },
};

/*------------------------
	0xfd30003c CTR11
------------------------*/
const DBG_REG_FIELD_T dbg_CTR11[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,0  ,"ahb22_sel"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,5  ,"ahb21_sel"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,9  ,"ahb20_sel"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,13 ,"ahb19_sel"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,17 ,"ahb18_sel"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,21 ,"ahb17_sel"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,25 ,"ahb9_sel"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,29 ,"ahb8_sel"                    },
};

/*------------------------
	0xfd300040 CTR12
------------------------*/
const DBG_REG_FIELD_T dbg_CTR12[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"rmii_clkoff_sel"             },
};

/*------------------------
	0xfd300044 CTR13
------------------------*/
const DBG_REG_FIELD_T dbg_CTR13[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,5  ,"ahb30_sel"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,9  ,"ahb29_sel"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,13 ,"ahb28_sel"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,17 ,"ahb27_sel"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,21 ,"ahb26_sel"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,25 ,"ahb25_sel"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,29 ,"ahb24_sel"                   },
};

/*------------------------
	0xfd300048 CTR14
------------------------*/
const DBG_REG_FIELD_T dbg_CTR14[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,0  ,0  ,"swrst_ahb31"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,1  ,1  ,"swrst_ahb30"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,2  ,2  ,"swrst_ahb29"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,3  ,"swrst_ahb28"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,4  ,4  ,"swrst_ahb27"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,5  ,5  ,"swrst_ahb26"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,6  ,6  ,"swrst_ahb25"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,7  ,"swrst_ahb24"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,8  ,8  ,"swrst_ahb23"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,9  ,9  ,"swrst_ahb22"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,10 ,10 ,"swrst_ahb21"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,11 ,"swrst_ahb20"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,12 ,12 ,"swrst_ahb19"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,13 ,13 ,"swrst_ahb18"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,14 ,14 ,"swrst_ahb17"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"swrst_ahb16"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,16 ,16 ,"swrst_ahb15"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,17 ,17 ,"swrst_ahb14"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,18 ,18 ,"swrst_ahb13"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"swrst_ahb12"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,20 ,20 ,"swrst_ahb11"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,21 ,21 ,"swrst_ahb10"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,22 ,22 ,"swrst_ahb9"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"swrst_ahb8"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,24 ,24 ,"swrst_ahb7"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,25 ,25 ,"swrst_ahb6"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,26 ,26 ,"swrst_ahb5"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"swrst_ahb4"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,28 ,28 ,"swrst_ahb3"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,29 ,29 ,"swrst_ahb2"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,30 ,30 ,"swrst_ahb1"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"swrst_ahb0"                  },
};

/*------------------------
	0xfd30004c CTR15
------------------------*/
const DBG_REG_FIELD_T dbg_CTR15[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,26 ,26 ,"swrst_ahb37"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"swrst_ahb36"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,28 ,28 ,"swrst_ahb35"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,29 ,29 ,"swrst_ahb34"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,30 ,30 ,"swrst_ahb33"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"swrst_ahb32"                 },
};

/*------------------------
	0xfd300050 CTR16
------------------------*/
const DBG_REG_FIELD_T dbg_CTR16[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,1  ,"bus7_sel"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,5  ,"bus6_sel"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,9  ,"bus5_sel"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,13 ,"bus4_sel"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,17 ,"bus3_sel"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,21 ,"bus2_sel"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,25 ,"bus1_sel"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,29 ,"bus0_sel"                    },
};

/*------------------------
	0xfd300054 CTR17
------------------------*/
const DBG_REG_FIELD_T dbg_CTR17[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,21 ,"bus10_sel"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,25 ,"bus9_sel"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,29 ,"bus8_sel"                    },
};

/*------------------------
	0xfd300058 CTR18
------------------------*/
const DBG_REG_FIELD_T dbg_CTR18[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,21 ,21 ,"swrst_bus10"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,22 ,22 ,"swrst_bus9"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"swrst_bus8"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,24 ,24 ,"swrst_bus7"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,25 ,25 ,"swrst_bus6"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,26 ,26 ,"swrst_bus5"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"swrst_bus4"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,28 ,28 ,"swrst_bus3"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,29 ,29 ,"swrst_bus2"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,30 ,30 ,"swrst_bus1"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"swrst_bus0"                  },
};

/*------------------------
	0xfd30005c CTR19
------------------------*/
const DBG_REG_FIELD_T dbg_CTR19[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,1  ,"apb0_7_sel"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,5  ,"apb0_6_sel"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,9  ,"apb0_5_sel"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,13 ,"apb0_4_sel"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,17 ,"apb0_3_sel"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,21 ,"apb0_2_sel"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,25 ,"apb0_1_sel"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,29 ,"apb0_0_sel"                  },
};

/*------------------------
	0xfd300060 CTR20
------------------------*/
const DBG_REG_FIELD_T dbg_CTR20[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,1  ,"apb1_7_sel"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,5  ,"apb1_6_sel"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,9  ,"apb1_5_sel"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,13 ,"apb1_4_sel"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,17 ,"apb1_3_sel"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,21 ,"apb1_2_sel"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,25 ,"apb1_1_sel"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,29 ,"apb1_0_sel"                  },
};

/*------------------------
	0xfd300064 CTR21
------------------------*/
const DBG_REG_FIELD_T dbg_CTR21[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,1  ,"apb1_15_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,5  ,"apb1_14_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,9  ,"apb1_13_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,13 ,"apb1_12_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,17 ,"apb1_11_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,21 ,"apb1_10_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,25 ,"apb1_9_sel"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,29 ,"apb1_8_sel"                  },
};

/*------------------------
	0xfd300068 CTR22
------------------------*/
const DBG_REG_FIELD_T dbg_CTR22[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,1  ,"apb1_23_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,5  ,"apb1_22_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,9  ,"apb1_21_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,13 ,"apb1_20_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,17 ,"apb1_19_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,21 ,"apb1_18_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,25 ,"apb1_17_sel"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,29 ,"apb1_16_sel"                 },
};

/*------------------------
	0xfd30006c CTR23
------------------------*/
const DBG_REG_FIELD_T dbg_CTR23[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,29 ,"apb1_24_sel"                 },
};

/*------------------------
	0xfd300070 CTR24
------------------------*/
const DBG_REG_FIELD_T dbg_CTR24[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,0  ,0  ,"swrst_apb1_23"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,1  ,1  ,"swrst_apb1_22"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,2  ,2  ,"swrst_apb1_21"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,3  ,"swrst_apb1_20"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,4  ,4  ,"swrst_apb1_19"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,5  ,5  ,"swrst_apb1_18"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,6  ,6  ,"swrst_apb1_17"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,7  ,"swrst_apb1_16"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,8  ,8  ,"swrst_apb1_15"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,9  ,9  ,"swrst_apb1_14"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,10 ,10 ,"swrst_apb1_13"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,11 ,"swrst_apb1_12"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,12 ,12 ,"swrst_apb1_11"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,13 ,13 ,"swrst_apb1_10"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,14 ,14 ,"swrst_apb1_9"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"swrst_apb1_8"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,16 ,16 ,"swrst_apb1_7"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,17 ,17 ,"swrst_apb1_6"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,18 ,18 ,"swrst_apb1_5"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"swrst_apb1_4"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,20 ,20 ,"swrst_apb1_3"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,21 ,21 ,"swrst_apb1_2"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,22 ,22 ,"swrst_apb1_1"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"swrst_apb1_0"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,24 ,24 ,"swrst_apb0_7"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,25 ,25 ,"swrst_apb0_6"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,26 ,26 ,"swrst_apb0_5"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"swrst_apb0_4"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,28 ,28 ,"swrst_apb0_3"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,29 ,29 ,"swrst_apb0_2"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,30 ,30 ,"swrst_apb0_1"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"swrst_apb0_0"                },
};

/*------------------------
	0xfd300074 CTR25
------------------------*/
const DBG_REG_FIELD_T dbg_CTR25[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"swrst_apb1_24"               },
};

/*------------------------
	0xfd300078 CTR26
------------------------*/
const DBG_REG_FIELD_T dbg_CTR26[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,6  ,"ssclk_sel"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,10 ,"msclk_sel"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,13 ,12 ,"extb_sel"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,14 ,"exta_sel"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"ch3pix_clk_inv"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"cvd27_clk_inv"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"cvd54_clk_inv"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"cvd54_clk_sel"               },
};

/*------------------------
	0xfd30007c CTR27
------------------------*/
const DBG_REG_FIELD_T dbg_CTR27[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,0  ,0  ,"ext_en_i"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,3  ,"f27_18_sel"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,5  ,"sc_ext_sel"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,11 ,"slt_disp"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,12 ,12 ,"reg_s_te"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,13 ,13 ,"reg_sys"                     },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,14 ,14 ,"disp_h_dp"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"disp_h"                      },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,16 ,16 ,"disp_dp"                     },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,17 ,17 ,"disp_de"                     },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,18 ,18 ,"lvds_clk_sel"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"dco_sel"                     },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,20 ,20 ,"swrst_cvd"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,21 ,21 ,"swrst_se"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,22 ,22 ,"swrst_cve"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"swrst_chb"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,24 ,24 ,"swrst_cha"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,25 ,25 ,"swrst_extb"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,26 ,26 ,"swrst_exta"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"swrst_ch3"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,28 ,28 ,"swrst_scart"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,29 ,29 ,"swrst_vbi"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,30 ,30 ,"swrst_cvd27"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"swrst_cvd54"                 },
};

/*------------------------
	0xfd300080 CTR28
------------------------*/
const DBG_REG_FIELD_T dbg_CTR28[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,0  ,0  ,"wd_reset"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,3  ,"neon_reset"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,7  ,"scu_reset"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,11 ,"cpu_h"                       },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"cpu_reset"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,17 ,17 ,"l2_reset"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,18 ,"a_stdby_cpu"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"pll_sel_core"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"pll_sel_m2"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"pll_sel_m1"                  },
};

/*------------------------
	0xfd300084 CTR29
------------------------*/
const DBG_REG_FIELD_T dbg_CTR29[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,0  ,0  ,"ext_intr_pol_sel0"           },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,1  ,1  ,"ext_intr_pol_sel1"           },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,2  ,2  ,"ext_intr_pol_sel2"           },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,3  ,"ext_intr_pol_sel3"           },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,10 ,8  ,"jtag_sel"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,12 ,12 ,"m2_h_reset"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,13 ,13 ,"m2_reset"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,14 ,"a_stdby_m2"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,20 ,20 ,"m1_h_reset"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,21 ,21 ,"m1_reset"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,22 ,"a_stdby_m1"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,28 ,28 ,"m0_h_reset"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,29 ,29 ,"m0_reset"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,30 ,"a_stdby_m0"                  },
};

/*------------------------
	0xfd300088 CTR30
------------------------*/
const DBG_REG_FIELD_T dbg_CTR30[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,3  ,"trace_en"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,11 ,"usb_mon_en"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"gfx_mon_en"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"aad_mon_en"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"aud_mon_en"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"cpu_mon_en"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"de_mon_en"                   },
};

/*------------------------
	0xfd30008c CTR31
------------------------*/
const DBG_REG_FIELD_T dbg_CTR31[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,0  ,0  ,"trace_en_b00"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,4  ,4  ,"aad_mon_en_b04"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,5  ,5  ,"aud_mon_en_b05"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,6  ,6  ,"cpu_mon_en_b06"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,7  ,"de_mon_en_b07"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,10 ,10 ,"trace_en_b10"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,11 ,"usb_mon_en"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"gfx_mon_en"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"aad_mon_en_b19"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"aud_mon_en_b23"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"cpu_mon_en_b27"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"de_mon_en_b31"               },
};

/*------------------------
	0xfd300090 CTR32
------------------------*/
const DBG_REG_FIELD_T dbg_CTR32[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"stpio_en_ctrl"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,24 ,24 ,"gp4_0_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,25 ,25 ,"gp4_1_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,26 ,26 ,"gp4_2_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"gp4_3_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,28 ,28 ,"gp4_4_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,29 ,29 ,"gp4_5_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,30 ,30 ,"gp4_6_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"gp4_7_mux_en"                },
};

/*------------------------
	0xfd300094 CTR33
------------------------*/
const DBG_REG_FIELD_T dbg_CTR33[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,0  ,0  ,"trace_en"                    },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"cpu_mon_31_en"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"aad_mon_en"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"aud_mon_en"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"cpu_mon_en"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"de_mon_en"                   },
};

/*------------------------
	0xfd300098 CTR34
------------------------*/
const DBG_REG_FIELD_T dbg_CTR34[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,0  ,0  ,"gp6_3_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,7  ,"gp6_4_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"gp6_5_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"gp6_6_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"gp6_7_mux_en"                },
};

/*------------------------
	0xfd30009c CTR35
------------------------*/
const DBG_REG_FIELD_T dbg_CTR35[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,11 ,"gp7_5_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"gp7_6_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"gp7_7_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"gp6_0_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"gp6_1_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"gp6_2_mux_en"                },
};

/*------------------------
	0xfd3000a0 CTR36
------------------------*/
const DBG_REG_FIELD_T dbg_CTR36[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,10 ,"rx_sel_de"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,18 ,"rx_sel_sp"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,22 ,"uart2_sel"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,26 ,"uart1_sel"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,30 ,"uart0_sel"                   },
};

/*------------------------
	0xfd3000a4 CTR37
------------------------*/
const DBG_REG_FIELD_T dbg_CTR37[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,3  ,"gp8_5_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,7  ,"gp8_6_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,11 ,"gp8_7_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"gp7_0_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"gp7_1_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"gp7_2_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"gp7_3_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"gp7_4_mux_en"                },
};

/*------------------------
	0xfd3000a8 CTR38
------------------------*/
const DBG_REG_FIELD_T dbg_CTR38[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"gp8_1_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"gp8_2_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"gp8_3_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"gp8_4_mux_en"                },
};

/*------------------------
	0xfd3000ac CTR39
------------------------*/
const DBG_REG_FIELD_T dbg_CTR39[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,16 ,16 ,"sc_clk_sel"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,17 ,17 ,"gp10_2_mux_en"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,18 ,18 ,"gp10_3_mux_en"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"gp10_4_mux_en"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,20 ,20 ,"gp10_5_mux_en"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,21 ,21 ,"gp10_6_mux_en"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,22 ,22 ,"gp10_7_mux_en"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"gp9_0_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,24 ,24 ,"gp9_1_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,25 ,25 ,"gp9_2_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,26 ,26 ,"gp9_3_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"gp9_4_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,28 ,28 ,"gp9_5_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,29 ,29 ,"gp9_6_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,30 ,30 ,"gp9_7_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"gp8_0_mux_en"                },
};

/*------------------------
	0xfd3000b0 CTR40
------------------------*/
const DBG_REG_FIELD_T dbg_CTR40[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"gpu_pwrdnbypass"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"gpu_testin_b23"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"gpu_testin_b31"              },
};

/*------------------------
	0xfd3000b4 CTR41
------------------------*/
const DBG_REG_FIELD_T dbg_CTR41[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,7  ,"pull_up_control_b07"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,11 ,"pull_up_control_b11"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"pull_up_control_b15"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"pull_up_control_b19"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"pull_up_control_b23"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"pull_up_control_b27"         },
};

/*------------------------
	0xfd3000b8 CTR42
------------------------*/
const DBG_REG_FIELD_T dbg_CTR42[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,3  ,"pull_up_control_b03"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,7  ,"pull_up_control_b07"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,11 ,"pull_up_control_b11"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"pull_up_control_b15"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"pull_up_control_b19"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"pull_up_control_b23"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"pull_up_control_b27"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"pull_up_control_b31"         },
};

/*------------------------
	0xfd3000bc CTR43
------------------------*/
const DBG_REG_FIELD_T dbg_CTR43[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"pull_up_control_b15"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"pull_up_control_b23"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"pull_up_control_b31"         },
};

/*------------------------
	0xfd3000c0 CTR44
------------------------*/
const DBG_REG_FIELD_T dbg_CTR44[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"pull_up_control_b15"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"pull_up_control_b23"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"pull_up_control_b31"         },
};

/*------------------------
	0xfd3000c4 CTR45
------------------------*/
const DBG_REG_FIELD_T dbg_CTR45[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,11 ,"pull_up_control_b11"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"pull_up_control_b15"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"pull_up_control_b19"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"pull_up_control_b23"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"pull_up_control_b27"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"pull_up_control_b31"         },
};

/*------------------------
	0xfd3000c8 CTR46
------------------------*/
const DBG_REG_FIELD_T dbg_CTR46[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"pull_up_control_b23"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"pull_up_control_b27"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"pull_up_control_b31"         },
};

/*------------------------
	0xfd3000cc CTR47
------------------------*/
const DBG_REG_FIELD_T dbg_CTR47[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,3  ,"pull_up_control_b03"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,7  ,"pull_up_control_b07"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,11 ,"pull_up_control_b11"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"pull_up_control_b15"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"pull_up_control_b19"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"pull_up_control_b23"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"pull_up_control_b27"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"pull_up_control_b31"         },
};

/*------------------------
	0xfd3000d0 CTR48
------------------------*/
const DBG_REG_FIELD_T dbg_CTR48[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,11 ,"pull_up_control_b11"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"pull_up_control_b15"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"pull_up_control_b19"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"pull_up_control_b23"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"pull_up_control_b27"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"pull_up_control_b31"         },
};

/*------------------------
	0xfd3000d4 CTR49
------------------------*/
const DBG_REG_FIELD_T dbg_CTR49[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,7  ,"pull_up_control_b07"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"pull_up_control_b15"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"pull_up_control_b19"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"pull_up_control_b23"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"pull_up_control_b27"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"pull_up_control_b31"         },
};

/*------------------------
	0xfd3000d8 CTR50
------------------------*/
const DBG_REG_FIELD_T dbg_CTR50[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"tpo_sel_ctrl"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"swrst"                       },
};

/*------------------------
	0xfd3000dc CTR51
------------------------*/
const DBG_REG_FIELD_T dbg_CTR51[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"gp11_6_mux_en"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"gp11_7_mux_en"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"gp10_0_mux_en"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"gp10_1_mux_en"               },
};

/*------------------------
	0xfd3000e0 CTR52
------------------------*/
const DBG_REG_FIELD_T dbg_CTR52[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,11 ,"gp11_0_mux_en"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"gp11_1_mux_en"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"gp11_2_mux_en"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"gp11_3_mux_en"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"gp11_4_mux_en"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"gp11_5_mux_en"               },
};

/*------------------------
	0xfd3000e4 CTR53
------------------------*/
const DBG_REG_FIELD_T dbg_CTR53[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,3  ,"gp5_0_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,7  ,"gp5_1_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,11 ,"gp5_2_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"gp5_3_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"gp5_4_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"gp5_5_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"gp5_6_mux_en"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"gp5_7_mux_en"                },
};

/*------------------------
	0xfd3000e8 CTR54
------------------------*/
const DBG_REG_FIELD_T dbg_CTR54[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,0  ,0  ,"dftclk_ctrl"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,15 ,"pd"                          },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,18 ,18 ,"bt_testclk1_ctrl"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"bt_txpreemphasistune1_ctrl"  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,20 ,20 ,"bt_txrisetune1_ctrl"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,21 ,21 ,"bt_testclk0_ctrl"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,22 ,22 ,"bt_txpreemphasistune0_ctrl"  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"bt_txrisetune0_ctrl"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,24 ,24 ,"bt_commononn_ctrl"           },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,26 ,25 ,"bt_refclksel_ctrl"           },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,28 ,27 ,"bt_refclkdiv_ctrl"           },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,29 ,29 ,"bt_testen_en_ctrl"           },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,30 ,30 ,"bt_vatestenb_en_ctrl"        },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"bt_siddq_en_ctrl"            },
};

/*------------------------
	0xfd3000ec CTR55
------------------------*/
const DBG_REG_FIELD_T dbg_CTR55[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,28 ,"testaddr1"                   },
};

/*------------------------
	0xfd3000f0 CTR56
------------------------*/
const DBG_REG_FIELD_T dbg_CTR56[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,0  ,"testdatain1"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,8  ,8  ,"testdataoutsel1"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,12 ,9  ,"txvreftune1"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,16 ,13 ,"txfslstune1"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,17 ,17 ,"vregtune1"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,18 ,"txhsxvtune1"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,22 ,20 ,"sqrxtune1"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,25 ,23 ,"compdistune1"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,26 ,26 ,"loopbackenb1"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"portreset1"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,28 ,"testaddr0"                   },
};

/*------------------------
	0xfd3000f4 CTR57
------------------------*/
const DBG_REG_FIELD_T dbg_CTR57[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,10 ,3  ,"testdatain0"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,11 ,"testdataoutsel0"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,12 ,"txvreftune0"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,16 ,"txfslstune0"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,20 ,20 ,"vregtune0"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,22 ,21 ,"txhsxvtune0"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,25 ,23 ,"sqrxtune0"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,28 ,26 ,"compdistune0"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,29 ,29 ,"loopbackenb0"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,30 ,30 ,"otgdisable0"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"portreset0"                  },
};

/*------------------------
	0xfd3000f8 CTR58
------------------------*/
const DBG_REG_FIELD_T dbg_CTR58[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,18 ,18 ,"testclk1_ctrl"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,19 ,"txpreemphasistune1_ctrl"     },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,20 ,20 ,"txrisetune1_ctrl"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,21 ,21 ,"testclk0_ctrl"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,22 ,22 ,"txpreemphasistune0_ctrl"     },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"txrisetune0_ctrl"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,24 ,24 ,"commononn_ctrl"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,26 ,25 ,"refclksel_ctrl"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,28 ,27 ,"refclkdiv_ctrl"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,29 ,29 ,"testen_en_ctrl"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,30 ,30 ,"vatestenb_en_ctrl"           },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"siddq_en_ctrl"               },
};

/*------------------------
	0xfd3000fc CTR59
------------------------*/
const DBG_REG_FIELD_T dbg_CTR59[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,28 ,"testaddr1"                   },
};

/*------------------------
	0xfd300100 CTR60
------------------------*/
const DBG_REG_FIELD_T dbg_CTR60[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,7  ,0  ,"testdatain1"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,8  ,8  ,"testdataoutsel1"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,12 ,9  ,"txvreftune1"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,16 ,13 ,"txfslstune1"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,17 ,17 ,"vregtune1"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,18 ,"txhsxvtune1"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,22 ,20 ,"sqrxtune1"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,25 ,23 ,"compdistune1"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,26 ,26 ,"loopbackenb1"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"portreset1"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,28 ,"testaddr0"                   },
};

/*------------------------
	0xfd300104 CTR61
------------------------*/
const DBG_REG_FIELD_T dbg_CTR61[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,10 ,3  ,"testdatain0"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,11 ,"testdataoutsel0"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,12 ,"txvreftune0"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,16 ,"txfslstune0"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,20 ,20 ,"vregtune0"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,22 ,21 ,"txhsxvtune0"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,25 ,23 ,"sqrxtune0"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,28 ,26 ,"compdistune0"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,29 ,29 ,"loopbackenb0"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,30 ,30 ,"otgdisable0"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"portreset0"                  },
};

/*------------------------
	0xfd300108 CTR62
------------------------*/
const DBG_REG_FIELD_T dbg_CTR62[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,29 ,28 ,"disp_pre_fd_ctrl"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,30 ,"disp_od_fout_ctrl"           },
};

/*------------------------
	0xfd30010c CTR63
------------------------*/
const DBG_REG_FIELD_T dbg_CTR63[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,4  ,0  ,"disp_udex_ctrl"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,13 ,5  ,"disp_msex_ctrl"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,17 ,14 ,"disp_nsc_ctrl"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,18 ,"disp_npc_ctrl"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,26 ,24 ,"disp_m_ctrl"                 },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,29 ,27 ,"disp_cih_ctrl"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,30 ,30 ,"disp_lf_ctrl"                },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"disp_pdb_ctrl"               },
};

/*------------------------
	0xfd3001bc CTR64
------------------------*/
const DBG_REG_FIELD_T dbg_CTR64[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,20 ,18 ,"sd_dco_g_ctrl"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,22 ,21 ,"sd_dco_lpf_ctrl"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"sd_dco_resetb_ctrl"          },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,24 ,24 ,"sd_dco_rom_test_ctrl"        },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,25 ,"de_dco_g_ctrl"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,29 ,28 ,"de_dco_lpf_ctrl"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,30 ,30 ,"de_dco_resetb_ctrl"          },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"de_dco_rom_test_ctrl"        },
};

/*------------------------
	0xfd3001c0 CTR65
------------------------*/
const DBG_REG_FIELD_T dbg_CTR65[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,21 ,0  ,"sd_dco_fcw_ctrl"             },
};

/*------------------------
	0xfd3001c4 CTR66
------------------------*/
const DBG_REG_FIELD_T dbg_CTR66[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,25 ,24 ,"m2_dr3p_pre_fd_ctrl"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,26 ,"m2_dr3p_od_fout_ctrl"        },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,28 ,"m2_dr3p_dtr_sel_ctrl"        },
};

/*------------------------
	0xfd3001c8 CTR67
------------------------*/
const DBG_REG_FIELD_T dbg_CTR67[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,4  ,0  ,"m2_dr3p_udex_ctrl"           },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,13 ,5  ,"m2_dr3p_msex_ctrl"           },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,17 ,14 ,"m2_dr3p_nsc_ctrl"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,18 ,"m2_dr3p_npc_ctrl"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,26 ,24 ,"m2_dr3p_m_ctrl"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,29 ,27 ,"m2_dr3p_cih_ctrl"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,30 ,30 ,"m2_dr3p_lf_ctrl"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"m2_dr3p_pdb_ctrl"            },
};

/*------------------------
	0xfd3001cc CTR68
------------------------*/
const DBG_REG_FIELD_T dbg_CTR68[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,25 ,24 ,"m1_dr3p_pre_fd_ctrl"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,26 ,"m1_dr3p_od_fout_ctrl"        },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,28 ,"m1_dr3p_dtr_sel_ctrl"        },
};

/*------------------------
	0xfd3001d0 CTR69
------------------------*/
const DBG_REG_FIELD_T dbg_CTR69[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,4  ,0  ,"m1_dr3p_udex_ctrl"           },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,13 ,5  ,"m1_dr3p_msex_ctrl"           },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,17 ,14 ,"m1_dr3p_nsc_ctrl"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,18 ,"m1_dr3p_npc_ctrl"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,26 ,24 ,"m1_dr3p_m_ctrl"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,29 ,27 ,"m1_dr3p_cih_ctrl"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,30 ,30 ,"m1_dr3p_lf_ctrl"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"m1_dr3p_pdb_ctrl"            },
};

/*------------------------
	0xfd3001d4 CTR70
------------------------*/
const DBG_REG_FIELD_T dbg_CTR70[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,25 ,24 ,"c_dr3p_pre_fd_ctrl"          },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,26 ,"c_dr3p_od_fout_ctrl"         },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,28 ,"c_dr3p_dtr_sel_ctrl"         },
};

/*------------------------
	0xfd3001d8 CTR71
------------------------*/
const DBG_REG_FIELD_T dbg_CTR71[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,4  ,0  ,"c_dr3p_udex_ctrl"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,13 ,5  ,"c_dr3p_msex_ctrl"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,17 ,14 ,"c_dr3p_nsc_ctrl"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,18 ,"c_dr3p_npc_ctrl"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,26 ,24 ,"c_dr3p_m_ctrl"               },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,29 ,27 ,"c_dr3p_cih_ctrl"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,30 ,30 ,"c_dr3p_lf_ctrl"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"c_dr3p_pdb_ctrl"             },
};

/*------------------------
	0xfd3001dc CTR72
------------------------*/
const DBG_REG_FIELD_T dbg_CTR72[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,11 ,0  ,"lv_flip_en_ctrl"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,18 ,"lv_pemp_te2_ctrl"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,21 ,20 ,"lv_pemp_td2_ctrl"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,22 ,"lv_pemp_tclk2_ctrl"          },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,25 ,24 ,"lv_pemp_tc2_ctrl"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,26 ,"lv_pemp_tb2_ctrl"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,29 ,28 ,"lv_pemp_ta2_ctrl"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,30 ,"lv_pemp_te1_ctrl"            },
};

/*------------------------
	0xfd3001e0 CTR73
------------------------*/
const DBG_REG_FIELD_T dbg_CTR73[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,2  ,1  ,"lv_pemp_td1_ctrl"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,4  ,3  ,"lv_pemp_tclk1_ctrl"          },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,6  ,5  ,"lv_pemp_tc1_ctrl"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,8  ,7  ,"lv_pemp_tb1_ctrl"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,10 ,9  ,"lv_pemp_ta1_ctrl"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,12 ,12 ,"lv_rf_ctrl"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,13 ,13 ,"lv_rmlvds_en_ctrl"           },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,16 ,14 ,"lv_swc_te2_ctrl"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,19 ,17 ,"lv_swc_td2_ctrl"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,22 ,20 ,"lv_swc_tclk2_ctrl"           },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,25 ,23 ,"lv_swc_tc2_ctrl"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,28 ,26 ,"lv_swc_tb2_ctrl"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,29 ,"lv_swc_ta2_ctrl"             },
};

/*------------------------
	0xfd3001e4 CTR74
------------------------*/
const DBG_REG_FIELD_T dbg_CTR74[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,3  ,1  ,"lv_swc_te1_ctrl"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,6  ,4  ,"lv_swc_td1_ctrl"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,9  ,7  ,"lv_swc_tclk1_ctrl"           },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,12 ,10 ,"lv_swc_tc1_ctrl"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,15 ,13 ,"lv_swc_tb1_ctrl"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,18 ,16 ,"lv_swc_ta1_ctrl"             },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,30 ,19 ,"lv_pdb_ch_ctrl"              },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"lv_pdb_ctrl"                 },
};

/*------------------------
	0xfd3001e8 CTR75
------------------------*/
const DBG_REG_FIELD_T dbg_CTR75[] = {
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,20 ,20 ,"rx2_sel_speed_en"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,21 ,21 ,"rx1_sel_speed_en"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,22 ,22 ,"rx0_sel_speed_en"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,23 ,23 ,"rx2_sel_8b10b_en"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,24 ,24 ,"rx1_sel_8b10b_en"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,25 ,25 ,"rx0_sel_8b10b_en"            },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,26 ,26 ,"rx2_rf_en"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,27 ,27 ,"rx1_rf_en"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,28 ,28 ,"rx0_rf_en"                   },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,29 ,29 ,"rx2_pdb_en"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,30 ,30 ,"rx1_pdb_en"                  },
	{ REG_XS_N_RW ,0 ,0 ,0 ,0 ,31 ,31 ,"rx0_pdb_en"                  },
};

DBG_REG_T gDbgRegCTOP_CTRL[] = {
	{ 0xfd300010 ,N_FLD(dbg_CTR00)       ,"CTR00"         ,dbg_CTR00       },
	{ 0xfd300014 ,N_FLD(dbg_CTR01)       ,"CTR01"         ,dbg_CTR01       },
	{ 0xfd300018 ,N_FLD(dbg_CTR02)       ,"CTR02"         ,dbg_CTR02       },
	{ 0xfd30001c ,N_FLD(dbg_CTR03)       ,"CTR03"         ,dbg_CTR03       },
	{ 0xfd300020 ,N_FLD(dbg_CTR04)       ,"CTR04"         ,dbg_CTR04       },
	{ 0xfd300024 ,N_FLD(dbg_CTR05)       ,"CTR05"         ,dbg_CTR05       },
	{ 0xfd300028 ,N_FLD(dbg_CTR06)       ,"CTR06"         ,dbg_CTR06       },
	{ 0xfd30002c ,N_FLD(dbg_CTR07)       ,"CTR07"         ,dbg_CTR07       },
	{ 0xfd300030 ,N_FLD(dbg_CTR08)       ,"CTR08"         ,dbg_CTR08       },
	{ 0xfd300034 ,N_FLD(dbg_CTR09)       ,"CTR09"         ,dbg_CTR09       },
	{ 0xfd300038 ,N_FLD(dbg_CTR10)       ,"CTR10"         ,dbg_CTR10       },
	{ 0xfd30003c ,N_FLD(dbg_CTR11)       ,"CTR11"         ,dbg_CTR11       },
	{ 0xfd300040 ,N_FLD(dbg_CTR12)       ,"CTR12"         ,dbg_CTR12       },
	{ 0xfd300044 ,N_FLD(dbg_CTR13)       ,"CTR13"         ,dbg_CTR13       },
	{ 0xfd300048 ,N_FLD(dbg_CTR14)       ,"CTR14"         ,dbg_CTR14       },
	{ 0xfd30004c ,N_FLD(dbg_CTR15)       ,"CTR15"         ,dbg_CTR15       },
	{ 0xfd300050 ,N_FLD(dbg_CTR16)       ,"CTR16"         ,dbg_CTR16       },
	{ 0xfd300054 ,N_FLD(dbg_CTR17)       ,"CTR17"         ,dbg_CTR17       },
	{ 0xfd300058 ,N_FLD(dbg_CTR18)       ,"CTR18"         ,dbg_CTR18       },
	{ 0xfd30005c ,N_FLD(dbg_CTR19)       ,"CTR19"         ,dbg_CTR19       },
	{ 0xfd300060 ,N_FLD(dbg_CTR20)       ,"CTR20"         ,dbg_CTR20       },
	{ 0xfd300064 ,N_FLD(dbg_CTR21)       ,"CTR21"         ,dbg_CTR21       },
	{ 0xfd300068 ,N_FLD(dbg_CTR22)       ,"CTR22"         ,dbg_CTR22       },
	{ 0xfd30006c ,N_FLD(dbg_CTR23)       ,"CTR23"         ,dbg_CTR23       },
	{ 0xfd300070 ,N_FLD(dbg_CTR24)       ,"CTR24"         ,dbg_CTR24       },
	{ 0xfd300074 ,N_FLD(dbg_CTR25)       ,"CTR25"         ,dbg_CTR25       },
	{ 0xfd300078 ,N_FLD(dbg_CTR26)       ,"CTR26"         ,dbg_CTR26       },
	{ 0xfd30007c ,N_FLD(dbg_CTR27)       ,"CTR27"         ,dbg_CTR27       },
	{ 0xfd300080 ,N_FLD(dbg_CTR28)       ,"CTR28"         ,dbg_CTR28       },
	{ 0xfd300084 ,N_FLD(dbg_CTR29)       ,"CTR29"         ,dbg_CTR29       },
	{ 0xfd300088 ,N_FLD(dbg_CTR30)       ,"CTR30"         ,dbg_CTR30       },
	{ 0xfd30008c ,N_FLD(dbg_CTR31)       ,"CTR31"         ,dbg_CTR31       },
	{ 0xfd300090 ,N_FLD(dbg_CTR32)       ,"CTR32"         ,dbg_CTR32       },
	{ 0xfd300094 ,N_FLD(dbg_CTR33)       ,"CTR33"         ,dbg_CTR33       },
	{ 0xfd300098 ,N_FLD(dbg_CTR34)       ,"CTR34"         ,dbg_CTR34       },
	{ 0xfd30009c ,N_FLD(dbg_CTR35)       ,"CTR35"         ,dbg_CTR35       },
	{ 0xfd3000a0 ,N_FLD(dbg_CTR36)       ,"CTR36"         ,dbg_CTR36       },
	{ 0xfd3000a4 ,N_FLD(dbg_CTR37)       ,"CTR37"         ,dbg_CTR37       },
	{ 0xfd3000a8 ,N_FLD(dbg_CTR38)       ,"CTR38"         ,dbg_CTR38       },
	{ 0xfd3000ac ,N_FLD(dbg_CTR39)       ,"CTR39"         ,dbg_CTR39       },
	{ 0xfd3000b0 ,N_FLD(dbg_CTR40)       ,"CTR40"         ,dbg_CTR40       },
	{ 0xfd3000b4 ,N_FLD(dbg_CTR41)       ,"CTR41"         ,dbg_CTR41       },
	{ 0xfd3000b8 ,N_FLD(dbg_CTR42)       ,"CTR42"         ,dbg_CTR42       },
	{ 0xfd3000bc ,N_FLD(dbg_CTR43)       ,"CTR43"         ,dbg_CTR43       },
	{ 0xfd3000c0 ,N_FLD(dbg_CTR44)       ,"CTR44"         ,dbg_CTR44       },
	{ 0xfd3000c4 ,N_FLD(dbg_CTR45)       ,"CTR45"         ,dbg_CTR45       },
	{ 0xfd3000c8 ,N_FLD(dbg_CTR46)       ,"CTR46"         ,dbg_CTR46       },
	{ 0xfd3000cc ,N_FLD(dbg_CTR47)       ,"CTR47"         ,dbg_CTR47       },
	{ 0xfd3000d0 ,N_FLD(dbg_CTR48)       ,"CTR48"         ,dbg_CTR48       },
	{ 0xfd3000d4 ,N_FLD(dbg_CTR49)       ,"CTR49"         ,dbg_CTR49       },
	{ 0xfd3000d8 ,N_FLD(dbg_CTR50)       ,"CTR50"         ,dbg_CTR50       },
	{ 0xfd3000dc ,N_FLD(dbg_CTR51)       ,"CTR51"         ,dbg_CTR51       },
	{ 0xfd3000e0 ,N_FLD(dbg_CTR52)       ,"CTR52"         ,dbg_CTR52       },
	{ 0xfd3000e4 ,N_FLD(dbg_CTR53)       ,"CTR53"         ,dbg_CTR53       },
	{ 0xfd3000e8 ,N_FLD(dbg_CTR54)       ,"CTR54"         ,dbg_CTR54       },
	{ 0xfd3000ec ,N_FLD(dbg_CTR55)       ,"CTR55"         ,dbg_CTR55       },
	{ 0xfd3000f0 ,N_FLD(dbg_CTR56)       ,"CTR56"         ,dbg_CTR56       },
	{ 0xfd3000f4 ,N_FLD(dbg_CTR57)       ,"CTR57"         ,dbg_CTR57       },
	{ 0xfd3000f8 ,N_FLD(dbg_CTR58)       ,"CTR58"         ,dbg_CTR58       },
	{ 0xfd3000fc ,N_FLD(dbg_CTR59)       ,"CTR59"         ,dbg_CTR59       },
	{ 0xfd300100 ,N_FLD(dbg_CTR60)       ,"CTR60"         ,dbg_CTR60       },
	{ 0xfd300104 ,N_FLD(dbg_CTR61)       ,"CTR61"         ,dbg_CTR61       },
	{ 0xfd300108 ,N_FLD(dbg_CTR62)       ,"CTR62"         ,dbg_CTR62       },
	{ 0xfd30010c ,N_FLD(dbg_CTR63)       ,"CTR63"         ,dbg_CTR63       },
	{ 0xfd3001bc ,N_FLD(dbg_CTR64)       ,"CTR64"         ,dbg_CTR64       },
	{ 0xfd3001c0 ,N_FLD(dbg_CTR65)       ,"CTR65"         ,dbg_CTR65       },
	{ 0xfd3001c4 ,N_FLD(dbg_CTR66)       ,"CTR66"         ,dbg_CTR66       },
	{ 0xfd3001c8 ,N_FLD(dbg_CTR67)       ,"CTR67"         ,dbg_CTR67       },
	{ 0xfd3001cc ,N_FLD(dbg_CTR68)       ,"CTR68"         ,dbg_CTR68       },
	{ 0xfd3001d0 ,N_FLD(dbg_CTR69)       ,"CTR69"         ,dbg_CTR69       },
	{ 0xfd3001d4 ,N_FLD(dbg_CTR70)       ,"CTR70"         ,dbg_CTR70       },
	{ 0xfd3001d8 ,N_FLD(dbg_CTR71)       ,"CTR71"         ,dbg_CTR71       },
	{ 0xfd3001dc ,N_FLD(dbg_CTR72)       ,"CTR72"         ,dbg_CTR72       },
	{ 0xfd3001e0 ,N_FLD(dbg_CTR73)       ,"CTR73"         ,dbg_CTR73       },
	{ 0xfd3001e4 ,N_FLD(dbg_CTR74)       ,"CTR74"         ,dbg_CTR74       },
	{ 0xfd3001e8 ,N_FLD(dbg_CTR75)       ,"CTR75"         ,dbg_CTR75       },
	{ 0 , } // end marker
};

#endif
