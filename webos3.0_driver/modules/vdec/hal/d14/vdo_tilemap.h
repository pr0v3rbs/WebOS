typedef enum {
	TILED_MAP_LINEAR  = 0,
	TILED_MAP_FRAME_V = 1,
	TILED_MAP_MIXED_V = 2,
#if defined(CHIP_NAME_d13)
	TILED_MAP_FRAME_V_HEVC = 3,
#endif
	TILED_MAP_MAX
} TILED_MAP_T;

typedef struct {
	UINT8		u8Data[0x40];
	UINT32		u32Config;
} TILED_MAP_CONFIG_T;

const static TILED_MAP_CONFIG_T	_gastTiledMapConfig[TILED_MAP_MAX][4] =
{
	{	// TILED_MAP_LINEAR
		{
			{	// > 2048
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,	/*xy2axiLumMap*/
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,

				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,	/*xy2axiChrMap*/
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40
			},
			0x00000000		/*xy2axiConfig*/
		},
		{
			{	// <= 2048
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,

				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40
			},
			0x00000000
		},
		{
			{	// <= 1024
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,

				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40
			},
			0x00000000
		},
		{
			{	// <= 512
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,

				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40
			},
			0x00000000
		}
	},
	{	// TILED_FRAME_V_MAP
		{
			{	// > 2048
				0x40, 0x40, 0x40, 0x10, 0x11, 0x12, 0x13, 0x03,
				0x04, 0x05, 0x06, 0x14, 0x07, 0x15, 0x08, 0x09,
				0x0A, 0x0B, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,

				0x40, 0x40, 0x40, 0x10, 0x11, 0x12, 0x13, 0x03,
				0x04, 0x05, 0x06, 0x15, 0x87, 0x94, 0x08, 0x09,
				0x0A, 0x0B, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40
			},
			0x0003F0F0
		},
		{
			{	// <= 2048
				0x40, 0x40, 0x40, 0x10, 0x11, 0x12, 0x13, 0x03,
				0x04, 0x05, 0x06, 0x14, 0x07, 0x15, 0x08, 0x09,
				0x0A, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,

				0x40, 0x40, 0x40, 0x10, 0x11, 0x12, 0x13, 0x03,
				0x04, 0x05, 0x06, 0x15, 0x87, 0x94, 0x08, 0x09,
				0x0A, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40
			},
			0x0003F0F0
		},
		{
			{	// <= 1024
				0x40, 0x40, 0x40, 0x10, 0x11, 0x12, 0x13, 0x03,
				0x04, 0x05, 0x06, 0x14, 0x07, 0x15, 0x08, 0x09,
				0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,

				0x40, 0x40, 0x40, 0x10, 0x11, 0x12, 0x13, 0x03,
				0x04, 0x05, 0x06, 0x15, 0x87, 0x94, 0x08, 0x09,
				0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40
			},
			0x0003F0F0
		},
		{
			{	// <= 512
				0x40, 0x40, 0x40, 0x10, 0x11, 0x12, 0x13, 0x03,
				0x04, 0x05, 0x06, 0x14, 0x07, 0x15, 0x08, 0x16,
				0x17, 0x18, 0x19, 0x1A, 0x1B, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,

				0x40, 0x40, 0x40, 0x10, 0x11, 0x12, 0x13, 0x03,
				0x04, 0x05, 0x06, 0x15, 0x87, 0x94, 0x08, 0x16,
				0x17, 0x18, 0x19, 0x1A, 0x1B, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40
			},
			0x0003F0F0
		}
	},
	{	// TILED_MIXED_V_MAP
		{
			{	// > 2048
				0x40, 0x40, 0x40, 0x11, 0x12, 0x13, 0x10, 0x03,
				0x04, 0x05, 0x06, 0x14, 0x07, 0x15, 0x08, 0x09,
				0x0A, 0x0B, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,

				0x40, 0x40, 0x40, 0x11, 0x12, 0x13, 0x10, 0x03,
				0x04, 0x05, 0x06, 0x15, 0x87, 0x94, 0x08, 0x09,
				0x0A, 0x0B, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40
			},
			0x00077777
		},
		{
			{	// <= 2048
				0x40, 0x40, 0x40, 0x11, 0x12, 0x13, 0x10, 0x03,
				0x04, 0x05, 0x06, 0x14, 0x07, 0x15, 0x08, 0x09,
				0x0A, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,

				0x40, 0x40, 0x40, 0x11, 0x12, 0x13, 0x10, 0x03,
				0x04, 0x05, 0x06, 0x15, 0x87, 0x94, 0x08, 0x09,
				0x0A, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40
			},
			0x00077777
		},
		{
			{	// <= 1024
				0x40, 0x40, 0x40, 0x11, 0x12, 0x13, 0x10, 0x03,
				0x04, 0x05, 0x06, 0x14, 0x07, 0x15, 0x08, 0x09,
				0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,

				0x40, 0x40, 0x40, 0x11, 0x12, 0x13, 0x10, 0x03,
				0x04, 0x05, 0x06, 0x15, 0x87, 0x94, 0x08, 0x09,
				0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40
			},
			0x00077777
		},
		{
			{	// <= 512
				0x40, 0x40, 0x40, 0x11, 0x12, 0x13, 0x10, 0x03,
				0x04, 0x05, 0x06, 0x14, 0x07, 0x15, 0x08, 0x16,
				0x17, 0x18, 0x19, 0x1A, 0x1B, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,

				0x40, 0x40, 0x40, 0x11, 0x12, 0x13, 0x10, 0x03,
				0x04, 0x05, 0x06, 0x15, 0x87, 0x94, 0x08, 0x16,
				0x17, 0x18, 0x19, 0x1A, 0x1B, 0x40, 0x40, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40
			},
			0x00077777
		}
	},
	{	// TILED_MAP_FRAME_V For HEVC
		{
			{	// > 2048
				0x40, 0x40, 0x40, 0x10, 0x11, 0x12, 0x13, 0x14,
				0x15, 0x03, 0x04, 0x05, 0x06, 0x07, 0x16, 0x08,
				0x09, 0x0a, 0x0b, 0x17, 0x18, 0x19, 0x1a, 0x1b,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,

				0x40, 0x40, 0x40, 0x10, 0x11, 0x12, 0x13, 0x14,
				0x15, 0x03, 0x04, 0x05, 0x06, 0x07, 0x16, 0x08,
				0x09, 0x0a, 0x0b, 0x17, 0x18, 0x19, 0x1a, 0x1b,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40
			},
			0x0003f0f0
		},
		{
			{	// <= 2048
				0x40, 0x40, 0x40, 0x10, 0x11, 0x12, 0x13, 0x14,
				0x15, 0x03, 0x04, 0x05, 0x06, 0x07, 0x16, 0x08,
				0x09, 0x0a, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,

				0x40, 0x40, 0x40, 0x10, 0x11, 0x12, 0x13, 0x14,
				0x15, 0x03, 0x04, 0x05, 0x06, 0x07, 0x16, 0x08,
				0x09, 0x0a, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40
			},
			0x0003f0f0
		},
		{
			{	// <= 2048
				0x40, 0x40, 0x40, 0x10, 0x11, 0x12, 0x13, 0x14,
				0x15, 0x03, 0x04, 0x05, 0x06, 0x07, 0x16, 0x08,
				0x09, 0x0a, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,

				0x40, 0x40, 0x40, 0x10, 0x11, 0x12, 0x13, 0x14,
				0x15, 0x03, 0x04, 0x05, 0x06, 0x07, 0x16, 0x08,
				0x09, 0x0a, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40
			},
			0x0003f0f0
		},
		{
			{	// <= 2048
				0x40, 0x40, 0x40, 0x10, 0x11, 0x12, 0x13, 0x14,
				0x15, 0x03, 0x04, 0x05, 0x06, 0x07, 0x16, 0x08,
				0x09, 0x0a, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,

				0x40, 0x40, 0x40, 0x10, 0x11, 0x12, 0x13, 0x14,
				0x15, 0x03, 0x04, 0x05, 0x06, 0x07, 0x16, 0x08,
				0x09, 0x0a, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x40,
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40
			},
			0x0003f0f0
		}
	}
};
