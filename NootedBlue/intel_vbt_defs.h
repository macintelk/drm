/*
 * Copyright © 2006-2016 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *
 */

/*
 * This information is private to VBT parsing in intel_bios.c.
 *
 * Please do NOT include anywhere else.
 */


#ifndef _INTEL_VBT_DEFS_H_
#define _INTEL_VBT_DEFS_H_

#ifdef __cplusplus
//extern "C" {
#endif

#include "intel_dsi_vbt_defs.h"





/* EDID derived structures */
struct bdb_edid_pnp_id {
	u16 mfg_name;
	u16 product_code;
	u32 serial;
	u8 mfg_week;
	u8 mfg_year;
} __packed;

struct bdb_edid_product_name {
	char name[13];
} __packed;

struct bdb_edid_dtd {
	u16 clock;		/**< In 10khz */
	u8 hactive_lo;
	u8 hblank_lo;
	u8 hblank_hi:4;
	u8 hactive_hi:4;
	u8 vactive_lo;
	u8 vblank_lo;
	u8 vblank_hi:4;
	u8 vactive_hi:4;
	u8 hsync_off_lo;
	u8 hsync_pulse_width_lo;
	u8 vsync_pulse_width_lo:4;
	u8 vsync_off_lo:4;
	u8 vsync_pulse_width_hi:2;
	u8 vsync_off_hi:2;
	u8 hsync_pulse_width_hi:2;
	u8 hsync_off_hi:2;
	u8 himage_lo;
	u8 vimage_lo;
	u8 vimage_hi:4;
	u8 himage_hi:4;
	u8 h_border;
	u8 v_border;
	u8 rsvd1:3;
	u8 digital:2;
	u8 vsync_positive:1;
	u8 hsync_positive:1;
	u8 non_interlaced:1;
} __packed;

/**
 * struct vbt_header - VBT Header structure
 * @signature:		VBT signature, always starts with "$VBT"
 * @version:		Version of this structure
 * @header_size:	Size of this structure
 * @vbt_size:		Size of VBT (VBT Header, BDB Header and data blocks)
 * @vbt_checksum:	Checksum
 * @reserved0:		Reserved
 * @bdb_offset:		Offset of &struct bdb_header from beginning of VBT
 * @aim_offset:		Offsets of add-in data blocks from beginning of VBT
 */
struct vbt_header {
	u8 signature[20];
	u16 version;
	u16 header_size;
	u16 vbt_size;
	u8 vbt_checksum;
	u8 reserved0;
	u32 bdb_offset;
	u32 aim_offset[4];
} __packed;

/**
 * struct bdb_header - BDB Header structure
 * @signature:		BDB signature "BIOS_DATA_BLOCK"
 * @version:		Version of the data block definitions
 * @header_size:	Size of this structure
 * @bdb_size:		Size of BDB (BDB Header and data blocks)
 */
struct bdb_header {
	u8 signature[16];
	u16 version;
	u16 header_size;
	u16 bdb_size;
} __packed;

/*
 * BDB version number dependencies are documented as:
 *
 * <start>+
 *    indicates the field was introduced in version <start>
 *    and is still valid
 *
 * <start>-<end>
 *    indicates the field was introduced in version <start>
 *    and obsoleted in version <end>+1.
 *
 * ??? indicates the specific version number is unknown
 */

/*
 * There are several types of BIOS data blocks (BDBs), each block has
 * an ID and size in the first 3 bytes (ID in first, size in next 2).
 * Known types are listed below.
 */
enum bdb_block_id {
	BDB_GENERAL_FEATURES		= 1,
	BDB_GENERAL_DEFINITIONS		= 2,
	BDB_DISPLAY_TOGGLE		= 3,
	BDB_MODE_SUPPORT_LIST		= 4,
	BDB_GENERIC_MODE_TABLE		= 5,
	BDB_EXT_MMIO_REGS		= 6, /* VBIOS only */
	BDB_SWF_IO			= 7, /* VBIOS only */
	BDB_SWF_MMIO			= 8, /* VBIOS only */
	BDB_DOT_CLOCK_OVERRIDE_ALM	= 9,
	BDB_PSR				= 9, /* 165+ */
	BDB_MODE_REMOVAL_TABLE		= 10,
	BDB_CHILD_DEVICE_TABLE		= 11,
	BDB_DRIVER_FEATURES		= 12,
	BDB_DRIVER_PERSISTENCE		= 13,
	BDB_EXT_TABLE_PTRS		= 14, /* VBIOS only */
	BDB_DOT_CLOCK_OVERRIDE		= 15,
	BDB_DISPLAY_SELECT_OLD		= 16,
	BDB_SV_TEST_FUNCTIONS		= 17,
	BDB_DRIVER_ROTATION		= 18,
	BDB_DISPLAY_REMOVE_OLD		= 19,
	BDB_OEM_CUSTOM			= 20,
	BDB_EFP_LIST			= 21, /* workarounds for VGA hsync/vsync */
	BDB_SDVO_LVDS_OPTIONS		= 22,
	BDB_SDVO_LVDS_DTD		= 23,
	BDB_SDVO_LVDS_PNP_ID		= 24,
	BDB_SDVO_LVDS_PPS		= 25,
	BDB_TV_OPTIONS			= 26,
	BDB_EDP				= 27,
	BDB_EFP_DTD			= 28, /* 161+ */
	BDB_DISPLAY_SELECT_IVB		= 29, /* 164+ */
	BDB_DISPLAY_REMOVE_IVB		= 30, /* 164+ */
	BDB_DISPLAY_SELECT_HSW		= 31, /* 166+ */
	BDB_DISPLAY_REMOVE_HSW		= 32, /* 166+ */
	BDB_LFP_OPTIONS			= 40,
	BDB_LFP_DATA_PTRS		= 41,
	BDB_LFP_DATA			= 42,
	BDB_LFP_BACKLIGHT		= 43,
	BDB_LFP_POWER			= 44,
	BDB_EDP_BFI			= 45, /* 160+ */
	BDB_CHROMATICITY		= 46, /* 169+ */
	BDB_MIPI			= 50, /* 170-172 */
	BDB_FIXED_SET_MODE		= 51, /* 172+ */
	BDB_MIPI_CONFIG			= 52, /* 175+ */
	BDB_MIPI_SEQUENCE		= 53, /* 177+ */
	BDB_RGB_PALETTE			= 54, /* 180+ */
	BDB_COMPRESSION_PARAMETERS_OLD	= 55, /* 198-212 */
	BDB_COMPRESSION_PARAMETERS	= 56, /* 213+ */
	BDB_VSWING_PREEMPH		= 57, /* 218+ */
	BDB_GENERIC_DTD			= 58, /* 229+ */
	BDB_INT15_HOOK			= 252, /* VBIOS only */
	BDB_PRD_TABLE			= 253,
	BDB_SKIP			= 254, /* VBIOS only */
};

/*
 * Block 1 - General Bit Definitions
 */

struct bdb_general_features {
        /* bits 1 */
	u8 panel_fitting:2;
	u8 flexaim:1;
	u8 msg_enable:1;
	u8 clear_screen:3;
	u8 color_flip:1;

        /* bits 2 */
	u8 download_ext_vbt:1;
	u8 enable_ssc:1;
	u8 ssc_freq:1;
	u8 enable_lfp_on_override:1;
	u8 disable_ssc_ddt:1;
	u8 underscan_vga_timings:1;
	u8 display_clock_mode:1;
	u8 vbios_hotplug_support:1;

        /* bits 3 */
	u8 disable_smooth_vision:1;
	u8 single_dvi:1;
	u8 rotate_180:1;					/* 181+ */
	u8 fdi_rx_polarity_inverted:1;
	u8 vbios_extended_mode:1;				/* 160+ */
	u8 copy_ilfp_dtd_to_sdvo_lvds_dtd:1;			/* 160+ */
	u8 panel_best_fit_timing:1;				/* 160+ */
	u8 ignore_strap_state:1;				/* 160+ */

        /* bits 4 */
	u8 legacy_monitor_detect;

        /* bits 5 */
	u8 int_crt_support:1;
	u8 int_tv_support:1;
	u8 int_efp_support:1;
	u8 dp_ssc_enable:1;	/* PCH attached eDP supports SSC */
	u8 dp_ssc_freq:1;	/* SSC freq for PCH attached eDP */
	u8 dp_ssc_dongle_supported:1;
	u8 rsvd11:2; /* finish byte */

	/* bits 6 */
	u8 tc_hpd_retry_timeout:7;				/* 242+ */
	u8 rsvd12:1;

	/* bits 7 */
	u8 afc_startup_config:2;				/* 249+ */
	u8 rsvd13:6;
} __packed;

/*
 * Block 2 - General Bytes Definition
 */

/* pre-915 */
#define GPIO_PIN_DVI_LVDS	0x03 /* "DVI/LVDS DDC GPIO pins" */
#define GPIO_PIN_ADD_I2C	0x05 /* "ADDCARD I2C GPIO pins" */
#define GPIO_PIN_ADD_DDC	0x04 /* "ADDCARD DDC GPIO pins" */
#define GPIO_PIN_ADD_DDC_I2C	0x06 /* "ADDCARD DDC/I2C GPIO pins" */

/* Device handle */
#define DEVICE_HANDLE_CRT	0x0001
#define DEVICE_HANDLE_TV	0x0002 /* ???-214 */
#define DEVICE_HANDLE_EFP1	0x0004
#define DEVICE_HANDLE_EFP2	0x0040
#define DEVICE_HANDLE_EFP3	0x0020
#define DEVICE_HANDLE_EFP4	0x0010
#define DEVICE_HANDLE_EFP5	0x0002 /* 215+ */
#define DEVICE_HANDLE_EFP6	0x0001 /* 217+ */
#define DEVICE_HANDLE_EFP7	0x0100 /* 217+ */
#define DEVICE_HANDLE_EFP8	0x0200 /* 217+ */
#define DEVICE_HANDLE_LFP1	0x0008
#define DEVICE_HANDLE_LFP2	0x0080

/* Pre 915 */
#define DEVICE_TYPE_NONE	0x00
#define DEVICE_TYPE_CRT		0x01
#define DEVICE_TYPE_TV		0x09
#define DEVICE_TYPE_EFP		0x12
#define DEVICE_TYPE_LFP		0x22
/* On 915+ */
#define DEVICE_TYPE_CRT_DPMS		0x6001
#define DEVICE_TYPE_CRT_DPMS_HOTPLUG	0x4001
#define DEVICE_TYPE_TV_COMPOSITE	0x0209
#define DEVICE_TYPE_TV_MACROVISION	0x0289
#define DEVICE_TYPE_TV_RF_COMPOSITE	0x020c
#define DEVICE_TYPE_TV_SVIDEO_COMPOSITE	0x0609
#define DEVICE_TYPE_TV_SCART		0x0209
#define DEVICE_TYPE_TV_CODEC_HOTPLUG_PWR 0x6009
#define DEVICE_TYPE_EFP_HOTPLUG_PWR	0x6012
#define DEVICE_TYPE_EFP_DVI_HOTPLUG_PWR	0x6052
#define DEVICE_TYPE_EFP_DVI_I		0x6053
#define DEVICE_TYPE_EFP_DVI_D_DUAL	0x6152
#define DEVICE_TYPE_EFP_DVI_D_HDCP	0x60d2
#define DEVICE_TYPE_OPENLDI_HOTPLUG_PWR	0x6062
#define DEVICE_TYPE_OPENLDI_DUALPIX	0x6162
#define DEVICE_TYPE_LFP_PANELLINK	0x5012
#define DEVICE_TYPE_LFP_CMOS_PWR	0x5042
#define DEVICE_TYPE_LFP_LVDS_PWR	0x5062
#define DEVICE_TYPE_LFP_LVDS_DUAL	0x5162
#define DEVICE_TYPE_LFP_LVDS_DUAL_HDCP	0x51e2

/* Add the device class for LFP, TV, HDMI */
#define DEVICE_TYPE_INT_LFP		0x1022
#define DEVICE_TYPE_INT_TV		0x1009
#define DEVICE_TYPE_HDMI		0x60D2
#define DEVICE_TYPE_DP			0x68C6
#define DEVICE_TYPE_DP_DUAL_MODE	0x60D6
#define DEVICE_TYPE_eDP			0x78C6

#define DEVICE_TYPE_CLASS_EXTENSION	(1 << 15)
#define DEVICE_TYPE_POWER_MANAGEMENT	(1 << 14)
#define DEVICE_TYPE_HOTPLUG_SIGNALING	(1 << 13)
#define DEVICE_TYPE_INTERNAL_CONNECTOR	(1 << 12)
#define DEVICE_TYPE_NOT_HDMI_OUTPUT	(1 << 11)
#define DEVICE_TYPE_MIPI_OUTPUT		(1 << 10)
#define DEVICE_TYPE_COMPOSITE_OUTPUT	(1 << 9)
#define DEVICE_TYPE_DUAL_CHANNEL	(1 << 8)
#define DEVICE_TYPE_HIGH_SPEED_LINK	(1 << 6)
#define DEVICE_TYPE_LVDS_SIGNALING	(1 << 5)
#define DEVICE_TYPE_TMDS_DVI_SIGNALING	(1 << 4)
#define DEVICE_TYPE_VIDEO_SIGNALING	(1 << 3)
#define DEVICE_TYPE_DISPLAYPORT_OUTPUT	(1 << 2)
#define DEVICE_TYPE_DIGITAL_OUTPUT	(1 << 1)
#define DEVICE_TYPE_ANALOG_OUTPUT	(1 << 0)

#define DEVICE_CFG_NONE		0x00
#define DEVICE_CFG_12BIT_DVOB	0x01
#define DEVICE_CFG_12BIT_DVOC	0x02
#define DEVICE_CFG_24BIT_DVOBC	0x09
#define DEVICE_CFG_24BIT_DVOCB	0x0a
#define DEVICE_CFG_DUAL_DVOB	0x11
#define DEVICE_CFG_DUAL_DVOC	0x12
#define DEVICE_CFG_DUAL_DVOBC	0x13
#define DEVICE_CFG_DUAL_LINK_DVOBC	0x19
#define DEVICE_CFG_DUAL_LINK_DVOCB	0x1a

#define DEVICE_WIRE_NONE	0x00
#define DEVICE_WIRE_DVOB	0x01
#define DEVICE_WIRE_DVOC	0x02
#define DEVICE_WIRE_DVOBC	0x03
#define DEVICE_WIRE_DVOBB	0x05
#define DEVICE_WIRE_DVOCC	0x06
#define DEVICE_WIRE_DVOB_MASTER 0x0d
#define DEVICE_WIRE_DVOC_MASTER 0x0e

/* dvo_port pre BDB 155 */
#define DEVICE_PORT_DVOA	0x00 /* none on 845+ */
#define DEVICE_PORT_DVOB	0x01
#define DEVICE_PORT_DVOC	0x02

/* dvo_port BDB 155+ */
#define DVO_PORT_HDMIA		0
#define DVO_PORT_HDMIB		1
#define DVO_PORT_HDMIC		2
#define DVO_PORT_HDMID		3
#define DVO_PORT_LVDS		4
#define DVO_PORT_TV		5
#define DVO_PORT_CRT		6
#define DVO_PORT_DPB		7
#define DVO_PORT_DPC		8
#define DVO_PORT_DPD		9
#define DVO_PORT_DPA		10
#define DVO_PORT_DPE		11				/* 193+ */
#define DVO_PORT_HDMIE		12				/* 193+ */
#define DVO_PORT_DPF		13				/* N/A */
#define DVO_PORT_HDMIF		14				/* N/A */
#define DVO_PORT_DPG		15				/* 217+ */
#define DVO_PORT_HDMIG		16				/* 217+ */
#define DVO_PORT_DPH		17				/* 217+ */
#define DVO_PORT_HDMIH		18				/* 217+ */
#define DVO_PORT_DPI		19				/* 217+ */
#define DVO_PORT_HDMII		20				/* 217+ */
#define DVO_PORT_MIPIA		21				/* 171+ */
#define DVO_PORT_MIPIB		22				/* 171+ */
#define DVO_PORT_MIPIC		23				/* 171+ */
#define DVO_PORT_MIPID		24				/* 171+ */

#define HDMI_MAX_DATA_RATE_PLATFORM	0			/* 204+ */
#define HDMI_MAX_DATA_RATE_297		1			/* 204+ */
#define HDMI_MAX_DATA_RATE_165		2			/* 204+ */
#define HDMI_MAX_DATA_RATE_594		3			/* 249+ */
#define HDMI_MAX_DATA_RATE_340		4			/* 249+ */
#define HDMI_MAX_DATA_RATE_300		5			/* 249+ */

#define LEGACY_CHILD_DEVICE_CONFIG_SIZE		33

/* DDC Bus DDI Type 155+ */
enum vbt_gmbus_ddi {
	DDC_BUS_DDI_B = 0x1,
	DDC_BUS_DDI_C,
	DDC_BUS_DDI_D,
	DDC_BUS_DDI_F,
	ICL_DDC_BUS_DDI_A = 0x1,
	ICL_DDC_BUS_DDI_B,
	TGL_DDC_BUS_DDI_C,
	RKL_DDC_BUS_DDI_D = 0x3,
	RKL_DDC_BUS_DDI_E,
	ICL_DDC_BUS_PORT_1 = 0x4,
	ICL_DDC_BUS_PORT_2,
	ICL_DDC_BUS_PORT_3,
	ICL_DDC_BUS_PORT_4,
	TGL_DDC_BUS_PORT_5,
	TGL_DDC_BUS_PORT_6,
	ADLS_DDC_BUS_PORT_TC1 = 0x2,
	ADLS_DDC_BUS_PORT_TC2,
	ADLS_DDC_BUS_PORT_TC3,
	ADLS_DDC_BUS_PORT_TC4,
	ADLP_DDC_BUS_PORT_TC1 = 0x3,
	ADLP_DDC_BUS_PORT_TC2,
	ADLP_DDC_BUS_PORT_TC3,
	ADLP_DDC_BUS_PORT_TC4

};

#define DP_AUX_A 0x40
#define DP_AUX_B 0x10
#define DP_AUX_C 0x20
#define DP_AUX_D 0x30
#define DP_AUX_E 0x50
#define DP_AUX_F 0x60
#define DP_AUX_G 0x70
#define DP_AUX_H 0x80
#define DP_AUX_I 0x90

/* DP max link rate 216+ */
#define BDB_216_VBT_DP_MAX_LINK_RATE_HBR3	0
#define BDB_216_VBT_DP_MAX_LINK_RATE_HBR2	1
#define BDB_216_VBT_DP_MAX_LINK_RATE_HBR	2
#define BDB_216_VBT_DP_MAX_LINK_RATE_LBR	3

/* DP max link rate 230+ */
#define BDB_230_VBT_DP_MAX_LINK_RATE_DEF	0
#define BDB_230_VBT_DP_MAX_LINK_RATE_LBR	1
#define BDB_230_VBT_DP_MAX_LINK_RATE_HBR	2
#define BDB_230_VBT_DP_MAX_LINK_RATE_HBR2	3
#define BDB_230_VBT_DP_MAX_LINK_RATE_HBR3	4
#define BDB_230_VBT_DP_MAX_LINK_RATE_UHBR10	5
#define BDB_230_VBT_DP_MAX_LINK_RATE_UHBR13P5	6
#define BDB_230_VBT_DP_MAX_LINK_RATE_UHBR20	7

/* EDP link rate 263+ */
#define BDB_263_VBT_EDP_LINK_RATE_1_62		BIT_U32(0)
#define BDB_263_VBT_EDP_LINK_RATE_2_16		BIT_U32(1)
#define BDB_263_VBT_EDP_LINK_RATE_2_43		BIT_U32(2)
#define BDB_263_VBT_EDP_LINK_RATE_2_7		BIT_U32(3)
#define BDB_263_VBT_EDP_LINK_RATE_3_24		BIT_U32(4)
#define BDB_263_VBT_EDP_LINK_RATE_4_32		BIT_U32(5)
#define BDB_263_VBT_EDP_LINK_RATE_5_4		BIT_U32(6)
#define BDB_263_VBT_EDP_LINK_RATE_6_75		BIT_U32(7)
#define BDB_263_VBT_EDP_LINK_RATE_8_1		BIT_U32(8)
#define BDB_263_VBT_EDP_LINK_RATE_10		BIT_U32(9)
#define BDB_263_VBT_EDP_LINK_RATE_13_5		BIT_U32(10)
#define BDB_263_VBT_EDP_LINK_RATE_20		BIT_U32(11)
#define BDB_263_VBT_EDP_NUM_RATES		12
#define BDB_263_VBT_EDP_RATES_MASK		GENMASK(BDB_263_VBT_EDP_NUM_RATES - 1, 0)

/*
 * The child device config, aka the display device data structure, provides a
 * description of a port and its configuration on the platform.
 *
 * The child device config size has been increased, and fields have been added
 * and their meaning has changed over time. Care must be taken when accessing
 * basically any of the fields to ensure the correct interpretation for the BDB
 * version in question.
 *
 * When we copy the child device configs to display->vbt.child_dev, we
 * reserve space for the full structure below, and initialize the tail not
 * actually present in VBT to zeros. Accessing those fields is fine, as long as
 * the default zero is taken into account, again according to the BDB version.
 *
 * BDB versions 155 and below are considered legacy, and version 155 seems to be
 * a baseline for some of the VBT documentation. When adding new fields, please
 * include the BDB version when the field was added, if it's above that.
 */
struct child_device_config {
	u16 handle;
	u16 device_type; /* See DEVICE_TYPE_* above */

	union {
		u8  device_id[10]; /* ascii string */
		struct {
			u8 i2c_speed;
			u8 dp_onboard_redriver_preemph:3;	/* 158+ */
			u8 dp_onboard_redriver_vswing:3;	/* 158+ */
			u8 dp_onboard_redriver_present:1;	/* 158+ */
			u8 reserved0:1;
			u8 dp_ondock_redriver_preemph:3;	/* 158+ */
			u8 dp_ondock_redriver_vswing:3;		/* 158+ */
			u8 dp_ondock_redriver_present:1;	/* 158+ */
			u8 reserved1:1;
			u8 hdmi_level_shifter_value:5;		/* 158+ */
			u8 hdmi_max_data_rate:3;		/* 204+ */
			u16 dtd_buf_ptr;			/* 161+ */
			u8 edidless_efp:1;			/* 161+ */
			u8 compression_enable:1;		/* 198+ */
			u8 compression_method_cps:1;		/* 198+ */
			u8 ganged_edp:1;			/* 202+ */
			u8 lttpr_non_transparent:1;		/* 235+ */
			u8 disable_compression_for_ext_disp:1;	/* 251+ */
			u8 reserved2:2;
			u8 compression_structure_index:4;	/* 198+ */
			u8 reserved3:4;
			u8 hdmi_max_frl_rate:4;			/* 237+ */
			u8 hdmi_max_frl_rate_valid:1;		/* 237+ */
			u8 reserved4:3;				/* 237+ */
			u8 reserved5;
		} __packed;
	} __packed;

	u16 addin_offset;
	u8 dvo_port; /* See DEVICE_PORT_* and DVO_PORT_* above */
	u8 i2c_pin;
	u8 target_addr;
	u8 ddc_pin;
	u16 edid_ptr;
	u8 dvo_cfg; /* See DEVICE_CFG_* above */

	union {
		struct {
			u8 dvo2_port;
			u8 i2c2_pin;
			u8 target2_addr;
			u8 ddc2_pin;
		} __packed;
		struct {
			u8 efp_routed:1;			/* 158+ */
			u8 lane_reversal:1;			/* 184+ */
			u8 lspcon:1;				/* 192+ */
			u8 iboost:1;				/* 196+ */
			u8 hpd_invert:1;			/* 196+ */
			u8 use_vbt_vswing:1;			/* 218+ */
			u8 dp_max_lane_count:2;			/* 244+ */
			u8 hdmi_support:1;			/* 158+ */
			u8 dp_support:1;			/* 158+ */
			u8 tmds_support:1;			/* 158+ */
			u8 support_reserved:5;
			u8 aux_channel;
			u8 dongle_detect;
		} __packed;
	} __packed;

	u8 pipe_cap:2;
	u8 sdvo_stall:1;					/* 158+ */
	u8 hpd_status:2;
	u8 integrated_encoder:1;
	u8 capabilities_reserved:2;
	u8 dvo_wiring; /* See DEVICE_WIRE_* above */

	union {
		u8 dvo2_wiring;
		u8 mipi_bridge_type;				/* 171+ */
	} __packed;

	u16 extended_type;
	u8 dvo_function;
	u8 dp_usb_type_c:1;					/* 195+ */
	u8 tbt:1;						/* 209+ */
	u8 dedicated_external:1;				/* 264+ */
	u8 dyn_port_over_tc:1;					/* 264+ */
	u8 dp_port_trace_length:4;				/* 209+ */
	u8 dp_gpio_index;					/* 195+ */
	u16 dp_gpio_pin_num;					/* 195+ */
	u8 dp_iboost_level:4;					/* 196+ */
	u8 hdmi_iboost_level:4;					/* 196+ */
	u8 dp_max_link_rate:3;					/* 216+ */
	u8 dp_max_link_rate_reserved:5;				/* 216+ */
	u8 efp_index;						/* 256+ */
	u32 edp_data_rate_override:12;				/* 263+ */
	u32 edp_data_rate_override_reserved:20;			/* 263+ */
} __packed;

struct bdb_general_definitions {
	/* DDC GPIO */
	u8 crt_ddc_gmbus_pin;

	/* DPMS bits */
	u8 dpms_non_acpi:1;
	u8 skip_boot_crt_detect:1;
	u8 dpms_aim:1;
	u8 rsvd1:5; /* finish byte */

	/* boot device bits */
	u8 boot_display[2];
	u8 child_dev_size;

	/*
	 * Device info:
	 * If TV is present, it'll be at devices[0].
	 * LVDS will be next, either devices[0] or [1], if present.
	 * On some platforms the number of device is 6. But could be as few as
	 * 4 if both TV and LVDS are missing.
	 * And the device num is related with the size of general definition
	 * block. It is obtained by using the following formula:
	 * number = (block_size - sizeof(bdb_general_definitions))/
	 *	     defs->child_dev_size;
	 */
	u8 devices[];
} __packed;

/*
 * Block 3 - Display Toggle Option Block
 */

struct bdb_display_toggle {
	u8 feature_bits;
	u16 num_entries;					/* ALM only */
	u16 list[];						/* ALM only */
} __packed;

/*
 * Block 4 - Mode Support List
 */

struct bdb_mode_support_list {
	u8 intel_mode_number[0];
	u16 mode_list_length;
} __packed;

/*
 * Block 5 - Generic Mode Table
 */

struct generic_mode_table {
	u16 x_res;
	u16 y_res;
	u8 color_depths;
	u8 refresh_rate[3];
	u8 reserved;
	u8 text_cols;
	u8 text_rows;
	u8 font_height;
	u16 page_size;
	u8 misc;
} __packed;

struct generic_mode_timings {
	u32 dotclock_khz;
	u16 hdisplay;
	u16 htotal;
	u16 hblank_start;
	u16 hblank_end;
	u16 hsync_start;
	u16 hsync_end;
	u16 vdisplay;
	u16 vtotal;
	u16 vblank_start;
	u16 vblank_end;
	u16 vsync_start;
	u16 vsync_end;
} __packed;

struct generic_mode_timings_alm {
	struct generic_mode_timings timings;
	u8 wm_8bpp;
	u8 burst_8bpp;
	u8 wm_16bpp;
	u8 burst_16bpp;
	u8 wm_32bpp;
	u8 burst_32bpp;
} __packed;

struct bdb_generic_mode_table_alm {
	struct generic_mode_table table;
	struct generic_mode_timings_alm timings[3];
} __packed;

struct bdb_generic_mode_table_mgm {
	u16 mode_flag;
	struct generic_mode_table table;
	struct generic_mode_timings timings[3];
} __packed;

/*
 * Block 6 - Extended MMIO Register Table, VBIOS only
 * Block 7 - IO Software Flag Table, VBIOS only
 * Block 8 - MMIO SWF Register Table, VBIOS only
 */
struct bdb_reg_table {
	u16 table_id;
	u8 data_access_size;
	/*
	 * offset,value tuples:
	 * data_access_size==0xce -> u8,u8
	 * data_access_size==0x02 -> u32,u32
	 */
	/* u16 table_end_marker; */
} __packed;

/*
 * Block 9 - Undocumented table (ALM only)
 */

struct dot_clock_override_entry_gen2 {
	u32 dotclock;
	u8 n;
	u8 m1;
	u8 m2;
	u8 p1:5;
	u8 p1_div_by_2:1;
	u8 reserved:1;
	u8 p2_div_by_4:1;
} __packed;

struct bdb_dot_clock_override_alm {
	struct dot_clock_override_entry_gen2 t[0];
} __packed;

/*
 * Block 9 - SRD Feature Block
 */

struct psr_table {
	/* Feature bits */
	u8 full_link:1;						/* 165+ */
	u8 require_aux_to_wakeup:1;				/* 165+ */
	u8 feature_bits_rsvd:6;

	/* Wait times */
	u8 idle_frames:4;					/* 165+ */
	u8 lines_to_wait:3;					/* 165+ */
	u8 wait_times_rsvd:1;

	/* TP wake up time in multiple of 100 */
	u16 tp1_wakeup_time;					/* 165+ */
	u16 tp2_tp3_wakeup_time;				/* 165+ */
} __packed;

struct bdb_psr {
	struct psr_table psr_table[16];

	/* PSR2 TP2/TP3 wakeup time for 16 panels */
	u32 psr2_tp2_tp3_wakeup_time;				/* 226+ */
} __packed;

/*
 * Block 10 - Mode Removal Table
 */

struct mode_removal_table {
	u16 x_res;
	u16 y_res;
	u8 bpp;
	u16 refresh_rate;
	u8 removal_flags;
	u16 panel_flags;
} __packed;

struct bdb_mode_removal {
	u8 row_size; /* 8 or 10 bytes */
	/*
	 * VBT spec says this is always 20 entries,
	 * but ALM seems to have only 15 entries.
	 */
	struct mode_removal_table modes[];
	/* u16 terminator; 0x0000 */
} __packed;

/*
 * Block 12 - Driver Features Data Block
 */

#define BDB_DRIVER_FEATURE_NO_LVDS		0
#define BDB_DRIVER_FEATURE_INT_LVDS		1
#define BDB_DRIVER_FEATURE_SDVO_LVDS		2
#define BDB_DRIVER_FEATURE_INT_SDVO_LVDS	3

struct bdb_driver_features {
	/* Driver bits */
	u8 boot_dev_algorithm:1;
	u8 allow_display_switch_dvd:1;
	u8 allow_display_switch_dos:1;
	u8 hotplug_dvo:1;
	u8 dual_view_zoom:1;
	u8 int15h_hook:1;
	u8 sprite_in_clone:1;
	u8 primary_lfp_id:1;

	u16 boot_mode_x;
	u16 boot_mode_y;
	u8 boot_mode_bpp;
	u8 boot_mode_refresh;

	/* Extended Driver Bits 1 */
	u16 enable_lfp_primary:1;
	u16 selective_mode_pruning:1;
	u16 dual_frequency:1;
	u16 render_clock_freq:1; /* 0: high freq; 1: low freq */
	u16 nt_clone_support:1;
	u16 power_scheme_ui:1; /* 0: CUI; 1: 3rd party */
	u16 sprite_display_assign:1; /* 0: secondary; 1: primary */
	u16 cui_aspect_scaling:1;
	u16 preserve_aspect_ratio:1;
	u16 sdvo_device_power_down:1;
	u16 crt_hotplug:1;
	u16 lvds_config:2;
	u16 tv_hotplug:1;
	u16 hdmi_config:2;

	/* Driver Flags 1 */
	u8 static_display:1;					/* 163+ */
	u8 embedded_platform:1;					/* 163+ */
	u8 display_subsystem_enable:1;				/* 163+ */
	u8 reserved0:5;

	u16 legacy_crt_max_x;
	u16 legacy_crt_max_y;
	u8 legacy_crt_max_refresh;

	/* Extended Driver Bits 2 */
	u8 hdmi_termination:1;
	u8 cea861d_hdmi_support:1;
	u8 self_refresh_enable:1;
	u8 reserved1:5;

	u8 custom_vbt_version;					/* 155+ */

	/* Driver Feature Flags */
	u16 rmpm_enabled:1;					/* 159+ */
	u16 s2ddt_enabled:1;					/* 159+ */
	u16 dpst_enabled:1;					/* 159-227 */
	u16 bltclt_enabled:1;					/* 159+ */
	u16 adb_enabled:1;					/* 159-227 */
	u16 drrs_enabled:1;					/* 159-227 */
	u16 grs_enabled:1;					/* 159+ */
	u16 gpmt_enabled:1;					/* 159+ */
	u16 tbt_enabled:1;					/* 159+ */
	u16 psr_enabled:1;					/* 165-227 */
	u16 ips_enabled:1;					/* 165+ */
	u16 dfps_enabled:1;					/* 165+ */
	u16 dmrrs_enabled:1;					/* 174-227 */
	u16 adt_enabled:1;					/* ???-228 */
	u16 hpd_wake:1;						/* 201-240 */
	u16 pc_feature_valid:1;					/* 159+ */
} __packed;

/*
 * Block 13 - Driver Persistent Algorithm
 */

struct bdb_driver_persistence {
	u16 hotkey_persistent_algorithm:1;
	u16 lid_switch_persistent_algorithm:1;
	u16 power_management_persistent_algorithm:1;
	u16 hotkey_persistent_on_mds_twin:1;
	u16 hotkey_persistent_on_refresh_rate:1;
	u16 hotkey_persistent_on_restore_pipe:1;
	u16 hotkey_persistent_on_mode:1;
	u16 edid_persistent_on_mode:1;
	u16 dvo_hotplug_persistent_on_mode:1;
	u16 docking_persistent_algorithm:1;
	u16 rsvd:6;
	u8 persistent_max_config;
} __packed;

/*
 * Block 15 - Dot Clock Override Table
 */

struct dot_clock_override_entry_gen3 {
	u32 dotclock;
	u8 n;
	u8 m1;
	u8 m2;
	u8 p1;
	u8 p2;
} __packed;

struct bdb_dot_clock_override {
	u8 row_size; /* 8 == gen2, 9 == gen3+ */
	u8 num_rows;
	struct dot_clock_override_entry_gen3 table[]; /* or _gen2 */
} __packed;

/*
 * Block 16 - Toggle List Block (pre-HSW)
 */

struct toggle_list_entry_old {
	u8 display_select_pipe_a;
	u8 display_select_pipe_b;
	u8 caps;
} __packed;

struct toggle_list_table_old {
	u16 num_entries;
	u8 entry_size;
	struct toggle_list_entry_old list[];
} __packed;

struct bdb_display_select_old {
	/* each table has variable size! */
	struct toggle_list_table_old tables[4];
} __packed;

/*
 * Block 17 - SV Test Functions
 */

struct bdb_sv_test_functions {
	u8 sv_bits[8];
} __packed;

/*
 * Block 18 - Driver Rotation
 */

struct bdb_driver_rotation {
	u8 rotation_enable;
	u8 rotation_flags_1;
	u16 rotation_flags_2;
	u32 rotation_flags_3;
	u32 rotation_flags_4;
} __packed;

/*
 * Block 19 - Display Configuration Removal Table (pre-IVB)
 */

struct display_remove_entry_old {
	u8 display_select_pipe_a;
	u8 display_select_pipe_b;
} __packed;

struct bdb_display_remove_old {
	u8 num_entries;
	u8 entry_size;
	struct display_remove_entry_old table[];
} __packed;

/*
 * Block 20 - OEM Customizable Modes
 */

struct oem_mode {
	u8 enable_in_vbios:1;
	u8 enable_in_os:1;
	u8 enable_in_gop:1;					/* 207+ */
	u8 reserved:5;
	u8 display_flags;					/* ???-216 */
	u16 x_res;
	u16 y_res;
	u8 color_depth;
	u8 refresh_rate;
	struct bdb_edid_dtd dtd;
	u16 display_flags_2;					/* 217+ */
} __packed;

struct bdb_oem_custom {
	u8 num_entries;
	u8 entry_size;
	struct oem_mode modes[];
} __packed;

/*
 * Block 21 - EFP List
 */

struct efp_entry {
	u16 mfg_name;
	u16 product_code;
} __packed;

struct bdb_efp_list {
	u8 num_entries;
	u8 entry_size;
	struct efp_entry efp[];
} __packed;

/*
 * Block 22 - SDVO LVDS General Options
 */

struct bdb_sdvo_lvds_options {
	u8 panel_backlight;
	u8 h40_set_panel_type;
	u8 panel_type;
	u8 ssc_clk_freq;
	u16 als_low_trip;
	u16 als_high_trip;
	u8 sclalarcoeff_tab_row_num;
	u8 sclalarcoeff_tab_row_size;
	u8 coefficient[8];
	u8 panel_misc_bits_1;
	u8 panel_misc_bits_2;
	u8 panel_misc_bits_3;
	u8 panel_misc_bits_4;
} __packed;

/*
 * Block 23 - SDVO LVDS DTD
 */

struct bdb_sdvo_lvds_dtd {
	struct bdb_edid_dtd dtd[4];
} __packed;

/*
 * Block 24 - SDVO LVDS PnP ID
 */

struct bdb_sdvo_lvds_pnp_id {
	struct bdb_edid_pnp_id pnp_id[4];
} __packed;

/*
 * Block 25 - SDVO LVDS PPS
 */

struct sdvo_lvds_pps {
	u16 t0; /* power on */
	u16 t1; /* backlight on */
	u16 t2; /* backlight off */
	u16 t3; /* power off */
	u16 t4; /* power cycle */
} __packed;

struct bdb_sdvo_lvds_pps {
	struct sdvo_lvds_pps pps[4];
} __packed;

/*
 * Block 26 - TV Options Block
 */

struct bdb_tv_options {
	u16 underscan_overscan_hdtv_component:2;
	u16 rsvd1:10;
	u16 underscan_overscan_hdtv_dvi:2;
	u16 add_modes_to_avoid_overscan_issue:1;
	u16 d_connector_support:1;
} __packed;

/*
 * Block 27 - eDP VBT Block
 */

struct edp_power_seq {
	u16 t1_t3;
	u16 t8;
	u16 t9;
	u16 t10;
	u16 t11_t12;
} __packed;

#define EDP_18BPP	0
#define EDP_24BPP	1
#define EDP_30BPP	2
#define EDP_RATE_1_62	0
#define EDP_RATE_2_7	1
#define EDP_RATE_5_4	2
#define EDP_LANE_1	0
#define EDP_LANE_2	1
#define EDP_LANE_4	3
#define EDP_PREEMPHASIS_NONE	0
#define EDP_PREEMPHASIS_3_5dB	1
#define EDP_PREEMPHASIS_6dB	2
#define EDP_PREEMPHASIS_9_5dB	3
#define EDP_VSWING_0_4V		0
#define EDP_VSWING_0_6V		1
#define EDP_VSWING_0_8V		2
#define EDP_VSWING_1_2V		3


struct edp_fast_link_params {
	u8 rate:4;						/* ???-223 */
	u8 lanes:4;
	u8 preemphasis:4;
	u8 vswing:4;
} __packed;

struct edp_pwm_delays {
	u16 pwm_on_to_backlight_enable;
	u16 backlight_disable_to_pwm_off;
} __packed;

struct edp_full_link_params {
	u8 preemphasis:4;
	u8 vswing:4;
} __packed;

struct edp_apical_params {
	u32 panel_oui;
	u32 dpcd_base_address;
	u32 dpcd_idridix_control_0;
	u32 dpcd_option_select;
	u32 dpcd_backlight;
	u32 ambient_light;
	u32 backlight_scale;
} __packed;

struct bdb_edp {
	struct edp_power_seq power_seqs[16];
	u32 color_depth;
	struct edp_fast_link_params fast_link_params[16];
	u32 sdrrs_msa_timing_delay;

	/* ith bit indicates enabled/disabled for (i+1)th panel */
	u16 edp_s3d_feature;					/* 162+ */
	u16 edp_t3_optimization;				/* 165+ */
	u64 edp_vswing_preemph;					/* 173+ */
	u16 fast_link_training;					/* 182+ */
	u16 dpcd_600h_write_required;				/* 185+ */
	struct edp_pwm_delays pwm_delays[16];			/* 186+ */
	u16 full_link_params_provided;				/* 199+ */
	struct edp_full_link_params full_link_params[16];	/* 199+ */
	u16 apical_enable;					/* 203+ */
	struct edp_apical_params apical_params[16];		/* 203+ */
	u16 edp_fast_link_training_rate[16];			/* 224+ */
	u16 edp_max_port_link_rate[16];				/* 244+ */
	u16 edp_dsc_disable;					/* 251+ */
	u16 t6_delay_support;					/* 260+ */
	u16 link_idle_time[16];					/* 260+ */
	u16 pipe_joiner_enable;					/* 261+ */
} __packed;

/*
 * Block 28 - EFP DTD Block
 */

struct bdb_efp_dtd {
	struct bdb_edid_dtd dtd[3];
} __packed;

/*
 * Block 29 - Toggle List Block (IVB)
 */

struct toggle_list_entry_ivb {
	u8 display_select;
} __packed;

struct toggle_list_table_ivb {
	u16 num_entries;
	u8 entry_size;
	struct toggle_list_entry_ivb list[];
} __packed;

struct bdb_display_select_ivb {
	/* each table has variable size! */
	struct toggle_list_table_ivb tables[4];
} __packed;

/*
 * Block 30 - Display Configuration Removal Table (IVB)
 */

struct display_remove_entry_ivb {
	u8 display_select;
} __packed;

struct bdb_display_remove_ivb {
	u8 num_entries;
	u8 entry_size;
	struct display_remove_entry_ivb table[];
} __packed;

/*
 * Block 31 - Toggle List Block (HSW+)
 */

struct toggle_list_entry_hsw {
	u16 display_select;
} __packed;

struct toggle_list_table_hsw {
	u16 num_entries;
	u8 entry_size;
	struct toggle_list_entry_hsw list[];
} __packed;

struct bdb_display_select_hsw {
	/* each table has variable size! */
	struct toggle_list_table_hsw tables[4];
} __packed;

/*
 * Block 32 - Display Configuration Removal Table (HSW+)
 */

struct display_remove_entry_hsw {
	u16 display_select;
} __packed;

struct bdb_display_remove_hsw {
	u8 num_entries;
	u8 entry_size;
	struct display_remove_entry_hsw table[];
} __packed;

/*
 * Block 40 - LFP Data Block
 */

struct bdb_lfp_options {
	u8 panel_type;
	u8 panel_type2;						/* 212+ */
	/* LVDS capabilities, stored in a dword */
	u8 pfit_mode:2;
	u8 pfit_text_mode_enhanced:1;
	u8 pfit_gfx_mode_enhanced:1;
	u8 pfit_ratio_auto:1;
	u8 pixel_dither:1;
	u8 lvds_edid:1;						/* ???-240 */
	u8 rsvd2:1;
	u8 rsvd4;
	/* LVDS Panel channel bits stored here */
	u32 lvds_panel_channel_bits;
	/* LVDS SSC (Spread Spectrum Clock) bits stored here. */
	u16 ssc_bits;
	u16 ssc_freq;
	u16 ssc_ddt;
	/* Panel color depth defined here */
	u16 panel_color_depth;
	/* LVDS panel type bits stored here */
	u32 dps_panel_type_bits;
	/* LVDS backlight control type bits stored here */
	u32 blt_control_type_bits;				/* ???-240 */

	u16 lcdvcc_s0_enable;					/* 200+ */
	u32 rotation;						/* 228+ */
	u32 position;						/* 240+ */
} __packed;

/*
 * Block 41 - LFP Data Table Pointers
 */
struct lfp_data_ptr_table {
	u16 offset; /* offsets are from start of bdb */
	u8 table_size;
} __packed;

/* LFP pointer table contains entries to the struct below */
struct lfp_data_ptr {
	struct lfp_data_ptr_table fp_timing;
	struct lfp_data_ptr_table dvo_timing;
	struct lfp_data_ptr_table panel_pnp_id;
} __packed;

struct bdb_lfp_data_ptrs {
	u8 num_entries;
	struct lfp_data_ptr ptr[16];
	struct lfp_data_ptr_table panel_name;		/* (156-163?)+ */
} __packed;

/*
 * Block 42 - LFP Data Tables
 */

/* LFP data has 3 blocks per entry */
struct fp_timing {
	u16 x_res;
	u16 y_res;
	u32 lvds_reg;
	u32 lvds_reg_val;
	u32 pp_on_reg;
	u32 pp_on_reg_val;
	u32 pp_off_reg;
	u32 pp_off_reg_val;
	u32 pp_cycle_reg;
	u32 pp_cycle_reg_val;
	u32 pfit_reg;
	u32 pfit_reg_val;
	u16 terminator;
} __packed;

/*
 * For reference only. fp_timing has variable size so
 * the data must be accessed using the data table pointers.
 * Do not use this directly!
 */
struct lfp_data_entry {
	struct fp_timing fp_timing;
	struct bdb_edid_dtd dvo_timing;
	struct bdb_edid_pnp_id pnp_id;
} __packed;

struct bdb_lfp_data {
	struct lfp_data_entry data[16];
} __packed;

struct lfp_black_border {
	u8 top;		/* 227+ */
	u8 bottom;	/* 227+ */
	u8 left;	/* 238+ */
	u8 right;	/* 238+ */
} __packed;

struct bdb_lfp_data_tail {
	struct bdb_edid_product_name panel_name[16];		/* (156-163?)+ */
	u16 scaling_enable;					/* 187+ */
	u8 seamless_drrs_min_refresh_rate[16];			/* 188+ */
	u8 pixel_overlap_count[16];				/* 208+ */
	struct lfp_black_border black_border[16];		/* 227+ */
	u16 dual_lfp_port_sync_enable;				/* 231+ */
	u16 gpu_dithering_for_banding_artifacts;		/* 245+ */
} __packed;

/*
 * Block 43 - LFP Backlight Control Data Block
 */

#define BDB_BACKLIGHT_TYPE_NONE	0
#define BDB_BACKLIGHT_TYPE_PWM	2

struct lfp_backlight_data_entry {
	u8 type:2;
	u8 active_low_pwm:1;
	u8 i2c_pin:3;						/* obsolete since ? */
	u8 i2c_speed:2;						/* obsolete since ? */
	u16 pwm_freq_hz;
	u8 min_brightness;					/* ???-233 */
	u8 i2c_address;						/* obsolete since ? */
	u8 i2c_command;						/* obsolete since ? */
} __packed;

struct lfp_backlight_control_method {
	u8 type:4;
	u8 controller:4;
} __packed;

struct lfp_brightness_level {
	u16 level;
	u16 reserved;
} __packed;

struct bdb_lfp_backlight {
	u8 entry_size;
	struct lfp_backlight_data_entry data[16];
	u8 level[16];							/* 162-233 */
	struct lfp_backlight_control_method backlight_control[16];	/* 191+ */
	struct lfp_brightness_level brightness_level[16];		/* 234+ */
	struct lfp_brightness_level brightness_min_level[16];		/* 234+ */
	u8 brightness_precision_bits[16];				/* 236+ */
	u16 hdr_dpcd_refresh_timeout[16];				/* 239+ */
} __packed;

/*
 * Block 44 - LFP Power Conservation Features Block
 */
struct lfp_power_features {
	u8 dpst_support:1;						/* ???-159 */
	u8 power_conservation_pref:3;
	u8 reserved2:1;
	u8 lace_enabled_status:1;					/* 210+ */
	u8 lace_support:1;						/* 210+ */
	u8 als_enable:1;
} __packed;

struct als_data_entry {
	u16 backlight_adjust;
	u16 lux;
} __packed;

struct aggressiveness_profile_entry {
	u8 dpst_aggressiveness : 4;		/* (228/252)-256 */
	u8 lace_aggressiveness : 4;
} __packed;

struct aggressiveness_profile2_entry {
	u8 opst_aggressiveness : 4;
	u8 elp_aggressiveness : 4;
} __packed;

struct aggressiveness_profile3_entry {
	u8 apd_aggressiveness:4;
	u8 pixoptix_aggressiveness:4;
} __packed;

struct aggressiveness_profile4_entry {
	u8 xpst_aggressiveness:4;
	u8 tcon_aggressiveness:4;
} __packed;

struct panel_identification {
	u8 panel_technology:4;
	u8 reserved:4;
} __packed;

struct bdb_lfp_power {
	struct lfp_power_features features;				/* ???-227 */
	struct als_data_entry als[5];
	u8 lace_aggressiveness_profile:3;				/* 210-227 */
	u8 reserved1:5;
	u16 dpst;							/* 228-256 */
	u16 psr;							/* 228+ */
	u16 drrs;							/* 228+ */
	u16 lace_support;						/* 228+ */
	u16 adt;							/* 228+ */
	u16 dmrrs;							/* 228+ */
	u16 adb;							/* 228+ */
	u16 lace_enabled_status;					/* 228+ */
	struct aggressiveness_profile_entry aggressiveness[16];
	u16 hobl;							/* 232+ */
	u16 vrr_feature_enabled;					/* 233+ */
	u16 elp;							/* 247-256 */
	u16 opst;							/* 247-256 */
	struct aggressiveness_profile2_entry aggressiveness2[16];	/* 247-256 */
	u16 apd;							/* 253-256 */
	u16 pixoptix;							/* 253-256 */
	struct aggressiveness_profile3_entry aggressiveness3[16];	/* 253-256 */
	struct panel_identification panel_identification[16];		/* 257+ */
	u16 xpst_support;						/* 257+ */
	u16 tcon_based_backlight_optimization;				/* 257+ */
	struct aggressiveness_profile4_entry aggressiveness4[16];	/* 257+ */
	u16 tcon_backlight_xpst_coexistence;				/* 257+ */
} __packed;

/*
 * Block 45 - eDP BFI Block
 */

struct edp_bfi {
	u8 enable_bfi_in_driver:1;
	u8 enable_brightness_control_in_cui:1;
	u8 reserved:6;
	u8 brightness_percentage_when_bfi_disabled;
} __packed;

struct bdb_edp_bfi {
	u8 bfi_structure_size;
	struct edp_bfi bfi[16];
} __packed;

/*
 * Block 46 - Chromaticity For Narrow Gamut Panel Configuration Block
 */

struct chromaticity {
	u8 chromaticity_enable:1;
	u8 chromaticity_from_edid_base_block:1;
	u8 rsvd:6;

	u8 green_y_lo:2;
	u8 green_x_lo:2;
	u8 red_y_lo:2;
	u8 red_x_lo:2;
	u8 white_y_lo:2;
	u8 white_x_lo:2;
	u8 blue_y_lo:2;
	u8 blue_x_lo:2;

	u8 red_x_hi;
	u8 red_y_hi;
	u8 green_x_hi;
	u8 green_y_hi;
	u8 blue_x_hi;
	u8 blue_y_hi;
	u8 white_x_hi;
	u8 white_y_hi;
} __packed;

struct luminance_and_gamma {
	u8 luminance_enable:1;						/* 211+ */
	u8 gamma_enable:1;						/* 211+ */
	u8 rsvd:6;

	u16 min_luminance;						/* 211+ */
	u16 max_luminance;						/* 211+ */
	u16 one_percent_max_luminance;					/* 211+ */
	u8 gamma;							/* 211+ */
} __packed;

struct bdb_chromaticity {
	struct chromaticity chromaticity[16];
	struct luminance_and_gamma luminance_and_gamma[16];		/* 211+ */
} __packed;

/*
 * Block 50 - MIPI Block
 */

struct mipi_data {
	u16 panel_identifier;
	u16 bridge_revision;

	u32 dithering:1;
	u32 pixel_format_18bpp:1;
	u32 reserved1:1;
	u32 dphy_params_valid:1;
	u32 reserved2:28;

	u16 port_info;

	u16 reserved3:2;
	u16 num_lanes:2;
	u16 reserved4:12;

	u16 virtual_channel_num:2;
	u16 video_transfer_mode:2;
	u16 reserved5:12;

	u32 dsi_ddr_clock;
	u32 renesas_bridge_ref_clock;
	u16 power_conservation;

	u32 prepare_count:5;
	u32 reserved6:3;
	u32 clk_zero_count:8;
	u32 trail_count:5;
	u32 reserved7:3;
	u32 exit_zero_count:6;
	u32 reserved8:2;

	u32 high_low_switch_count;
	u32 lp_byte_clock;
	u32 clock_lane_switch_time_counter;
	u32 panel_color_depth;
} __packed;

struct bdb_mipi {
	struct mipi_data mipi[16];
} __packed;

/*
 * Block 51 - Fixed Set Mode Table
 */

struct bdb_fixed_set_mode {
	u8 enable;
	u32 x_res;
	u32 y_res;
} __packed;

/*
 * Block 52 - MIPI Configuration Block
 */

#define MAX_MIPI_CONFIGURATIONS	6

struct bdb_mipi_config {
	struct mipi_config config[MAX_MIPI_CONFIGURATIONS];		/* 175+ */
	struct mipi_pps_data pps[MAX_MIPI_CONFIGURATIONS];		/* 177+ */
	struct edp_pwm_delays pwm_delays[MAX_MIPI_CONFIGURATIONS];	/* 186+ */
	u8 pmic_i2c_bus_number[MAX_MIPI_CONFIGURATIONS];		/* 190+ */
} __packed;

/*
 * Block 53 - MIPI Sequence Block
 */

struct bdb_mipi_sequence {
	u8 version;
	u8 data[]; /* up to 6 variable length blocks */
} __packed;

/*
 * Block 55 - RGB Palette Table
 */

struct bdb_rgb_palette {
	u8 is_enabled;
	u8 red[256];
	u8 blue[256];
	u8 green[256];
} __packed;

/*
 * Block 56 - Compression Parameters
 */

#define VBT_RC_BUFFER_BLOCK_SIZE_1KB	0
#define VBT_RC_BUFFER_BLOCK_SIZE_4KB	1
#define VBT_RC_BUFFER_BLOCK_SIZE_16KB	2
#define VBT_RC_BUFFER_BLOCK_SIZE_64KB	3

#define VBT_DSC_LINE_BUFFER_DEPTH(vbt_value)	((vbt_value) + 8) /* bits */
#define VBT_DSC_MAX_BPP(vbt_value)		(6 + (vbt_value) * 2)

struct dsc_compression_parameters_entry {
	u8 version_major:4;
	u8 version_minor:4;

	u8 rc_buffer_block_size:2;
	u8 reserved1:6;

	/*
	 * Buffer size in bytes:
	 *
	 * 4 ^ rc_buffer_block_size * 1024 * (rc_buffer_size + 1) bytes
	 */
	u8 rc_buffer_size;
	u32 slices_per_line;

	u8 line_buffer_depth:4;
	u8 reserved2:4;

	/* Flag Bits 1 */
	u8 block_prediction_enable:1;
	u8 reserved3:7;

	u8 max_bpp; /* mapping */

	/* Color depth capabilities */
	u8 reserved4:1;
	u8 support_8bpc:1;
	u8 support_10bpc:1;
	u8 support_12bpc:1;
	u8 reserved5:4;

	u16 slice_height;
} __packed;

struct bdb_compression_parameters {
	u16 entry_size;
	struct dsc_compression_parameters_entry data[16];
} __packed;

/*
 * Block 57 -  Vswing PreEmphasis Table
 */

struct bdb_vswing_preemph {
	u8 num_tables;
	u8 num_columns;
	u32 tables[];
} __packed;

/*
 * Block 58 - Generic DTD Block
 */

struct generic_dtd_entry {
	u32 pixel_clock;
	u16 hactive;
	u16 hblank;
	u16 hfront_porch;
	u16 hsync;
	u16 vactive;
	u16 vblank;
	u16 vfront_porch;
	u16 vsync;
	u16 width_mm;
	u16 height_mm;

	/* Flags */
	u8 rsvd_flags:6;
	u8 vsync_positive_polarity:1;
	u8 hsync_positive_polarity:1;

	u8 rsvd[3];
} __packed;

struct bdb_generic_dtd {
	u16 gdtd_size;
	struct generic_dtd_entry dtd[];	/* up to 24 DTD's */
} __packed;

/*
 * Block 253 - PRD Table
 */

struct prd_entry_old {
	u8 displays_attached;
	u8 display_in_pipe_a;
	u8 display_in_pipe_b;
} __packed;

struct bdb_prd_table_old {
	struct prd_entry_old list[0];				/* ???-216 */
	u16 num_entries;					/* ???-216 */
} __packed;

struct prd_entry_new {
	u16 primary_display;
	u16 secondary_display;
} __packed;

struct bdb_prd_table_new {
	u16 num_entries;					/* 217+ */
	struct prd_entry_new list[];				/* 217+ */
} __packed;

struct intel_bios_encoder_data {
	struct intel_display *display;

	struct child_device_config child;
	struct dsc_compression_parameters_entry *dsc;
	struct list_head node;
};




static const struct {
	enum bdb_block_id section_id;
	size_t min_size;
} bdb_blocks[] = {
	{ .section_id = BDB_GENERAL_FEATURES,
	  .min_size = sizeof(struct bdb_general_features), },
	{ .section_id = BDB_GENERAL_DEFINITIONS,
	  .min_size = sizeof(struct bdb_general_definitions), },
	{ .section_id = BDB_PSR,
	  .min_size = sizeof(struct bdb_psr), },
	{ .section_id = BDB_DRIVER_FEATURES,
	  .min_size = sizeof(struct bdb_driver_features), },
	{ .section_id = BDB_SDVO_LVDS_OPTIONS,
	  .min_size = sizeof(struct bdb_sdvo_lvds_options), },
	{ .section_id = BDB_SDVO_LVDS_DTD,
	  .min_size = sizeof(struct bdb_sdvo_lvds_dtd), },
	{ .section_id = BDB_EDP,
	  .min_size = sizeof(struct bdb_edp), },
	{ .section_id = BDB_LFP_OPTIONS,
	  .min_size = sizeof(struct bdb_lfp_options), },

	{ .section_id = BDB_LFP_DATA_PTRS,
	  .min_size = sizeof(struct bdb_lfp_data_ptrs), },
	{ .section_id = BDB_LFP_DATA,
	  .min_size = 0, /* special case */ },
	{ .section_id = BDB_LFP_BACKLIGHT,
	  .min_size = sizeof(struct bdb_lfp_backlight), },
	{ .section_id = BDB_LFP_POWER,
	  .min_size = sizeof(struct bdb_lfp_power), },
	{ .section_id = BDB_MIPI_CONFIG,
	  .min_size = sizeof(struct bdb_mipi_config), },
	{ .section_id = BDB_MIPI_SEQUENCE,
	  .min_size = sizeof(struct bdb_mipi_sequence) },
	{ .section_id = BDB_COMPRESSION_PARAMETERS,
	  .min_size = sizeof(struct bdb_compression_parameters), },
	{ .section_id = BDB_GENERIC_DTD,
	  .min_size = sizeof(struct bdb_generic_dtd), },
};


enum port {
	PORT_A = 0,
	PORT_B,
	PORT_C,
	PORT_D,
	PORT_E,
	PORT_F,
	PORT_G,
	PORT_H,
	PORT_I,
	PORT_TC1 = 10, // Type-C Port 1
	PORT_TC2,
	PORT_TC3,
	PORT_TC4,
	PORT_TC5,
	PORT_TC6,
	PORT_D_XELPD, // Special mapping for XeHPD
	PORT_E_XELPD,
	PORT_NONE,
};

enum phy {
	PHY_NONE = -1,

	PHY_A = 0,
	PHY_B,
	PHY_C,
	PHY_D,
	PHY_E,
	PHY_F,
	PHY_G,
	PHY_H,
	PHY_I,

	I915_MAX_PHYS
};
enum aux_ch {
	AUX_CH_NONE = -1,

	AUX_CH_A,
	AUX_CH_B,
	AUX_CH_C,
	AUX_CH_D,
	AUX_CH_E, /* ICL+ */
	AUX_CH_F,
	AUX_CH_G,
	AUX_CH_H,
	AUX_CH_I,

	/* tgl+ */
	AUX_CH_USBC1 = AUX_CH_D,
	AUX_CH_USBC2,
	AUX_CH_USBC3,
	AUX_CH_USBC4,
	AUX_CH_USBC5,
	AUX_CH_USBC6,

	/* XE_LPD repositions D/E offsets and bitfields */
	AUX_CH_D_XELPD = AUX_CH_USBC5,
	AUX_CH_E_XELPD,
};

static const u8 adlp_aux_ch_map[] = {
	[AUX_CH_A] = DP_AUX_A,
	[AUX_CH_B] = DP_AUX_B,
	[AUX_CH_C] = DP_AUX_C,
	[AUX_CH_D_XELPD] = DP_AUX_D,
	[AUX_CH_E_XELPD] = DP_AUX_E,
	[AUX_CH_USBC1] = DP_AUX_F,
	[AUX_CH_USBC2] = DP_AUX_G,
	[AUX_CH_USBC3] = DP_AUX_H,
	[AUX_CH_USBC4] = DP_AUX_I,
};


static const u8 adls_aux_ch_map[] = {
	[AUX_CH_A] = DP_AUX_A,
	[AUX_CH_USBC1] = DP_AUX_B,
	[AUX_CH_USBC2] = DP_AUX_C,
	[AUX_CH_USBC3] = DP_AUX_D,
	[AUX_CH_USBC4] = DP_AUX_E,
};


static const u8 rkl_aux_ch_map[] = {
	[AUX_CH_A] = DP_AUX_A,
	[AUX_CH_B] = DP_AUX_B,
	[AUX_CH_USBC1] = DP_AUX_C,
	[AUX_CH_USBC2] = DP_AUX_D,
};

static const u8 direct_aux_ch_map[] = {
	[AUX_CH_A] = DP_AUX_A,
	[AUX_CH_B] = DP_AUX_B,
	[AUX_CH_C] = DP_AUX_C,
	[AUX_CH_D] = DP_AUX_D, /* aka AUX_CH_USBC1 */
	[AUX_CH_E] = DP_AUX_E, /* aka AUX_CH_USBC2 */
	[AUX_CH_F] = DP_AUX_F, /* aka AUX_CH_USBC3 */
	[AUX_CH_G] = DP_AUX_G, /* aka AUX_CH_USBC4 */
	[AUX_CH_H] = DP_AUX_H, /* aka AUX_CH_USBC5 */
	[AUX_CH_I] = DP_AUX_I, /* aka AUX_CH_USBC6 */
};

struct pps_registers {
	u32 pp_ctrl;
	u32 pp_stat;
	u32 pp_on;
	u32 pp_off;
	u32 pp_div;
};
struct intel_pps_delays {
	u16 power_up;      /* eDP: T1+T3,   LVDS: T1+T2 */
	u16 backlight_on;  /* eDP: T8,      LVDS: T5 */
	u16 backlight_off; /* eDP: T9,      LVDS: T6/TX */
	u16 power_down;    /* eDP: T10,     LVDS: T3 */
	u16 power_cycle;   /* eDP: T11+T12, LVDS: T7+T4 */
};

enum drrs_type {
	DRRS_TYPE_NONE,
	DRRS_TYPE_STATIC,
	DRRS_TYPE_SEAMLESS,
};

struct intel_vbt_panel_data {
	void *lfp_vbt_mode; /* if any */
	void *sdvo_lvds_vbt_mode; /* if any */
	
	/* Feature bits */
	int panel_type;
	unsigned int lvds_dither:1;
	unsigned int bios_lvds_val; /* initial [PCH_]LVDS reg val in VBIOS */
	
	bool vrr;
	
	u8 seamless_drrs_min_refresh_rate;
	enum drrs_type drrs_type;
	
	struct {
		int max_link_rate;
		int rate;
		int lanes;
		int preemphasis;
		int vswing;
		int bpp;
		struct intel_pps_delays pps;
		u8 drrs_msa_timing_delay;
		bool low_vswing;
		bool hobl;
		bool dsc_disable;
		bool pipe_joiner_enable;
	} edp;
	
	struct {
		bool enable;
		bool full_link;
		bool require_aux_wakeup;
		int idle_frames;
		int tp1_wakeup_time_us;
		int tp2_tp3_wakeup_time_us;
		int psr2_tp2_tp3_wakeup_time_us;
	} psr;
	
	struct {
		u16 pwm_freq_hz;
		u16 brightness_precision_bits;
		u16 hdr_dpcd_refresh_timeout;
		bool present;
		bool active_low_pwm;
		u8 min_brightness;	/* min_brightness/255 of max */
		s8 controller;		/* brightness controller number */
		enum intel_backlight_type type;
	} backlight;
	
};



struct intel_pps {
	int panel_power_up_delay;
	int panel_power_down_delay;
	int panel_power_cycle_delay;
	int backlight_on_delay;
	int backlight_off_delay;
	//struct delayed_work panel_vdd_work;
	bool want_panel_vdd;
	bool initializing;
	unsigned long last_power_on;
	unsigned long last_backlight_off;
	//ktime_t panel_power_off_time;
	//struct ref_tracker *vdd_wakeref;
	u32 mmio_base;
	union {
		/*
		 * Pipe whose power sequencer is currently locked into
		 * this port. Only relevant on VLV/CHV.
		 */
		//enum pipe vlv_pps_pipe;

		/*
		 * Power sequencer index. Only relevant on BXT+.
		 */
		int pps_idx;
	};

	/*
	 * Pipe currently driving the port. Used for preventing
	 * the use of the PPS for any pipe currentrly driving
	 * external DP as that will mess things up on VLV.
	 */
	//enum pipe vlv_active_pipe;
	/*
	 * Set if the sequencer may be reset due to a power transition,
	 * requiring a reinitialization. Only relevant on BXT+.
	 */
	bool bxt_pps_reset;
	struct intel_pps_delays pps_delays;
	struct intel_pps_delays bios_pps_delays;
};

struct intel_panel {
	/* Simple drm_panel */
	void *base;

	/* Fixed EDID for eDP and LVDS. May hold ERR_PTR for invalid EDID. */
	void *fixed_edid;

	struct list_head fixed_modes;

	
	/* backlight */
	struct {
		bool present;
		u32 level;
		u32 min;
		u32 max;
		bool enabled;
		bool combination_mode;	/* gen 2/4 only */
		bool active_low_pwm;
		bool alternate_pwm_increment;	/* lpt+ */

		/* PWM chip */
		u32 pwm_level_min;
		u32 pwm_level_max;
		bool pwm_enabled;
		bool util_pin_active_low;	/* bxt+ */
		u8 controller;		/* bxt+ only */
		void *pwm;
		//struct pwm_state pwm_state;

		/* DPCD backlight */
		union {
			struct {
				//struct drm_edp_backlight_info info;
				bool luminance_control_support;
			} vesa;
			struct {
				bool sdr_uses_aux;
				bool supports_2084_decode;
				bool supports_2020_gamut;
				bool supports_segmented_backlight;
				bool supports_sdp_colorimetry;
				bool supports_tone_mapping;
			} intel_cap;
		} edp;

		//struct backlight_device *device;

		//const struct intel_panel_bl_funcs *funcs;
		//const struct intel_panel_bl_funcs *pwm_funcs;
		//void (*power)(struct intel_connector *, bool enable);
	} backlight;

	
	struct intel_vbt_panel_data vbt;
	struct intel_pps pps;
	
	u32 rawclk_freq;
	struct pps_registers regs;
};
enum drm_panel_orientation {
	DRM_MODE_PANEL_ORIENTATION_UNKNOWN = -1,
	DRM_MODE_PANEL_ORIENTATION_NORMAL = 0,
	DRM_MODE_PANEL_ORIENTATION_BOTTOM_UP,
	DRM_MODE_PANEL_ORIENTATION_LEFT_UP,
	DRM_MODE_PANEL_ORIENTATION_RIGHT_UP,
};
struct intel_vbt_data {
	/* bdb version */
	u16 version;

	/* Feature bits */
	unsigned int int_tv_support:1;
	unsigned int int_crt_support:1;
	unsigned int lvds_use_ssc:1;
	unsigned int int_lvds_support:1;
	unsigned int display_clock_mode:1;
	unsigned int fdi_rx_polarity_inverted:1;
	int lvds_ssc_freq;
	enum drm_panel_orientation orientation;

	bool override_afc_startup;
	u8 override_afc_startup_val;

	int crt_ddc_pin;

	struct list_head display_devices;
	struct list_head bdb_blocks;

	struct sdvo_device_mapping {
		u8 initialized;
		u8 dvo_port;
		u8 target_addr;
		u8 dvo_wiring;
		u8 i2c_pin;
		u8 ddc_pin;
	} sdvo_mappings[2];
};



struct bdb_block_entry {
	struct list_head node;
	enum bdb_block_id section_id;
	u8 data[];
};


enum pipe {
	INVALID_PIPE = -1,

	PIPE_A = 0,
	PIPE_B,
	PIPE_C,
	PIPE_D,
	_PIPE_EDP,

	I915_MAX_PIPES = _PIPE_EDP
};

enum transcoder {
	INVALID_TRANSCODER = -1,
	/*
	 * The following transcoders have a 1:1 transcoder -> pipe mapping,
	 * keep their values fixed: the code assumes that TRANSCODER_A=0, the
	 * rest have consecutive values and match the enum values of the pipes
	 * they map to.
	 */
	TRANSCODER_A = PIPE_A,
	TRANSCODER_B = PIPE_B,
	TRANSCODER_C = PIPE_C,
	TRANSCODER_D = PIPE_D,

	/*
	 * The following transcoders can map to any pipe, their enum value
	 * doesn't need to stay fixed.
	 */
	TRANSCODER_EDP,
	TRANSCODER_DSI_0,
	TRANSCODER_DSI_1,
	TRANSCODER_DSI_A = TRANSCODER_DSI_0,	/* legacy DSI */
	TRANSCODER_DSI_C = TRANSCODER_DSI_1,	/* legacy DSI */

	I915_MAX_TRANSCODERS
};

struct intel_display_ip_ver {
	u16 ver;
	u16 rel;
	u16 step; /* hardware */
};

struct intel_display_runtime_info {
	struct intel_display_ip_ver ip;
	int step; /* symbolic */
	char step_name[3]; /* empty string if not applicable */

	u32 rawclk_freq;

	u8 pipe_mask;
	u8 cpu_transcoder_mask;
	u16 port_mask;

	u8 num_sprites[I915_MAX_PIPES];
	u8 num_scalers[I915_MAX_PIPES];

	u8 fbc_mask;

	bool has_hdcp;
	bool has_dmc;
	bool has_dsc;
	bool edp_typec_support;
	bool has_dbuf_overlap_detection;
};
#define DEV_INFO_DISPLAY_FOR_EACH_FLAG(func) \
/* Keep in alphabetical order */ \
func(cursor_needs_physical); \
func(has_cdclk_crawl); \
func(has_cdclk_squash); \
func(has_ddi); \
func(has_dp_mst); \
func(has_dsb); \
func(has_fpga_dbg); \
func(has_gmch); \
func(has_hotplug); \
func(has_hti); \
func(has_ipc); \
func(has_overlay); \
func(has_psr); \
func(has_psr_hw_tracking); \
func(overlay_needs_physical); \
func(supports_tv);
struct intel_display_device_info {
	/* Initial runtime info. */
	const struct intel_display_runtime_info __runtime_defaults;

	u8 abox_mask;

	struct {
		u16 size; /* in blocks */
		u8 slice_mask;
	} dbuf;

#define DEFINE_FLAG(name) u8 name:1
	DEV_INFO_DISPLAY_FOR_EACH_FLAG(DEFINE_FLAG);
#undef DEFINE_FLAG

	/* Global register offset for the display engine */
	u32 mmio_offset;

	/* Register offsets for the various display pipes and transcoders */
	u32 pipe_offsets[I915_MAX_TRANSCODERS];
	u32 trans_offsets[I915_MAX_TRANSCODERS];
	u32 cursor_offsets[I915_MAX_PIPES];

	struct {
		u32 degamma_lut_size;
		u32 gamma_lut_size;
		u32 degamma_lut_tests;
		u32 gamma_lut_tests;
	} color;
};
enum dbuf_slice {
	DBUF_S1,
	DBUF_S2,
	DBUF_S3,
	DBUF_S4,
	I915_MAX_DBUF_SLICES
};
enum drm_color_lut_tests {

	DRM_COLOR_LUT_EQUAL_CHANNELS = BIT(0),

	DRM_COLOR_LUT_NON_DECREASING = BIT(1),
};
enum intel_fbc_id {
	INTEL_FBC_A,
	INTEL_FBC_B,
	INTEL_FBC_C,
	INTEL_FBC_D,

	I915_MAX_FBCS,
};
#define PIPE_A_OFFSET		0x70000
#define PIPE_B_OFFSET		0x71000
#define PIPE_C_OFFSET		0x72000
#define PIPE_D_OFFSET		0x73000
#define CHV_PIPE_C_OFFSET	0x74000
/*
* There's actually no pipe EDP. Some pipe registers have
* simply shifted from the pipe to the transcoder, while
* keeping their original offset. Thus we need PIPE_EDP_OFFSET
* to access such registers in transcoder EDP.
*/
#define PIPE_EDP_OFFSET	0x7f000

/* ICL DSI 0 and 1 */
#define PIPE_DSI0_OFFSET	0x7b000
#define PIPE_DSI1_OFFSET	0x7b800

#define TRANSCODER_A_OFFSET 0x60000
#define TRANSCODER_B_OFFSET 0x61000
#define TRANSCODER_C_OFFSET 0x62000
#define CHV_TRANSCODER_C_OFFSET 0x63000
#define TRANSCODER_D_OFFSET 0x63000
#define TRANSCODER_EDP_OFFSET 0x6f000
#define TRANSCODER_DSI0_OFFSET	0x6b000
#define TRANSCODER_DSI1_OFFSET	0x6b800

#define CURSOR_A_OFFSET 0x70080
#define CURSOR_B_OFFSET 0x700c0
#define CHV_CURSOR_C_OFFSET 0x700e0
#define IVB_CURSOR_B_OFFSET 0x71080
#define IVB_CURSOR_C_OFFSET 0x72080
#define TGL_CURSOR_D_OFFSET 0x73080

#define IVB_CURSOR_OFFSETS \
.cursor_offsets = { \
[PIPE_A] = CURSOR_A_OFFSET, \
[PIPE_B] = IVB_CURSOR_B_OFFSET, \
[PIPE_C] = IVB_CURSOR_C_OFFSET, \
}

#define TGL_CURSOR_OFFSETS \
.cursor_offsets = { \
[PIPE_A] = CURSOR_A_OFFSET, \
[PIPE_B] = IVB_CURSOR_B_OFFSET, \
[PIPE_C] = IVB_CURSOR_C_OFFSET, \
[PIPE_D] = TGL_CURSOR_D_OFFSET, \
}

#define XE_LPDP_FEATURES							\
.abox_mask = GENMASK(1, 0),						\
.color = {								\
.degamma_lut_size = 129, .gamma_lut_size = 1024,		\
.degamma_lut_tests = DRM_COLOR_LUT_NON_DECREASING |		\
DRM_COLOR_LUT_EQUAL_CHANNELS,					\
},									\
.dbuf.size = 4096,							\
.dbuf.slice_mask = BIT(DBUF_S1) | BIT(DBUF_S2) | BIT(DBUF_S3) |		\
BIT(DBUF_S4),							\
.has_cdclk_crawl = 1,							\
.has_cdclk_squash = 1,							\
.has_ddi = 1,								\
.has_dp_mst = 1,							\
.has_dsb = 1,								\
.has_fpga_dbg = 1,							\
.has_hotplug = 1,							\
.has_ipc = 1,								\
.has_psr = 1,								\
.pipe_offsets = {							\
[TRANSCODER_A] = PIPE_A_OFFSET,					\
[TRANSCODER_B] = PIPE_B_OFFSET,					\
[TRANSCODER_C] = PIPE_C_OFFSET,					\
[TRANSCODER_D] = PIPE_D_OFFSET,					\
},									\
.trans_offsets = {							\
[TRANSCODER_A] = TRANSCODER_A_OFFSET,				\
[TRANSCODER_B] = TRANSCODER_B_OFFSET,				\
[TRANSCODER_C] = TRANSCODER_C_OFFSET,				\
[TRANSCODER_D] = TRANSCODER_D_OFFSET,				\
},									\
TGL_CURSOR_OFFSETS,							\
						\
.__runtime_defaults.cpu_transcoder_mask =				\
BIT(TRANSCODER_A) | BIT(TRANSCODER_B) |				\
BIT(TRANSCODER_C) | BIT(TRANSCODER_D),				\
.__runtime_defaults.fbc_mask = BIT(INTEL_FBC_A) | BIT(INTEL_FBC_B),	\
.__runtime_defaults.has_dmc = 1,					\
.__runtime_defaults.has_dsc = 1,					\
.__runtime_defaults.has_hdcp = 1,					\
.__runtime_defaults.pipe_mask =						\
BIT(PIPE_A) | BIT(PIPE_B) | BIT(PIPE_C) | BIT(PIPE_D),		\
.__runtime_defaults.port_mask = BIT(PORT_A) | BIT(PORT_B) |		\
BIT(PORT_TC1) | BIT(PORT_TC2) | BIT(PORT_TC3) | BIT(PORT_TC4)

static const struct intel_display_device_info xe_lpdp_display = {
	XE_LPDP_FEATURES,
};
static const struct intel_display_device_info xe2_hpd_display = {
	XE_LPDP_FEATURES,
	.__runtime_defaults.port_mask = BIT(PORT_A) |
		BIT(PORT_TC1) | BIT(PORT_TC2) | BIT(PORT_TC3) | BIT(PORT_TC4),
};
static const struct intel_display_device_info xe2_lpd_display = {
	XE_LPDP_FEATURES,

	.__runtime_defaults.fbc_mask =
		BIT(INTEL_FBC_A) | BIT(INTEL_FBC_B) |
		BIT(INTEL_FBC_C) | BIT(INTEL_FBC_D),
	.__runtime_defaults.has_dbuf_overlap_detection = true,
};
static const struct intel_display_device_info wcl_display = {
	XE_LPDP_FEATURES,

	.__runtime_defaults.cpu_transcoder_mask =
		BIT(TRANSCODER_A) | BIT(TRANSCODER_B) | BIT(TRANSCODER_C),
	.__runtime_defaults.pipe_mask =
		BIT(PIPE_A) | BIT(PIPE_B) | BIT(PIPE_C),
	.__runtime_defaults.fbc_mask =
		BIT(INTEL_FBC_A) | BIT(INTEL_FBC_B) | BIT(INTEL_FBC_C),
	.__runtime_defaults.port_mask =
		BIT(PORT_A) | BIT(PORT_B) | BIT(PORT_TC1) | BIT(PORT_TC2),
};

static const struct {
	u16 ver;
	u16 rel;
	const struct intel_display_device_info *display;
} gmdid_display_map[] = {
	{ 14,  0, &xe_lpdp_display },
	{ 14,  1, &xe2_hpd_display },
	{ 20,  0, &xe2_lpd_display },
	{ 30,  0, &xe2_lpd_display },
	{ 30,  2, &wcl_display },
	{ 35,  0, &xe2_lpd_display },
};

#define INTEL_DISPLAY_DEVICE(_id, _desc) { .devid = (_id), .desc = (_desc) }
#define __NUM_PLATFORMS (INTEL_DISPLAY_PLATFORMS(__COUNT) 0)

#define DECLARE_BITMAP(name,bits) \
unsigned long name[BITS_TO_LONGS(bits)]

#define INTEL_DISPLAY_PLATFORMS(func) \
/* Platform group aliases */ \
func(g4x) /* g45 and gm45 */ \
func(mobile) /* mobile platforms */ \
func(dgfx) /* discrete graphics */ \
/* Display ver 2 */ \
func(i830) \
func(i845g) \
func(i85x) \
func(i865g) \
/* Display ver 3 */ \
func(i915g) \
func(i915gm) \
func(i945g) \
func(i945gm) \
func(g33) \
func(pineview) \
/* Display ver 4 */ \
func(i965g) \
func(i965gm) \
func(g45) \
func(gm45) \
/* Display ver 5 */ \
func(ironlake) \
/* Display ver 6 */ \
func(sandybridge) \
/* Display ver 7 */ \
func(ivybridge) \
func(valleyview) \
func(haswell) \
func(haswell_ult) \
func(haswell_ulx) \
/* Display ver 8 */ \
func(broadwell) \
func(broadwell_ult) \
func(broadwell_ulx) \
func(cherryview) \
/* Display ver 9 */ \
func(skylake) \
func(skylake_ult) \
func(skylake_ulx) \
func(broxton) \
func(kabylake) \
func(kabylake_ult) \
func(kabylake_ulx) \
func(geminilake) \
func(coffeelake) \
func(coffeelake_ult) \
func(coffeelake_ulx) \
func(cometlake) \
func(cometlake_ult) \
func(cometlake_ulx) \
/* Display ver 11 */ \
func(icelake) \
func(icelake_port_f) \
func(jasperlake) \
func(elkhartlake) \
/* Display ver 12 */ \
func(tigerlake) \
func(tigerlake_uy) \
func(rocketlake) \
func(dg1) \
func(alderlake_s) \
func(alderlake_s_raptorlake_s) \
/* Display ver 13 */ \
func(alderlake_p) \
func(alderlake_p_alderlake_n) \
func(alderlake_p_raptorlake_p) \
func(alderlake_p_raptorlake_u) \
func(dg2) \
func(dg2_g10) \
func(dg2_g11) \
func(dg2_g12) \
/* Display ver 14 (based on GMD ID) */ \
func(meteorlake) \
func(meteorlake_u) \
/* Display ver 20 (based on GMD ID) */ \
func(lunarlake) \
/* Display ver 14.1 (based on GMD ID) */ \
func(battlemage) \
/* Display ver 30 (based on GMD ID) */ \
func(pantherlake) \
func(pantherlake_wildcatlake) \
/* Display ver 35 (based on GMD ID) */ \
func(novalake)

#define __MEMBER(name) unsigned long name:1;
#define __COUNT(x) 1 +

#define __NUM_PLATFORMS (INTEL_DISPLAY_PLATFORMS(__COUNT) 0)

struct intel_display_platforms {
union {
struct {
INTEL_DISPLAY_PLATFORMS(__MEMBER);
};
DECLARE_BITMAP(bitmap, __NUM_PLATFORMS);
};
};
#define STEP_ENUM_VAL(name)  STEP_##name,

#define STEP_NAME_LIST(func)		\
func(A0)			\
func(A1)			\
func(A2)			\
func(A3)			\
func(B0)			\
func(B1)			\
func(B2)			\
func(B3)			\
func(C0)			\
func(C1)			\
func(C2)			\
func(C3)			\
func(D0)			\
func(D1)			\
func(D2)			\
func(D3)			\
func(E0)			\
func(E1)			\
func(E2)			\
func(E3)			\
func(F0)			\
func(F1)			\
func(F2)			\
func(F3)			\
func(G0)			\
func(G1)			\
func(G2)			\
func(G3)			\
func(H0)			\
func(H1)			\
func(H2)			\
func(H3)			\
func(I0)			\
func(I1)			\
func(I2)			\
func(I3)			\
func(J0)			\
func(J1)			\
func(J2)			\
func(J3)

/*
* Symbolic steppings that do not match the hardware. These are valid both as gt
* and display steppings as symbolic names.
*/
enum intel_step {
STEP_NONE = 0,
STEP_NAME_LIST(STEP_ENUM_VAL)
STEP_FUTURE,
STEP_FOREVER,
};

struct stepping_desc {
	const enum intel_step *map; /* revid to step map */
	size_t size; /* map size */
};
struct subplatform_desc {
	struct intel_display_platforms platforms;
	const char *name;
	const u16 *pciidlist;
	struct stepping_desc step_info;
};



struct platform_desc {
	struct intel_display_platforms platforms;
	const char *name;
	const struct subplatform_desc *subplatforms;
	const struct intel_display_device_info *info; /* NULL for GMD ID */
	struct stepping_desc step_info;
};



#define PLATFORM(_platform)			 \
.platforms._platform = 1,		 \
.name = #_platform

#define SUBPLATFORM(_platform, _subplatform)				\
.platforms._platform##_##_subplatform = 1,			\
.name = #_subplatform



#define STEP_INFO(_map)				\
	.step_info.map = _map,			\
	.step_info.size = ARRAY_SIZE(_map)

#define ID(x, ...) x


/* RKL */
#define INTEL_RKL_IDS(MACRO__, ...) \
	MACRO__(0x4C80, ## __VA_ARGS__), \
	MACRO__(0x4C8A, ## __VA_ARGS__), \
	MACRO__(0x4C8B, ## __VA_ARGS__), \
	MACRO__(0x4C8C, ## __VA_ARGS__), \
	MACRO__(0x4C90, ## __VA_ARGS__), \
	MACRO__(0x4C9A, ## __VA_ARGS__)

/* DG1 */
#define INTEL_DG1_IDS(MACRO__, ...) \
	MACRO__(0x4905, ## __VA_ARGS__), \
	MACRO__(0x4906, ## __VA_ARGS__), \
	MACRO__(0x4907, ## __VA_ARGS__), \
	MACRO__(0x4908, ## __VA_ARGS__), \
	MACRO__(0x4909, ## __VA_ARGS__)

/* ADL-S */
#define INTEL_ADLS_IDS(MACRO__, ...) \
	MACRO__(0x4680, ## __VA_ARGS__), \
	MACRO__(0x4682, ## __VA_ARGS__), \
	MACRO__(0x4688, ## __VA_ARGS__), \
	MACRO__(0x468A, ## __VA_ARGS__), \
	MACRO__(0x468B, ## __VA_ARGS__), \
	MACRO__(0x4690, ## __VA_ARGS__), \
	MACRO__(0x4692, ## __VA_ARGS__), \
	MACRO__(0x4693, ## __VA_ARGS__)

/* ADL-P */
#define INTEL_ADLP_IDS(MACRO__, ...) \
	MACRO__(0x46A0, ## __VA_ARGS__), \
	MACRO__(0x46A1, ## __VA_ARGS__), \
	MACRO__(0x46A2, ## __VA_ARGS__), \
	MACRO__(0x46A3, ## __VA_ARGS__), \
	MACRO__(0x46A6, ## __VA_ARGS__), \
	MACRO__(0x46A8, ## __VA_ARGS__), \
	MACRO__(0x46AA, ## __VA_ARGS__), \
	MACRO__(0x462A, ## __VA_ARGS__), \
	MACRO__(0x4626, ## __VA_ARGS__), \
	MACRO__(0x4628, ## __VA_ARGS__), \
	MACRO__(0x46B0, ## __VA_ARGS__), \
	MACRO__(0x46B1, ## __VA_ARGS__), \
	MACRO__(0x46B2, ## __VA_ARGS__), \
	MACRO__(0x46B3, ## __VA_ARGS__), \
	MACRO__(0x46C0, ## __VA_ARGS__), \
	MACRO__(0x46C1, ## __VA_ARGS__), \
	MACRO__(0x46C2, ## __VA_ARGS__), \
	MACRO__(0x46C3, ## __VA_ARGS__)

/* ADL-N */
#define INTEL_ADLN_IDS(MACRO__, ...) \
	MACRO__(0x46D0, ## __VA_ARGS__), \
	MACRO__(0x46D1, ## __VA_ARGS__), \
	MACRO__(0x46D2, ## __VA_ARGS__), \
	MACRO__(0x46D3, ## __VA_ARGS__), \
	MACRO__(0x46D4, ## __VA_ARGS__)

/* RPL-S */
#define INTEL_RPLS_IDS(MACRO__, ...) \
	MACRO__(0xA780, ## __VA_ARGS__), \
	MACRO__(0xA781, ## __VA_ARGS__), \
	MACRO__(0xA782, ## __VA_ARGS__), \
	MACRO__(0xA783, ## __VA_ARGS__), \
	MACRO__(0xA788, ## __VA_ARGS__), \
	MACRO__(0xA789, ## __VA_ARGS__), \
	MACRO__(0xA78A, ## __VA_ARGS__), \
	MACRO__(0xA78B, ## __VA_ARGS__)

/* RPL-U */
#define INTEL_RPLU_IDS(MACRO__, ...) \
	MACRO__(0xA721, ## __VA_ARGS__), \
	MACRO__(0xA7A1, ## __VA_ARGS__), \
	MACRO__(0xA7A9, ## __VA_ARGS__), \
	MACRO__(0xA7AC, ## __VA_ARGS__), \
	MACRO__(0xA7AD, ## __VA_ARGS__)

/* RPL-P */
#define INTEL_RPLP_IDS(MACRO__, ...) \
	MACRO__(0xA720, ## __VA_ARGS__), \
	MACRO__(0xA7A0, ## __VA_ARGS__), \
	MACRO__(0xA7A8, ## __VA_ARGS__), \
	MACRO__(0xA7AA, ## __VA_ARGS__), \
	MACRO__(0xA7AB, ## __VA_ARGS__)

#define INTEL_TGL_GT1_IDS(MACRO__, ...) \
	MACRO__(0x9A60, ## __VA_ARGS__), \
	MACRO__(0x9A68, ## __VA_ARGS__), \
	MACRO__(0x9A70, ## __VA_ARGS__)

#define INTEL_TGL_GT2_IDS(MACRO__, ...) \
	MACRO__(0x9A40, ## __VA_ARGS__), \
	MACRO__(0x9A49, ## __VA_ARGS__), \
	MACRO__(0x9A59, ## __VA_ARGS__), \
	MACRO__(0x9A78, ## __VA_ARGS__), \
	MACRO__(0x9AC0, ## __VA_ARGS__), \
	MACRO__(0x9AC9, ## __VA_ARGS__), \
	MACRO__(0x9AD9, ## __VA_ARGS__), \
	MACRO__(0x9AF8, ## __VA_ARGS__)

#define INTEL_TGL_IDS(MACRO__, ...) \
INTEL_TGL_GT1_IDS(MACRO__, ## __VA_ARGS__), \
INTEL_TGL_GT2_IDS(MACRO__, ## __VA_ARGS__)


#define XE_D_DISPLAY \
.abox_mask = GENMASK(2, 1), \
.dbuf.size = 2048, \
.dbuf.slice_mask = BIT(DBUF_S1) | BIT(DBUF_S2), \
.has_ddi = 1, \
.has_dp_mst = 1, \
.has_dsb = 1, \
.has_fpga_dbg = 1, \
.has_hotplug = 1, \
.has_ipc = 1, \
.has_psr = 1, \
.has_psr_hw_tracking = 1, \
.pipe_offsets = { \
[TRANSCODER_A] = PIPE_A_OFFSET, \
[TRANSCODER_B] = PIPE_B_OFFSET, \
[TRANSCODER_C] = PIPE_C_OFFSET, \
[TRANSCODER_D] = PIPE_D_OFFSET, \
[TRANSCODER_DSI_0] = PIPE_DSI0_OFFSET, \
[TRANSCODER_DSI_1] = PIPE_DSI1_OFFSET, \
}, \
.trans_offsets = { \
[TRANSCODER_A] = TRANSCODER_A_OFFSET, \
[TRANSCODER_B] = TRANSCODER_B_OFFSET, \
[TRANSCODER_C] = TRANSCODER_C_OFFSET, \
[TRANSCODER_D] = TRANSCODER_D_OFFSET, \
[TRANSCODER_DSI_0] = TRANSCODER_DSI0_OFFSET, \
[TRANSCODER_DSI_1] = TRANSCODER_DSI1_OFFSET, \
}, \
TGL_CURSOR_OFFSETS, \
ICL_COLORS, \
\
.__runtime_defaults.ip.ver = 12, \
.__runtime_defaults.has_dmc = 1, \
.__runtime_defaults.has_dsc = 1, \
.__runtime_defaults.has_hdcp = 1, \
.__runtime_defaults.pipe_mask = \
BIT(PIPE_A) | BIT(PIPE_B) | BIT(PIPE_C) | BIT(PIPE_D), \
.__runtime_defaults.cpu_transcoder_mask = \
BIT(TRANSCODER_A) | BIT(TRANSCODER_B) | \
BIT(TRANSCODER_C) | BIT(TRANSCODER_D) | \
BIT(TRANSCODER_DSI_0) | BIT(TRANSCODER_DSI_1), \
.__runtime_defaults.fbc_mask = BIT(INTEL_FBC_A)

static const u16 tgl_uy_ids[] = {
INTEL_TGL_GT2_IDS(ID),
0
};

static const enum intel_step tgl_steppings[] = {
[0] = STEP_B0,
[1] = STEP_D0,
};

static const enum intel_step tgl_uy_steppings[] = {
[0] = STEP_A0,
[1] = STEP_C0,
[2] = STEP_C0,
[3] = STEP_D0,
};

#define ICL_COLORS \
.color = { \
.degamma_lut_size = 33, .gamma_lut_size = 262145, \
.degamma_lut_tests = DRM_COLOR_LUT_NON_DECREASING | \
	 DRM_COLOR_LUT_EQUAL_CHANNELS, \
.gamma_lut_tests = DRM_COLOR_LUT_NON_DECREASING, \
}

static const struct intel_display_device_info tgli=
{
	XE_D_DISPLAY,
	.__runtime_defaults.port_mask = BIT(PORT_A) | BIT(PORT_B) |
	BIT(PORT_TC1) | BIT(PORT_TC2) | BIT(PORT_TC3) | BIT(PORT_TC4) | BIT(PORT_TC5) | BIT(PORT_TC6)
};

static const struct subplatform_desc tgl_s[] = {
	{
		SUBPLATFORM(tigerlake, uy),
		.pciidlist = tgl_uy_ids,
		STEP_INFO(tgl_uy_steppings),
	},
};

static const struct platform_desc tgl_desc = {
PLATFORM(tigerlake),
.info = &tgli,
.subplatforms=tgl_s,
STEP_INFO(tgl_steppings),
};

static const enum intel_step rkl_steppings[] = {
	[0] = STEP_A0,
	[1] = STEP_B0,
	[4] = STEP_C0,
};
static const struct intel_display_device_info rkli={
	XE_D_DISPLAY,
	.abox_mask = BIT(0),
	.has_hti = 1,
	.has_psr_hw_tracking = 0,

	.__runtime_defaults.pipe_mask = BIT(PIPE_A) | BIT(PIPE_B) | BIT(PIPE_C),
	.__runtime_defaults.cpu_transcoder_mask =
	BIT(TRANSCODER_A) | BIT(TRANSCODER_B) | BIT(TRANSCODER_C),
	.__runtime_defaults.port_mask = BIT(PORT_A) | BIT(PORT_B) |
	BIT(PORT_TC1) | BIT(PORT_TC2),
};

static const struct platform_desc rkl_desc = {
	PLATFORM(rocketlake),
	.info = &rkli,
	STEP_INFO(rkl_steppings),
};

static const u16 adls_rpls_ids[] = {
	INTEL_RPLS_IDS(ID),
	0
};

static const enum intel_step adl_s_steppings[] = {
	[0x0] = STEP_A0,
	[0x1] = STEP_A2,
	[0x4] = STEP_B0,
	[0x8] = STEP_B0,
	[0xC] = STEP_C0,
};

static const enum intel_step adl_s_rpl_s_steppings[] = {
	[0x4] = STEP_D0,
	[0xC] = STEP_C0,
};

static const struct subplatform_desc adl_s_s[] = {
	{
		SUBPLATFORM(alderlake_s, raptorlake_s),
		.pciidlist = adls_rpls_ids,
		STEP_INFO(adl_s_rpl_s_steppings),
	},
};

static const struct intel_display_device_info adl_s_descs={
	XE_D_DISPLAY,
	   .has_hti = 1,
	   .has_psr_hw_tracking = 0,

	   .__runtime_defaults.port_mask = BIT(PORT_A) |
	   BIT(PORT_TC1) | BIT(PORT_TC2) | BIT(PORT_TC3) | BIT(PORT_TC4),
};

static const struct platform_desc adl_s_desc = {
	PLATFORM(alderlake_s),
	.subplatforms = adl_s_s,
	.info = &adl_s_descs,
	STEP_INFO(adl_s_steppings),
};

#define XE_LPD_FEATURES \
.abox_mask = GENMASK(1, 0),						\
.color = {								\
.degamma_lut_size = 129, .gamma_lut_size = 1024,		\
.degamma_lut_tests = DRM_COLOR_LUT_NON_DECREASING |		\
DRM_COLOR_LUT_EQUAL_CHANNELS,					\
},									\
.dbuf.size = 4096,							\
.dbuf.slice_mask = BIT(DBUF_S1) | BIT(DBUF_S2) | BIT(DBUF_S3) |		\
BIT(DBUF_S4),							\
.has_ddi = 1,								\
.has_dp_mst = 1,							\
.has_dsb = 1,								\
.has_fpga_dbg = 1,							\
.has_hotplug = 1,							\
.has_ipc = 1,								\
.has_psr = 1,								\
.pipe_offsets = {							\
[TRANSCODER_A] = PIPE_A_OFFSET,					\
[TRANSCODER_B] = PIPE_B_OFFSET,					\
[TRANSCODER_C] = PIPE_C_OFFSET,					\
[TRANSCODER_D] = PIPE_D_OFFSET,					\
[TRANSCODER_DSI_0] = PIPE_DSI0_OFFSET,				\
[TRANSCODER_DSI_1] = PIPE_DSI1_OFFSET,				\
},									\
.trans_offsets = {							\
[TRANSCODER_A] = TRANSCODER_A_OFFSET,				\
[TRANSCODER_B] = TRANSCODER_B_OFFSET,				\
[TRANSCODER_C] = TRANSCODER_C_OFFSET,				\
[TRANSCODER_D] = TRANSCODER_D_OFFSET,				\
[TRANSCODER_DSI_0] = TRANSCODER_DSI0_OFFSET,			\
[TRANSCODER_DSI_1] = TRANSCODER_DSI1_OFFSET,			\
},									\
TGL_CURSOR_OFFSETS,							\
						\
.__runtime_defaults.ip.ver = 13,					\
.__runtime_defaults.has_dmc = 1,					\
.__runtime_defaults.has_dsc = 1,					\
.__runtime_defaults.fbc_mask = BIT(INTEL_FBC_A),			\
.__runtime_defaults.has_hdcp = 1,					\
.__runtime_defaults.pipe_mask =						\
BIT(PIPE_A) | BIT(PIPE_B) | BIT(PIPE_C) | BIT(PIPE_D)

static const struct intel_display_device_info xe_lpd_display = {
XE_LPD_FEATURES,
.has_cdclk_crawl = 1,
.has_psr_hw_tracking = 0,

.__runtime_defaults.cpu_transcoder_mask =
BIT(TRANSCODER_A) | BIT(TRANSCODER_B) |
BIT(TRANSCODER_C) | BIT(TRANSCODER_D) |
BIT(TRANSCODER_DSI_0) | BIT(TRANSCODER_DSI_1),
.__runtime_defaults.port_mask = BIT(PORT_A) | BIT(PORT_B) |
BIT(PORT_TC1) | BIT(PORT_TC2) | BIT(PORT_TC3) | BIT(PORT_TC4),
};


static const u16 adlp_adln_ids[] = {
	INTEL_ADLN_IDS(ID),
	0
};

static const u16 adlp_rplu_ids[] = {
	INTEL_RPLU_IDS(ID),
	0
};

static const u16 adlp_rplp_ids[] = {
	INTEL_RPLP_IDS(ID),
	0
};

static const enum intel_step adl_p_steppings[] = {
	[0x0] = STEP_A0,
	[0x4] = STEP_B0,
	[0x8] = STEP_C0,
	[0xC] = STEP_D0,
};

static const enum intel_step adl_p_adl_n_steppings[] = {
	[0x0] = STEP_D0,
};

static const enum intel_step adl_p_rpl_pu_steppings[] = {
	[0x4] = STEP_E0,
};

static const struct subplatform_desc adl_p_descs[]={
	{
		SUBPLATFORM(alderlake_p, alderlake_n),
		.pciidlist = adlp_adln_ids,
		STEP_INFO(adl_p_adl_n_steppings),
	},
	{
		SUBPLATFORM(alderlake_p, raptorlake_p),
		.pciidlist = adlp_rplp_ids,
		STEP_INFO(adl_p_rpl_pu_steppings),
	},
	{
		SUBPLATFORM(alderlake_p, raptorlake_u),
		.pciidlist = adlp_rplu_ids,
		STEP_INFO(adl_p_rpl_pu_steppings),
	},
};

static const struct platform_desc adl_p_desc = {
	PLATFORM(alderlake_p),
	.subplatforms = adl_p_descs,
	.info = &xe_lpd_display,
	STEP_INFO(adl_p_steppings),
};



static const struct {
u32 devid;
const struct platform_desc *desc;
} intel_display_ids[] = {
INTEL_TGL_IDS(INTEL_DISPLAY_DEVICE, &tgl_desc),
INTEL_RKL_IDS(INTEL_DISPLAY_DEVICE, &rkl_desc),
INTEL_ADLS_IDS(INTEL_DISPLAY_DEVICE, &adl_s_desc),
INTEL_RPLS_IDS(INTEL_DISPLAY_DEVICE, &adl_s_desc),
INTEL_ADLP_IDS(INTEL_DISPLAY_DEVICE, &adl_p_desc),
INTEL_ADLN_IDS(INTEL_DISPLAY_DEVICE, &adl_p_desc),
INTEL_RPLU_IDS(INTEL_DISPLAY_DEVICE, &adl_p_desc),
INTEL_RPLP_IDS(INTEL_DISPLAY_DEVICE, &adl_p_desc),
};

struct stepping_info {
	char stepping;
	char substepping;
};

#define _PIPEDMC_FPQ_TS_A		0x5f134
#define _PIPEDMC_FPQ_TS_B		0x5f534
#define PIPEDMC_FPQ_TS(pipe)		_MMIO_PIPE((pipe), _PIPEDMC_FPQ_TS_A, _PIPEDMC_FPQ_TS_B)

#define _PIPEDMC_SCANLINE_RO_A		0x5f144
#define _PIPEDMC_SCANLINE_RO_B		0x5f544
#define PIPEDMC_SCANLINE_RO(pipe)	_MMIO_PIPE((pipe), _PIPEDMC_SCANLINE_RO_A, _PIPEDMC_SCANLINE_RO_B)

#define _PIPEDMC_FPQ_CTL1_A		0x5f160
#define _PIPEDMC_FPQ_CTL1_B		0x5f560
#define PIPEDMC_FPQ_CTL1(pipe)		_MMIO_PIPE((pipe), _PIPEDMC_FPQ_CTL1_A, _PIPEDMC_FPQ_CTL1_B)
#define   PIPEDMC_SW_DMC_WAKE		REG_BIT(0)

#define _PIPEDMC_FPQ_CTL2_A		0x5f164
#define _PIPEDMC_FPQ_CTL2_B		0x5f564
#define PIPEDMC_FPQ_CTL2(pipe)		_MMIO_PIPE((pipe), _PIPEDMC_FPQ_CTL2_A, _PIPEDMC_FPQ_CTL2_B)
#define   PIPEDMC_DMC_INT_AT_DELAYED_VBLANK	REG_BIT(1)
#define   PIPEDMC_W1_DMC_WAKE			REG_BIT(0)

#define _PIPEDMC_INTERRUPT_A		0x5f190 /* lnl+ */
#define _PIPEDMC_INTERRUPT_B		0x5f590 /* lnl+ */
#define PIPEDMC_INTERRUPT(pipe)		_MMIO_PIPE((pipe), _PIPEDMC_INTERRUPT_A, _PIPEDMC_INTERRUPT_B)
#define _PIPEDMC_INTERRUPT_MASK_A	0x5f194 /* lnl+ */
#define _PIPEDMC_INTERRUPT_MASK_B	0x5f594 /* lnl+ */
#define PIPEDMC_INTERRUPT_MASK(pipe)	_MMIO_PIPE((pipe), _PIPEDMC_INTERRUPT_MASK_A, _PIPEDMC_INTERRUPT_MASK_B)
#define   PIPEDMC_FLIPQ_PROG_DONE	REG_BIT(3)
#define   PIPEDMC_ERROR			REG_BIT(2)
#define   PIPEDMC_GTT_FAULT		REG_BIT(1)
#define   PIPEDMC_ATS_FAULT		REG_BIT(0)

#define PIPEDMC_BLOCK_PKGC_SW_A	0x5f1d0
#define PIPEDMC_BLOCK_PKGC_SW_B	0x5F5d0
#define PIPEDMC_BLOCK_PKGC_SW(pipe)				_MMIO_PIPE(pipe, \
									   PIPEDMC_BLOCK_PKGC_SW_A, \
									   PIPEDMC_BLOCK_PKGC_SW_B)
#define PIPEDMC_BLOCK_PKGC_SW_BLOCK_PKGC_ALWAYS			BIT(31)
#define PIPEDMC_BLOCK_PKGC_SW_BLOCK_PKGC_UNTIL_NEXT_FRAMESTART	BIT(15)

#define _ADLP_PIPEDMC_REG_MMIO_BASE_A	0x5f000
#define _TGL_PIPEDMC_REG_MMIO_BASE_A	0x92000

#define __PIPEDMC_REG_MMIO_BASE(i915, dmc_id) \
	((DISPLAY_VER(i915) >= 13 ? _ADLP_PIPEDMC_REG_MMIO_BASE_A : \
					_TGL_PIPEDMC_REG_MMIO_BASE_A) + \
	 0x400 * ((dmc_id) - 1))

#define __DMC_REG_MMIO_BASE		0x8f000

#define _DMC_REG_MMIO_BASE(i915, dmc_id) \
	((dmc_id) == DMC_FW_MAIN ? __DMC_REG_MMIO_BASE : \
				   __PIPEDMC_REG_MMIO_BASE(i915, dmc_id))

#define _DMC_REG(i915, dmc_id, reg) \
	((reg) - __DMC_REG_MMIO_BASE + _DMC_REG_MMIO_BASE(i915, dmc_id))

#define DMC_EVENT_HANDLER_COUNT_GEN12	8

#define _DMC_EVT_HTP_0			0x8f004

#define DMC_EVT_HTP(i915, dmc_id, handler) \
	_MMIO(_DMC_REG(i915, dmc_id, _DMC_EVT_HTP_0) + 4 * (handler))

#define _DMC_EVT_CTL_0			0x8f034

#define DMC_EVT_CTL(i915, dmc_id, handler) \
	_MMIO(_DMC_REG(i915, dmc_id, _DMC_EVT_CTL_0) + 4 * (handler))

#define DMC_EVT_CTL_ENABLE		REG_BIT(31)
#define DMC_EVT_CTL_RECURRING		REG_BIT(30)
#define DMC_EVT_CTL_TYPE_MASK		REG_GENMASK(17, 16)
#define DMC_EVT_CTL_TYPE_LEVEL_0	0
#define DMC_EVT_CTL_TYPE_LEVEL_1	1
#define DMC_EVT_CTL_TYPE_EDGE_1_0	2
#define DMC_EVT_CTL_TYPE_EDGE_0_1	3
#define DMC_EVT_CTL_EVENT_ID_MASK	REG_GENMASK(15, 8)

#define DMC_HTP_ADDR_SKL	0x00500034
#define DMC_SSP_BASE		_MMIO(0x8F074)
#define DMC_HTP_SKL		_MMIO(0x8F004)
#define DMC_LAST_WRITE		_MMIO(0x8F034)
#define DMC_LAST_WRITE_VALUE	0xc003b400
#define DMC_MMIO_START_RANGE	0x80000
#define DMC_MMIO_END_RANGE     0x8FFFF
#define DMC_V1_MMIO_START_RANGE		0x80000
#define TGL_MAIN_MMIO_START		0x8F000
#define TGL_MAIN_MMIO_END		0x8FFFF
#define _TGL_PIPEA_MMIO_START		0x92000
#define _TGL_PIPEA_MMIO_END		0x93FFF
#define _TGL_PIPEB_MMIO_START		0x96000
#define _TGL_PIPEB_MMIO_END		0x97FFF
#define ADLP_PIPE_MMIO_START		0x5F000
#define ADLP_PIPE_MMIO_END		0x5FFFF

#define DMC_PROGRAM(addr, i)	_MMIO((addr) + (i) * 4)
enum dmc_event_id {
	DMC_EVENT_TRUE = 0x0,
	DMC_EVENT_FALSE = 0x1,
};

enum intel_pch {
	PCH_NOP = -1,	/* PCH without south display */
	PCH_NONE = 0,	/* No PCH present */
	PCH_IBX,	/* Ibexpeak PCH */
	PCH_CPT,	/* Cougarpoint/Pantherpoint PCH */
	PCH_LPT_H,	/* Lynxpoint/Wildcatpoint H PCH */
	PCH_LPT_LP,	/* Lynxpoint/Wildcatpoint LP PCH */
	PCH_SPT,        /* Sunrisepoint/Kaby Lake PCH */
	PCH_CNP,        /* Cannon/Comet Lake PCH */
	PCH_ICP,	/* Ice Lake/Jasper Lake PCH */
	PCH_TGP,	/* Tiger Lake/Mule Creek Canyon PCH */
	PCH_ADP,	/* Alder Lake PCH */

	/* Fake PCHs, functionality handled on the same PCI dev */
	PCH_DG1 = 1024,
	PCH_DG2,
	PCH_MTL,
	PCH_LNL,
};

#define INTEL_PCH_DEVICE_ID_MASK		0xff80
#define INTEL_PCH_IBX_DEVICE_ID_TYPE		0x3b00
#define INTEL_PCH_CPT_DEVICE_ID_TYPE		0x1c00
#define INTEL_PCH_PPT_DEVICE_ID_TYPE		0x1e00
#define INTEL_PCH_LPT_DEVICE_ID_TYPE		0x8c00
#define INTEL_PCH_LPT_LP_DEVICE_ID_TYPE		0x9c00
#define INTEL_PCH_WPT_DEVICE_ID_TYPE		0x8c80
#define INTEL_PCH_WPT_LP_DEVICE_ID_TYPE		0x9c80
#define INTEL_PCH_SPT_DEVICE_ID_TYPE		0xA100
#define INTEL_PCH_SPT_LP_DEVICE_ID_TYPE		0x9D00
#define INTEL_PCH_KBP_DEVICE_ID_TYPE		0xA280
#define INTEL_PCH_CNP_DEVICE_ID_TYPE		0xA300
#define INTEL_PCH_CNP_LP_DEVICE_ID_TYPE		0x9D80
#define INTEL_PCH_CMP_DEVICE_ID_TYPE		0x0280
#define INTEL_PCH_CMP2_DEVICE_ID_TYPE		0x0680
#define INTEL_PCH_CMP_V_DEVICE_ID_TYPE		0xA380
#define INTEL_PCH_ICP_DEVICE_ID_TYPE		0x3480
#define INTEL_PCH_ICP2_DEVICE_ID_TYPE		0x3880
#define INTEL_PCH_MCC_DEVICE_ID_TYPE		0x4B00
#define INTEL_PCH_TGP_DEVICE_ID_TYPE		0xA080
#define INTEL_PCH_TGP2_DEVICE_ID_TYPE		0x4380
#define INTEL_PCH_JSP_DEVICE_ID_TYPE		0x4D80
#define INTEL_PCH_ADP_DEVICE_ID_TYPE		0x7A80
#define INTEL_PCH_ADP2_DEVICE_ID_TYPE		0x5180
#define INTEL_PCH_ADP3_DEVICE_ID_TYPE		0x7A00
#define INTEL_PCH_ADP4_DEVICE_ID_TYPE		0x5480
#define INTEL_PCH_P2X_DEVICE_ID_TYPE		0x7100
#define INTEL_PCH_P3X_DEVICE_ID_TYPE		0x7000
#define INTEL_PCH_QEMU_DEVICE_ID_TYPE		0x2900 



#define DISPLAY_INFO(__display)		((__display)->info.__device_info)
#define DISPLAY_RUNTIME_INFO(__display)	(&(__display)->info.__runtime_info)

#define DISPLAY_VER(__display)		(DISPLAY_RUNTIME_INFO(__display)->ip.ver)
#define DISPLAY_VERx100(__display)	(DISPLAY_RUNTIME_INFO(__display)->ip.ver * 100 + \
					 DISPLAY_RUNTIME_INFO(__display)->ip.rel)

#define DMC_DEFAULT_FW_OFFSET		0xFFFFFFFF
#define PACKAGE_MAX_FW_INFO_ENTRIES	20
#define PACKAGE_V2_MAX_FW_INFO_ENTRIES	32
#define DMC_V1_MAX_MMIO_COUNT		8
#define DMC_V3_MAX_MMIO_COUNT		20
#define DMC_V1_MMIO_START_RANGE		0x80000

#define _PIPEDMC_CONTROL_A		0x45250
#define _PIPEDMC_CONTROL_B		0x45254
#define  PIPEDMC_ENABLE			REG_BIT(0)
#define  DC_STATE_DEBUG                  (0x45520)
#define  DC_STATE_DEBUG_MASK_CORES	(1 << 0)
#define  DC_STATE_DEBUG_MASK_MEMORY_UP	(1 << 1)

#define DMC_HTP_ADDR_SKL	0x00500034
#define DMC_SSP_BASE		_MMIO(0x8F074)
#define DMC_HTP_SKL		_MMIO(0x8F004)
#define DMC_LAST_WRITE		_MMIO(0x8F034)
#define DMC_LAST_WRITE_VALUE	0xc003b400
#define DMC_MMIO_START_RANGE	0x80000
#define DMC_MMIO_END_RANGE     0x8FFFF
#define DMC_V1_MMIO_START_RANGE		0x80000
#define TGL_MAIN_MMIO_START		0x8F000
#define TGL_MAIN_MMIO_END		0x8FFFF
#define _TGL_PIPEA_MMIO_START		0x92000
#define _TGL_PIPEA_MMIO_END		0x93FFF
#define _TGL_PIPEB_MMIO_START		0x96000
#define _TGL_PIPEB_MMIO_END		0x97FFF
#define ADLP_PIPE_MMIO_START		0x5F000
#define ADLP_PIPE_MMIO_END		0x5FFFF

#define TGL_PIPE_MMIO_START(dmc_id)	_PICK_EVEN(((dmc_id) - 1), _TGL_PIPEA_MMIO_START,\
						  _TGL_PIPEB_MMIO_START)

#define TGL_PIPE_MMIO_END(dmc_id)	_PICK_EVEN(((dmc_id) - 1), _TGL_PIPEA_MMIO_END,\
						  _TGL_PIPEB_MMIO_END)

#define PACKAGE_MAX_FW_INFO_ENTRIES   20
#define PACKAGE_V2_MAX_FW_INFO_ENTRIES 32

// ============================================================================
// STRUCTURES
// ============================================================================

struct intel_css_header {
	uint32_t module_type;
	uint32_t header_len;
	uint32_t header_ver;
	uint32_t module_id;
	uint32_t module_vendor;
	uint32_t date;
	uint32_t size;
	uint32_t key_size;
	uint32_t modulus_size;
	uint32_t exponent_size;
	uint32_t reserved1[12];
	uint32_t version;
	uint32_t reserved2[8];
	uint32_t kernel_header_info;
} __attribute__((packed));

struct intel_package_header {
	uint8_t  header_len;
	uint8_t  header_ver;
	uint8_t  reserved[10];
	uint32_t num_entries;
} __attribute__((packed));

struct intel_fw_info {
	uint8_t  reserved1;
	uint8_t  dmc_id;
	char     stepping;
	char     substepping;
	uint32_t offset;
	uint32_t reserved2;
} __attribute__((packed));

struct intel_dmc_header_base {
	uint32_t signature;
	uint8_t  header_len;
	uint8_t  header_ver;
	uint16_t dmcc_ver;
	uint32_t project;
	uint32_t fw_size;
	uint32_t fw_version;
} __attribute__((packed));

struct intel_dmc_header_v1 {
	struct intel_dmc_header_base base;
	uint32_t mmio_count;
	uint32_t mmioaddr[DMC_V1_MAX_MMIO_COUNT];
	uint32_t mmiodata[DMC_V1_MAX_MMIO_COUNT];
	char dfile[32];
	uint32_t reserved1[2];
} __attribute__((packed));

struct intel_dmc_header_v3 {
	struct intel_dmc_header_base base;
	uint32_t start_mmioaddr;
	uint32_t reserved[9];
	char dfile[32];
	uint32_t mmio_count;
	uint32_t mmioaddr[DMC_V3_MAX_MMIO_COUNT];
	uint32_t mmiodata[DMC_V3_MAX_MMIO_COUNT];
} __attribute__((packed));

struct dmc_fw_info {
	u32 mmio_count;
	u32 mmioaddr[20];
	u32 mmiodata[20];
	u32 dmc_offset;
	u32 start_mmioaddr;
	u32 dmc_fw_size; /*dwords */
	u32 *payload;
	bool present;
};

enum intel_dmc_id {
	DMC_FW_MAIN = 0,
	DMC_FW_PIPEA,
	DMC_FW_PIPEB,
	DMC_FW_PIPEC,
	DMC_FW_PIPED,
	DMC_FW_MAX
};

struct intel_dmc {
	struct intel_display *display;
	//struct work_struct work;
	const char *fw_path;
	u32 max_fw_size; /* bytes */
	u32 version;
	struct {
		u32 dc5_start;
		u32 count;
	} dc6_allowed;
	struct dmc_fw_info dmc_info[DMC_FW_MAX];
};

enum intel_platform {
	INTEL_PLATFORM_UNINITIALIZED = 0,
	/* gen2 */
	INTEL_I830,
	INTEL_I845G,
	INTEL_I85X,
	INTEL_I865G,
	/* gen3 */
	INTEL_I915G,
	INTEL_I915GM,
	INTEL_I945G,
	INTEL_I945GM,
	INTEL_G33,
	INTEL_PINEVIEW,
	/* gen4 */
	INTEL_I965G,
	INTEL_I965GM,
	INTEL_G45,
	INTEL_GM45,
	/* gen5 */
	INTEL_IRONLAKE,
	/* gen6 */
	INTEL_SANDYBRIDGE,
	/* gen7 */
	INTEL_IVYBRIDGE,
	INTEL_VALLEYVIEW,
	INTEL_HASWELL,
	/* gen8 */
	INTEL_BROADWELL,
	INTEL_CHERRYVIEW,
	/* gen9 */
	INTEL_SKYLAKE,
	INTEL_BROXTON,
	INTEL_KABYLAKE,
	INTEL_GEMINILAKE,
	INTEL_COFFEELAKE,
	INTEL_COMETLAKE,
	/* gen11 */
	INTEL_ICELAKE,
	INTEL_ELKHARTLAKE,
	INTEL_JASPERLAKE,
	/* gen12 */
	INTEL_TIGERLAKE,
	INTEL_ROCKETLAKE,
	INTEL_DG1,
	INTEL_ALDERLAKE_S,
	INTEL_ALDERLAKE_P,
	INTEL_DG2,
	INTEL_METEORLAKE,
	INTEL_MAX_PLATFORMS
};

#define PLATFORM_NAME(x) [INTEL_##x] = #x
static const char * const platform_names[] = {
	PLATFORM_NAME(I830),
	PLATFORM_NAME(I845G),
	PLATFORM_NAME(I85X),
	PLATFORM_NAME(I865G),
	PLATFORM_NAME(I915G),
	PLATFORM_NAME(I915GM),
	PLATFORM_NAME(I945G),
	PLATFORM_NAME(I945GM),
	PLATFORM_NAME(G33),
	PLATFORM_NAME(PINEVIEW),
	PLATFORM_NAME(I965G),
	PLATFORM_NAME(I965GM),
	PLATFORM_NAME(G45),
	PLATFORM_NAME(GM45),
	PLATFORM_NAME(IRONLAKE),
	PLATFORM_NAME(SANDYBRIDGE),
	PLATFORM_NAME(IVYBRIDGE),
	PLATFORM_NAME(VALLEYVIEW),
	PLATFORM_NAME(HASWELL),
	PLATFORM_NAME(BROADWELL),
	PLATFORM_NAME(CHERRYVIEW),
	PLATFORM_NAME(SKYLAKE),
	PLATFORM_NAME(BROXTON),
	PLATFORM_NAME(KABYLAKE),
	PLATFORM_NAME(GEMINILAKE),
	PLATFORM_NAME(COFFEELAKE),
	PLATFORM_NAME(COMETLAKE),
	PLATFORM_NAME(ICELAKE),
	PLATFORM_NAME(ELKHARTLAKE),
	PLATFORM_NAME(JASPERLAKE),
	PLATFORM_NAME(TIGERLAKE),
	PLATFORM_NAME(ROCKETLAKE),
	PLATFORM_NAME(DG1),
	PLATFORM_NAME(ALDERLAKE_S),
	PLATFORM_NAME(ALDERLAKE_P),
	PLATFORM_NAME(DG2),
	PLATFORM_NAME(METEORLAKE),
};


static const u16 subplatform_uy_ids[] = {
	INTEL_TGL_GT2_IDS(ID),
};

static const u16 subplatform_n_ids[] = {
	INTEL_ADLN_IDS(ID),
};

static const u16 subplatform_rpl_ids[] = {
	INTEL_RPLS_IDS(ID),
	INTEL_RPLU_IDS(ID),
	INTEL_RPLP_IDS(ID),
};

static const u16 subplatform_rplu_ids[] = {
	INTEL_RPLU_IDS(ID),
};



typedef u32 intel_engine_mask_t;
#define INTEL_SUBPLATFORM_N    1
#define INTEL_SUBPLATFORM_RPLU  2

#define INTEL_SUBPLATFORM_ARL_H	0
#define INTEL_SUBPLATFORM_ARL_U	1
#define INTEL_SUBPLATFORM_ARL_S	2


#define DEV_INFO_FOR_EACH_FLAG(func) \
	func(is_mobile); \
	func(require_force_probe); \
	func(is_dgfx); \
	/* Keep has_* in alphabetical order */ \
	func(has_64bit_reloc); \
	func(has_64k_pages); \
	func(gpu_reset_clobbers_display); \
	func(has_reset_engine); \
	func(has_3d_pipeline); \
	func(has_flat_ccs); \
	func(has_global_mocs); \
	func(has_gmd_id); \
	func(has_gt_uc); \
	func(has_heci_pxp); \
	func(has_heci_gscfi); \
	func(has_guc_deprivilege); \
	func(has_guc_tlb_invalidation); \
	func(has_l3_ccs_read); \
	func(has_l3_dpf); \
	func(has_llc); \
	func(has_logical_ring_contexts); \
	func(has_logical_ring_elsq); \
	func(has_media_ratio_mode); \
	func(has_mslice_steering); \
	func(has_oa_bpc_reporting); \
	func(has_oa_slice_contrib_limits); \
	func(has_oam); \
	func(has_one_eu_per_fuse_bit); \
	func(has_pxp); \
	func(has_rc6); \
	func(has_rc6p); \
	func(has_rps); \
	func(has_runtime_pm); \
	func(has_snoop); \
	func(has_coherent_ggtt); \
	func(tuning_thread_rr_after_dep); \
	func(unfenced_needs_alignment); \
	func(hws_needs_physical);

struct intel_ip_version {
	u8 ver;
	u8 rel;
	u8 step;
};
struct intel_step_info {
	u8 graphics_step;	/* Represents the compute tile on Xe_HPC */
	u8 media_step;
};

#define I915_GEM_PPGTT_NONE	0
#define I915_GEM_PPGTT_ALIASING	1
#define I915_GEM_PPGTT_FULL	2
enum intel_ppgtt_type {
	INTEL_PPGTT_NONE = I915_GEM_PPGTT_NONE,
	INTEL_PPGTT_ALIASING = I915_GEM_PPGTT_ALIASING,
	INTEL_PPGTT_FULL = I915_GEM_PPGTT_FULL,
};

enum intel_gt_type {
	GT_PRIMARY,
	GT_TILE,
	GT_MEDIA,
};

struct intel_runtime_info {
	/*
	 * Single "graphics" IP version that represents
	 * render, compute and copy behavior.
	 */
	struct {
		struct intel_ip_version ip;
	} graphics;
	struct {
		struct intel_ip_version ip;
	} media;

	/*
	 * Platform mask is used for optimizing or-ed IS_PLATFORM calls into
	 * single runtime conditionals, and also to provide groundwork for
	 * future per platform, or per SKU build optimizations.
	 *
	 * Array can be extended when necessary if the corresponding
	 * BUILD_BUG_ON is hit.
	 */
	u32 platform_mask[2];

	u16 device_id;

	struct intel_step_info step;

	unsigned int page_sizes; /* page sizes supported by the HW */

	enum intel_ppgtt_type ppgtt_type;
	unsigned int ppgtt_size; /* log2, e.g. 31/32/48 bits */

	bool has_pooled_eu;
};
enum i915_cache_level {

	I915_CACHE_NONE = 0,

	I915_CACHE_LLC,

	I915_CACHE_L3_LLC,

	I915_CACHE_WT,

	I915_MAX_CACHE_LEVEL,
};
struct intel_driver_caps {
	unsigned int scheduler;
	bool has_logical_contexts:1;
};

struct intel_gt_definition {
	enum intel_gt_type type;
	char *name;
	u32 mapping_base;
	u32 gsi_offset;
	intel_engine_mask_t engine_mask;
};

struct intel_device_info {
	enum intel_platform platform;

	unsigned int dma_mask_size; /* available DMA address bits */

	const struct intel_gt_definition *extra_gt_list;

	u8 gt; /* GT number, 0 if undefined */

	intel_engine_mask_t platform_engine_mask; /* Engines supported by the HW */
	u32 memory_regions; /* regions supported by the HW */

#define DEFINE_FLAG(name) u8 name:1
	DEV_INFO_FOR_EACH_FLAG(DEFINE_FLAG);
#undef DEFINE_FLAG

	/*
	 * Initial runtime info. Do not access outside of i915_driver_create().
	 */
	const struct intel_runtime_info __runtime;

	u32 cachelevel_to_pat[I915_MAX_CACHE_LEVEL];
	u32 max_pat_index;
};





#define I915_GTT_PAGE_SIZE_4K	BIT_ULL(12)
#define I915_GTT_PAGE_SIZE_64K	BIT_ULL(16)
#define I915_GTT_PAGE_SIZE_2M	BIT_ULL(21)

enum intel_region_id {
	INTEL_REGION_SMEM = 0,
	INTEL_REGION_LMEM_0,
	INTEL_REGION_LMEM_1,
	INTEL_REGION_LMEM_2,
	INTEL_REGION_LMEM_3,
	INTEL_REGION_STOLEN_SMEM,
	INTEL_REGION_STOLEN_LMEM,
	INTEL_REGION_UNKNOWN, /* Should be last */
};

#define GEN9_CLKGATE_DIS_0		_MMIO(0x46530)
#define   DARBF_GATING_DIS		REG_BIT(27)
#define CLKREQ_POLICY			_MMIO(0x101038)
#define  CLKREQ_POLICY_MEM_UP_OVRD	REG_BIT(1)

enum intel_engine_id {
	RCS0 = 0,
	BCS0,
	BCS1,
	BCS2,
	BCS3,
	BCS4,
	BCS5,
	BCS6,
	BCS7,
	BCS8,
#define _BCS(n) (BCS0 + (n))
	VCS0,
	VCS1,
	VCS2,
	VCS3,
	VCS4,
	VCS5,
	VCS6,
	VCS7,
#define _VCS(n) (VCS0 + (n))
	VECS0,
	VECS1,
	VECS2,
	VECS3,
#define _VECS(n) (VECS0 + (n))
	CCS0,
	CCS1,
	CCS2,
	CCS3,
#define _CCS(n) (CCS0 + (n))
	GSC0,
	I915_NUM_ENGINES
#define INVALID_ENGINE ((enum intel_engine_id)-1)
};


#define MTL_MEDIA_GSI_BASE		0x380000
#define GMD_ID_GRAPHICS				_MMIO(0xd8c)
#define GMD_ID_MEDIA				_MMIO(MTL_MEDIA_GSI_BASE + 0xd8c)
#define   GMD_ID_ARCH_MASK			REG_GENMASK(31, 22)
#define   GMD_ID_RELEASE_MASK			REG_GENMASK(21, 14)
#define   GMD_ID_STEP				REG_GENMASK(5, 0)

#define PLATFORM2(x) .platform = (x)
#define GEN(x) \
	.__runtime.graphics.ip.ver = (x), \
	.__runtime.media.ip.ver = (x)

#define LEGACY_CACHELEVEL \
	.cachelevel_to_pat = { \
		[I915_CACHE_NONE]   = 0, \
		[I915_CACHE_LLC]    = 1, \
		[I915_CACHE_L3_LLC] = 2, \
		[I915_CACHE_WT]     = 3, \
	}

#define TGL_CACHELEVEL \
	.cachelevel_to_pat = { \
		[I915_CACHE_NONE]   = 3, \
		[I915_CACHE_LLC]    = 0, \
		[I915_CACHE_L3_LLC] = 0, \
		[I915_CACHE_WT]     = 2, \
	}

#define MTL_CACHELEVEL \
	.cachelevel_to_pat = { \
		[I915_CACHE_NONE]   = 2, \
		[I915_CACHE_LLC]    = 3, \
		[I915_CACHE_L3_LLC] = 3, \
		[I915_CACHE_WT]     = 1, \
	}

/* Keep in gen based order, and chronological order within a gen */

#define GEN_DEFAULT_PAGE_SIZES \
	.__runtime.page_sizes = I915_GTT_PAGE_SIZE_4K

#define GEN_DEFAULT_REGIONS \
	.memory_regions = BIT(INTEL_REGION_SMEM) | BIT(INTEL_REGION_STOLEN_SMEM)

#define I830_FEATURES \
	GEN(2), \
	.is_mobile = 1, \
	.gpu_reset_clobbers_display = true, \
	.has_3d_pipeline = 1, \
	.hws_needs_physical = 1, \
	.unfenced_needs_alignment = 1, \
	.platform_engine_mask = BIT(RCS0), \
	.has_snoop = true, \
	.has_coherent_ggtt = false, \
	.dma_mask_size = 32, \
	.max_pat_index = 3, \
	GEN_DEFAULT_PAGE_SIZES, \
	GEN_DEFAULT_REGIONS, \
	LEGACY_CACHELEVEL

#define I845_FEATURES \
	GEN(2), \
	.has_3d_pipeline = 1, \
	.gpu_reset_clobbers_display = true, \
	.hws_needs_physical = 1, \
	.unfenced_needs_alignment = 1, \
	.platform_engine_mask = BIT(RCS0), \
	.has_snoop = true, \
	.has_coherent_ggtt = false, \
	.dma_mask_size = 32, \
	.max_pat_index = 3, \
	GEN_DEFAULT_PAGE_SIZES, \
	GEN_DEFAULT_REGIONS, \
	LEGACY_CACHELEVEL

static const struct intel_device_info i830_info = {
	I830_FEATURES,
	PLATFORM2(INTEL_I830),
};

static const struct intel_device_info i845g_info = {
	I845_FEATURES,
	PLATFORM2(INTEL_I845G),
};

static const struct intel_device_info i85x_info = {
	I830_FEATURES,
	PLATFORM2(INTEL_I85X),
};

static const struct intel_device_info i865g_info = {
	I845_FEATURES,
	PLATFORM2(INTEL_I865G),
};

#define GEN3_FEATURES \
	GEN(3), \
	.gpu_reset_clobbers_display = true, \
	.platform_engine_mask = BIT(RCS0), \
	.has_3d_pipeline = 1, \
	.has_snoop = true, \
	.has_coherent_ggtt = true, \
	.dma_mask_size = 32, \
	.max_pat_index = 3, \
	GEN_DEFAULT_PAGE_SIZES, \
	GEN_DEFAULT_REGIONS, \
	LEGACY_CACHELEVEL

static const struct intel_device_info i915g_info = {
	GEN3_FEATURES,
	PLATFORM2(INTEL_I915G),
	.has_coherent_ggtt = false,
	.hws_needs_physical = 1,
	.unfenced_needs_alignment = 1,
};

static const struct intel_device_info i915gm_info = {
	GEN3_FEATURES,
	PLATFORM2(INTEL_I915GM),
	.is_mobile = 1,
	.hws_needs_physical = 1,
	.unfenced_needs_alignment = 1,
};

static const struct intel_device_info i945g_info = {
	GEN3_FEATURES,
	PLATFORM2(INTEL_I945G),
	.hws_needs_physical = 1,
	.unfenced_needs_alignment = 1,
};

static const struct intel_device_info i945gm_info = {
	GEN3_FEATURES,
	PLATFORM2(INTEL_I945GM),
	.is_mobile = 1,
	.hws_needs_physical = 1,
	.unfenced_needs_alignment = 1,
};

static const struct intel_device_info g33_info = {
	GEN3_FEATURES,
	PLATFORM2(INTEL_G33),
	.dma_mask_size = 36,
};

static const struct intel_device_info pnv_g_info = {
	GEN3_FEATURES,
	PLATFORM2(INTEL_PINEVIEW),
	.dma_mask_size = 36,
};

static const struct intel_device_info pnv_m_info = {
	GEN3_FEATURES,
	PLATFORM2(INTEL_PINEVIEW),
	.is_mobile = 1,
	.dma_mask_size = 36,
};

#define GEN4_FEATURES \
	GEN(4), \
	.gpu_reset_clobbers_display = true, \
	.platform_engine_mask = BIT(RCS0), \
	.has_3d_pipeline = 1, \
	.has_snoop = true, \
	.has_coherent_ggtt = true, \
	.dma_mask_size = 36, \
	.max_pat_index = 3, \
	GEN_DEFAULT_PAGE_SIZES, \
	GEN_DEFAULT_REGIONS, \
	LEGACY_CACHELEVEL

static const struct intel_device_info i965g_info = {
	GEN4_FEATURES,
	PLATFORM2(INTEL_I965G),
	.hws_needs_physical = 1,
	.has_snoop = false,
};

static const struct intel_device_info i965gm_info = {
	GEN4_FEATURES,
	PLATFORM2(INTEL_I965GM),
	.is_mobile = 1,
	.hws_needs_physical = 1,
	.has_snoop = false,
};

static const struct intel_device_info g45_info = {
	GEN4_FEATURES,
	PLATFORM2(INTEL_G45),
	.platform_engine_mask = BIT(RCS0) | BIT(VCS0),
	.gpu_reset_clobbers_display = false,
};

static const struct intel_device_info gm45_info = {
	GEN4_FEATURES,
	PLATFORM2(INTEL_GM45),
	.is_mobile = 1,
	.platform_engine_mask = BIT(RCS0) | BIT(VCS0),
	.gpu_reset_clobbers_display = false,
};

#define GEN5_FEATURES \
	GEN(5), \
	.platform_engine_mask = BIT(RCS0) | BIT(VCS0), \
	.has_3d_pipeline = 1, \
	.has_snoop = true, \
	.has_coherent_ggtt = true, \
	/* ilk does support rc6, but we do not implement [power] contexts */ \
	.has_rc6 = 0, \
	.dma_mask_size = 36, \
	.max_pat_index = 3, \
	GEN_DEFAULT_PAGE_SIZES, \
	GEN_DEFAULT_REGIONS, \
	LEGACY_CACHELEVEL

static const struct intel_device_info ilk_d_info = {
	GEN5_FEATURES,
	PLATFORM2(INTEL_IRONLAKE),
};

static const struct intel_device_info ilk_m_info = {
	GEN5_FEATURES,
	PLATFORM2(INTEL_IRONLAKE),
	.is_mobile = 1,
	.has_rps = true,
};

#define GEN6_FEATURES \
	GEN(6), \
	.platform_engine_mask = BIT(RCS0) | BIT(VCS0) | BIT(BCS0), \
	.has_3d_pipeline = 1, \
	.has_coherent_ggtt = true, \
	.has_llc = 1, \
	.has_rc6 = 1, \
	/* snb does support rc6p, but enabling it causes various issues */ \
	.has_rc6p = 0, \
	.has_rps = true, \
	.dma_mask_size = 40, \
	.max_pat_index = 3, \
	.__runtime.ppgtt_type = INTEL_PPGTT_ALIASING, \
	.__runtime.ppgtt_size = 31, \
	GEN_DEFAULT_PAGE_SIZES, \
	GEN_DEFAULT_REGIONS, \
	LEGACY_CACHELEVEL

#define SNB_D_PLATFORM \
	GEN6_FEATURES, \
	PLATFORM2(INTEL_SANDYBRIDGE)

static const struct intel_device_info snb_d_gt1_info = {
	SNB_D_PLATFORM,
	.gt = 1,
};

static const struct intel_device_info snb_d_gt2_info = {
	SNB_D_PLATFORM,
	.gt = 2,
};

#define SNB_M_PLATFORM \
	GEN6_FEATURES, \
	PLATFORM2(INTEL_SANDYBRIDGE), \
	.is_mobile = 1


static const struct intel_device_info snb_m_gt1_info = {
	SNB_M_PLATFORM,
	.gt = 1,
};

static const struct intel_device_info snb_m_gt2_info = {
	SNB_M_PLATFORM,
	.gt = 2,
};

#define GEN7_FEATURES  \
	GEN(7), \
	.platform_engine_mask = BIT(RCS0) | BIT(VCS0) | BIT(BCS0), \
	.has_3d_pipeline = 1, \
	.has_coherent_ggtt = true, \
	.has_llc = 1, \
	.has_rc6 = 1, \
	.has_rc6p = 1, \
	.has_reset_engine = true, \
	.has_rps = true, \
	.dma_mask_size = 40, \
	.max_pat_index = 3, \
	.__runtime.ppgtt_type = INTEL_PPGTT_ALIASING, \
	.__runtime.ppgtt_size = 31, \
	GEN_DEFAULT_PAGE_SIZES, \
	GEN_DEFAULT_REGIONS, \
	LEGACY_CACHELEVEL

#define IVB_D_PLATFORM \
	GEN7_FEATURES, \
	PLATFORM2(INTEL_IVYBRIDGE), \
	.has_l3_dpf = 1

static const struct intel_device_info ivb_d_gt1_info = {
	IVB_D_PLATFORM,
	.gt = 1,
};

static const struct intel_device_info ivb_d_gt2_info = {
	IVB_D_PLATFORM,
	.gt = 2,
};

#define IVB_M_PLATFORM \
	GEN7_FEATURES, \
	PLATFORM2(INTEL_IVYBRIDGE), \
	.is_mobile = 1, \
	.has_l3_dpf = 1

static const struct intel_device_info ivb_m_gt1_info = {
	IVB_M_PLATFORM,
	.gt = 1,
};

static const struct intel_device_info ivb_m_gt2_info = {
	IVB_M_PLATFORM,
	.gt = 2,
};

static const struct intel_device_info ivb_q_info = {
	GEN7_FEATURES,
	PLATFORM2(INTEL_IVYBRIDGE),
	.gt = 2,
	.has_l3_dpf = 1,
};

static const struct intel_device_info vlv_info = {
	PLATFORM2(INTEL_VALLEYVIEW),
	GEN(7),
	.has_runtime_pm = 1,
	.has_rc6 = 1,
	.has_reset_engine = true,
	.has_rps = true,
	.dma_mask_size = 40,
	.max_pat_index = 3,
	.__runtime.ppgtt_type = INTEL_PPGTT_ALIASING,
	.__runtime.ppgtt_size = 31,
	.has_snoop = true,
	.has_coherent_ggtt = false,
	.platform_engine_mask = BIT(RCS0) | BIT(VCS0) | BIT(BCS0),
	GEN_DEFAULT_PAGE_SIZES,
	GEN_DEFAULT_REGIONS,
	LEGACY_CACHELEVEL,
};

#define G75_FEATURES  \
	GEN7_FEATURES, \
	.platform_engine_mask = BIT(RCS0) | BIT(VCS0) | BIT(BCS0) | BIT(VECS0), \
	.has_rc6p = 0 /* RC6p removed-by HSW */, \
	.has_runtime_pm = 1

#define HSW_PLATFORM \
	G75_FEATURES, \
	PLATFORM2(INTEL_HASWELL), \
	.has_l3_dpf = 1

static const struct intel_device_info hsw_gt1_info = {
	HSW_PLATFORM,
	.gt = 1,
};

static const struct intel_device_info hsw_gt2_info = {
	HSW_PLATFORM,
	.gt = 2,
};

static const struct intel_device_info hsw_gt3_info = {
	HSW_PLATFORM,
	.gt = 3,
};

#define GEN8_FEATURES \
	G75_FEATURES, \
	GEN(8), \
	.has_logical_ring_contexts = 1, \
	.dma_mask_size = 39, \
	.__runtime.ppgtt_type = INTEL_PPGTT_FULL, \
	.__runtime.ppgtt_size = 48, \
	.has_64bit_reloc = 1

#define BDW_PLATFORM \
	GEN8_FEATURES, \
	PLATFORM2(INTEL_BROADWELL)

static const struct intel_device_info bdw_gt1_info = {
	BDW_PLATFORM,
	.gt = 1,
};

static const struct intel_device_info bdw_gt2_info = {
	BDW_PLATFORM,
	.gt = 2,
};

static const struct intel_device_info bdw_rsvd_info = {
	BDW_PLATFORM,
	.gt = 3,
	/* According to the device ID those devices are GT3, they were
	 * previously treated as not GT3, keep it like that.
	 */
};

static const struct intel_device_info bdw_gt3_info = {
	BDW_PLATFORM,
	.gt = 3,
	.platform_engine_mask =
		BIT(RCS0) | BIT(VCS0) | BIT(BCS0) | BIT(VECS0) | BIT(VCS1),
};

static const struct intel_device_info chv_info = {
	PLATFORM2(INTEL_CHERRYVIEW),
	GEN(8),
	.platform_engine_mask = BIT(RCS0) | BIT(VCS0) | BIT(BCS0) | BIT(VECS0),
	.has_64bit_reloc = 1,
	.has_runtime_pm = 1,
	.has_rc6 = 1,
	.has_rps = true,
	.has_logical_ring_contexts = 1,
	.dma_mask_size = 39,
	.max_pat_index = 3,
	.__runtime.ppgtt_type = INTEL_PPGTT_FULL,
	.__runtime.ppgtt_size = 32,
	.has_reset_engine = 1,
	.has_snoop = true,
	.has_coherent_ggtt = false,
	GEN_DEFAULT_PAGE_SIZES,
	GEN_DEFAULT_REGIONS,
	LEGACY_CACHELEVEL,
};

#define GEN9_DEFAULT_PAGE_SIZES \
	.__runtime.page_sizes = I915_GTT_PAGE_SIZE_4K | \
		I915_GTT_PAGE_SIZE_64K

#define GEN9_FEATURES \
	GEN8_FEATURES, \
	GEN(9), \
	GEN9_DEFAULT_PAGE_SIZES, \
	.has_gt_uc = 1

#define SKL_PLATFORM \
	GEN9_FEATURES, \
	PLATFORM2(INTEL_SKYLAKE)

static const struct intel_device_info skl_gt1_info = {
	SKL_PLATFORM,
	.gt = 1,
};

static const struct intel_device_info skl_gt2_info = {
	SKL_PLATFORM,
	.gt = 2,
};

#define SKL_GT3_PLUS_PLATFORM \
	SKL_PLATFORM, \
	.platform_engine_mask = \
		BIT(RCS0) | BIT(VCS0) | BIT(BCS0) | BIT(VECS0) | BIT(VCS1)


static const struct intel_device_info skl_gt3_info = {
	SKL_GT3_PLUS_PLATFORM,
	.gt = 3,
};

static const struct intel_device_info skl_gt4_info = {
	SKL_GT3_PLUS_PLATFORM,
	.gt = 4,
};

#define GEN9_LP_FEATURES \
	GEN(9), \
	.platform_engine_mask = BIT(RCS0) | BIT(VCS0) | BIT(BCS0) | BIT(VECS0), \
	.has_3d_pipeline = 1, \
	.has_64bit_reloc = 1, \
	.has_runtime_pm = 1, \
	.has_rc6 = 1, \
	.has_rps = true, \
	.has_logical_ring_contexts = 1, \
	.has_gt_uc = 1, \
	.dma_mask_size = 39, \
	.__runtime.ppgtt_type = INTEL_PPGTT_FULL, \
	.__runtime.ppgtt_size = 48, \
	.has_reset_engine = 1, \
	.has_snoop = true, \
	.has_coherent_ggtt = false, \
	.max_pat_index = 3, \
	GEN9_DEFAULT_PAGE_SIZES, \
	GEN_DEFAULT_REGIONS, \
	LEGACY_CACHELEVEL

static const struct intel_device_info bxt_info = {
	GEN9_LP_FEATURES,
	PLATFORM2(INTEL_BROXTON),
};

static const struct intel_device_info glk_info = {
	GEN9_LP_FEATURES,
	PLATFORM2(INTEL_GEMINILAKE),
};

#define KBL_PLATFORM \
	GEN9_FEATURES, \
	PLATFORM2(INTEL_KABYLAKE)

static const struct intel_device_info kbl_gt1_info = {
	KBL_PLATFORM,
	.gt = 1,
};

static const struct intel_device_info kbl_gt2_info = {
	KBL_PLATFORM,
	.gt = 2,
};

static const struct intel_device_info kbl_gt3_info = {
	KBL_PLATFORM,
	.gt = 3,
	.platform_engine_mask =
		BIT(RCS0) | BIT(VCS0) | BIT(BCS0) | BIT(VECS0) | BIT(VCS1),
};

#define CFL_PLATFORM \
	GEN9_FEATURES, \
	PLATFORM2(INTEL_COFFEELAKE)

static const struct intel_device_info cfl_gt1_info = {
	CFL_PLATFORM,
	.gt = 1,
};

static const struct intel_device_info cfl_gt2_info = {
	CFL_PLATFORM,
	.gt = 2,
};

static const struct intel_device_info cfl_gt3_info = {
	CFL_PLATFORM,
	.gt = 3,
	.platform_engine_mask =
		BIT(RCS0) | BIT(VCS0) | BIT(BCS0) | BIT(VECS0) | BIT(VCS1),
};

#define CML_PLATFORM \
	GEN9_FEATURES, \
	PLATFORM2(INTEL_COMETLAKE)

static const struct intel_device_info cml_gt1_info = {
	CML_PLATFORM,
	.gt = 1,
};

static const struct intel_device_info cml_gt2_info = {
	CML_PLATFORM,
	.gt = 2,
};

#define GEN11_DEFAULT_PAGE_SIZES \
	.__runtime.page_sizes = I915_GTT_PAGE_SIZE_4K | \
		I915_GTT_PAGE_SIZE_64K |		\
		I915_GTT_PAGE_SIZE_2M

#define GEN11_FEATURES \
	GEN9_FEATURES, \
	GEN11_DEFAULT_PAGE_SIZES, \
	GEN(11), \
	.has_coherent_ggtt = false, \
	.has_logical_ring_elsq = 1

static const struct intel_device_info icl_info = {
	GEN11_FEATURES,
	PLATFORM2(INTEL_ICELAKE),
	.platform_engine_mask =
		BIT(RCS0) | BIT(BCS0) | BIT(VECS0) | BIT(VCS0) | BIT(VCS2),
};

static const struct intel_device_info ehl_info = {
	GEN11_FEATURES,
	PLATFORM2(INTEL_ELKHARTLAKE),
	.platform_engine_mask = BIT(RCS0) | BIT(BCS0) | BIT(VCS0) | BIT(VECS0),
	.__runtime.ppgtt_size = 36,
};

static const struct intel_device_info jsl_info = {
	GEN11_FEATURES,
	PLATFORM2(INTEL_JASPERLAKE),
	.platform_engine_mask = BIT(RCS0) | BIT(BCS0) | BIT(VCS0) | BIT(VECS0),
	.__runtime.ppgtt_size = 36,
};

#define GEN12_FEATURES \
	GEN11_FEATURES, \
	GEN(12), \
	TGL_CACHELEVEL, \
	.has_global_mocs = 1, \
	.has_pxp = 1, \
	.max_pat_index = 3

static const struct intel_device_info tgl_info = {
	GEN12_FEATURES,
	PLATFORM2(INTEL_TIGERLAKE),
	.platform_engine_mask =
		BIT(RCS0) | BIT(BCS0) | BIT(VECS0) | BIT(VCS0) | BIT(VCS2),
};

static const struct intel_device_info adl_s_info = {
	GEN12_FEATURES,
	PLATFORM2(INTEL_ALDERLAKE_S),
	.platform_engine_mask =
		BIT(RCS0) | BIT(BCS0) | BIT(VECS0) | BIT(VCS0) | BIT(VCS2),
	.dma_mask_size = 39,
};

static const struct intel_device_info adl_p_info = {
	GEN12_FEATURES,
	PLATFORM2(INTEL_ALDERLAKE_P),
	.platform_engine_mask =
		BIT(RCS0) | BIT(BCS0) | BIT(VECS0) | BIT(VCS0) | BIT(VCS2),
	.__runtime.ppgtt_size = 48,
	.dma_mask_size = 39,
};

#define DGFX_FEATURES \
	.is_dgfx = 1

#define XE_HP_PAGE_SIZES \
	.__runtime.page_sizes = I915_GTT_PAGE_SIZE_4K | \
		I915_GTT_PAGE_SIZE_64K |		\
		I915_GTT_PAGE_SIZE_2M

#define XE_HP_FEATURES \
	XE_HP_PAGE_SIZES, \
	TGL_CACHELEVEL, \
	.dma_mask_size = 46, \
	.has_3d_pipeline = 1, \
	.has_64bit_reloc = 1, \
	.has_flat_ccs = 1, \
	.has_global_mocs = 1, \
	.has_gt_uc = 1, \
	.has_llc = 1, \
	.has_logical_ring_contexts = 1, \
	.has_logical_ring_elsq = 1, \
	.has_mslice_steering = 1, \
	.has_oa_bpc_reporting = 1, \
	.has_oa_slice_contrib_limits = 1, \
	.has_oam = 1, \
	.has_rc6 = 1, \
	.has_reset_engine = 1, \
	.has_rps = 1, \
	.has_runtime_pm = 1, \
	.max_pat_index = 3, \
	.__runtime.ppgtt_size = 48, \
	.__runtime.ppgtt_type = INTEL_PPGTT_FULL

#define DG2_FEATURES \
	XE_HP_FEATURES, \
	DGFX_FEATURES, \
	.__runtime.graphics.ip.ver = 12, \
	.__runtime.graphics.ip.rel = 55, \
	.__runtime.media.ip.ver = 12, \
	.__runtime.media.ip.rel = 55, \
	PLATFORM2(INTEL_DG2), \
	.has_64k_pages = 1, \
	.has_guc_deprivilege = 1, \
	.has_heci_pxp = 1, \
	.has_media_ratio_mode = 1, \
	.platform_engine_mask = \
		BIT(RCS0) | BIT(BCS0) | \
		BIT(VECS0) | BIT(VECS1) | \
		BIT(VCS0) | BIT(VCS2) | \
		BIT(CCS0) | BIT(CCS1) | BIT(CCS2) | BIT(CCS3)

static const struct intel_device_info dg2_info = {
	DG2_FEATURES,
};

static const struct intel_device_info ats_m_info = {
	DG2_FEATURES,
	.require_force_probe = 1,
	.tuning_thread_rr_after_dep = 1,
};

static const struct intel_gt_definition xelpmp_extra_gt[] = {
	{
		.type = GT_MEDIA,
		.name = (char *)"Standalone Media GT",
		.gsi_offset = MTL_MEDIA_GSI_BASE,
		.engine_mask = BIT(VECS0) | BIT(VCS0) | BIT(VCS2) | BIT(GSC0),
	},
	{}
};

static const struct intel_device_info mtl_info = {
	XE_HP_FEATURES,
	.__runtime.graphics.ip.ver = 12,
	.__runtime.graphics.ip.rel = 70,
	.__runtime.media.ip.ver = 13,
	PLATFORM2(INTEL_METEORLAKE),
	.extra_gt_list = xelpmp_extra_gt,
	.has_flat_ccs = 0,
	.has_gmd_id = 1,
	.has_guc_deprivilege = 1,
	.has_guc_tlb_invalidation = 1,
	.has_llc = 0,
	.has_mslice_steering = 0,
	.has_snoop = 1,
	.max_pat_index = 4,
	.has_pxp = 1,
	.memory_regions = BIT(INTEL_REGION_SMEM) | BIT(INTEL_REGION_STOLEN_LMEM),
	.platform_engine_mask = BIT(RCS0) | BIT(BCS0) | BIT(CCS0),
	MTL_CACHELEVEL,
};

struct pci_device_id {
	unsigned int  vendor, device;
	unsigned int  subvendor, subdevice;
	unsigned int  classb, class_mask;
	unsigned long  driver_data;
	unsigned int  override_only;
};


static const struct {
u32 devid;
const struct intel_device_info *desc;
} intel_platform_ids[] = {
	INTEL_TGL_IDS(INTEL_DISPLAY_DEVICE, &tgl_info),
	INTEL_ADLS_IDS(INTEL_DISPLAY_DEVICE, &adl_s_info),
	INTEL_ADLP_IDS(INTEL_DISPLAY_DEVICE, &adl_p_info),
	INTEL_ADLN_IDS(INTEL_DISPLAY_DEVICE, &adl_p_info),
	INTEL_RPLS_IDS(INTEL_DISPLAY_DEVICE, &adl_s_info),
	INTEL_RPLU_IDS(INTEL_DISPLAY_DEVICE, &adl_p_info),
	INTEL_RPLP_IDS(INTEL_DISPLAY_DEVICE, &adl_p_info),
};

struct drm_i915_private {
	
	struct intel_display *display;
	const struct intel_device_info *__info; /* Use INTEL_INFO() to access. */
	struct intel_runtime_info __runtime; /* Use RUNTIME_INFO() to access. */
	struct intel_driver_caps caps;
};

#define GEN11_CHICKEN_DCPR_2			_MMIO(0x46434)
#define   DCPR_MASK_MAXLATENCY_MEMUP_CLR	REG_BIT(27)
#define   DCPR_MASK_LPMODE			REG_BIT(26)
#define   DCPR_SEND_RESP_IMM			REG_BIT(25)
#define   DCPR_CLEAR_MEMSTAT_DIS		REG_BIT(24)


#define INTEL_WAKEREF_DEF ERR_PTR(-ENOENT)


enum intel_display_power_domain {
	POWER_DOMAIN_DISPLAY_CORE,
	POWER_DOMAIN_PIPE_A,
	POWER_DOMAIN_PIPE_B,
	POWER_DOMAIN_PIPE_C,
	POWER_DOMAIN_PIPE_D,
	POWER_DOMAIN_PIPE_PANEL_FITTER_A,
	POWER_DOMAIN_PIPE_PANEL_FITTER_B,
	POWER_DOMAIN_PIPE_PANEL_FITTER_C,
	POWER_DOMAIN_PIPE_PANEL_FITTER_D,
	POWER_DOMAIN_TRANSCODER_A,
	POWER_DOMAIN_TRANSCODER_B,
	POWER_DOMAIN_TRANSCODER_C,
	POWER_DOMAIN_TRANSCODER_D,
	POWER_DOMAIN_TRANSCODER_EDP,
	POWER_DOMAIN_TRANSCODER_DSI_A,
	POWER_DOMAIN_TRANSCODER_DSI_C,

	/* VDSC/joining for eDP/DSI transcoder (ICL) or pipe A (TGL) */
	POWER_DOMAIN_TRANSCODER_VDSC_PW2,

	POWER_DOMAIN_PORT_DDI_LANES_A,
	POWER_DOMAIN_PORT_DDI_LANES_B,
	POWER_DOMAIN_PORT_DDI_LANES_C,
	POWER_DOMAIN_PORT_DDI_LANES_D,
	POWER_DOMAIN_PORT_DDI_LANES_E,
	POWER_DOMAIN_PORT_DDI_LANES_F,

	POWER_DOMAIN_PORT_DDI_LANES_TC1,
	POWER_DOMAIN_PORT_DDI_LANES_TC2,
	POWER_DOMAIN_PORT_DDI_LANES_TC3,
	POWER_DOMAIN_PORT_DDI_LANES_TC4,
	POWER_DOMAIN_PORT_DDI_LANES_TC5,
	POWER_DOMAIN_PORT_DDI_LANES_TC6,

	POWER_DOMAIN_PORT_DDI_IO_A,
	POWER_DOMAIN_PORT_DDI_IO_B,
	POWER_DOMAIN_PORT_DDI_IO_C,
	POWER_DOMAIN_PORT_DDI_IO_D,
	POWER_DOMAIN_PORT_DDI_IO_E,
	POWER_DOMAIN_PORT_DDI_IO_F,

	POWER_DOMAIN_PORT_DDI_IO_TC1,
	POWER_DOMAIN_PORT_DDI_IO_TC2,
	POWER_DOMAIN_PORT_DDI_IO_TC3,
	POWER_DOMAIN_PORT_DDI_IO_TC4,
	POWER_DOMAIN_PORT_DDI_IO_TC5,
	POWER_DOMAIN_PORT_DDI_IO_TC6,

	POWER_DOMAIN_PORT_DSI,
	POWER_DOMAIN_PORT_CRT,
	POWER_DOMAIN_PORT_OTHER,
	POWER_DOMAIN_VGA,
	POWER_DOMAIN_AUDIO_MMIO,
	POWER_DOMAIN_AUDIO_PLAYBACK,

	POWER_DOMAIN_AUX_IO_A,
	POWER_DOMAIN_AUX_IO_B,
	POWER_DOMAIN_AUX_IO_C,
	POWER_DOMAIN_AUX_IO_D,
	POWER_DOMAIN_AUX_IO_E,
	POWER_DOMAIN_AUX_IO_F,

	POWER_DOMAIN_AUX_A,
	POWER_DOMAIN_AUX_B,
	POWER_DOMAIN_AUX_C,
	POWER_DOMAIN_AUX_D,
	POWER_DOMAIN_AUX_E,
	POWER_DOMAIN_AUX_F,

	POWER_DOMAIN_AUX_USBC1,
	POWER_DOMAIN_AUX_USBC2,
	POWER_DOMAIN_AUX_USBC3,
	POWER_DOMAIN_AUX_USBC4,
	POWER_DOMAIN_AUX_USBC5,
	POWER_DOMAIN_AUX_USBC6,

	POWER_DOMAIN_AUX_TBT1,
	POWER_DOMAIN_AUX_TBT2,
	POWER_DOMAIN_AUX_TBT3,
	POWER_DOMAIN_AUX_TBT4,
	POWER_DOMAIN_AUX_TBT5,
	POWER_DOMAIN_AUX_TBT6,

	POWER_DOMAIN_GMBUS,
	POWER_DOMAIN_GT_IRQ,
	POWER_DOMAIN_DC_OFF,
	POWER_DOMAIN_TC_COLD_OFF,
	POWER_DOMAIN_INIT,

	POWER_DOMAIN_NUM,
	POWER_DOMAIN_INVALID = POWER_DOMAIN_NUM,
};

#define POWER_DOMAIN_PIPE(pipe) \
	((enum intel_display_power_domain)((pipe) - PIPE_A + POWER_DOMAIN_PIPE_A))
#define POWER_DOMAIN_PIPE_PANEL_FITTER(pipe) \
	((enum intel_display_power_domain)((pipe) - PIPE_A + POWER_DOMAIN_PIPE_PANEL_FITTER_A))
#define POWER_DOMAIN_TRANSCODER(tran) \
	((tran) == TRANSCODER_EDP ? POWER_DOMAIN_TRANSCODER_EDP : \
	 (enum intel_display_power_domain)((tran) - TRANSCODER_A + POWER_DOMAIN_TRANSCODER_A))

struct intel_power_domain_mask {
	DECLARE_BITMAP(bits, POWER_DOMAIN_NUM);
};

struct i915_power_well_regs {
	u32 bios;
	u32 driver;
	u32 kvmr;
	u32 debug;
};

struct i915_power_well_ops {
	const struct i915_power_well_regs *regs;

};
struct i915_power_well_desc {
	const struct i915_power_well_ops *ops;
	const struct i915_power_well_instance_list {
		const struct i915_power_well_instance *list;
		u8 count;
	} *instances;

	/* Mask of pipes whose IRQ logic is backed by the pw */
	u16 irq_pipe_mask:4;
	u16 always_on:1;
	/*
	 * Instead of waiting for the status bit to ack enables,
	 * just wait a specific amount of time and then consider
	 * the well enabled.
	 */
	u16 fixed_enable_delay:1;
	u16 has_fuses:1;
	/*
	 * The pw is for an ICL+ TypeC PHY port in
	 * Thunderbolt mode.
	 */
	u16 is_tc_tbt:1;
	/* Enable timeout if greater than the default 1ms */
	u16 enable_timeout;
};

struct i915_power_well {
	const struct i915_power_well_desc *desc;
	struct intel_power_domain_mask domains;
	/* power well enable/disable usage count */
	int count;
	/* cached hw enabled state */
	bool hw_enabled;
	/* index into desc->instances->list */
	u8 instance_idx;
};

struct i915_power_domains {
	/*
	 * Power wells needed for initialization at driver init and suspend
	 * time are on. They are kept on until after the first modeset.
	 */
	bool initializing;
	bool display_core_suspended;
	int power_well_count;

	u32 dc_state;
	u32 target_dc_state;
	u32 allowed_dc_mask;

	//struct ref_tracker *init_wakeref;
	//struct ref_tracker *disable_wakeref;

	IOSimpleLock *lock;
	int domain_use_count[POWER_DOMAIN_NUM];

	//struct delayed_work async_put_work;
	//struct ref_tracker *async_put_wakeref;
	struct intel_power_domain_mask async_put_domains[2];
	int async_put_next_delay;

	struct i915_power_well *power_wells;
};


#define DC_STATE_EN			_MMIO(0x45504)
#define  DC_STATE_DISABLE		0
#define  DC_STATE_EN_DC3CO		REG_BIT(30)
#define  DC_STATE_DC3CO_STATUS		REG_BIT(29)
#define  HOLD_PHY_CLKREQ_PG1_LATCH	REG_BIT(21)
#define  HOLD_PHY_PG1_LATCH		REG_BIT(20)
#define  DC_STATE_EN_UPTO_DC5		(1 << 0)
#define  DC_STATE_EN_DC9		(1 << 3)
#define  DC_STATE_EN_UPTO_DC6		(2 << 0)
#define  DC_STATE_EN_UPTO_DC5_DC6_MASK   0x3
#define DG1_DMC_DEBUG_DC5_COUNT	_MMIO(0x134154)


#define _TRANS(tran, a, b)		_PICK_EVEN(tran, a, b)
#define _MMIO_TRANS(tran, a, b)		_MMIO(_TRANS(tran, a, b))
#define DISPLAY_MMIO_BASE(display)	(DISPLAY_INFO((display))->mmio_offset)
#define INTEL_DISPLAY_DEVICE_TRANS_OFFSET(display, trans) \
	(DISPLAY_INFO((display))->trans_offsets[(trans)] - \
	 DISPLAY_INFO((display))->trans_offsets[TRANSCODER_A] + \
	 DISPLAY_MMIO_BASE((display)))
#define _MMIO_TRANS2(display, trans, reg)	_MMIO(INTEL_DISPLAY_DEVICE_TRANS_OFFSET((display), (trans)) + (reg))
#define _TRANS_DDI_FUNC_CTL_A		0x60400
#define _TRANS_DDI_FUNC_CTL_B		0x61400
#define _TRANS_DDI_FUNC_CTL_C		0x62400
#define _TRANS_DDI_FUNC_CTL_D		0x63400
#define _TRANS_DDI_FUNC_CTL_EDP		0x6F400
#define _TRANS_DDI_FUNC_CTL_DSI0	0x6b400
#define _TRANS_DDI_FUNC_CTL_DSI1	0x6bc00
#define TRANS_DDI_FUNC_CTL(dev_priv, tran) _MMIO_TRANS2(dev_priv, tran, _TRANS_DDI_FUNC_CTL_A)
#define _ICL_COMBOPHY_A				0x162000
#define _ICL_COMBOPHY_B				0x6C000
#define _EHL_COMBOPHY_C				0x160000
#define _RKL_COMBOPHY_D				0x161000
#define _ADL_COMBOPHY_E				0x16B000

#define _ICL_COMBOPHY(phy)			_PICK(phy, _ICL_COMBOPHY_A, \
							  _ICL_COMBOPHY_B, \
							  _EHL_COMBOPHY_C, \
							  _RKL_COMBOPHY_D, \
							  _ADL_COMBOPHY_E)

#define _ICL_PORT_CL_DW(dw, phy)		(_ICL_COMBOPHY(phy) + \
						 4 * (dw))
#define ICL_PORT_CL_DW10(phy)			_MMIO(_ICL_PORT_CL_DW(10, phy))
#define  PWR_DOWN_LN_MASK			REG_GENMASK(7, 4)
#define  PWR_DOWN_LN_3_2			REG_FIELD_PREP(PWR_DOWN_LN_MASK, 0xc)
#define  SPLITTER_ENABLE			(1 << 31)
#define  SPLITTER_CONFIGURATION_MASK		REG_GENMASK(26, 25)
#define  OVERLAP_PIXELS_MASK			(0xf << 16)
#define _ICL_PIPE_DSS_CTL1_PB			0x78200
#define _ICL_PIPE_DSS_CTL1_PC			0x78400
#define ICL_PIPE_DSS_CTL1(pipe)			_MMIO_PIPE((pipe) - PIPE_B, \
							   _ICL_PIPE_DSS_CTL1_PB, \
							   _ICL_PIPE_DSS_CTL1_PC)
#define DP_SET_POWER                        0x600
#define DP_SOURCE_OUI			    0x300
#define DP_SET_POWER_D0                    0x1
#define  PWR_DOWN_LN_1_0			REG_FIELD_PREP(PWR_DOWN_LN_MASK, 0x3)
#define  PWR_UP_ALL_LANES			REG_FIELD_PREP(PWR_DOWN_LN_MASK, 0x0)



#define _ICL_PORT_TX_GRP			0x680
#define   TX_TRAINING_EN			REG_BIT(31)
#define _ICL_PORT_TX_LN(ln)			(0x880 + (ln) * 0x100)
#define   SUS_CLOCK_CONFIG			REG_GENMASK(1, 0)
#define   LOADGEN_SELECT			REG_BIT(31)
#define _ICL_PORT_PCS_GRP			0x600
#define   COMMON_KEEPER_EN			REG_BIT(26)
#define _ICL_PORT_PCS_LN(ln)			(0x800 + (ln) * 0x100)
#define _ICL_PORT_PCS_DW_LN(dw, ln, phy)	 (_ICL_COMBOPHY(phy) + \
						  _ICL_PORT_PCS_LN(ln) + 4 * (dw))
#define ICL_PORT_PCS_DW1_LN(ln, phy)		_MMIO(_ICL_PORT_PCS_DW_LN(1, ln, phy))
#define _ICL_PORT_PCS_DW_GRP(dw, phy)		(_ICL_COMBOPHY(phy) + \
						 _ICL_PORT_PCS_GRP + 4 * (dw))
#define ICL_PORT_PCS_DW1_GRP(phy)		_MMIO(_ICL_PORT_PCS_DW_GRP(1, phy))
#define ICL_PORT_TX_DW4_LN(ln, phy)		_MMIO(_ICL_PORT_TX_DW_LN(4, ln, phy))
#define _ICL_PORT_CL_DW(dw, phy)		(_ICL_COMBOPHY(phy) + \
						 4 * (dw))
#define _ICL_PORT_TX_DW_LN(dw, ln, phy) 	(_ICL_COMBOPHY(phy) + \
						  _ICL_PORT_TX_LN(ln) + 4 * (dw))
#define _ICL_PORT_TX_DW_GRP(dw, phy)		(_ICL_COMBOPHY(phy) + \
						 _ICL_PORT_TX_GRP + 4 * (dw))
#define ICL_PORT_CL_DW5(phy)			_MMIO(_ICL_PORT_CL_DW(5, phy))
#define ICL_PORT_TX_DW5_LN(ln, phy)		_MMIO(_ICL_PORT_TX_DW_LN(5, ln, phy))
#define ICL_PORT_TX_DW5_GRP(phy)		_MMIO(_ICL_PORT_TX_DW_GRP(5, phy))


struct hsw_ddi_buf_trans {
	u32 trans1;	/* balance leg enable, de-emph level */
	u32 trans2;	/* vref sel, vswing */
	u8 i_boost;	/* SKL: I_boost; valid: 0x0, 0x1, 0x3, 0x7 */
};

struct bxt_ddi_buf_trans {
	u8 margin;	/* swing value */
	u8 scale;	/* scale value */
	u8 enable;	/* scale enable */
	u8 deemphasis;
};

struct icl_ddi_buf_trans {
	u8 dw2_swing_sel;
	u8 dw7_n_scalar;
	u8 dw4_cursor_coeff;
	u8 dw4_post_cursor_2;
	u8 dw4_post_cursor_1;
};

struct icl_mg_phy_ddi_buf_trans {
	u8 cri_txdeemph_override_11_6;
	u8 cri_txdeemph_override_5_0;
	u8 cri_txdeemph_override_17_12;
};

struct tgl_dkl_phy_ddi_buf_trans {
	u8 vswing;
	u8 preshoot;
	u8 de_emphasis;
};

struct dg2_snps_phy_buf_trans {
	u8 vswing;
	u8 pre_cursor;
	u8 post_cursor;
};

struct xe3plpd_lt_phy_buf_trans {
	u8 txswing;
	u8 txswing_level;
	u8 pre_cursor;
	u8 main_cursor;
	u8 post_cursor;
};
union intel_ddi_buf_trans_entry {
	struct hsw_ddi_buf_trans hsw;
	struct bxt_ddi_buf_trans bxt;
	struct icl_ddi_buf_trans icl;
	struct icl_mg_phy_ddi_buf_trans mg;
	struct tgl_dkl_phy_ddi_buf_trans dkl;
	struct dg2_snps_phy_buf_trans snps;
	struct xe3plpd_lt_phy_buf_trans lt;
};

struct intel_ddi_buf_trans {
	const union intel_ddi_buf_trans_entry *entries;
	u8 num_entries;
	u8 hdmi_default_entry;
};
static const union intel_ddi_buf_trans_entry _tgl_combo_phy_trans_dp_hbr[] = {
							/* NT mV Trans mV db    */
	{ .icl = { 0xA, 0x32, 0x3F, 0x00, 0x00 } },	/* 350   350      0.0   */
	{ .icl = { 0xA, 0x4F, 0x37, 0x00, 0x08 } },	/* 350   500      3.1   */
	{ .icl = { 0xC, 0x71, 0x2F, 0x00, 0x10 } },	/* 350   700      6.0   */
	{ .icl = { 0x6, 0x7D, 0x2B, 0x00, 0x14 } },	/* 350   900      8.2   */
	{ .icl = { 0xA, 0x4C, 0x3F, 0x00, 0x00 } },	/* 500   500      0.0   */
	{ .icl = { 0xC, 0x73, 0x34, 0x00, 0x0B } },	/* 500   700      2.9   */
	{ .icl = { 0x6, 0x7F, 0x2F, 0x00, 0x10 } },	/* 500   900      5.1   */
	{ .icl = { 0xC, 0x6C, 0x3C, 0x00, 0x03 } },	/* 650   700      0.6   */
	{ .icl = { 0x6, 0x7F, 0x35, 0x00, 0x0A } },	/* 600   900      3.5   */
	{ .icl = { 0x6, 0x7F, 0x3F, 0x00, 0x00 } },	/* 900   900      0.0   */
};
static const union intel_ddi_buf_trans_entry _tgl_uy_combo_phy_trans_dp_hbr2[] = {
							/* NT mV Trans mV db    */
	{ .icl = { 0xA, 0x35, 0x3F, 0x00, 0x00 } },	/* 350   350      0.0   */
	{ .icl = { 0xA, 0x4F, 0x36, 0x00, 0x09 } },	/* 350   500      3.1   */
	{ .icl = { 0xC, 0x60, 0x32, 0x00, 0x0D } },	/* 350   700      6.0   */
	{ .icl = { 0xC, 0x7F, 0x2D, 0x00, 0x12 } },	/* 350   900      8.2   */
	{ .icl = { 0xC, 0x47, 0x3F, 0x00, 0x00 } },	/* 500   500      0.0   */
	{ .icl = { 0xC, 0x6F, 0x36, 0x00, 0x09 } },	/* 500   700      2.9   */
	{ .icl = { 0x6, 0x7D, 0x32, 0x00, 0x0D } },	/* 500   900      5.1   */
	{ .icl = { 0x6, 0x60, 0x3C, 0x00, 0x03 } },	/* 650   700      0.6   */
	{ .icl = { 0x6, 0x7F, 0x34, 0x00, 0x0B } },	/* 600   900      3.5   */
	{ .icl = { 0x6, 0x7F, 0x3F, 0x00, 0x00 } },	/* 900   900      0.0   */
};

static const union intel_ddi_buf_trans_entry _tgl_combo_phy_trans_dp_hbr2[] = {
							/* NT mV Trans mV db    */
	{ .icl = { 0xA, 0x35, 0x3F, 0x00, 0x00 } },	/* 350   350      0.0   */
	{ .icl = { 0xA, 0x4F, 0x37, 0x00, 0x08 } },	/* 350   500      3.1   */
	{ .icl = { 0xC, 0x63, 0x2F, 0x00, 0x10 } },	/* 350   700      6.0   */
	{ .icl = { 0x6, 0x7F, 0x2B, 0x00, 0x14 } },	/* 350   900      8.2   */
	{ .icl = { 0xA, 0x47, 0x3F, 0x00, 0x00 } },	/* 500   500      0.0   */
	{ .icl = { 0xC, 0x63, 0x34, 0x00, 0x0B } },	/* 500   700      2.9   */
	{ .icl = { 0x6, 0x7F, 0x2F, 0x00, 0x10 } },	/* 500   900      5.1   */
	{ .icl = { 0xC, 0x61, 0x3C, 0x00, 0x03 } },	/* 650   700      0.6   */
	{ .icl = { 0x6, 0x7B, 0x35, 0x00, 0x0A } },	/* 600   900      3.5   */
	{ .icl = { 0x6, 0x7F, 0x3F, 0x00, 0x00 } },	/* 900   900      0.0   */
};

static const union intel_ddi_buf_trans_entry _icl_combo_phy_trans_hdmi[] = {
							/* NT mV Trans mV db    */
	{ .icl = { 0xA, 0x60, 0x3F, 0x00, 0x00 } },	/* 450   450      0.0   */
	{ .icl = { 0xB, 0x73, 0x36, 0x00, 0x09 } },	/* 450   650      3.2   */
	{ .icl = { 0x6, 0x7F, 0x31, 0x00, 0x0E } },	/* 450   850      5.5   */
	{ .icl = { 0xB, 0x73, 0x3F, 0x00, 0x00 } },	/* 650   650      0.0   ALS */
	{ .icl = { 0x6, 0x7F, 0x37, 0x00, 0x08 } },	/* 650   850      2.3   */
	{ .icl = { 0x6, 0x7F, 0x3F, 0x00, 0x00 } },	/* 850   850      0.0   */
	{ .icl = { 0x6, 0x7F, 0x35, 0x00, 0x0A } },	/* 600   850      3.0   */
};

static const union intel_ddi_buf_trans_entry _tgl_combo_phy_trans_edp_hbr2_hobl[] = {
							/* VS	pre-emp	*/
	{ .icl = { 0x6, 0x7F, 0x3F, 0x00, 0x00 } },	/* 0	0	*/
	{ .icl = { 0x6, 0x7F, 0x3F, 0x00, 0x00 } },	/* 0	1	*/
	{ .icl = { 0x6, 0x7F, 0x3F, 0x00, 0x00 } },	/* 0	2	*/
	{ .icl = { 0x6, 0x7F, 0x3F, 0x00, 0x00 } },	/* 0	3	*/
	{ .icl = { 0x6, 0x7F, 0x3F, 0x00, 0x00 } },	/* 1	0	*/
	{ .icl = { 0x6, 0x7F, 0x3F, 0x00, 0x00 } },	/* 1	1	*/
	{ .icl = { 0x6, 0x7F, 0x3F, 0x00, 0x00 } },	/* 1	2	*/
	{ .icl = { 0x6, 0x7F, 0x3F, 0x00, 0x00 } },	/* 2	0	*/
	{ .icl = { 0x6, 0x7F, 0x3F, 0x00, 0x00 } },	/* 2	1	*/
};

static const union intel_ddi_buf_trans_entry _icl_combo_phy_trans_dp_hbr2_edp_hbr3[] = {
							/* NT mV Trans mV db    */
	{ .icl = { 0xA, 0x35, 0x3F, 0x00, 0x00 } },	/* 350   350      0.0   */
	{ .icl = { 0xA, 0x4F, 0x37, 0x00, 0x08 } },	/* 350   500      3.1   */
	{ .icl = { 0xC, 0x71, 0x2F, 0x00, 0x10 } },	/* 350   700      6.0   */
	{ .icl = { 0x6, 0x7F, 0x2B, 0x00, 0x14 } },	/* 350   900      8.2   */
	{ .icl = { 0xA, 0x4C, 0x3F, 0x00, 0x00 } },	/* 500   500      0.0   */
	{ .icl = { 0xC, 0x73, 0x34, 0x00, 0x0B } },	/* 500   700      2.9   */
	{ .icl = { 0x6, 0x7F, 0x2F, 0x00, 0x10 } },	/* 500   900      5.1   */
	{ .icl = { 0xC, 0x6C, 0x3C, 0x00, 0x03 } },	/* 650   700      0.6   */
	{ .icl = { 0x6, 0x7F, 0x35, 0x00, 0x0A } },	/* 600   900      3.5   */
	{ .icl = { 0x6, 0x7F, 0x3F, 0x00, 0x00 } },	/* 900   900      0.0   */
};

static const union intel_ddi_buf_trans_entry _icl_combo_phy_trans_edp_hbr2[] = {
							/* NT mV Trans mV db    */
	{ .icl = { 0x0, 0x7F, 0x3F, 0x00, 0x00 } },	/* 200   200      0.0   */
	{ .icl = { 0x8, 0x7F, 0x38, 0x00, 0x07 } },	/* 200   250      1.9   */
	{ .icl = { 0x1, 0x7F, 0x33, 0x00, 0x0C } },	/* 200   300      3.5   */
	{ .icl = { 0x9, 0x7F, 0x31, 0x00, 0x0E } },	/* 200   350      4.9   */
	{ .icl = { 0x8, 0x7F, 0x3F, 0x00, 0x00 } },	/* 250   250      0.0   */
	{ .icl = { 0x1, 0x7F, 0x38, 0x00, 0x07 } },	/* 250   300      1.6   */
	{ .icl = { 0x9, 0x7F, 0x35, 0x00, 0x0A } },	/* 250   350      2.9   */
	{ .icl = { 0x1, 0x7F, 0x3F, 0x00, 0x00 } },	/* 300   300      0.0   */
	{ .icl = { 0x9, 0x7F, 0x38, 0x00, 0x07 } },	/* 300   350      1.3   */
	{ .icl = { 0x9, 0x7F, 0x3F, 0x00, 0x00 } },	/* 350   350      0.0   */
};

static const struct intel_ddi_buf_trans icl_combo_phy_trans_edp_hbr2 = {
	.entries = _icl_combo_phy_trans_edp_hbr2,
	.num_entries = ARRAY_SIZE(_icl_combo_phy_trans_edp_hbr2),
};

static const struct intel_ddi_buf_trans icl_combo_phy_trans_dp_hbr2_edp_hbr3 = {
	.entries = _icl_combo_phy_trans_dp_hbr2_edp_hbr3,
	.num_entries = ARRAY_SIZE(_icl_combo_phy_trans_dp_hbr2_edp_hbr3),
};

static const struct intel_ddi_buf_trans tgl_combo_phy_trans_edp_hbr2_hobl = {
	.entries = _tgl_combo_phy_trans_edp_hbr2_hobl,
	.num_entries = ARRAY_SIZE(_tgl_combo_phy_trans_edp_hbr2_hobl),
};

static const struct intel_ddi_buf_trans icl_combo_phy_trans_hdmi = {
	.entries = _icl_combo_phy_trans_hdmi,
	.num_entries = ARRAY_SIZE(_icl_combo_phy_trans_hdmi),
	.hdmi_default_entry = ARRAY_SIZE(_icl_combo_phy_trans_hdmi) - 1,
};

static const struct intel_ddi_buf_trans tgl_uy_combo_phy_trans_dp_hbr2 = {
	.entries = _tgl_uy_combo_phy_trans_dp_hbr2,
	.num_entries = ARRAY_SIZE(_tgl_uy_combo_phy_trans_dp_hbr2),
};
static const struct intel_ddi_buf_trans tgl_combo_phy_trans_dp_hbr = {
	.entries = _tgl_combo_phy_trans_dp_hbr,
	.num_entries = ARRAY_SIZE(_tgl_combo_phy_trans_dp_hbr),
};

static const struct intel_ddi_buf_trans tgl_combo_phy_trans_dp_hbr2 = {
	.entries = _tgl_combo_phy_trans_dp_hbr2,
	.num_entries = ARRAY_SIZE(_tgl_combo_phy_trans_dp_hbr2),
};

#define   SCALING_MODE_SEL_MASK			REG_GENMASK(20, 18)
#define   RTERM_SELECT_MASK			REG_GENMASK(5, 3)
#define   COEFF_POLARITY			REG_BIT(25)
#define   CURSOR_PROGRAM			REG_BIT(26)
#define   TAP2_DISABLE				REG_BIT(30)
#define   TAP3_DISABLE				REG_BIT(29)
#define   SCALING_MODE_SEL_MASK			REG_GENMASK(20, 18)
#define   SCALING_MODE_SEL(x)			REG_FIELD_PREP(SCALING_MODE_SEL_MASK, (x))
#define   RTERM_SELECT_MASK			REG_GENMASK(5, 3)
#define   RTERM_SELECT(x)			REG_FIELD_PREP(RTERM_SELECT_MASK, (x))

#define ICL_PORT_TX_DW2_LN(ln, phy)		_MMIO(_ICL_PORT_TX_DW_LN(2, ln, phy))
#define   SWING_SEL_UPPER_MASK			REG_BIT(15)
#define   SWING_SEL_LOWER_MASK			REG_GENMASK(13, 11)
#define   RCOMP_SCALAR_MASK			REG_GENMASK(7, 0)
#define   RCOMP_SCALAR(x)			REG_FIELD_PREP(RCOMP_SCALAR_MASK, (x))
#define   SWING_SEL_UPPER(x)			REG_FIELD_PREP(SWING_SEL_UPPER_MASK, (x) >> 3)
#define   SWING_SEL_LOWER(x)			REG_FIELD_PREP(SWING_SEL_LOWER_MASK, (x) & 0x7)
#define   POST_CURSOR_1_MASK			REG_GENMASK(17, 12)
#define   POST_CURSOR_1(x)			REG_FIELD_PREP(POST_CURSOR_1_MASK, (x))
#define   POST_CURSOR_2_MASK			REG_GENMASK(11, 6)
#define   POST_CURSOR_2(x)			REG_FIELD_PREP(POST_CURSOR_2_MASK, (x))
#define   CURSOR_COEFF_MASK			REG_GENMASK(5, 0)
#define   CURSOR_COEFF(x)			REG_FIELD_PREP(CURSOR_COEFF_MASK, (x))
#define ICL_PORT_TX_DW7_AUX(phy)		_MMIO(_ICL_PORT_TX_DW_AUX(7, phy))
#define ICL_PORT_TX_DW7_GRP(phy)		_MMIO(_ICL_PORT_TX_DW_GRP(7, phy))
#define ICL_PORT_TX_DW7_LN(ln, phy)		_MMIO(_ICL_PORT_TX_DW_LN(7, ln, phy))
#define   N_SCALAR_MASK				REG_GENMASK(30, 24)
#define   N_SCALAR(x)				REG_FIELD_PREP(N_SCALAR_MASK, (x))

static const u8 index_to_dp_signal_levels[] = {
	[0] = DP_TRAIN_VOLTAGE_SWING_LEVEL_0 | DP_TRAIN_PRE_EMPH_LEVEL_0,
	[1] = DP_TRAIN_VOLTAGE_SWING_LEVEL_0 | DP_TRAIN_PRE_EMPH_LEVEL_1,
	[2] = DP_TRAIN_VOLTAGE_SWING_LEVEL_0 | DP_TRAIN_PRE_EMPH_LEVEL_2,
	[3] = DP_TRAIN_VOLTAGE_SWING_LEVEL_0 | DP_TRAIN_PRE_EMPH_LEVEL_3,
	[4] = DP_TRAIN_VOLTAGE_SWING_LEVEL_1 | DP_TRAIN_PRE_EMPH_LEVEL_0,
	[5] = DP_TRAIN_VOLTAGE_SWING_LEVEL_1 | DP_TRAIN_PRE_EMPH_LEVEL_1,
	[6] = DP_TRAIN_VOLTAGE_SWING_LEVEL_1 | DP_TRAIN_PRE_EMPH_LEVEL_2,
	[7] = DP_TRAIN_VOLTAGE_SWING_LEVEL_2 | DP_TRAIN_PRE_EMPH_LEVEL_0,
	[8] = DP_TRAIN_VOLTAGE_SWING_LEVEL_2 | DP_TRAIN_PRE_EMPH_LEVEL_1,
	[9] = DP_TRAIN_VOLTAGE_SWING_LEVEL_3 | DP_TRAIN_PRE_EMPH_LEVEL_0,
};

enum drm_dp_phy {
	DP_PHY_DPRX,

	DP_PHY_LTTPR1,
	DP_PHY_LTTPR2,
	DP_PHY_LTTPR3,
	DP_PHY_LTTPR4,
	DP_PHY_LTTPR5,
	DP_PHY_LTTPR6,
	DP_PHY_LTTPR7,
	DP_PHY_LTTPR8,

	DP_MAX_LTTPR_COUNT = DP_PHY_LTTPR8,
};

#define DP_LINK_STATUS_SIZE	   6
#define DP_TRAINING_PATTERN_SET	            0x102
# define DP_TRAINING_PATTERN_DISABLE	    0
# define DP_TRAINING_PATTERN_1		    1
# define DP_TRAINING_PATTERN_2		    2
# define DP_TRAINING_PATTERN_2_CDS	    3	    /* 2.0 E11 */
# define DP_TRAINING_PATTERN_3		    3	    /* 1.2 */
# define DP_TRAINING_PATTERN_4              7       /* 1.4 */
# define DP_TRAINING_PATTERN_MASK	    0x3
# define DP_TRAINING_PATTERN_MASK_1_4	    0xf
# define DP_LINK_SCRAMBLING_DISABLE	    (1 << 5)
#define DP_PHY_LTTPR(i)					    (DP_PHY_LTTPR1 + (i))

#define __DP_LTTPR1_BASE				    0xf0010 /* 1.3 */
#define __DP_LTTPR2_BASE				    0xf0060 /* 1.3 */
#define DP_LTTPR_BASE(dp_phy) \
	(__DP_LTTPR1_BASE + (__DP_LTTPR2_BASE - __DP_LTTPR1_BASE) * \
		((dp_phy) - DP_PHY_LTTPR1))

#define DP_LTTPR_REG(dp_phy, lttpr1_reg) \
	(DP_LTTPR_BASE(dp_phy) - DP_LTTPR_BASE(DP_PHY_LTTPR1) + (lttpr1_reg))
#define DP_TRAINING_PATTERN_SET_PHY_REPEATER1		    0xf0010 /* 1.3 */
#define DP_TRAINING_PATTERN_SET_PHY_REPEATER(dp_phy) \
	DP_LTTPR_REG(dp_phy, DP_TRAINING_PATTERN_SET_PHY_REPEATER1)
#define _DP_TP_CTL_A			0x64040
#define _DP_TP_CTL_B			0x64140
#define _TGL_DP_TP_CTL_A		0x60540
#define DP_TP_CTL(port) _MMIO_PORT(port, _DP_TP_CTL_A, _DP_TP_CTL_B)
#define TGL_DP_TP_CTL(dev_priv, tran) _MMIO_TRANS2(dev_priv, (tran), _TGL_DP_TP_CTL_A)
#define   DP_TP_CTL_LINK_TRAIN_MASK		REG_GENMASK(10, 8)
#define   DP_TP_CTL_LINK_TRAIN_MASK		REG_GENMASK(10, 8)
#define   DP_TP_CTL_LINK_TRAIN_PAT1		REG_FIELD_PREP(DP_TP_CTL_LINK_TRAIN_MASK, 0)
#define   DP_TP_CTL_LINK_TRAIN_PAT2		REG_FIELD_PREP(DP_TP_CTL_LINK_TRAIN_MASK, 1)
#define   DP_TP_CTL_LINK_TRAIN_PAT3		REG_FIELD_PREP(DP_TP_CTL_LINK_TRAIN_MASK, 4)
#define   DP_TP_CTL_LINK_TRAIN_PAT4		REG_FIELD_PREP(DP_TP_CTL_LINK_TRAIN_MASK, 5)
#define   DP_TP_CTL_LINK_TRAIN_IDLE		REG_FIELD_PREP(DP_TP_CTL_LINK_TRAIN_MASK, 2)
#define   DP_TP_CTL_LINK_TRAIN_NORMAL		REG_FIELD_PREP(DP_TP_CTL_LINK_TRAIN_MASK, 3)
#define   DP_TP_CTL_SCRAMBLE_DISABLE		REG_BIT(7)
#define DP_LANE0_1_STATUS		    0x202
#define DP_LANE2_3_STATUS		    0x203
#define DP_LANE_CR_DONE		    (1 << 0)
#define DP_SINK_STATUS			    0x205
# define DP_RECEIVE_PORT_0_STATUS	    (1 << 0)
# define DP_RECEIVE_PORT_1_STATUS	    (1 << 1)
# define DP_STREAM_REGENERATION_STATUS      (1 << 2) /* 2.0 */
# define DP_INTRA_HOP_AUX_REPLY_INDICATION	(1 << 3) /* 2.0 */

#define DP_ADJUST_REQUEST_LANE0_1	    0x206
#define DP_ADJUST_REQUEST_LANE2_3	    0x207
# define DP_ADJUST_VOLTAGE_SWING_LANE0_MASK  0x03
# define DP_ADJUST_VOLTAGE_SWING_LANE0_SHIFT 0
# define DP_ADJUST_PRE_EMPHASIS_LANE0_MASK   0x0c
# define DP_ADJUST_PRE_EMPHASIS_LANE0_SHIFT  2
# define DP_ADJUST_VOLTAGE_SWING_LANE1_MASK  0x30
# define DP_ADJUST_VOLTAGE_SWING_LANE1_SHIFT 4
# define DP_ADJUST_PRE_EMPHASIS_LANE1_MASK   0xc0
# define DP_ADJUST_PRE_EMPHASIS_LANE1_SHIFT  6
# define DP_TRAIN_PRE_EMPHASIS_SHIFT	    3
#define DP_TRAINING_LANE0_SET		    0x103
#define DP_TRAINING_LANE0_SET_PHY_REPEATER1		    0xf0011 /* 1.3 */
#define DP_TRAINING_LANE0_SET_PHY_REPEATER(dp_phy) \
	DP_LTTPR_REG(dp_phy, DP_TRAINING_LANE0_SET_PHY_REPEATER1)
# define DP_TRAIN_MAX_PRE_EMPHASIS_REACHED  (1 << 5)

#define DP_SET_ANSI_8B10B		    (1 << 0)
#define DP_DOWNSPREAD_CTRL		    0x107
#define	DP_LINK_BW_SET		            0x100

enum hpd_pin {
	HPD_NONE = 0,
	HPD_TV = HPD_NONE,     /* TV is known to be unreliable */
	HPD_CRT,
	HPD_SDVO_B,
	HPD_SDVO_C,
	HPD_PORT_A,
	HPD_PORT_B,
	HPD_PORT_C,
	HPD_PORT_D,
	HPD_PORT_E,
	HPD_PORT_TC1,
	HPD_PORT_TC2,
	HPD_PORT_TC3,
	HPD_PORT_TC4,
	HPD_PORT_TC5,
	HPD_PORT_TC6,

	HPD_NUM_PINS
};



struct intel_psr {
	/* Mutex for PSR state of the transcoder */
	//struct mutex lock;

#define I915_PSR_DEBUG_MODE_MASK		0x0f
#define I915_PSR_DEBUG_DEFAULT			0x00
#define I915_PSR_DEBUG_DISABLE			0x01
#define I915_PSR_DEBUG_ENABLE			0x02
#define I915_PSR_DEBUG_FORCE_PSR1		0x03
#define I915_PSR_DEBUG_ENABLE_SEL_FETCH		0x4
#define I915_PSR_DEBUG_IRQ			0x10
#define I915_PSR_DEBUG_SU_REGION_ET_DISABLE	0x20
#define I915_PSR_DEBUG_PANEL_REPLAY_DISABLE	0x40

	u32 debug;
	bool sink_support;
	bool source_support;
	bool enabled;
	int pause_counter;
	enum pipe pipe;
	enum transcoder transcoder;
	bool active;
	//struct work_struct work;
	unsigned int busy_frontbuffer_bits;
	bool link_standby;
	bool sel_update_enabled;
	bool psr2_sel_fetch_enabled;
	bool psr2_sel_fetch_cff_enabled;
	bool su_region_et_enabled;
	bool req_psr2_sdp_prior_scanline;
	//ktime_t last_entry_attempt;
	//ktime_t last_exit;
	bool sink_not_reliable;
	bool irq_aux_error;
	u16 su_w_granularity;
	u16 su_y_granularity;
	bool source_panel_replay_support;
	bool sink_panel_replay_support;
	bool panel_replay_enabled;
	u32 dc3co_exitline;
	u32 dc3co_exit_delay;
	//struct delayed_work dc3co_work;
	u8 entry_setup_frames;

	u8 io_wake_lines;
	u8 fast_wake_lines;

	bool link_ok;
	bool pkg_c_latency_used;

	u8 active_non_psr_pipes;

	const char *no_psr_reason;
};

struct drm_rect {
	int x1, y1, x2, y2;
};
struct dpll {
	/* given values */
	int n;
	int m1, m2;
	int p1, p2;
	/* derived values */
	int	dot;
	int	vco;
	int	m;
	int	p;
};
struct intel_link_m_n {
	u32 tu;
	u32 data_m;
	u32 data_n;
	u32 link_m;
	u32 link_n;
};
enum intel_panel_replay_dsc_support {
	INTEL_DP_PANEL_REPLAY_DSC_NOT_SUPPORTED,
	INTEL_DP_PANEL_REPLAY_DSC_FULL_FRAME_ONLY,
	INTEL_DP_PANEL_REPLAY_DSC_SELECTIVE_UPDATE,
};
struct intel_scaler {
	u32 mode;
	bool in_use;
	int hscale;
	int vscale;
};

struct intel_crtc_scaler_state {
#define SKL_NUM_SCALERS 2
	struct intel_scaler scalers[SKL_NUM_SCALERS];

#define SKL_CRTC_INDEX 31
	unsigned scaler_users;

	/* scaler used by crtc for panel fitting purpose */
	int scaler_id;
};

enum intel_output_format {
	INTEL_OUTPUT_FORMAT_RGB,
	INTEL_OUTPUT_FORMAT_YCBCR420,
	INTEL_OUTPUT_FORMAT_YCBCR444,
};

enum drm_mode_status {
	MODE_OK = 0,
	MODE_HSYNC,
	MODE_VSYNC,
	MODE_H_ILLEGAL,
	MODE_V_ILLEGAL,
	MODE_BAD_WIDTH,
	MODE_NOMODE,
	MODE_NO_INTERLACE,
	MODE_NO_DBLESCAN,
	MODE_NO_VSCAN,
	MODE_MEM,
	MODE_VIRTUAL_X,
	MODE_VIRTUAL_Y,
	MODE_MEM_VIRT,
	MODE_NOCLOCK,
	MODE_CLOCK_HIGH,
	MODE_CLOCK_LOW,
	MODE_CLOCK_RANGE,
	MODE_BAD_HVALUE,
	MODE_BAD_VVALUE,
	MODE_BAD_VSCAN,
	MODE_HSYNC_NARROW,
	MODE_HSYNC_WIDE,
	MODE_HBLANK_NARROW,
	MODE_HBLANK_WIDE,
	MODE_VSYNC_NARROW,
	MODE_VSYNC_WIDE,
	MODE_VBLANK_NARROW,
	MODE_VBLANK_WIDE,
	MODE_PANEL,
	MODE_INTERLACE_WIDTH,
	MODE_ONE_WIDTH,
	MODE_ONE_HEIGHT,
	MODE_ONE_SIZE,
	MODE_NO_REDUCED,
	MODE_NO_STEREO,
	MODE_NO_420,
	MODE_STALE = -3,
	MODE_BAD = -2,
	MODE_ERROR = -1
};

struct drm_display_mode {

	int clock;		/* in kHz */
	u16 hdisplay;
	u16 hsync_start;
	u16 hsync_end;
	u16 htotal;
	u16 hskew;
	u16 vdisplay;
	u16 vsync_start;
	u16 vsync_end;
	u16 vtotal;
	u16 vscan;


	u32 flags;


	int crtc_clock;
	u16 crtc_hdisplay;
	u16 crtc_hblank_start;
	u16 crtc_hblank_end;
	u16 crtc_hsync_start;
	u16 crtc_hsync_end;
	u16 crtc_htotal;
	u16 crtc_hskew;
	u16 crtc_vdisplay;
	u16 crtc_vblank_start;
	u16 crtc_vblank_end;
	u16 crtc_vsync_start;
	u16 crtc_vsync_end;
	u16 crtc_vtotal;


	u16 width_mm;


	u16 height_mm;


	u8 type;

	bool expose_to_userspace;

	/**
	 * @head:
	 *
	 * struct list_head for mode lists.
	 */
	struct list_head head;

	char name[32];

	/**
	 * @status:
	 *
	 * Status of the mode, used to filter out modes not supported by the
	 * hardware. See enum &drm_mode_status.
	 */
	enum drm_mode_status status;

	/**
	 * @picture_aspect_ratio:
	 *
	 * Field for setting the HDMI picture aspect ratio of a mode.
	 */
	//enum hdmi_picture_aspect picture_aspect_ratio;

};
struct intel_crtc_state {

	struct {
		bool active, enable;
		/* logical state of LUTs */
		//struct drm_property_blob *degamma_lut, *gamma_lut, *ctm;
		struct drm_display_mode mode, pipe_mode, adjusted_mode;
		//enum drm_scaling_filter scaling_filter;
		//struct intel_casf casf_params;
	} hw;
	
#define PIPE_CONFIG_QUIRK_MODE_SYNC_FLAGS	(1<<0) /* unreliable sync mode.flags */
	unsigned long quirks;

	unsigned fb_bits; /* framebuffers to flip */
	bool update_pipe; /* can a fast modeset be performed? */
	bool update_m_n; /* update M/N seamlessly during fastset? */
	bool update_lrr; /* update TRANS_VTOTAL/etc. during fastset? */
	bool disable_cxsr;
	bool update_wm_pre, update_wm_post; /* watermarks are updated */
	bool fifo_changed; /* FIFO split is changed */
	bool preload_luts;
	bool inherited; /* state inherited from BIOS? */

	/* Ask the hardware to actually async flip? */
	bool do_async_flip;

	/* Pipe source size (ie. panel fitter input size)
	 * All planes will be positioned inside this space,
	 * and get clipped at the edges. */
	struct drm_rect pipe_src;

	/*
	 * Pipe pixel rate, adjusted for
	 * panel fitter/pipe scaler downscaling.
	 */
	unsigned int pixel_rate;

	/* Whether to set up the PCH/FDI. Note that we never allow sharing
	 * between pch encoders and cpu encoders. */
	bool has_pch_encoder;

	/* Are we sending infoframes on the attached port */
	bool has_infoframe;

	/* CPU Transcoder for the pipe. Currently this can only differ from the
	 * pipe on Haswell and later (where we have a special eDP transcoder)
	 * and Broxton (where we have special DSI transcoders). */
	enum transcoder cpu_transcoder;

	/*
	 * Use reduced/limited/broadcast rbg range, compressing from the full
	 * range fed into the crtcs.
	 */
	bool limited_color_range;

	/* Bitmask of encoder types (enum intel_output_type)
	 * driven by the pipe.
	 */
	unsigned int output_types;

	/* Whether we should send NULL infoframes. Required for audio. */
	bool has_hdmi_sink;

	/* Audio enabled on this pipe. Only valid if either has_hdmi_sink or
	 * has_dp_encoder is set. */
	bool has_audio;

	/*
	 * Enable dithering, used when the selected pipe bpp doesn't match the
	 * plane bpp.
	 */
	bool dither;

	/*
	 * Dither gets enabled for 18bpp which causes CRC mismatch errors for
	 * compliance video pattern tests.
	 * Disable dither only if it is a compliance test request for
	 * 18bpp.
	 */
	bool dither_force_disable;

	/* Controls for the clock computation, to override various stages. */
	bool clock_set;

	/* SDVO TV has a bunch of special case. To make multifunction encoders
	 * work correctly, we need to track this at runtime.*/
	bool sdvo_tv_clock;

	/*
	 * crtc bandwidth limit, don't increase pipe bpp or clock if not really
	 * required. This is set in the 2nd loop of calling encoder's
	 * ->compute_config if the first pick doesn't work out.
	 */
	bool bw_constrained;

	/* Settings for the intel dpll used on pretty much everything but
	 * haswell. */
	struct dpll dpll;


	/* DSI PLL registers */
	struct {
		u32 ctrl, div;
	} dsi_pll;

	int max_link_bpp_x16;	/* in 1/16 bpp units */
	int max_pipe_bpp;	/* in 1 bpp units */
	int pipe_bpp;		/* in 1 bpp units */
	int min_hblank;
	struct intel_link_m_n dp_m_n;

	/* m2_n2 for eDP downclock */
	struct intel_link_m_n dp_m2_n2;
	bool has_drrs;

	/* PSR is supported but might not be enabled due the lack of enabled planes */
	bool has_psr;
	bool has_sel_update;
	bool enable_psr2_sel_fetch;
	bool enable_psr2_su_region_et;
	bool req_psr2_sdp_prior_scanline;
	bool has_panel_replay;
	bool link_off_after_as_sdp_when_pr_active;
	bool disable_as_sdp_when_pr_active;
	bool wm_level_disabled;
	bool pkg_c_latency_used;
	/* Only used for state verification. */
	enum intel_panel_replay_dsc_support panel_replay_dsc_support;
	u32 dc3co_exitline;
	u16 su_y_granularity;
	u8 active_non_psr_pipes;
	u8 entry_setup_frames;
	const char *no_psr_reason;

	/*
	 * Frequency the dpll for the port should run at. Differs from the
	 * adjusted dotclock e.g. for DP or 10/12bpc hdmi mode. This is also
	 * already multiplied by pixel_multiplier.
	 */
	int port_clock;

	/* Used by SDVO (and if we ever fix it, HDMI). */
	unsigned pixel_multiplier;

	/* I915_MODE_FLAG_* */
	u8 mode_flags;

	u8 lane_count;

	/*
	 * Used by platforms having DP/HDMI PHY with programmable lane
	 * latency optimization.
	 */
	u8 lane_lat_optim_mask;

	/* minimum acceptable voltage level */
	u8 min_voltage_level;

	/* Panel fitter controls for gen2-gen4 + VLV */
	struct {
		u32 control;
		u32 pgm_ratios;
		u32 lvds_border_bits;
	} gmch_pfit;

	/* Panel fitter placement and size for Ironlake+ */
	struct {
		struct drm_rect dst;
		bool enabled;
		bool force_thru;
	} pch_pfit;

	/* FDI configuration, only valid if has_pch_encoder is set. */
	int fdi_lanes;
	struct intel_link_m_n fdi_m_n;

	bool ips_enabled;

	bool crc_enabled;

	bool double_wide;

	struct intel_crtc_scaler_state scaler_state;

	/* w/a for waiting 2 vblanks during crtc enable */
	enum pipe hsw_workaround_pipe;


	int min_cdclk;

	int plane_min_cdclk[3];

	/* for packed/planar CbCr */
	u32 data_rate[3];
	/* for planar Y */
	u32 data_rate_y[3];

	/* FIXME unify with data_rate[]? */
	u64 rel_data_rate[3];
	u64 rel_data_rate_y[3];

	/* Gamma mode programmed on the pipe */
	u32 gamma_mode;

	union {
		/* CSC mode programmed on the pipe */
		u32 csc_mode;

		/* CHV CGM mode */
		u32 cgm_mode;
	};

	/* bitmask of logically enabled planes (enum plane_id) */
	u8 enabled_planes;

	/* bitmask of actually visible planes (enum plane_id) */
	u8 active_planes;
	u8 scaled_planes;
	u8 nv12_planes;
	u8 c8_planes;

	/* bitmask of planes that will be updated during the commit */
	u8 update_planes;

	/* bitmask of planes with async flip active */
	u8 async_flip_planes;

	u8 framestart_delay; /* 1-4 */
	u8 msa_timing_delay; /* 0-3 */

	struct {
		u32 enable;
		u32 gcp;

	} infoframes;

	u8 eld[128];

	/* HDMI scrambling status */
	bool hdmi_scrambling;

	/* HDMI High TMDS char rate ratio */
	bool hdmi_high_tmds_clock_ratio;

	/*
	 * Output format RGB/YCBCR etc., that is coming out
	 * at the end of the pipe.
	 */
	enum intel_output_format output_format;

	/*
	 * Sink output format RGB/YCBCR etc., that is going
	 * into the sink.
	 */
	enum intel_output_format sink_format;

	/* enable pipe gamma? */
	bool gamma_enable;

	/* enable pipe csc? */
	bool csc_enable;

	/* enable vlv/chv wgc csc? */
	bool wgc_enable;

	/* joiner pipe bitmask */
	u8 joiner_pipes;

	/* Display Stream compression state */
	struct {
		/* Only used for state computation, not read out from the HW. */
		bool compression_enabled_on_link;
		bool compression_enable;
		struct intel_dsc_slice_config {
			int pipes_per_line;
			int streams_per_pipe;
			int slices_per_stream;
		} slice_config;
		/* Compressed Bpp in U6.4 format (first 4 bits for fractional part) */
		u16 compressed_bpp_x16;
		//struct drm_dsc_config config;
	} dsc;

	/* DP tunnel used for BW allocation. */

	/* HSW+ linetime watermarks */
	u16 linetime;
	u16 ips_linetime;

	bool enhanced_framing;

	/*
	 * Forward Error Correction.
	 *
	 * Note: This will be false for 128b/132b, which will always have FEC
	 * enabled automatically.
	 */
	bool fec_enable;

	bool sdp_split_enable;

	/* Pointer to master transcoder in case of tiled displays */
	enum transcoder master_transcoder;

	/* Bitmask to indicate slaves attached */
	u8 sync_mode_slaves_mask;

	/* Only valid on TGL+ */
	enum transcoder mst_master_transcoder;

	/* For DSB based pipe updates */
	bool use_dsb;
	bool use_flipq;

	u32 psr2_man_track_ctl;

	u32 pipe_srcsz_early_tpt;

	struct drm_rect psr2_su_area;

	/* Variable Refresh Rate state */
	struct {
		bool enable, in_range;
		u8 pipeline_full;
		u16 flipline, vmin, vmax, guardband;
		u32 vsync_end, vsync_start;
		struct {
			bool enable;
			u16 vmin, vmax;
			u16 guardband, slope;
			u16 max_increase, max_decrease;
			u16 vblank_target;
		} dc_balance;
	} vrr;

	/* Content Match Refresh Rate state */
	struct {
		bool enable;
		u64 cmrr_n, cmrr_m;
	} cmrr;

	/* Stream Splitter for eDP MSO */
	struct {
		bool enable;
		u8 link_count;
		u8 pixel_overlap;
	} splitter;

	/* for loading single buffered registers during vblank */

	/* LOBF flag */
	bool has_lobf;

	/* W2 window or 'set context latency' lines */
	u16 set_context_latency;

	struct {
		u8 io_wake_lines;
		u8 fast_wake_lines;

		/* LNL and beyond */
		u8 check_entry_lines;
		u8 aux_less_wake_lines;
		u8 silence_period_sym_clocks;
		u8 lfps_half_cycle_num_of_syms;
	} alpm_state;

	/* to track changes in plane color blocks */
	bool plane_color_changed;
};

enum drm_dp_mst_mode {
	/**
	 * @DRM_DP_SST: The sink does not support MST nor single stream sideband
	 * messaging.
	 */
	DRM_DP_SST,
	/**
	 * @DRM_DP_MST: Sink supports MST, more than one stream and single
	 * stream sideband messaging.
	 */
	DRM_DP_MST,
	/**
	 * @DRM_DP_SST_SIDEBAND_MSG: Sink supports only one stream and single
	 * stream sideband messaging.
	 */
	DRM_DP_SST_SIDEBAND_MSG,
};

struct intel_dp {
	u32 output_reg;
	u32 DP;
	int link_rate;
	u8 lane_count;
	u8 sink_count;
	bool downstream_port_changed;
	bool needs_modeset_retry;
	bool use_max_params;
	
	struct {
		u8 dpcd[2];

		bool support;
		bool su_support;

		u16 su_w_granularity;
		u16 su_y_granularity;

		u8 sync_latency;
	} psr_caps;
	
	struct AGDCDPPortConfig_t *para;
	struct intel_psr psr;
	IOFBDPLinkConfig para0;
	
	u8 dpcd[0xf];
	u8 edp_dpcd[5];
	u8 lttpr_common_caps[8];
	
	u8 downstream_ports[0x10];
	/*u8 edp_dpcd[EDP_DISPLAY_CTL_CAP_SIZE];
	u8 lttpr_common_caps[DP_LTTPR_COMMON_CAP_SIZE];
	u8 lttpr_phy_caps[DP_MAX_LTTPR_COUNT][DP_LTTPR_PHY_CAP_SIZE];
	u8 pcon_dsc_dpcd[DP_PCON_DSC_ENCODER_CAP_SIZE];
*/
	int num_source_rates;
	const int *source_rates;
	/* sink rates as reported by DP_MAX_LINK_RATE/DP_SUPPORTED_LINK_RATES */
	int num_sink_rates;
	int sink_rates[8];
	bool use_rate_select;
	/* Max sink lane count as reported by DP_MAX_LANE_COUNT */
	int max_sink_lane_count;
	/* intersection of source and sink rates */
	int num_common_rates;
	//int common_rates[DP_MAX_SUPPORTED_RATES];
	struct {
		/* TODO: move the rest of link specific fields to here */
		bool active;
		/* common rate,lane_count configs in bw order */
		int num_configs;

		/* Max lane count for the current link */
		int max_lane_count;
		/* Max rate for the current link */
		int max_rate;
		/*
		 * Link parameters for which the MST topology was probed.
		 * Tracking these ensures that the MST path resources are
		 * re-enumerated whenever the link is retrained with new link
		 * parameters, as required by the DP standard.
		 */
		int mst_probed_lane_count;
		int mst_probed_rate;
		int force_lane_count;
		int force_rate;
		bool retrain_disabled;
		/* Sequential link training failures after a passing LT */
		int seq_train_failures;
		int force_train_failure;
		bool force_retrain;
	} link;
	bool reset_link_params;
	int mso_link_count;
	int mso_pixel_overlap;
	/* sink or branch descriptor */
	//struct drm_dp_desc desc;
	//struct drm_dp_aux aux;
	u32 aux_busy_last_status;
	u8 train_set[4];

	struct intel_pps pps;

	bool is_mst;
	enum drm_dp_mst_mode mst_detect;

	/* connector directly attached - won't be use for modeset in mst world */
	//struct intel_connector *attached_connector;
	bool as_sdp_supported;

	//struct drm_dp_tunnel *tunnel;
	bool tunnel_suspended:1;

	struct {
		//struct intel_dp_mst_encoder *stream_encoders[I915_MAX_PIPES];
		//struct drm_dp_mst_topology_mgr mgr;
		int active_streams;
	} mst;

	/* Downstream facing port caps */
	struct {
		int min_tmds_clock, max_tmds_clock;
		int max_dotclock;
		int pcon_max_frl_bw;
		u8 max_bpc;
		bool ycbcr_444_to_420;
		bool ycbcr420_passthrough;
		bool rgb_to_ycbcr;
	} dfp;

	/* Display stream compression testing */
	bool force_dsc_en;
	int force_dsc_output_format;
	bool force_dsc_fractional_bpp_en;
	int force_dsc_bpc;

	bool hobl_failed;
	bool hobl_active;


	/* When we last wrote the OUI for eDP */
	unsigned long last_oui_write;
	bool oui_valid;

	bool colorimetry_support;

	struct {
		enum transcoder transcoder;
		//struct mutex lock;

		bool lobf_disable_debug;
		bool sink_alpm_error;
	} alpm;

	u8 alpm_dpcd;

	struct {
		unsigned long mask;
	} quirks;
};


struct intel_hotplug {
	//struct delayed_work hotplug_work;
	enum hpd_pin hpd_pin;
	const u32 *hpd, *pch_hpd;

	struct {
		unsigned long last_jiffies;
		int count;
		int blocked_count;
		enum {
			HPD_ENABLED = 0,
			HPD_DISABLED = 1,
			HPD_MARK_DISABLED = 2
		} state;
	} stats[HPD_NUM_PINS];
	u32 event_bits;
	u32 retry_bits;
	//struct delayed_work reenable_work;

	u32 long_hpd_pin_mask;
	u32 short_hpd_pin_mask;
	//struct work_struct dig_port_work;

	//struct work_struct poll_init_work;
	bool poll_enabled;

	/*
	 * Queuing of hotplug_work, reenable_work and poll_init_work is
	 * enabled. Protected by intel_display::irq::lock.
	 */
	bool detection_work_enabled;

	unsigned int hpd_storm_threshold;
	/* Whether or not to count short HPD IRQs in HPD storms */
	u8 hpd_short_storm_enabled;

	/* Last state reported by oob_hotplug_event for each encoder */
	unsigned long oob_hotplug_last_state;

	/*
	 * if we get a HPD irq from DP and a HPD irq from non-DP
	 * the non-DP HPD could block the workqueue on a mode config
	 * mutex getting, that userspace may have taken. However
	 * userspace is waiting on the DP workqueue to run which is
	 * blocked behind the non-DP one.
	 */
	//struct workqueue_struct *dp_wq;

	/*
	 * Flag to track if long HPDs need not to be processed
	 *
	 * Some panels generate long HPDs while keep connected to the port.
	 * This can cause issues with CI tests results. In CI systems we
	 * don't expect to disconnect the panels and could ignore the long
	 * HPDs generated from the faulty panels. This flag can be used as
	 * cue to ignore the long HPDs and can be set / unset using debugfs.
	 */
	bool ignore_long_hpd;
};




struct intel_display {
	
	struct intel_display_platforms platform;
	struct intel_crtc_state crtc_state0;
	struct intel_dp intel_dp0;
	
	enum intel_pch pch_type;
	bool initok;

	
	struct intel_hotplug hotplug;
	
	int port_clock;
	
	struct {
		const struct intel_display_device_info *__device_info;

		struct intel_display_runtime_info __runtime_info;
	} info;
	
	struct {
		u8 enabled_slices;

		//struct intel_global_obj obj;
	} dbuf;

	struct {
		u32 mmio_base;
		IOSimpleLock *mutex;
	} pps;
	
	struct {
		struct i915_power_domains domains;

		u32 chv_phy_control;

		bool chv_phy_assert[2];
	} power;
	
	struct {
		struct intel_dmc *dmc;
		//struct ref_tracker *wakeref;
	} dmc;
	
	struct {
		IOSimpleLock *lock;
		bool vlv_display_irqs_enabled;
		u8 vblank_enabled;
		int vblank_enable_count;
		u32 vlv_imr_mask;
		u32 ilk_de_imr_mask;
		u32 de_pipe_imr_mask[I915_MAX_PIPES];
		u32 pipestat_irq_mask[I915_MAX_PIPES];
	} irq;
	
	struct intel_panel panel;
	ConnectorInfo bconnectors[6];
	const struct bdb_header *bdb;
	struct intel_vbt_data vbt;
	struct intel_opregion opregion;
	
	
	const struct child_device_config *child0;
	enum port port0;
	enum phy phy0;
	enum pipe pipe0;
	
};

#define   TGL_PCODE_TCCOLD			0x26
#define     TGL_PCODE_EXIT_TCCOLD_DATA_L_EXIT_FAILED	REG_BIT(0)
#define     TGL_PCODE_EXIT_TCCOLD_DATA_L_BLOCK_REQ	0
#define     TGL_PCODE_EXIT_TCCOLD_DATA_L_UNBLOCK_REQ	REG_BIT(0)

#define GEN6_PCODE_MAILBOX			_MMIO(0x138124)
#define   GEN6_PCODE_READY			(1 << 31)
#define   GEN6_PCODE_MB_PARAM2			REG_GENMASK(23, 16)
#define   GEN6_PCODE_MB_PARAM1			REG_GENMASK(15, 8)
#define   GEN6_PCODE_MB_COMMAND			REG_GENMASK(7, 0)
#define   GEN6_PCODE_ERROR_MASK			0xFF
#define     GEN6_PCODE_SUCCESS			0x0
#define     GEN6_PCODE_ILLEGAL_CMD		0x1
#define     GEN6_PCODE_MIN_FREQ_TABLE_GT_RATIO_OUT_OF_RANGE 0x2
#define     GEN6_PCODE_TIMEOUT			0x3
#define     GEN6_PCODE_UNIMPLEMENTED_CMD	0xFF
#define     GEN7_PCODE_TIMEOUT			0x2
#define     GEN7_PCODE_ILLEGAL_DATA		0x3
#define     GEN11_PCODE_ILLEGAL_SUBCOMMAND	0x4
#define     GEN11_PCODE_LOCKED			0x6
#define     GEN11_PCODE_REJECTED		0x11
#define     GEN7_PCODE_MIN_FREQ_TABLE_GT_RATIO_OUT_OF_RANGE 0x10
#define GEN6_PCODE_DATA				_MMIO(0x138128)
#define   GEN6_PCODE_FREQ_IA_RATIO_SHIFT	8
#define   GEN6_PCODE_FREQ_RING_RATIO_SHIFT	16
#define GEN6_PCODE_DATA1			_MMIO(0x13812C)

#define _PICK(__index, ...) (((const u32 []){ __VA_ARGS__ })[__index])

#define   PIPE_MISC_BPC_MASK			REG_GENMASK(7, 5)
#define   PIPE_MISC_BPC_8			REG_FIELD_PREP(PIPE_MISC_BPC_MASK, 0)
#define   PIPE_MISC_BPC_10			REG_FIELD_PREP(PIPE_MISC_BPC_MASK, 1)
#define   PIPE_MISC_BPC_6			REG_FIELD_PREP(PIPE_MISC_BPC_MASK, 2)
#define   PIPE_MISC_BPC_12_ADLP			REG_FIELD_PREP(PIPE_MISC_BPC_MASK, 4)
#define   PIPE_MISC_DITHER_ENABLE		REG_BIT(4)
#define   PIPE_MISC_DITHER_TYPE_MASK		REG_GENMASK(3, 2)
#define   PIPE_MISC_DITHER_TYPE_SP		REG_FIELD_PREP(PIPE_MISC_DITHER_TYPE_MASK, 0)
#define   PIPE_MISC_DITHER_TYPE_ST1		REG_FIELD_PREP(PIPE_MISC_DITHER_TYPE_MASK, 1)
#define   PIPE_MISC_DITHER_TYPE_ST2		REG_FIELD_PREP(PIPE_MISC_DITHER_TYPE_MASK, 2)
#define   PIPE_MISC_DITHER_TYPE_TEMP		REG_FIELD_PREP(PIPE_MISC_DITHER_TYPE_MASK, 3)
#define _PIPE_MISC_A			0x70030
#define _PIPE_MISC_B			0x71030
#define PIPE_MISC(pipe)			_MMIO_PIPE(pipe, _PIPE_MISC_A, _PIPE_MISC_B)
#define   PIPE_MISC_YUV420_ENABLE		REG_BIT(27)
#define   PIPE_MISC_YUV420_MODE_FULL_BLEND	REG_BIT(26)
#define   PIPE_MISC_HDR_MODE_PRECISION		REG_BIT(23)
#define   PIPE_MISC_PSR_MASK_PRIMARY_FLIP	REG_BIT(23)
#define   PIPE_MISC_PSR_MASK_SPRITE_ENABLE	REG_BIT(22)
#define   PIPE_MISC_PSR_MASK_PIPE_REG_WRITE	REG_BIT(21)
#define   PIPE_MISC_PSR_MASK_CURSOR_MOVE	REG_BIT(21)
#define   PIPE_MISC_PSR_MASK_VBLANK_VSYNC_INT	REG_BIT(20)
#define   PIPE_MISC_OUTPUT_COLORSPACE_YUV	REG_BIT(11)
#define   PIPE_MISC_PIXEL_ROUNDING_TRUNC	REG_BIT(8)
	
#define  TRANS_DDI_FUNC_ENABLE		(1 << 31)

#define  TRANS_DDI_PORT_SHIFT		28
#define  TGL_TRANS_DDI_PORT_SHIFT	27
#define  TRANS_DDI_PORT_MASK		(7 << TRANS_DDI_PORT_SHIFT)
#define  TGL_TRANS_DDI_PORT_MASK	(0xf << TGL_TRANS_DDI_PORT_SHIFT)
#define  TRANS_DDI_SELECT_PORT(x)	((x) << TRANS_DDI_PORT_SHIFT)
#define  TGL_TRANS_DDI_SELECT_PORT(x)	(((x) + 1) << TGL_TRANS_DDI_PORT_SHIFT)
#define  TRANS_DDI_MODE_SELECT_MASK	(7 << 24)
#define  TRANS_DDI_MODE_SELECT_HDMI	(0 << 24)
#define  TRANS_DDI_MODE_SELECT_DVI	(1 << 24)
#define  TRANS_DDI_MODE_SELECT_DP_SST	(2 << 24)
#define  TRANS_DDI_MODE_SELECT_DP_MST	(3 << 24)
#define  TRANS_DDI_MODE_SELECT_FDI_OR_128B132B	(4 << 24)
#define  TRANS_DDI_BPC_MASK		(7 << 20)
#define  TRANS_DDI_BPC_8		(0 << 20)
#define  TRANS_DDI_BPC_10		(1 << 20)
#define  TRANS_DDI_BPC_6		(2 << 20)
#define  TRANS_DDI_BPC_12		(3 << 20)
#define  TRANS_DDI_PORT_SYNC_MASTER_SELECT_MASK	REG_GENMASK(19, 18)
#define  TRANS_DDI_PORT_SYNC_MASTER_SELECT(x)	REG_FIELD_PREP(TRANS_DDI_PORT_SYNC_MASTER_SELECT_MASK, (x))
#define  TRANS_DDI_PVSYNC		(1 << 17)
#define  TRANS_DDI_PHSYNC		(1 << 16)
#define  TRANS_DDI_PORT_SYNC_ENABLE	REG_BIT(15)
#define  XE3_TRANS_DDI_HDCP_LINE_REKEY_DISABLE	REG_BIT(15)
#define  TRANS_DDI_EDP_INPUT_MASK	(7 << 12)
#define  TRANS_DDI_EDP_INPUT_A_ON	(0 << 12)
#define  TRANS_DDI_EDP_INPUT_A_ONOFF	(4 << 12)
#define  TRANS_DDI_EDP_INPUT_B_ONOFF	(5 << 12)
#define  TRANS_DDI_EDP_INPUT_C_ONOFF	(6 << 12)
#define  TRANS_DDI_EDP_INPUT_D_ONOFF	(7 << 12)
#define  TRANS_DDI_HDCP_LINE_REKEY_DISABLE	REG_BIT(12)
#define  TRANS_DDI_MST_TRANSPORT_SELECT_MASK	REG_GENMASK(11, 10)
#define  TRANS_DDI_MST_TRANSPORT_SELECT(trans)	\
	REG_FIELD_PREP(TRANS_DDI_MST_TRANSPORT_SELECT_MASK, trans)
#define  DDI_PORT_WIDTH_MASK			REG_GENMASK(3, 1)
#define  DDI_PORT_WIDTH_ENCODE(width)		((width) == 3 ? 4 : (width) - 1)
#define  DDI_PORT_WIDTH_DECODE(regval)		((regval) == 4 ? 3 : (regval) + 1)
#define  DDI_PORT_WIDTH(width)			REG_FIELD_PREP(DDI_PORT_WIDTH_MASK, \
								   DDI_PORT_WIDTH_ENCODE(width))
#define _TRANS_CLK_SEL_A		0x46140
#define _TRANS_CLK_SEL_B		0x46144
#define TRANS_CLK_SEL(tran) _MMIO_TRANS(tran, _TRANS_CLK_SEL_A, _TRANS_CLK_SEL_B)

#define  TRANS_CLK_SEL_DISABLED		(0x0 << 29)
#define  TRANS_CLK_SEL_PORT(x)		(((x) + 1) << 29)
#define  TGL_TRANS_CLK_SEL_DISABLED	(0x0 << 28)
#define  TGL_TRANS_CLK_SEL_PORT(x)	(((x) + 1) << 28)


#define DP_LANE_ALIGN_STATUS_UPDATED                    0x204
#define  DP_INTERLANE_ALIGN_DONE                        (1 << 0)
#define DP_LANE0_1_STATUS		    0x202
#define DP_LANE2_3_STATUS		    0x203
# define DP_LANE_CR_DONE		    (1 << 0)
# define DP_LANE_CHANNEL_EQ_DONE	    (1 << 1)
# define DP_LANE_SYMBOL_LOCKED		    (1 << 2)

#define DP_CHANNEL_EQ_BITS (DP_LANE_CR_DONE |		\
				DP_LANE_CHANNEL_EQ_DONE |	\
				DP_LANE_SYMBOL_LOCKED)

#define	DP_LINK_BW_SET		            0x100
# define DP_LINK_RATE_TABLE		    0x00    /* eDP 1.4 */
# define DP_LINK_BW_1_62		    0x06
# define DP_LINK_BW_2_7			    0x0a
# define DP_LINK_BW_5_4			    0x14    /* 1.2 */
# define DP_LINK_BW_8_1			    0x1e    /* 1.4 */
# define DP_LINK_BW_10                      0x01    /* 2.0 128b/132b Link Layer */
# define DP_LINK_BW_13_5                    0x04    /* 2.0 128b/132b Link Layer */
# define DP_LINK_BW_20                      0x02
# define DP_LANE_COUNT_ENHANCED_FRAME_EN    (1 << 7)

#define _PORT(port, a, b)		_PICK_EVEN(port, a, b)
#define _MMIO_PORT(port, a, b)		_MMIO(_PORT(port, a, b))


#define _DDI_BUF_CTL_A				0x64000
#define _DDI_BUF_CTL_B				0x64100
/* Known as DDI_CTL_DE in MTL+ */
#define DDI_BUF_CTL(port) _MMIO_PORT(port, _DDI_BUF_CTL_A, _DDI_BUF_CTL_B)
#define  DDI_BUF_CTL_ENABLE			REG_BIT(31)
#define  XE2LPD_DDI_BUF_D2D_LINK_ENABLE		REG_BIT(29)
#define  XE2LPD_DDI_BUF_D2D_LINK_STATE		REG_BIT(28)
#define  DDI_BUF_EMP_MASK			REG_GENMASK(27, 24)
#define  DDI_BUF_PORT_DATA_MASK			REG_GENMASK(19, 18)
#define  DDI_BUF_TRANS_SELECT(n)		REG_FIELD_PREP(DDI_BUF_EMP_MASK, (n))
#define  DDI_BUF_PORT_DATA_MASK			REG_GENMASK(19, 18)
#define  DDI_BUF_PORT_DATA_10BIT		REG_FIELD_PREP(DDI_BUF_PORT_DATA_MASK, 0)
#define  DDI_BUF_PORT_DATA_20BIT		REG_FIELD_PREP(DDI_BUF_PORT_DATA_MASK, 1)
#define  DDI_BUF_PORT_DATA_40BIT		REG_FIELD_PREP(DDI_BUF_PORT_DATA_MASK, 2)


#define   DP_TP_CTL_ENABLE			REG_BIT(31)
#define _DP_TP_CTL_A			0x64040
#define _DP_TP_CTL_B			0x64140
#define _TGL_DP_TP_CTL_A		0x60540
#define DP_TP_CTL(port) _MMIO_PORT(port, _DP_TP_CTL_A, _DP_TP_CTL_B)
#define TGL_DP_TP_CTL(dev_priv, tran) _MMIO_TRANS2(dev_priv, (tran), _TGL_DP_TP_CTL_A)
#define   DP_TP_CTL_ENABLE			REG_BIT(31)
#define   DP_TP_CTL_FEC_ENABLE			REG_BIT(30)
#define   DP_TP_CTL_MODE_MASK			REG_BIT(27)
#define   DP_TP_CTL_MODE_SST			REG_FIELD_PREP(DP_TP_CTL_MODE_MASK, 0)
#define   DP_TP_CTL_ENHANCED_FRAME_ENABLE	REG_BIT(18)
#define   DP_TP_CTL_MODE_MST			REG_FIELD_PREP(DP_TP_CTL_MODE_MASK, 1)

enum intel_dpll_id {
	/**
	 * @DPLL_ID_PRIVATE: non-shared dpll in use
	 */
	DPLL_ID_PRIVATE = -1,

	/**
	 * @DPLL_ID_PCH_PLL_A: DPLL A in ILK, SNB and IVB
	 */
	DPLL_ID_PCH_PLL_A = 0,
	/**
	 * @DPLL_ID_PCH_PLL_B: DPLL B in ILK, SNB and IVB
	 */
	DPLL_ID_PCH_PLL_B = 1,


	/**
	 * @DPLL_ID_WRPLL1: HSW and BDW WRPLL1
	 */
	DPLL_ID_WRPLL1 = 0,
	/**
	 * @DPLL_ID_WRPLL2: HSW and BDW WRPLL2
	 */
	DPLL_ID_WRPLL2 = 1,
	/**
	 * @DPLL_ID_SPLL: HSW and BDW SPLL
	 */
	DPLL_ID_SPLL = 2,
	/**
	 * @DPLL_ID_LCPLL_810: HSW and BDW 0.81 GHz LCPLL
	 */
	DPLL_ID_LCPLL_810 = 3,
	/**
	 * @DPLL_ID_LCPLL_1350: HSW and BDW 1.35 GHz LCPLL
	 */
	DPLL_ID_LCPLL_1350 = 4,
	/**
	 * @DPLL_ID_LCPLL_2700: HSW and BDW 2.7 GHz LCPLL
	 */
	DPLL_ID_LCPLL_2700 = 5,


	/**
	 * @DPLL_ID_SKL_DPLL0: SKL and later DPLL0
	 */
	DPLL_ID_SKL_DPLL0 = 0,
	/**
	 * @DPLL_ID_SKL_DPLL1: SKL and later DPLL1
	 */
	DPLL_ID_SKL_DPLL1 = 1,
	/**
	 * @DPLL_ID_SKL_DPLL2: SKL and later DPLL2
	 */
	DPLL_ID_SKL_DPLL2 = 2,
	/**
	 * @DPLL_ID_SKL_DPLL3: SKL and later DPLL3
	 */
	DPLL_ID_SKL_DPLL3 = 3,


	/**
	 * @DPLL_ID_ICL_DPLL0: ICL/TGL combo PHY DPLL0
	 */
	DPLL_ID_ICL_DPLL0 = 0,
	/**
	 * @DPLL_ID_ICL_DPLL1: ICL/TGL combo PHY DPLL1
	 */
	DPLL_ID_ICL_DPLL1 = 1,
	/**
	 * @DPLL_ID_EHL_DPLL4: EHL combo PHY DPLL4
	 */
	DPLL_ID_EHL_DPLL4 = 2,
	/**
	 * @DPLL_ID_ICL_TBTPLL: ICL/TGL TBT PLL
	 */
	DPLL_ID_ICL_TBTPLL = 2,
	/**
	 * @DPLL_ID_ICL_MGPLL1: ICL MG PLL 1 port 1 (C),
	 *                      TGL TC PLL 1 port 1 (TC1)
	 */
	DPLL_ID_ICL_MGPLL1 = 3,
	/**
	 * @DPLL_ID_ICL_MGPLL2: ICL MG PLL 1 port 2 (D)
	 *                      TGL TC PLL 1 port 2 (TC2)
	 */
	DPLL_ID_ICL_MGPLL2 = 4,
	/**
	 * @DPLL_ID_ICL_MGPLL3: ICL MG PLL 1 port 3 (E)
	 *                      TGL TC PLL 1 port 3 (TC3)
	 */
	DPLL_ID_ICL_MGPLL3 = 5,
	/**
	 * @DPLL_ID_ICL_MGPLL4: ICL MG PLL 1 port 4 (F)
	 *                      TGL TC PLL 1 port 4 (TC4)
	 */
	DPLL_ID_ICL_MGPLL4 = 6,
	/**
	 * @DPLL_ID_TGL_MGPLL5: TGL TC PLL port 5 (TC5)
	 */
	DPLL_ID_TGL_MGPLL5 = 7,
	/**
	 * @DPLL_ID_TGL_MGPLL6: TGL TC PLL port 6 (TC6)
	 */
	DPLL_ID_TGL_MGPLL6 = 8,

	/**
	 * @DPLL_ID_DG1_DPLL0: DG1 combo PHY DPLL0
	 */
	DPLL_ID_DG1_DPLL0 = 0,
	/**
	 * @DPLL_ID_DG1_DPLL1: DG1 combo PHY DPLL1
	 */
	DPLL_ID_DG1_DPLL1 = 1,
	/**
	 * @DPLL_ID_DG1_DPLL2: DG1 combo PHY DPLL2
	 */
	DPLL_ID_DG1_DPLL2 = 2,
	/**
	 * @DPLL_ID_DG1_DPLL3: DG1 combo PHY DPLL3
	 */
	DPLL_ID_DG1_DPLL3 = 3,
};

enum tc_port {
	TC_PORT_NONE = -1,

	TC_PORT_1 = 0,
	TC_PORT_2,
	TC_PORT_3,
	TC_PORT_4,
	TC_PORT_5,
	TC_PORT_6,

	I915_MAX_TC_PORTS
};

#define ICL_DPCLKA_CFGCR0			_MMIO(0x164280)
#define  ICL_DPCLKA_CFGCR0_DDI_CLK_OFF(phy)	(1 << _PICK(phy, 10, 11, 24, 4, 5))
#define  RKL_DPCLKA_CFGCR0_DDI_CLK_OFF(phy)	REG_BIT((phy) + 10)
#define  ICL_DPCLKA_CFGCR0_TC_CLK_OFF(tc_port)	(1 << ((tc_port) < TC_PORT_4 ? \
						   (tc_port) + 12 : \
						   (tc_port) - TC_PORT_4 + 21))
#define  ICL_DPCLKA_CFGCR0_DDI_CLK_SEL_SHIFT(phy)	((phy) * 2)
#define  ICL_DPCLKA_CFGCR0_DDI_CLK_SEL_MASK(phy)	(3 << ICL_DPCLKA_CFGCR0_DDI_CLK_SEL_SHIFT(phy))
#define  ICL_DPCLKA_CFGCR0_DDI_CLK_SEL(pll, phy)	((pll) << ICL_DPCLKA_CFGCR0_DDI_CLK_SEL_SHIFT(phy))
#define  RKL_DPCLKA_CFGCR0_DDI_CLK_SEL_SHIFT(phy)	_PICK(phy, 0, 2, 4, 27)
#define  RKL_DPCLKA_CFGCR0_DDI_CLK_SEL_MASK(phy) \
(3 << RKL_DPCLKA_CFGCR0_DDI_CLK_SEL_SHIFT(phy))
#define  RKL_DPCLKA_CFGCR0_DDI_CLK_SEL(pll, phy) \
((pll) << RKL_DPCLKA_CFGCR0_DDI_CLK_SEL_SHIFT(phy))

#define DP_PROTOCOL_CONVERTER_CONTROL_0		0x3050 /* DP 1.3 */
#define DP_PROTOCOL_CONVERTER_CONTROL_1		0x3051 /* DP 1.3 */
#define DP_PROTOCOL_CONVERTER_CONTROL_2		0x3052
# define DP_CONVERSION_RGB_YCBCR_MASK	       (7 << 4)
# define DP_MSA_TIMING_PAR_IGNORE_EN	    (1 << 7) /* eDP */


#define DP_PSR_ERROR_STATUS                 0x2006  /* XXX 1.2? */
#define DP_SINK_DEVICE_PR_AND_FRAME_LOCK_STATUS        0x2022  /* DP 2.1 */
#define DP_PSR_STATUS                       0x2008  /* XXX 1.2? */
#define DP_PANEL_REPLAY_ERROR_STATUS                   0x2020  /* DP 2.1*/
# define DP_PSR_SINK_STATE_MASK             0x07

#define   EDP_PSR_ENABLE			REG_BIT(31)
#define HSW_SRD_CTL				_MMIO(0x64800)
#define _SRD_CTL_A				0x60800
#define _SRD_CTL_EDP				0x6f800
#define EDP_PSR_CTL(dev_priv, tran)			_MMIO_TRANS2(dev_priv, tran, _SRD_CTL_A)
#define HSW_SRD_STATUS				_MMIO(0x64840)
#define _SRD_STATUS_A				0x60840
#define _SRD_STATUS_EDP				0x6f840
#define EDP_PSR_STATUS(dev_priv, tran)			_MMIO_TRANS2(dev_priv, tran, _SRD_STATUS_A)
#define   EDP_PSR_STATUS_STATE_MASK		REG_GENMASK(31, 29)
#define GEN8_CHICKEN_DCPR_1			_MMIO(0x46430)
#define   _LATENCY_REPORTING_REMOVED_PIPE_D	REG_BIT(31)
#define   SKL_SELECT_ALTERNATE_DC_EXIT		REG_BIT(30)
#define   _LATENCY_REPORTING_REMOVED_PIPE_C	REG_BIT(25)
#define   _LATENCY_REPORTING_REMOVED_PIPE_B	REG_BIT(24)
#define   _LATENCY_REPORTING_REMOVED_PIPE_A	REG_BIT(23)
#define   LATENCY_REPORTING_REMOVED(pipe)	_PICK((pipe), \
							  _LATENCY_REPORTING_REMOVED_PIPE_A, \
							  _LATENCY_REPORTING_REMOVED_PIPE_B, \
							  _LATENCY_REPORTING_REMOVED_PIPE_C, \
							  _LATENCY_REPORTING_REMOVED_PIPE_D)
#define _PSR2_CTL_A				0x60900
#define _PSR2_CTL_EDP				0x6f900
#define EDP_PSR2_CTL(dev_priv, tran)			_MMIO_TRANS2(dev_priv, tran, _PSR2_CTL_A)
#define   EDP_PSR2_IDLE_FRAMES_MASK		REG_GENMASK(3, 0)
#define   EDP_PSR2_IDLE_FRAMES(x)		REG_FIELD_PREP(EDP_PSR2_IDLE_FRAMES_MASK, (x))
#define DP_PSR_EN_CFG				0x170   /* XXX 1.2? */
#define DP_RECEIVER_ALPM_CONFIG		    0x116   /* eDP 1.4 */
#define DP_PSR_ESI                          0x2007  /* XXX 1.2? */
# define DP_PSR_CAPS_CHANGE                 (1 << 0)
# define DP_PSR_SINK_INTERNAL_ERROR         7
#define DP_PSR_ERROR_STATUS                 0x2006  /* XXX 1.2? */
# define DP_PSR_LINK_CRC_ERROR              (1 << 0)
# define DP_PSR_RFB_STORAGE_ERROR           (1 << 1)
# define DP_PSR_VSC_SDP_UNCORRECTABLE_ERROR (1 << 2) /* eDP 1.4 */
#define   DP_PORT_EN			REG_BIT(31)
#define   PP_ON				REG_BIT(31)
#define  EDP_FORCE_VDD			REG_BIT(3)


enum intel_display_wa {
	INTEL_DISPLAY_WA_1409120013,
	INTEL_DISPLAY_WA_1409767108,
	INTEL_DISPLAY_WA_13012396614,
	INTEL_DISPLAY_WA_14010477008,
	INTEL_DISPLAY_WA_14010480278,
	INTEL_DISPLAY_WA_14010547955,
	INTEL_DISPLAY_WA_14010685332,
	INTEL_DISPLAY_WA_14011294188,
	INTEL_DISPLAY_WA_14011503030,
	INTEL_DISPLAY_WA_14011503117,
	INTEL_DISPLAY_WA_14011508470,
	INTEL_DISPLAY_WA_14011765242,
	INTEL_DISPLAY_WA_14014143976,
	INTEL_DISPLAY_WA_14016740474,
	INTEL_DISPLAY_WA_14020863754,
	INTEL_DISPLAY_WA_14025769978,
	INTEL_DISPLAY_WA_15013987218,
	INTEL_DISPLAY_WA_15018326506,
	INTEL_DISPLAY_WA_16011181250,
	INTEL_DISPLAY_WA_16011303918,
	INTEL_DISPLAY_WA_16011342517,
	INTEL_DISPLAY_WA_16011863758,
	INTEL_DISPLAY_WA_16023588340,
	INTEL_DISPLAY_WA_16025573575,
	INTEL_DISPLAY_WA_16025596647,
	INTEL_DISPLAY_WA_18034343758,
	INTEL_DISPLAY_WA_22010178259,
	INTEL_DISPLAY_WA_22010947358,
	INTEL_DISPLAY_WA_22011320316,
	INTEL_DISPLAY_WA_22012278275,
	INTEL_DISPLAY_WA_22012358565,
	INTEL_DISPLAY_WA_22014263786,
	INTEL_DISPLAY_WA_22021048059,
};

#define IS_DISPLAY_VERx100(__display, from, until) ( \
	(DISPLAY_VERx100(__display) >= (from) && \
	 DISPLAY_VERx100(__display) <= (until)))
#define IS_DISPLAY_VERx100_STEP(__display, ipver, from, until) \
	(IS_DISPLAY_VERx100((__display), (ipver), (ipver)) && \
	 IS_DISPLAY_STEP((__display), (from), (until)))
#define INTEL_DISPLAY_STEP(__display)	(DISPLAY_RUNTIME_INFO(__display)->step)
#define INTEL_PCH_TYPE(_display)		((_display)->pch_type)
#define IS_DISPLAY_STEP(__display, since, until) \
	 INTEL_DISPLAY_STEP(__display) >= (since) && INTEL_DISPLAY_STEP(__display) < (until)

bool __intel_display_wa(struct intel_display *display, enum intel_display_wa wa, const char *name)
{
	switch (wa) {
	case INTEL_DISPLAY_WA_1409120013:
		return IS_DISPLAY_VER(display, 11, 12);
	case INTEL_DISPLAY_WA_1409767108:
		return (display->platform.alderlake_s ||
			(display->platform.rocketlake &&
			 IS_DISPLAY_STEP(display, STEP_A0, STEP_B0)));
	case INTEL_DISPLAY_WA_13012396614:
		return DISPLAY_VERx100(display) == 3000 ||
			DISPLAY_VERx100(display) == 3500;
	case INTEL_DISPLAY_WA_14010477008:
		return display->platform.dg1 || display->platform.rocketlake ||
			(display->platform.tigerlake &&
			 IS_DISPLAY_STEP(display, STEP_A0, STEP_D0));
	case INTEL_DISPLAY_WA_14010480278:
		return (IS_DISPLAY_VER(display, 10, 12));
	case INTEL_DISPLAY_WA_14010547955:
		return display->platform.dg2;
	case INTEL_DISPLAY_WA_14010685332:
		return INTEL_PCH_TYPE(display) >= PCH_CNP &&
			INTEL_PCH_TYPE(display) < PCH_DG1;
	case INTEL_DISPLAY_WA_14011294188:
		return INTEL_PCH_TYPE(display) >= PCH_TGP &&
			INTEL_PCH_TYPE(display) < PCH_DG1;
	case INTEL_DISPLAY_WA_14011503030:
	case INTEL_DISPLAY_WA_14011503117:
	case INTEL_DISPLAY_WA_22012358565:
		return DISPLAY_VER(display) == 13;
	case INTEL_DISPLAY_WA_14011508470:
		return (IS_DISPLAY_VERx100(display, 1200, 1300));
	case INTEL_DISPLAY_WA_14011765242:
		return display->platform.alderlake_s &&
			IS_DISPLAY_STEP(display, STEP_A0, STEP_A2);
	case INTEL_DISPLAY_WA_14014143976:
		return IS_DISPLAY_STEP(display, STEP_E0, STEP_FOREVER);
	case INTEL_DISPLAY_WA_14016740474:
		return IS_DISPLAY_VERx100_STEP(display, 1400, STEP_A0, STEP_C0);
	case INTEL_DISPLAY_WA_14020863754:
		return DISPLAY_VERx100(display) == 3000 ||
			DISPLAY_VERx100(display) == 2000 ||
			DISPLAY_VERx100(display) == 1401;
	case INTEL_DISPLAY_WA_14025769978:
		return DISPLAY_VER(display) == 35;
	case INTEL_DISPLAY_WA_15013987218:
		return DISPLAY_VER(display) == 20;
	case INTEL_DISPLAY_WA_15018326506:
		return display->platform.battlemage;
	case INTEL_DISPLAY_WA_16011303918:
	case INTEL_DISPLAY_WA_22011320316:
		return display->platform.alderlake_p &&
			IS_DISPLAY_STEP(display, STEP_A0, STEP_B0);
	case INTEL_DISPLAY_WA_16011181250:
		return display->platform.rocketlake || display->platform.alderlake_s ||
			display->platform.dg2;
	case INTEL_DISPLAY_WA_16011342517:
		return display->platform.alderlake_p &&
			IS_DISPLAY_STEP(display, STEP_A0, STEP_D0);
	case INTEL_DISPLAY_WA_16011863758:
		return DISPLAY_VER(display) >= 11;
	case INTEL_DISPLAY_WA_16023588340:
		return false;
	case INTEL_DISPLAY_WA_16025573575:
		return false;
	case INTEL_DISPLAY_WA_16025596647:
		return DISPLAY_VER(display) == 20 &&
			IS_DISPLAY_VERx100_STEP(display, 3000,
						STEP_A0, STEP_B0);
	case INTEL_DISPLAY_WA_18034343758:
		return DISPLAY_VER(display) == 20 ||
			(display->platform.pantherlake &&
			 IS_DISPLAY_STEP(display, STEP_A0, STEP_B0));
	case INTEL_DISPLAY_WA_22010178259:
		return DISPLAY_VER(display) == 12;
	case INTEL_DISPLAY_WA_22010947358:
		return display->platform.alderlake_p;
	case INTEL_DISPLAY_WA_22012278275:
		return display->platform.alderlake_p &&
			IS_DISPLAY_STEP(display, STEP_A0, STEP_E0);
	case INTEL_DISPLAY_WA_22014263786:
		return IS_DISPLAY_VERx100(display, 1100, 1400);
	case INTEL_DISPLAY_WA_22021048059:
		return IS_DISPLAY_VER(display, 14, 35);
	default:
		break;
	}

	return false;
}

#define __stringify_1(x...)	#x
#define __stringify(x...)	__stringify_1(x)

#define intel_display_wa(__display, __wa) \
	__intel_display_wa((__display), __wa, __stringify(__wa))

#define _CLKGATE_DIS_PSL_EXT_A		0x4654C
#define _CLKGATE_DIS_PSL_EXT_B		0x46550
#define   PIPEDMC_GATING_DIS		REG_BIT(12)

#define CLKGATE_DIS_PSL_EXT(pipe) \
	_MMIO_PIPE(pipe, _CLKGATE_DIS_PSL_EXT_A, _CLKGATE_DIS_PSL_EXT_B)

#define _PIPEDMC_CONTROL_A		0x45250
#define _PIPEDMC_CONTROL_B		0x45254
#define PIPEDMC_CONTROL(pipe)		_MMIO_PIPE(pipe, \
						   _PIPEDMC_CONTROL_A, \
						   _PIPEDMC_CONTROL_B)
# define DP_CONVERSION_TO_YCBCR420_ENABLE	(1 << 0)
# define DP_CONVERSION_BT709_RGB_YCBCR_ENABLE  (1 << 5)
#define DP_A			_MMIO(0x64000) /* eDP */
#define DP_B			_MMIO(0x64100)
#define DP_C			_MMIO(0x64200)
#define DP_D			_MMIO(0x64300)
#define   EDP_PLL_FREQ_MASK		REG_GENMASK(17, 16)
#define   EDP_PLL_FREQ_162MHZ		REG_FIELD_PREP(EDP_PLL_FREQ_MASK, 1)
#define  DDI_BUF_PORT_REVERSAL			REG_BIT(16)
#define DP_DSC_ENABLE                       0x160   /* DP 1.4 */
# define DP_DECOMPRESSION_EN                (1 << 0)
# define DP_DSC_PASSTHROUGH_EN		    (1 << 1)
#define PANEL_REPLAY_CONFIG                             0x1b0  /* DP 2.0 */
# define DP_PANEL_REPLAY_ENABLE                         (1 << 0)
#define _ICL_PIPE_DSS_CTL2_PB			0x78204
#define _ICL_PIPE_DSS_CTL2_PC			0x78404
#define ICL_PIPE_DSS_CTL2(pipe)			_MMIO_PIPE((pipe) - PIPE_B, \
							   _ICL_PIPE_DSS_CTL2_PB, \
							   _ICL_PIPE_DSS_CTL2_PC)
#define  VDSC0_ENABLE				REG_BIT(31)
#define DP_EDP_DPCD_REV			    0x700    /* eDP 1.2 */
# define DP_EDP_11			    0x00
# define DP_EDP_12			    0x01
# define DP_EDP_13			    0x02
# define DP_EDP_14			    0x03
# define DP_EDP_14a                         0x04    /* eDP 1.4a */
# define DP_EDP_14b                         0x05    /* eDP 1.4b */
# define DP_EDP_15			    0x06    /* eDP 1.5 */

#define DP_MSA_MISC_SYNC_CLOCK			(1 << 0)
#define DP_MSA_MISC_INTERLACE_VTOTAL_EVEN	(1 << 8)
#define DP_MSA_MISC_STEREO_NO_3D		(0 << 9)
#define DP_MSA_MISC_STEREO_PROG_RIGHT_EYE	(1 << 9)
#define DP_MSA_MISC_STEREO_PROG_LEFT_EYE	(3 << 9)
/* bits per component for non-RAW */
#define DP_MSA_MISC_6_BPC			(0 << 5)
#define DP_MSA_MISC_8_BPC			(1 << 5)
#define DP_MSA_MISC_10_BPC			(2 << 5)
#define DP_MSA_MISC_12_BPC			(3 << 5)
#define DP_MSA_MISC_16_BPC			(4 << 5)
/* bits per component for RAW */
#define DP_MSA_MISC_RAW_6_BPC			(1 << 5)
#define DP_MSA_MISC_RAW_7_BPC			(2 << 5)
#define DP_MSA_MISC_RAW_8_BPC			(3 << 5)
#define DP_MSA_MISC_RAW_10_BPC			(4 << 5)
#define DP_MSA_MISC_RAW_12_BPC			(5 << 5)
#define DP_MSA_MISC_RAW_14_BPC			(6 << 5)
#define DP_MSA_MISC_RAW_16_BPC			(7 << 5)
/* pixel encoding/colorimetry format */
#define _DP_MSA_MISC_COLOR(misc1_7, misc0_21, misc0_3, misc0_4) \
	((misc1_7) << 15 | (misc0_4) << 4 | (misc0_3) << 3 | ((misc0_21) << 1))
#define DP_MSA_MISC_COLOR_RGB			_DP_MSA_MISC_COLOR(0, 0, 0, 0)
#define DP_MSA_MISC_COLOR_CEA_RGB		_DP_MSA_MISC_COLOR(0, 0, 1, 0)
#define DP_MSA_MISC_COLOR_RGB_WIDE_FIXED	_DP_MSA_MISC_COLOR(0, 3, 0, 0)
#define DP_MSA_MISC_COLOR_RGB_WIDE_FLOAT	_DP_MSA_MISC_COLOR(0, 3, 0, 1)
#define DP_MSA_MISC_COLOR_Y_ONLY		_DP_MSA_MISC_COLOR(1, 0, 0, 0)
#define DP_MSA_MISC_COLOR_RAW			_DP_MSA_MISC_COLOR(1, 1, 0, 0)
#define DP_MSA_MISC_COLOR_YCBCR_422_BT601	_DP_MSA_MISC_COLOR(0, 1, 1, 0)
#define DP_MSA_MISC_COLOR_YCBCR_422_BT709	_DP_MSA_MISC_COLOR(0, 1, 1, 1)
#define DP_MSA_MISC_COLOR_YCBCR_444_BT601	_DP_MSA_MISC_COLOR(0, 2, 1, 0)
#define DP_MSA_MISC_COLOR_YCBCR_444_BT709	_DP_MSA_MISC_COLOR(0, 2, 1, 1)
#define DP_MSA_MISC_COLOR_XVYCC_422_BT601	_DP_MSA_MISC_COLOR(0, 1, 0, 0)
#define DP_MSA_MISC_COLOR_XVYCC_422_BT709	_DP_MSA_MISC_COLOR(0, 1, 0, 1)
#define DP_MSA_MISC_COLOR_XVYCC_444_BT601	_DP_MSA_MISC_COLOR(0, 2, 0, 0)
#define DP_MSA_MISC_COLOR_XVYCC_444_BT709	_DP_MSA_MISC_COLOR(0, 2, 0, 1)
#define DP_MSA_MISC_COLOR_OPRGB			_DP_MSA_MISC_COLOR(0, 0, 1, 1)
#define DP_MSA_MISC_COLOR_DCI_P3		_DP_MSA_MISC_COLOR(0, 3, 1, 0)
#define DP_MSA_MISC_COLOR_COLOR_PROFILE		_DP_MSA_MISC_COLOR(0, 3, 1, 1)
#define DP_MSA_MISC_COLOR_VSC_SDP		(1 << 14)
#define _TRANSA_MSA_MISC		0x60410
#define _TRANSB_MSA_MISC		0x61410
#define _TRANSC_MSA_MISC		0x62410
#define _TRANS_EDP_MSA_MISC		0x6f410
#define TRANS_MSA_MISC(dev_priv, tran) _MMIO_TRANS2(dev_priv, tran, _TRANSA_MSA_MISC)


#define DMC_DEFAULT_FW_OFFSET		0xFFFFFFFF
#define PACKAGE_MAX_FW_INFO_ENTRIES	20
#define PACKAGE_V2_MAX_FW_INFO_ENTRIES	32
#define DMC_V1_MAX_MMIO_COUNT		8
#define DMC_V3_MAX_MMIO_COUNT		20
#define DMC_V1_MMIO_START_RANGE		0x80000

#define PIPE_TO_DMC_ID(pipe)		 (DMC_FW_PIPEA + ((pipe) - PIPE_A))
#define DP_EDP_CONFIGURATION_SET            0x10a   /* XXX 1.2? */
# define DP_ALTERNATE_SCRAMBLER_RESET_ENABLE (1 << 0)
# define DP_FRAMING_CHANGE_ENABLE	    (1 << 1)
# define DP_PANEL_SELF_TEST_ENABLE	    (1 << 7)
#define HSW_NDE_RSTWRN_OPT	_MMIO(0x46408)
#define  MTL_RESET_PICA_HANDSHAKE_EN	REG_BIT(6)
#define  RESET_PCH_HANDSHAKE_ENABLE	REG_BIT(4)
#define  RESET_PCH_HANDSHAKE_ENABLE	REG_BIT(4)
#define HAS_PCH_NOP(display)			(INTEL_PCH_TYPE(display) == PCH_NOP)

bool intel_phy_is_combo(struct intel_display *display, enum phy phy)
{
	if (phy == PHY_NONE)
		return false;
	else if (display->platform.alderlake_s)
		return phy <= PHY_E;
	else if (display->platform.dg1 || display->platform.rocketlake)
		return phy <= PHY_D;
	else if (display->platform.jasperlake || display->platform.elkhartlake)
		return phy <= PHY_C;
	else if (display->platform.alderlake_p || IS_DISPLAY_VER(display, 11, 12))
		return phy <= PHY_B;
	else
		return false;
}

#define for_each_if(condition) if (!(condition)) {} else

#define for_each_combo_phy(__display, __phy) \
	for ((__phy) = static_cast<decltype(__phy)>(PHY_A); \
		 static_cast<int>(__phy) < static_cast<int>(I915_MAX_PHYS); \
		 (__phy) = static_cast<decltype(__phy)>(static_cast<int>(__phy) + 1)) \
		for_each_if(intel_phy_is_combo(__display, __phy))


#define for_each_cpu_transcoder(__dev_priv, __t) \
	for ((__t) = static_cast<decltype(__t)>(0); \
		 static_cast<int>(__t) < I915_MAX_TRANSCODERS; \
		 (__t) = static_cast<decltype(__t)>(static_cast<int>(__t) + 1)) \
		for_each_if (DISPLAY_RUNTIME_INFO(__dev_priv)->cpu_transcoder_mask & BIT(__t))

#define for_each_cpu_transcoder_masked(__dev_priv, __t, __mask) \
	for_each_cpu_transcoder(__dev_priv, __t) \
		for_each_if ((__mask) & BIT(__t))


enum {
	PROCMON_0_85V_DOT_0,
	PROCMON_0_95V_DOT_0,
	PROCMON_0_95V_DOT_1,
	PROCMON_1_05V_DOT_0,
	PROCMON_1_05V_DOT_1,
};

static const struct icl_procmon {
	const char *name;
	u32 dw1, dw9, dw10;
} icl_procmon_values[] = {
	[PROCMON_0_85V_DOT_0] = {
		.name = "0.85V dot0 (low-voltage)",
		.dw1 = 0x00000000, .dw9 = 0x62AB67BB, .dw10 = 0x51914F96,
	},
	[PROCMON_0_95V_DOT_0] = {
		.name = "0.95V dot0",
		.dw1 = 0x00000000, .dw9 = 0x86E172C7, .dw10 = 0x77CA5EAB,
	},
	[PROCMON_0_95V_DOT_1] = {
		.name = "0.95V dot1",
		.dw1 = 0x00000000, .dw9 = 0x93F87FE1, .dw10 = 0x8AE871C5,
	},
	[PROCMON_1_05V_DOT_0] = {
		.name = "1.05V dot0",
		.dw1 = 0x00000000, .dw9 = 0x98FA82DD, .dw10 = 0x89E46DC1,
	},
	[PROCMON_1_05V_DOT_1] = {
		.name = "1.05V dot1",
		.dw1 = 0x00440000, .dw9 = 0x9A00AB25, .dw10 = 0x8AE38FF1,
	},
};
#define ICL_PORT_COMP_DW3(phy)			_MMIO(_ICL_PORT_COMP_DW(3, phy))
#define   PROCESS_INFO_MASK			REG_GENMASK(28, 26)
#define   PROCESS_INFO_DOT_0			REG_FIELD_PREP(PROCESS_INFO_MASK, 0)
#define   PROCESS_INFO_DOT_1			REG_FIELD_PREP(PROCESS_INFO_MASK, 1)
#define   PROCESS_INFO_DOT_4			REG_FIELD_PREP(PROCESS_INFO_MASK, 2)
#define   VOLTAGE_INFO_MASK			REG_GENMASK(25, 24)
#define   VOLTAGE_INFO_0_85V			REG_FIELD_PREP(VOLTAGE_INFO_MASK, 0)
#define   VOLTAGE_INFO_0_95V			REG_FIELD_PREP(VOLTAGE_INFO_MASK, 1)
#define   VOLTAGE_INFO_1_05V			REG_FIELD_PREP(VOLTAGE_INFO_MASK, 2)

#define _ICL_PORT_COMP				0x100
#define _ICL_PORT_COMP_DW(dw, phy)		(_ICL_COMBOPHY(phy) + \
						 _ICL_PORT_COMP + 4 * (dw))
#define ICL_PORT_COMP_DW0(phy)			_MMIO(_ICL_PORT_COMP_DW(0, phy))
#define ICL_PORT_COMP_DW8(phy)			_MMIO(_ICL_PORT_COMP_DW(8, phy))
#define   IREFGEN				REG_BIT(24)
#define   CL_POWER_DOWN_ENABLE			REG_BIT(4)
#define   DCC_MODE_SELECT_MASK			REG_GENMASK(21, 20)
#define   RUN_DCC_ONCE				REG_FIELD_PREP(DCC_MODE_SELECT_MASK, 0)
#define ICL_PORT_TX_DW8_AUX(phy)		_MMIO(_ICL_PORT_TX_DW_AUX(8, phy))
#define ICL_PORT_TX_DW8_GRP(phy)		_MMIO(_ICL_PORT_TX_DW_GRP(8, phy))
#define   COMP_INIT				REG_BIT(31)
#define ICL_PORT_TX_DW8_AUX(phy)		_MMIO(_ICL_PORT_TX_DW_AUX(8, phy))
#define ICL_PORT_TX_DW8_GRP(phy)		_MMIO(_ICL_PORT_TX_DW_GRP(8, phy))
#define ICL_PORT_TX_DW8_LN(ln, phy)		_MMIO(_ICL_PORT_TX_DW_LN(8, ln, phy))
#define   ICL_PORT_TX_DW8_ODCC_CLK_SEL		REG_BIT(31)
#define   ICL_PORT_TX_DW8_ODCC_CLK_DIV_SEL_MASK	REG_GENMASK(30, 29)
#define   ICL_PORT_TX_DW8_ODCC_CLK_DIV_SEL_DIV2	REG_FIELD_PREP(ICL_PORT_TX_DW8_ODCC_CLK_DIV_SEL_MASK, 0x1)
#define _ICL_PHY_MISC_A		0x64C00
#define _ICL_PHY_MISC_B		0x64C04
#define _DG2_PHY_MISC_TC1	0x64C14 /* TC1="PHY E" but offset as if "PHY F" */
#define ICL_PHY_MISC(port)	_MMIO_PORT(port, _ICL_PHY_MISC_A, _ICL_PHY_MISC_B)
#define  ICL_PHY_MISC_DE_IO_COMP_PWR_DOWN	(1 << 23)
#define _ICL_PORT_COMP				0x100
#define _ICL_PORT_COMP_DW(dw, phy)		(_ICL_COMBOPHY(phy) + \
						 _ICL_PORT_COMP + 4 * (dw))

#define ICL_PORT_COMP_DW0(phy)			_MMIO(_ICL_PORT_COMP_DW(0, phy))
#define   COMP_INIT				REG_BIT(31)

#define ICL_PORT_COMP_DW1(phy)			_MMIO(_ICL_PORT_COMP_DW(1, phy))
#define ICL_PORT_COMP_DW9(phy)			_MMIO(_ICL_PORT_COMP_DW(9, phy))

#define ICL_PORT_COMP_DW10(phy)			_MMIO(_ICL_PORT_COMP_DW(10, phy))

#define for_each_dbuf_slice(__dev_priv, __slice) \
	for ((__slice) = static_cast<enum dbuf_slice>(DBUF_S1); \
		 static_cast<int>(__slice) < static_cast<int>(I915_MAX_DBUF_SLICES); \
		 (__slice) = static_cast<enum dbuf_slice>(static_cast<int>(__slice) + 1)) \
		for_each_if(DISPLAY_INFO(__dev_priv)->dbuf.slice_mask & BIT(__slice))

#define for_each_dbuf_slice_in_mask(__dev_priv, __slice, __mask) \
	for_each_dbuf_slice((__dev_priv), (__slice)) \
		for_each_if((__mask) & BIT(__slice))

#define _DBUF_CTL_S0					0x45008
#define _DBUF_CTL_S1					0x44FE8
#define _DBUF_CTL_S2					0x44300
#define _DBUF_CTL_S3					0x44304
#define DBUF_CTL_S(slice)				_MMIO(_PICK(slice, \
									_DBUF_CTL_S0, \
									_DBUF_CTL_S1, \
									_DBUF_CTL_S2, \
									_DBUF_CTL_S3))

#define  DBUF_POWER_REQUEST				REG_BIT(31)
#define  DBUF_POWER_STATE				REG_BIT(30)
#define  DBUF_TRACKER_STATE_SERVICE_MASK		REG_GENMASK(23, 19)
#define  DBUF_TRACKER_STATE_SERVICE(x)			REG_FIELD_PREP(DBUF_TRACKER_STATE_SERVICE_MASK, x)
#define BIT_WORD(nr)		((nr) / BITS_PER_LONG)
static inline unsigned int find_next_bit(const unsigned long *addr,
										  unsigned int size,
										  unsigned int offset)
{
	if (offset >= size)
		return size;

	for (unsigned int i = offset; i < size; i++) {
		if (addr[BIT_WORD(i)] & (1UL << (i % BITS_PER_LONG)))
			return i;
	}
	return size;
}

#define for_each_set_bit(bit, addr, size) \
	for ((bit) = 0; \
		 (bit) = static_cast<unsigned int>(find_next_bit((addr), \
				  static_cast<unsigned int>(size), \
				  static_cast<unsigned int>(bit))), \
		 (bit) < (size); \
		 (bit)++)

#define _PICK_EVEN_2RANGES(__index, __c_index, __a, __b, __c, __d)		\
	 ((__index) < (__c_index) ? _PICK_EVEN(__index, __a, __b) :		\
				   _PICK_EVEN((__index) - (__c_index), __c, __d))


#define _MBUS_ABOX0_CTL			0x45038
#define _MBUS_ABOX1_CTL			0x45048
#define _MBUS_ABOX2_CTL			0x4504C
#define MBUS_ABOX_CTL(x)							\
	_MMIO(_PICK_EVEN_2RANGES(x, 2,						\
				 _MBUS_ABOX0_CTL, _MBUS_ABOX1_CTL,		\
				 _MBUS_ABOX2_CTL, _MBUS_ABOX2_CTL))

#define MBUS_ABOX_BW_CREDIT_MASK	(3 << 20)
#define MBUS_ABOX_BW_CREDIT(x)		((x) << 20)
#define MBUS_ABOX_B_CREDIT_MASK		(0xF << 16)
#define MBUS_ABOX_B_CREDIT(x)		((x) << 16)
#define MBUS_ABOX_BT_CREDIT_POOL2_MASK	(0x1F << 8)
#define MBUS_ABOX_BT_CREDIT_POOL2(x)	((x) << 8)
#define MBUS_ABOX_BT_CREDIT_POOL1_MASK	(0x1F << 0)
#define MBUS_ABOX_BT_CREDIT_POOL1(x)	((x) << 0)


#define _BW_BUDDY0_CTL			0x45130
#define _BW_BUDDY1_CTL			0x45140
#define BW_BUDDY_CTL(x)			_MMIO(_PICK_EVEN(x, \
							 _BW_BUDDY0_CTL, \
							 _BW_BUDDY1_CTL))
#define   BW_BUDDY_DISABLE		REG_BIT(31)
#define   BW_BUDDY_TLB_REQ_TIMER_MASK	REG_GENMASK(21, 16)
#define   BW_BUDDY_TLB_REQ_TIMER(x)	REG_FIELD_PREP(BW_BUDDY_TLB_REQ_TIMER_MASK, x)

#define _BW_BUDDY0_PAGE_MASK		0x45134
#define _BW_BUDDY1_PAGE_MASK		0x45144
#define BW_BUDDY_PAGE_MASK(x)		_MMIO(_PICK_EVEN(x, \
							 _BW_BUDDY0_PAGE_MASK, \
							 _BW_BUDDY1_PAGE_MASK))
#define   ICL_PCODE_MEM_SUBSYSYSTEM_INFO	0xd
#define     ICL_PCODE_MEM_SS_READ_GLOBAL_INFO	(0x0 << 8)

struct buddy_page_mask {
	u32 page_mask;
	u8 type;
	u8 num_channels;
};
enum intel_dram_type {
	INTEL_DRAM_UNKNOWN,
	INTEL_DRAM_DDR2,
	INTEL_DRAM_DDR3,
	INTEL_DRAM_DDR4,
	INTEL_DRAM_LPDDR3,
	INTEL_DRAM_LPDDR4,
	INTEL_DRAM_DDR5,
	INTEL_DRAM_LPDDR5,
	INTEL_DRAM_GDDR,
	INTEL_DRAM_GDDR_ECC,
	__INTEL_DRAM_TYPE_MAX,
};

struct dram_info {
	enum intel_dram_type type;
	unsigned int fsb_freq;
	unsigned int mem_freq;
	u8 num_channels;
	u8 num_qgv_points;
	u8 num_psf_gv_points;
	bool ecc_impacting_de_bw; /* Only valid from Xe3p_LPD onward. */
	bool symmetric_memory;
	bool has_16gb_dimms;
};

static const struct buddy_page_mask tgl_buddy_page_masks[] = {
	{ .num_channels = 1, .type = INTEL_DRAM_DDR4,   .page_mask = 0xF },
	{ .num_channels = 1, .type = INTEL_DRAM_DDR5,	.page_mask = 0xF },
	{ .num_channels = 2, .type = INTEL_DRAM_LPDDR4, .page_mask = 0x1C },
	{ .num_channels = 2, .type = INTEL_DRAM_LPDDR5, .page_mask = 0x1C },
	{ .num_channels = 2, .type = INTEL_DRAM_DDR4,   .page_mask = 0x1F },
	{ .num_channels = 2, .type = INTEL_DRAM_DDR5,   .page_mask = 0x1E },
	{ .num_channels = 4, .type = INTEL_DRAM_LPDDR4, .page_mask = 0x38 },
	{ .num_channels = 4, .type = INTEL_DRAM_LPDDR5, .page_mask = 0x38 },
	{}
};

static const struct buddy_page_mask wa_1409767108_buddy_page_masks[] = {
	{ .num_channels = 1, .type = INTEL_DRAM_LPDDR4, .page_mask = 0x1 },
	{ .num_channels = 1, .type = INTEL_DRAM_DDR4,   .page_mask = 0x1 },
	{ .num_channels = 1, .type = INTEL_DRAM_DDR5,   .page_mask = 0x1 },
	{ .num_channels = 1, .type = INTEL_DRAM_LPDDR5, .page_mask = 0x1 },
	{ .num_channels = 2, .type = INTEL_DRAM_LPDDR4, .page_mask = 0x3 },
	{ .num_channels = 2, .type = INTEL_DRAM_DDR4,   .page_mask = 0x3 },
	{ .num_channels = 2, .type = INTEL_DRAM_DDR5,   .page_mask = 0x3 },
	{ .num_channels = 2, .type = INTEL_DRAM_LPDDR5, .page_mask = 0x3 },
	{}
};

# define DP_SET_ANSI_128B132B               (1 << 1)
struct i915_irq_regs {
	u32 imr;
	u32 ier;
	u32 iir;
};
#define DP_SUPPORTED_LINK_RATES		    0x010
# define DP_MAX_SUPPORTED_RATES		     8
static inline constexpr i915_irq_regs make_i915_irq_regs(u32 imr, u32 ier, u32 iir)
{
	return { static_cast<u32>(imr), static_cast<u32>(ier), static_cast<u32>(iir) };
}

#define I915_IRQ_REGS(_imr, _ier, _iir) \
	make_i915_irq_regs((_imr), (_ier), (_iir))

#define GEN8_DE_PIPE_IRQ_REGS(pipe) \
	I915_IRQ_REGS(GEN8_DE_PIPE_IMR(pipe), \
			  GEN8_DE_PIPE_IER(pipe), \
			  GEN8_DE_PIPE_IIR(pipe))


#define GEN8_DE_MISC_ISR _MMIO(0x44460)
#define GEN8_DE_MISC_IMR _MMIO(0x44464)
#define GEN8_DE_MISC_IIR _MMIO(0x44468)
#define GEN8_DE_MISC_IER _MMIO(0x4446c)
#define  XELPDP_RM_TIMEOUT		REG_BIT(29)
#define  XELPDP_PMDEMAND_RSPTOUT_ERR	REG_BIT(27)
#define  GEN8_DE_MISC_GSE		REG_BIT(27)
#define  GEN8_DE_EDP_PSR		REG_BIT(19)
#define  XELPDP_PMDEMAND_RSP		REG_BIT(3)
#define  XE2LPD_DBUF_OVERLAP_DETECTED	REG_BIT(1)

#define GEN8_DE_MISC_IRQ_REGS		I915_IRQ_REGS(GEN8_DE_MISC_IMR, \
						  GEN8_DE_MISC_IER, \
						  GEN8_DE_MISC_IIR)
#define GEN11_DISPLAY_INT_CTL		_MMIO(0x44200)
#define  GEN11_DISPLAY_IRQ_ENABLE	(1 << 31)
#define  GEN11_AUDIO_CODEC_IRQ		(1 << 24)
#define  GEN11_DE_PCH_IRQ		(1 << 23)
#define  GEN11_DE_MISC_IRQ		(1 << 22)
#define  GEN11_DE_HPD_IRQ		(1 << 21)
#define  GEN11_DE_PORT_IRQ		(1 << 20)
#define  GEN11_DE_PIPE_C		(1 << 18)
#define  GEN11_DE_PIPE_B		(1 << 17)
#define  GEN11_DE_PIPE_A		(1 << 16)



#define _HPD_PIN_DDI(hpd_pin)	((hpd_pin) - HPD_PORT_A)
#define _HPD_PIN_TC(hpd_pin)	((hpd_pin) - HPD_PORT_TC1)

#define GEN11_DE_HPD_ISR		_MMIO(0x44470)
#define GEN11_DE_HPD_IMR		_MMIO(0x44474)
#define GEN11_DE_HPD_IIR		_MMIO(0x44478)
#define GEN11_DE_HPD_IER		_MMIO(0x4447c)
#define  GEN11_TC_HOTPLUG(hpd_pin)		REG_BIT(16 + _HPD_PIN_TC(hpd_pin))
#define  GEN11_DE_TC_HOTPLUG_MASK		(GEN11_TC_HOTPLUG(HPD_PORT_TC6) | \
					 GEN11_TC_HOTPLUG(HPD_PORT_TC5) | \
					 GEN11_TC_HOTPLUG(HPD_PORT_TC4) | \
					 GEN11_TC_HOTPLUG(HPD_PORT_TC3) | \
					 GEN11_TC_HOTPLUG(HPD_PORT_TC2) | \
					 GEN11_TC_HOTPLUG(HPD_PORT_TC1))
#define  GEN11_TBT_HOTPLUG(hpd_pin)		REG_BIT(_HPD_PIN_TC(hpd_pin))
#define  GEN11_DE_TBT_HOTPLUG_MASK		(GEN11_TBT_HOTPLUG(HPD_PORT_TC6) | \
					 GEN11_TBT_HOTPLUG(HPD_PORT_TC5) | \
					 GEN11_TBT_HOTPLUG(HPD_PORT_TC4) | \
					 GEN11_TBT_HOTPLUG(HPD_PORT_TC3) | \
					 GEN11_TBT_HOTPLUG(HPD_PORT_TC2) | \
					 GEN11_TBT_HOTPLUG(HPD_PORT_TC1))
#define GEN11_DE_HPD_IRQ_REGS		I915_IRQ_REGS(GEN11_DE_HPD_IMR, \
						  GEN11_DE_HPD_IER, \
						  GEN11_DE_HPD_IIR)

#define for_each_pipe(__dev_priv, __p) \
	for ((__p) = static_cast<enum pipe>(0); \
		 static_cast<int>(__p) < static_cast<int>(I915_MAX_PIPES); \
		 (__p) = static_cast<enum pipe>(static_cast<int>(__p) + 1)) \
		for_each_if(DISPLAY_RUNTIME_INFO(__dev_priv)->pipe_mask & BIT(__p))

#define _PIPEA_CHICKEN				0x70038
#define _PIPEB_CHICKEN				0x71038
#define _PIPEC_CHICKEN				0x72038
#define PIPE_CHICKEN(pipe)			_MMIO_PIPE(pipe, _PIPEA_CHICKEN,\
							   _PIPEB_CHICKEN)
#define   UNDERRUN_RECOVERY_DISABLE_ADLP	REG_BIT(30)
#define   UNDERRUN_RECOVERY_ENABLE_DG2		REG_BIT(30)
#define   PIXEL_ROUNDING_TRUNC_FB_PASSTHRU	REG_BIT(15)
#define   DG2_RENDER_CCSTAG_4_3_EN		REG_BIT(12)
#define   PER_PIXEL_ALPHA_BYPASS_EN		REG_BIT(7)

#define DPFC_RECOMP_CTL			_MMIO(0x320c)
#define ILK_DPFC_RECOMP_CTL(fbc_id)	_MMIO_PIPE((fbc_id), 0x4320c, 0x4324c)
#define   DPFC_RECOMP_STALL_EN			REG_BIT(27)
#define   DPFC_RECOMP_STALL_WM_MASK		REG_GENMASK(26, 16)
#define   DPFC_RECOMP_TIMER_COUNT_MASK		REG_GENMASK(5, 0)
#define DPFC_STATUS			_MMIO(0x3210)
#define ILK_DPFC_STATUS(fbc_id)		_MMIO_PIPE((fbc_id), 0x43210, 0x43250)
#define   DPFC_INVAL_SEG_MASK			REG_GENMASK(26, 16)
#define   DPFC_COMP_SEG_MASK			REG_GENMASK(10, 0)
#define DPFC_STATUS2			_MMIO(0x3214)
#define ILK_DPFC_STATUS2(fbc_id)	_MMIO_PIPE((fbc_id), 0x43214, 0x43254)
#define   DPFC_COMP_SEG_MASK_IVB		REG_GENMASK(11, 0)
#define DPFC_FENCE_YOFF			_MMIO(0x3218)
#define ILK_DPFC_FENCE_YOFF(fbc_id)	_MMIO_PIPE((fbc_id), 0x43218, 0x43258)
#define DPFC_CHICKEN			_MMIO(0x3224)
#define FBC_DEBUG_STATUS(fbc_id)	_MMIO_PIPE((fbc_id), 0x43220, 0x43260)
#define   FBC_UNDERRUN_DECMPR			REG_BIT(27)
#define ILK_DPFC_CHICKEN(fbc_id)	_MMIO_PIPE((fbc_id), 0x43224, 0x43264)
#define   DPFC_HT_MODIFY			REG_BIT(31) /* pre-ivb */
#define   DPFC_NUKE_ON_ANY_MODIFICATION		REG_BIT(23) /* bdw+ */
#define   DPFC_CHICKEN_COMP_DUMMY_PIXEL		REG_BIT(14) /* glk+ */
#define   DPFC_CHICKEN_FORCE_SLB_INVALIDATION	REG_BIT(13) /* icl+ */
#define   DPFC_DISABLE_DUMMY0			REG_BIT(8) /* ivb+ */
#define  DDI_BUF_IS_IDLE			REG_BIT(7)
#define   DP_TP_STATUS_IDLE_DONE		REG_BIT(25)

#define _DP_TP_STATUS_A			0x64044
#define _DP_TP_STATUS_B			0x64144
#define _TGL_DP_TP_STATUS_A		0x60544
#define DP_TP_STATUS(port) _MMIO_PORT(port, _DP_TP_STATUS_A, _DP_TP_STATUS_B)
#define TGL_DP_TP_STATUS(dev_priv, tran) _MMIO_TRANS2(dev_priv, (tran), _TGL_DP_TP_STATUS_A)
#define   DP_TP_STATUS_FEC_ENABLE_LIVE		REG_BIT(28)
#define   DP_TP_STATUS_IDLE_DONE		REG_BIT(25)
#define   DP_TP_STATUS_ACT_SENT			REG_BIT(24)
#define   DP_TP_STATUS_MODE_STATUS_MST		REG_BIT(23)
#define   DP_TP_STATUS_STREAMS_ENABLED_MASK	REG_GENMASK(18, 16) /* 17:16 on hsw but bit 18 mbz */
#define   DP_TP_STATUS_AUTOTRAIN_DONE		REG_BIT(12)
#define   DP_TP_STATUS_PAYLOAD_MAPPING_VC2_MASK REG_GENMASK(9, 8)
#define   DP_TP_STATUS_PAYLOAD_MAPPING_VC1_MASK	REG_GENMASK(5, 4)
#define   DP_TP_STATUS_PAYLOAD_MAPPING_VC0_MASK	REG_GENMASK(1, 0)

#define GEN8_DE_PIPE_ISR(pipe) _MMIO(0x44400 + (0x10 * (pipe)))
#define GEN8_DE_PIPE_IMR(pipe) _MMIO(0x44404 + (0x10 * (pipe)))
#define GEN8_DE_PIPE_IIR(pipe) _MMIO(0x44408 + (0x10 * (pipe)))
#define GEN8_DE_PIPE_IER(pipe) _MMIO(0x4440c + (0x10 * (pipe)))
#define  GEN8_PIPE_FIFO_UNDERRUN	REG_BIT(31)
#define  GEN8_PIPE_CDCLK_CRC_ERROR	REG_BIT(29)
#define  GEN8_PIPE_CDCLK_CRC_DONE	REG_BIT(28)
#define  GEN12_PIPEDMC_INTERRUPT	REG_BIT(26) /* tgl+ */
#define  GEN12_PIPEDMC_FAULT		REG_BIT(25) /* tgl-mtl */
#define  MTL_PIPEDMC_ATS_FAULT		REG_BIT(24) /* mtl */
#define  GEN12_PIPEDMC_FLIPQ_DONE	REG_BIT(24) /* tgl-adl */
#define  GEN11_PIPE_PLANE7_FAULT	REG_BIT(22) /* icl/tgl */
#define  GEN11_PIPE_PLANE6_FAULT	REG_BIT(21) /* icl/tgl */
#define  GEN11_PIPE_PLANE5_FAULT	REG_BIT(20) /* icl+ */
#define  GEN12_PIPE_VBLANK_UNMOD	REG_BIT(19) /* tgl+ */
#define  MTL_PLANE_ATS_FAULT		REG_BIT(18) /* mtl+ */
#define  GEN11_PIPE_PLANE7_FLIP_DONE	REG_BIT(18) /* icl/tgl */
#define  MTL_PIPEDMC_FLIPQ_DONE		REG_BIT(17) /* mtl */
#define  GEN11_PIPE_PLANE6_FLIP_DONE	REG_BIT(17) /* icl/tgl */
#define  GEN11_PIPE_PLANE5_FLIP_DONE	REG_BIT(16) /* icl+ */
#define  GEN12_DSB_2_INT		REG_BIT(15) /* tgl+ */
#define  GEN12_DSB_1_INT		REG_BIT(14) /* tgl+ */
#define  GEN12_DSB_0_INT		REG_BIT(13) /* tgl+ */
#define  GEN12_DSB_INT(dsb_id)		REG_BIT(13 + (dsb_id))
#define  GEN9_PIPE_CURSOR_FAULT		REG_BIT(11) /* skl+ */
#define  GEN9_PIPE_PLANE4_FAULT		REG_BIT(10) /* skl+ */
#define  GEN8_PIPE_CURSOR_FAULT		REG_BIT(10) /* bdw */
#define  GEN9_PIPE_PLANE3_FAULT		REG_BIT(9) /* skl+ */
#define  GEN8_PIPE_SPRITE_FAULT		REG_BIT(9) /* bdw */
#define  GEN9_PIPE_PLANE2_FAULT		REG_BIT(8) /* skl+ */
#define  GEN8_PIPE_PRIMARY_FAULT	REG_BIT(8) /* bdw */
#define  GEN9_PIPE_PLANE1_FAULT		REG_BIT(7) /* skl+ */
#define  GEN9_PIPE_PLANE4_FLIP_DONE	REG_BIT(6) /* skl+ */
#define  GEN9_PIPE_PLANE3_FLIP_DONE	REG_BIT(5) /* skl+ */
#define  GEN8_PIPE_SPRITE_FLIP_DONE	REG_BIT(5) /* bdw */
#define  GEN9_PIPE_PLANE2_FLIP_DONE	REG_BIT(4) /* skl+ */
#define  GEN8_PIPE_PRIMARY_FLIP_DONE	REG_BIT(4) /* bdw */
#define  GEN9_PIPE_PLANE1_FLIP_DONE	REG_BIT(3) /* skl+ */
#define  GEN9_PIPE_PLANE_FLIP_DONE(plane_id) \
	REG_BIT(((plane_id) >= PLANE_5 ? 16 - PLANE_5 : 3 - PLANE_1) + (plane_id)) /* skl+ */
#define  GEN8_PIPE_SCAN_LINE_EVENT	REG_BIT(2)
#define  GEN8_PIPE_VSYNC		REG_BIT(1)
#define  GEN8_PIPE_VBLANK		REG_BIT(0)

#define GEN8_DE_PORT_ISR _MMIO(0x44440)
#define GEN8_DE_PORT_IMR _MMIO(0x44444)
#define GEN8_DE_PORT_IIR _MMIO(0x44448)
#define GEN8_DE_PORT_IER _MMIO(0x4444c)
#define  DSI1_NON_TE			(1 << 31)
#define  DSI0_NON_TE			(1 << 30)
#define  ICL_AUX_CHANNEL_E		(1 << 29)
#define  ICL_AUX_CHANNEL_F		(1 << 28)
#define  GEN9_AUX_CHANNEL_D		(1 << 27)
#define  GEN9_AUX_CHANNEL_C		(1 << 26)
#define  GEN9_AUX_CHANNEL_B		(1 << 25)
#define  DSI1_TE			(1 << 24)
#define  DSI0_TE			(1 << 23)
#define  GEN8_DE_PORT_HOTPLUG(hpd_pin)	REG_BIT(3 + _HPD_PIN_DDI(hpd_pin))
#define  BXT_DE_PORT_HOTPLUG_MASK	(GEN8_DE_PORT_HOTPLUG(HPD_PORT_A) | \
					 GEN8_DE_PORT_HOTPLUG(HPD_PORT_B) | \
					 GEN8_DE_PORT_HOTPLUG(HPD_PORT_C))
#define  BDW_DE_PORT_HOTPLUG_MASK	GEN8_DE_PORT_HOTPLUG(HPD_PORT_A)
#define  BXT_DE_PORT_GMBUS		(1 << 1)
#define  GEN8_AUX_CHANNEL_A		(1 << 0)
#define  TGL_DE_PORT_AUX_USBC6		REG_BIT(13)
#define  XELPD_DE_PORT_AUX_DDIE		REG_BIT(13)
#define  TGL_DE_PORT_AUX_USBC5		REG_BIT(12)
#define  XELPD_DE_PORT_AUX_DDID		REG_BIT(12)
#define  TGL_DE_PORT_AUX_USBC4		REG_BIT(11)
#define  TGL_DE_PORT_AUX_USBC3		REG_BIT(10)
#define  TGL_DE_PORT_AUX_USBC2		REG_BIT(9)
#define  TGL_DE_PORT_AUX_USBC1		REG_BIT(8)
#define  TGL_DE_PORT_AUX_DDIC		REG_BIT(2)
#define  TGL_DE_PORT_AUX_DDIB		REG_BIT(1)
#define  TGL_DE_PORT_AUX_DDIA		REG_BIT(0)

#define GEN8_DE_PORT_IRQ_REGS		I915_IRQ_REGS(GEN8_DE_PORT_IMR, \
							  GEN8_DE_PORT_IER, \
							  GEN8_DE_PORT_IIR)
#define   DP_VOLTAGE_MASK		REG_GENMASK(27, 25)
#define   DP_VOLTAGE_0_4		REG_FIELD_PREP(DP_VOLTAGE_MASK, 0)
#define   DP_PRE_EMPHASIS_MASK		REG_GENMASK(24, 22)
#define   DP_PRE_EMPHASIS_0		REG_FIELD_PREP(DP_PRE_EMPHASIS_MASK, 0)
#define USLEEP_RANGE_UPPER_BOUND	20000
# define DP_DS_PORT_HPD			    (1 << 3)

#define DP_DPCD_REV                         0x000
# define DP_DPCD_REV_10                     0x10
# define DP_DPCD_REV_11                     0x11
# define DP_DPCD_REV_12                     0x12
# define DP_DPCD_REV_13                     0x13
# define DP_DPCD_REV_14                     0x14
#define DP_RECEIVER_CAP_SIZE		0xf
#define DP_DOWN_STREAM_PORT_COUNT	    0x007
# define DP_PORT_COUNT_MASK		    0x0f
#define DP_MAX_DOWNSTREAM_PORTS		    0x10
#define USEC_PER_MSEC	1000L
#define DP_MSTM_CAP			    0x021   /* 1.2 */
# define DP_MST_CAP			    (1 << 0)
#define DP_MAIN_LINK_CHANNEL_CODING         0x006
# define DP_CAP_ANSI_8B10B		    (1 << 0)
# define DP_CAP_ANSI_128B132B               (1 << 1)
# define DP_SINGLE_STREAM_SIDEBAND_MSG      (1 << 1)
#define DP_TRAINING_PATTERN_SET_PHY_REPEATER1		    0xf0010 /* 1.3 */
#define DP_TRAINING_PATTERN_SET_PHY_REPEATER(dp_phy) \
	DP_LTTPR_REG(dp_phy, DP_TRAINING_PATTERN_SET_PHY_REPEATER1)
#define DP_DOWNSTREAMPORT_PRESENT           0x005
# define DP_DWN_STRM_PORT_PRESENT           (1 << 0)

# define DP_DETAILED_CAP_INFO_AVAILABLE	    (1 << 4) /* DPI */
#define DP_DOWNSTREAM_PORT_0		    0x80
#define DP_TRAINING_LANE0_SET_PHY_REPEATER1		    0xf0011 /* 1.3 */
#define DP_TRAINING_LANE0_SET_PHY_REPEATER(dp_phy) \
	DP_LTTPR_REG(dp_phy, DP_TRAINING_LANE0_SET_PHY_REPEATER1)

#define DP_TRAINING_LANE1_SET_PHY_REPEATER1		    0xf0012 /* 1.3 */
#define DP_TRAINING_LANE2_SET_PHY_REPEATER1		    0xf0013 /* 1.3 */
#define DP_TRAINING_LANE3_SET_PHY_REPEATER1		    0xf0014 /* 1.3 */
#define DP_TRAINING_AUX_RD_INTERVAL_PHY_REPEATER1	    0xf0020 /* 1.4a */
#define DP_TRAINING_AUX_RD_INTERVAL_PHY_REPEATER(dp_phy)	\
	DP_LTTPR_REG(dp_phy, DP_TRAINING_AUX_RD_INTERVAL_PHY_REPEATER1)
#define DP_TRAINING_AUX_RD_INTERVAL             0x00e   /* XXX 1.2? */
# define DP_TRAINING_AUX_RD_MASK                0x7F    /* DP 1.3 */
# define DP_EXTENDED_RECEIVER_CAP_FIELD_PRESENT	(1 << 7) /* DP 1.3 */
#define DP_DP13_DPCD_REV                    0x2200
#define EDP_DISPLAY_CTL_CAP_SIZE	5
enum drm_colorspace {
	/* For Default case, driver will set the colorspace */
	DRM_MODE_COLORIMETRY_DEFAULT 		= 0,
	/* CEA 861 Normal Colorimetry options */
	DRM_MODE_COLORIMETRY_NO_DATA		= 0,
	DRM_MODE_COLORIMETRY_SMPTE_170M_YCC	= 1,
	DRM_MODE_COLORIMETRY_BT709_YCC		= 2,
	/* CEA 861 Extended Colorimetry Options */
	DRM_MODE_COLORIMETRY_XVYCC_601		= 3,
	DRM_MODE_COLORIMETRY_XVYCC_709		= 4,
	DRM_MODE_COLORIMETRY_SYCC_601		= 5,
	DRM_MODE_COLORIMETRY_OPYCC_601		= 6,
	DRM_MODE_COLORIMETRY_OPRGB		= 7,
	DRM_MODE_COLORIMETRY_BT2020_CYCC	= 8,
	DRM_MODE_COLORIMETRY_BT2020_RGB		= 9,
	DRM_MODE_COLORIMETRY_BT2020_YCC		= 10,
	/* Additional Colorimetry extension added as part of CTA 861.G */
	DRM_MODE_COLORIMETRY_DCI_P3_RGB_D65	= 11,
	DRM_MODE_COLORIMETRY_DCI_P3_RGB_THEATER	= 12,
	/* Additional Colorimetry Options added for DP 1.4a VSC Colorimetry Format */
	DRM_MODE_COLORIMETRY_RGB_WIDE_FIXED	= 13,
	DRM_MODE_COLORIMETRY_RGB_WIDE_FLOAT	= 14,
	DRM_MODE_COLORIMETRY_BT601_YCC		= 15,
	DRM_MODE_COLORIMETRY_COUNT
};

#define HAS_DSC(__display)		(DISPLAY_RUNTIME_INFO(__display)->has_dsc)

# define DP_ENHANCED_FRAME_CAP		    (1 << 7)
#define  SPLITTER_CONFIGURATION_MASK		REG_GENMASK(26, 25)
#define  SPLITTER_CONFIGURATION_2_SEGMENT	REG_FIELD_PREP(SPLITTER_CONFIGURATION_MASK, 0)
#define  SPLITTER_CONFIGURATION_4_SEGMENT	REG_FIELD_PREP(SPLITTER_CONFIGURATION_MASK, 1)
#define  OVERLAP_PIXELS_MASK			(0xf << 16)
#define  OVERLAP_PIXELS(pixels)			((pixels) << 16)
enum intel_output_type {
	INTEL_OUTPUT_UNUSED = 0,
	INTEL_OUTPUT_ANALOG = 1,
	INTEL_OUTPUT_DVO = 2,
	INTEL_OUTPUT_SDVO = 3,
	INTEL_OUTPUT_LVDS = 4,
	INTEL_OUTPUT_TVOUT = 5,
	INTEL_OUTPUT_HDMI = 6,
	INTEL_OUTPUT_DP = 7,
	INTEL_OUTPUT_EDP = 8,
	INTEL_OUTPUT_DSI = 9,
	INTEL_OUTPUT_DDI = 10,
	INTEL_OUTPUT_DP_MST = 11,
};
#define _TRANS_DDI_FUNC_CTL2_A		0x60404
#define _TRANS_DDI_FUNC_CTL2_B		0x61404
#define _TRANS_DDI_FUNC_CTL2_C		0x62404
#define _TRANS_DDI_FUNC_CTL2_EDP	0x6f404
#define _TRANS_DDI_FUNC_CTL2_DSI0	0x6b404
#define _TRANS_DDI_FUNC_CTL2_DSI1	0x6bc04
#define TRANS_DDI_FUNC_CTL2(dev_priv, tran)	_MMIO_TRANS2(dev_priv, tran, _TRANS_DDI_FUNC_CTL2_A)
#define  PORT_SYNC_MODE_ENABLE			REG_BIT(4)
#define  CMTG_SECONDARY_MODE			REG_BIT(3)
#define  PORT_SYNC_MODE_MASTER_SELECT_MASK	REG_GENMASK(2, 0)
#define  PORT_SYNC_MODE_MASTER_SELECT(x)	REG_FIELD_PREP(PORT_SYNC_MODE_MASTER_SELECT_MASK, (x))
#define HAS_GMCH(__display)		(DISPLAY_INFO(__display)->has_gmch)

#define INTEL_DISPLAY_DEVICE_PIPE_OFFSET(display, pipe) \
	(DISPLAY_INFO((display))->pipe_offsets[(pipe)] - \
	 DISPLAY_INFO((display))->pipe_offsets[PIPE_A] + \
	 DISPLAY_MMIO_BASE((display)))


#define _MMIO_PIPE2(display, pipe, reg)		_MMIO(INTEL_DISPLAY_DEVICE_PIPE_OFFSET((display), (pipe)) + (reg))

#define _PIPE_ARB_CTL_A			0x70028 /* icl+ */
#define PIPE_ARB_CTL(dev_priv, pipe)		_MMIO_PIPE2(dev_priv, pipe, _PIPE_ARB_CTL_A)
#define   PIPE_ARB_USE_PROG_SLOTS	REG_BIT(13)
#define   TRANSCONF_ENABLE			REG_BIT(31)
#define _TRANSACONF		0x70008
#define TRANSCONF(dev_priv, trans)	_MMIO_PIPE2(dev_priv, (trans), _TRANSACONF)
#define  EDP4K2K_MODE_OVRD_EN			REG_BIT(3)
#define  EDP4K2K_MODE_OVRD_OPTIMIZED		REG_BIT(2)

#define DRM_MODE_FLAG_PHSYNC			(1<<0)
#define DRM_MODE_FLAG_NHSYNC			(1<<1)
#define DRM_MODE_FLAG_PVSYNC			(1<<2)
#define DRM_MODE_FLAG_NVSYNC			(1<<3)
#define DRM_MODE_FLAG_INTERLACE			(1<<4)
#define DRM_MODE_FLAG_DBLSCAN			(1<<5)
#define DRM_MODE_FLAG_CSYNC			(1<<6)
#define DRM_MODE_FLAG_PCSYNC			(1<<7)
#define DRM_MODE_FLAG_NCSYNC			(1<<8)
#define DRM_MODE_FLAG_HSKEW			(1<<9) /* hskew provided */
#define DRM_MODE_FLAG_BCAST			(1<<10) /* deprecated */
#define DRM_MODE_FLAG_PIXMUX			(1<<11) /* deprecated */
#define DRM_MODE_FLAG_DBLCLK			(1<<12)
#define DRM_MODE_FLAG_CLKDIV2			(1<<13)
#define _PIPEADSL		0x70000
#define PIPEDSL(dev_priv, pipe)		_MMIO_PIPE2(dev_priv, pipe, _PIPEADSL)
#define   PIPEDSL_CURR_FIELD	REG_BIT(31) /* ctg+ */
#define   PIPEDSL_LINE_MASK	REG_GENMASK(19, 0)
#define HAS_DDI(__display)		(DISPLAY_INFO(__display)->has_ddi)
#define HAS_LSPCON(__display)		(IS_DISPLAY_VER(__display, 9, 10))

#define HAS_TRANSCODER(__display, trans)	((DISPLAY_RUNTIME_INFO(__display)->cpu_transcoder_mask & \
						  BIT(trans)) != 0)
#define HAS_MSO(__display)		(DISPLAY_VER(__display) >= 12)

#define _TRANS_HTOTAL_A		0x60000
#define _TRANS_HTOTAL_B		0x61000
#define TRANS_HTOTAL(dev_priv, trans)	_MMIO_TRANS2(dev_priv, (trans), _TRANS_HTOTAL_A)
#define   HTOTAL_MASK			REG_GENMASK(31, 16)
#define   HTOTAL(htotal)		REG_FIELD_PREP(HTOTAL_MASK, (htotal))
#define   HACTIVE_MASK			REG_GENMASK(15, 0)
#define   HACTIVE(hdisplay)		REG_FIELD_PREP(HACTIVE_MASK, (hdisplay))

#define _TRANS_HBLANK_A		0x60004
#define _TRANS_HBLANK_B		0x61004
#define TRANS_HBLANK(dev_priv, trans)	_MMIO_TRANS2(dev_priv, (trans), _TRANS_HBLANK_A)
#define   HBLANK_END_MASK		REG_GENMASK(31, 16)
#define   HBLANK_END(hblank_end)	REG_FIELD_PREP(HBLANK_END_MASK, (hblank_end))
#define   HBLANK_START_MASK		REG_GENMASK(15, 0)
#define   HBLANK_START(hblank_start)	REG_FIELD_PREP(HBLANK_START_MASK, (hblank_start))

#define _TRANS_HSYNC_A		0x60008
#define _TRANS_HSYNC_B		0x61008
#define TRANS_HSYNC(dev_priv, trans)	_MMIO_TRANS2(dev_priv, (trans), _TRANS_HSYNC_A)
#define   HSYNC_END_MASK		REG_GENMASK(31, 16)
#define   HSYNC_END(hsync_end)		REG_FIELD_PREP(HSYNC_END_MASK, (hsync_end))
#define   HSYNC_START_MASK		REG_GENMASK(15, 0)
#define   HSYNC_START(hsync_start)	REG_FIELD_PREP(HSYNC_START_MASK, (hsync_start))

#define _TRANS_VTOTAL_A		0x6000c
#define _TRANS_VTOTAL_B		0x6100c
#define TRANS_VTOTAL(dev_priv, trans)	_MMIO_TRANS2(dev_priv, (trans), _TRANS_VTOTAL_A)
#define   VTOTAL_MASK			REG_GENMASK(31, 16)
#define   VTOTAL(vtotal)		REG_FIELD_PREP(VTOTAL_MASK, (vtotal))
#define   VACTIVE_MASK			REG_GENMASK(15, 0)
#define   VACTIVE(vdisplay)		REG_FIELD_PREP(VACTIVE_MASK, (vdisplay))

#define _TRANS_VBLANK_A		0x60010
#define _TRANS_VBLANK_B		0x61010
#define TRANS_VBLANK(dev_priv, trans)	_MMIO_TRANS2(dev_priv, (trans), _TRANS_VBLANK_A)
#define   VBLANK_END_MASK		REG_GENMASK(31, 16)
#define   VBLANK_END(vblank_end)	REG_FIELD_PREP(VBLANK_END_MASK, (vblank_end))
#define   VBLANK_START_MASK		REG_GENMASK(15, 0)
#define   VBLANK_START(vblank_start)	REG_FIELD_PREP(VBLANK_START_MASK, (vblank_start))

#define _TRANS_VSYNC_A		0x60014
#define _TRANS_VSYNC_B		0x61014
#define TRANS_VSYNC(dev_priv, trans)	_MMIO_TRANS2(dev_priv, (trans), _TRANS_VSYNC_A)
#define   VSYNC_END_MASK		REG_GENMASK(31, 16)
#define   VSYNC_END(vsync_end)		REG_FIELD_PREP(VSYNC_END_MASK, (vsync_end))
#define   VSYNC_START_MASK		REG_GENMASK(15, 0)
#define   VSYNC_START(vsync_start)	REG_FIELD_PREP(VSYNC_START_MASK, (vsync_start))

#define _PIPEASRC		0x6001c
#define _PIPEBSRC		0x6101c
#define PIPESRC(dev_priv, pipe)		_MMIO_TRANS2(dev_priv, (pipe), _PIPEASRC)
#define   PIPESRC_WIDTH_MASK	REG_GENMASK(31, 16)
#define   PIPESRC_WIDTH(w)	REG_FIELD_PREP(PIPESRC_WIDTH_MASK, (w))
#define   PIPESRC_HEIGHT_MASK	REG_GENMASK(15, 0)
#define   PIPESRC_HEIGHT(h)	REG_FIELD_PREP(PIPESRC_HEIGHT_MASK, (h))

#define _BCLRPAT_A		0x60020
#define _BCLRPAT_B		0x61020
#define BCLRPAT(dev_priv, trans)		_MMIO_TRANS2(dev_priv, (trans), _BCLRPAT_A)

#define _TRANS_VSYNCSHIFT_A	0x60028
#define _TRANS_VSYNCSHIFT_B	0x61028
#define TRANS_VSYNCSHIFT(dev_priv, trans)	_MMIO_TRANS2(dev_priv, (trans), _TRANS_VSYNCSHIFT_A)

#define _TRANS_MULT_A		0x6002c
#define _TRANS_MULT_B		0x6102c
#define TRANS_MULT(dev_priv, trans)	_MMIO_TRANS2(dev_priv, (trans), _TRANS_MULT_A)
#define   TRANSCONF_INTERLACE_MASK_HSW		REG_GENMASK(22, 21) /* hsw+ */
#define   TRANSCONF_INTERLACE_MASK		REG_GENMASK(23, 21) /* gen3+ */
#define _TRANS_A_SET_CONTEXT_LATENCY		0x6007C
#define _TRANS_B_SET_CONTEXT_LATENCY		0x6107C
#define _TRANS_C_SET_CONTEXT_LATENCY		0x6207C
#define _TRANS_D_SET_CONTEXT_LATENCY		0x6307C
#define TRANS_SET_CONTEXT_LATENCY(dev_priv, tran)		_MMIO_TRANS2(dev_priv, tran, _TRANS_A_SET_CONTEXT_LATENCY)
#define  TRANS_SET_CONTEXT_LATENCY_MASK		REG_GENMASK(15, 0)
#define  TRANS_SET_CONTEXT_LATENCY_VALUE(x)	REG_FIELD_PREP(TRANS_SET_CONTEXT_LATENCY_MASK, (x))
#define _DP_MIN_HBLANK_CTL_A			0x600ac
#define _DP_MIN_HBLANK_CTL_B			0x610ac
#define DP_MIN_HBLANK_CTL(trans)		_MMIO_TRANS(trans, _DP_MIN_HBLANK_CTL_A, _DP_MIN_HBLANK_CTL_B)
#define DP_MAX_DOWNSPREAD                   0x003
# define DP_TPS4_SUPPORTED                  (1 << 7)
# define DP_TPS3_SUPPORTED		    (1 << 6) /* 1.2 */
#define DP_LANE_COUNT_SET	            0x101
#define DP_LINK_RATE_SET		    0x115 
#define DP_MAX_LANE_COUNT                   0x002

#define DP_PSR_SUPPORT                      0x070   /* XXX 1.2? */
# define DP_PSR_IS_SUPPORTED                1
# define DP_PSR2_IS_SUPPORTED		    2	    /* eDP 1.4 */
# define DP_PSR2_WITH_Y_COORD_IS_SUPPORTED  3	    /* eDP 1.4a */
# define DP_PSR2_WITH_Y_COORD_ET_SUPPORTED  4	    /* eDP 1.5, adopted eDP 1.4b SCR */

#define DP_PSR_CAPS                         0x071   /* XXX 1.2? */
# define DP_PSR_NO_TRAIN_ON_EXIT            1
# define DP_PSR_SETUP_TIME_330              (0 << 1)
# define DP_PSR_SETUP_TIME_275              (1 << 1)
# define DP_PSR_SETUP_TIME_220              (2 << 1)
# define DP_PSR_SETUP_TIME_165              (3 << 1)
# define DP_PSR_SETUP_TIME_110              (4 << 1)
# define DP_PSR_SETUP_TIME_55               (5 << 1)
# define DP_PSR_SETUP_TIME_0                (6 << 1)
# define DP_PSR_SETUP_TIME_MASK             (7 << 1)
# define DP_PSR_SETUP_TIME_SHIFT            1
# define DP_PSR2_SU_Y_COORDINATE_REQUIRED   (1 << 4)  /* eDP 1.4a */
# define DP_PSR2_SU_GRANULARITY_REQUIRED    (1 << 5)  /* eDP 1.4b */
# define DP_PSR2_SU_AUX_FRAME_SYNC_NOT_NEEDED (1 << 6)/* eDP 1.5, adopted eDP 1.4b SCR */

#define DP_PSR2_SU_X_GRANULARITY	    0x072 /* eDP 1.4b */
#define DP_PSR2_SU_Y_GRANULARITY	    0x074 /* eDP 1.4b */
#define DP_SYNCHRONIZATION_LATENCY_IN_SINK		0x2009 /* edp 1.4 */
# define DP_MAX_RESYNC_FRAME_COUNT_MASK			(0xf << 0)
# define DP_MAX_RESYNC_FRAME_COUNT_SHIFT		0
# define DP_LAST_ACTUAL_SYNCHRONIZATION_LATENCY_MASK	(0xf << 4)
# define DP_LAST_ACTUAL_SYNCHRONIZATION_LATENCY_SHIFT	4
#define DP_RECEIVER_ALPM_CAP		    0x02e   /* eDP 1.4 */
# define DP_ALPM_CAP			    (1 << 0)



#define RENDER_CLASS               0
#define VIDEO_DECODE_CLASS         1
#define VIDEO_ENHANCE_CLASS        2
#define COPY_ENGINE_CLASS          3
#define COMPUTE_CLASS              4

#define RENDER_RING_BASE           0x2000
#define GEN6_BSD_RING_BASE         0x12000
#define BLT_RING_BASE              0x22000
#define VEBOX_RING_BASE            0x1A000





#define GEN11_COMMON_SLICE_CHICKEN3         _MMIO(0x7304)
#define GEN12_DISABLE_CPS_AWARE_COLOR_PIPE  REG_BIT(6)

#define GEN8_CS_CHICKEN1                    _MMIO(0x2580)
#define GEN9_PREEMPT_GPGPU_LEVEL_MASK       REG_GENMASK(19, 14)
#define GEN9_PREEMPT_GPGPU_THREAD_GROUP_LEVEL REG_FIELD_PREP(GEN9_PREEMPT_GPGPU_LEVEL_MASK, 2)

#define GEN12_FF_MODE2                      _MMIO(0x6554)
#define FF_MODE2_TDS_TIMER_MASK             REG_GENMASK(12, 8)
#define FF_MODE2_TDS_TIMER_128              REG_FIELD_PREP(FF_MODE2_TDS_TIMER_MASK, 4)
#define FF_MODE2_GS_TIMER_MASK              REG_GENMASK(4, 0)
#define FF_MODE2_GS_TIMER_224               REG_FIELD_PREP(FF_MODE2_GS_TIMER_MASK, 14)

#define HIZ_CHICKEN                         _MMIO(0x2688)
#define HZ_DEPTH_TEST_LE_GE_OPT_DISABLE     REG_BIT(13)

#define COMMON_SLICE_CHICKEN4               _MMIO(0x7300)
#define DISABLE_TDC_LOAD_BALANCING_CALC     REG_BIT(6)

#define GEN8_WM_CHICKEN2                    _MMIO(0x4028)
#define WAIT_ON_DEPTH_STALL_DONE_DISABLE    REG_BIT(5)

#define PS_INVOCATION_COUNT                 _MMIO(0x2348)
#define GEN7_COMMON_SLICE_CHICKEN1          _MMIO(0x7010)

#define RING_CTX_TIMESTAMP(base)            _MMIO((base) + 0x3a8)
#define RING_FORCE_TO_NONPRIV(base, i)      _MMIO(((base) + 0x4D0) + (i) * 4)
#define   RING_FORCE_TO_NONPRIV_ACCESS_RD   (1 << 28)
#define   RING_FORCE_TO_NONPRIV_ACCESS_RW   (0 << 28)
#define   RING_FORCE_TO_NONPRIV_RANGE_1     (0 << 0)
#define   RING_FORCE_TO_NONPRIV_RANGE_4     (1 << 0)
#define GEN11_BSD_RING_BASE	0x1c0000
#define GEN11_BSD3_RING_BASE	0x1d0000
#define GEN11_VEBOX_RING_BASE		0x1c8000

#define MCR_REG(offset) offset
#define GEN10_SAMPLER_MODE			MCR_REG(0xe18c)
#define   GEN11_INDIRECT_STATE_BASE_ADDR_OVERRIDE	REG_BIT(0)
#define GEN8_GARBCNTL				_MMIO(0xb004)
#define   GEN12_BUS_HASH_CTL_BIT_EXC		REG_BIT(7)

#define GEN9_CS_DEBUG_MODE1			_MMIO(0x20ec)
#define   FF_DOP_CLOCK_GATE_DISABLE		REG_BIT(1)
#define GEN8_ROW_CHICKEN2			MCR_REG(0xe4f4)
#define   GEN12_DISABLE_EARLY_READ		REG_BIT(14)
#define GEN7_FF_THREAD_MODE		_MMIO(0x20a0)
#define   GEN12_FF_TESSELATION_DOP_GATE_DISABLE BIT(19)
#define   ENABLE_SMALLPL			REG_BIT(15)
#define   GEN12_PUSH_CONST_DEREF_HOLD_DIS	REG_BIT(8)
#define GEN9_ROW_CHICKEN4			MCR_REG(0xe48c)
#define   GEN12_DISABLE_TDL_PUSH		REG_BIT(9)
#define RING_PSMI_CTL(base)			_MMIO((base) + 0x50)
#define   GEN12_WAIT_FOR_EVENT_POWER_DOWN_DISABLE REG_BIT(7)
#define   GEN8_RC_SEMA_IDLE_MSG_DISABLE		REG_BIT(12)
#define GEN7_FF_SLICE_CS_CHICKEN1		_MMIO(0x20e0)
#define   GEN9_FFSC_PERCTX_PREEMPT_CTRL		(1 << 14)

template<unsigned long long Val>
constexpr int build_bug_on_zero_impl() {
	static_assert(Val == 0, "BUILD_BUG_ON_ZERO failed");
	return 0;
}

#define BUILD_BUG_ON_ZERO(cond) (build_bug_on_zero_impl<(cond)>())

#define REG_MASKED_FIELD(mask, value) \
	(BUILD_BUG_ON_ZERO(__builtin_choose_expr(__builtin_constant_p(mask), \
		(mask) & 0xffff0000, 0)) + \
	 BUILD_BUG_ON_ZERO(__builtin_choose_expr(__builtin_constant_p(value), \
		(value) & 0xffff0000, 0)) + \
	 BUILD_BUG_ON_ZERO(__builtin_choose_expr(__builtin_constant_p(mask) && \
		__builtin_constant_p(value), (value) & ~(mask), 0)) + \
	 ((mask) << 16 | (value)))

#define REG_MASKED_FIELD_ENABLE(a) \
	(__builtin_choose_expr(__builtin_constant_p(a), \
		REG_MASKED_FIELD((a), (a)), \
		([&]{ auto a_ = (a); return REG_MASKED_FIELD(a_, a_); }())))


struct i915_wa {
	u32 reg;
	u32        clr;
	u32        set;
	u32        val;
	bool       is_mcr;
	u32		masked_reg:1;
	u32	mcr_reg;
	u32		read;
};


struct i915_wa_list {
	const char          *name;
	struct i915_wa       wa[64];
	u32                  count;
	void    *dev;
};

struct intel_engine_cs {
	void    *dev;
	u32                  mmio_base;
	u32                  engine_class;
	bool                 is_dg1;
	struct i915_wa_list ctx_wa_list;
	struct i915_wa_list  wa_list;
	struct i915_wa_list  whitelist;
};


#define HSW_PWR_WELL_CTL1			_MMIO(0x45400)
#define HSW_PWR_WELL_CTL2			_MMIO(0x45404)
#define HSW_PWR_WELL_CTL3			_MMIO(0x45408)
#define HSW_PWR_WELL_CTL4			_MMIO(0x4540C)
#define   HSW_PWR_WELL_CTL_REQ(pw_idx)		(0x2 << ((pw_idx) * 2))
#define   HSW_PWR_WELL_CTL_STATE(pw_idx)	(0x1 << ((pw_idx) * 2))
#define ICL_PWR_WELL_CTL_AUX1			_MMIO(0x45440)
#define ICL_PWR_WELL_CTL_AUX2			_MMIO(0x45444)
#define ICL_PWR_WELL_CTL_AUX4			_MMIO(0x4544C)
#define   TGL_PW_CTL_IDX_AUX_TBT6		14
#define   TGL_PW_CTL_IDX_AUX_TBT5		13
#define   TGL_PW_CTL_IDX_AUX_TBT4		12
#define   ICL_PW_CTL_IDX_AUX_TBT4		11
#define   TGL_PW_CTL_IDX_AUX_TBT3		11
#define   ICL_PW_CTL_IDX_AUX_TBT3		10
#define   TGL_PW_CTL_IDX_AUX_TBT2		10
#define   ICL_PW_CTL_IDX_AUX_TBT2		9
#define   TGL_PW_CTL_IDX_AUX_TBT1		9
#define   ICL_PW_CTL_IDX_AUX_TBT1		8
#define   TGL_PW_CTL_IDX_AUX_TC6		8
#define   XELPD_PW_CTL_IDX_AUX_E			8
#define   TGL_PW_CTL_IDX_AUX_TC5		7
#define   XELPD_PW_CTL_IDX_AUX_D			7
#define   TGL_PW_CTL_IDX_AUX_TC4		6
#define   ICL_PW_CTL_IDX_AUX_F			5
#define   TGL_PW_CTL_IDX_AUX_TC3		5
#define   ICL_PW_CTL_IDX_AUX_E			4
#define   TGL_PW_CTL_IDX_AUX_TC2		4
#define   ICL_PW_CTL_IDX_AUX_D			3
#define   TGL_PW_CTL_IDX_AUX_TC1		3
#define   ICL_PW_CTL_IDX_AUX_C			2
#define   ICL_PW_CTL_IDX_AUX_B			1
#define   ICL_PW_CTL_IDX_AUX_A			0

#define ICL_PWR_WELL_CTL_DDI1			_MMIO(0x45450)
#define ICL_PWR_WELL_CTL_DDI2			_MMIO(0x45454)
#define ICL_PWR_WELL_CTL_DDI4			_MMIO(0x4545C)
#define TGL_PWR_WELL_CTL_AUX4 _MMIO(0x45470)//verify if ok


#define BXT_DE_PLL_ENABLE		_MMIO(0x46070)
#define   BXT_DE_PLL_PLL_ENABLE		(1 << 31)
#define   BXT_DE_PLL_LOCK		(1 << 30)
#define   BXT_DE_PLL_FREQ_REQ		(1 << 23)
#define   BXT_DE_PLL_FREQ_REQ_ACK	(1 << 22)
#define   ICL_CDCLK_PLL_RATIO(x)	(x)
#define   ICL_CDCLK_PLL_RATIO_MASK	0xff
#define   MASK_WAKEMEM				REG_BIT(13)
#define SKL_FUSE_STATUS				_MMIO(0x42000)
#define  SKL_FUSE_DOWNLOAD_STATUS		(1 << 31)
#define  SKL_FUSE_PG_DIST_STATUS(pg)		(1 << (27 - (pg)))
enum skl_power_gate {
	SKL_PG0,
	SKL_PG1,
	SKL_PG2,
	ICL_PG3,
	ICL_PG4,
};
struct intel_dbuf_state {
	//struct intel_global_state base;

	//struct skl_ddb_entry ddb[I915_MAX_PIPES];
	unsigned int weight[I915_MAX_PIPES];
	u8 slices[I915_MAX_PIPES];
	u8 enabled_slices;
	u8 active_pipes;
	u8 mdclk_cdclk_ratio;
	bool joined_mbus;
};

#define _PIPEA_MBUS_DBOX_CTL			0x7003C
#define _PIPEB_MBUS_DBOX_CTL			0x7103C
#define PIPE_MBUS_DBOX_CTL(pipe)		_MMIO_PIPE(pipe, _PIPEA_MBUS_DBOX_CTL, \
							   _PIPEB_MBUS_DBOX_CTL)
#define MBUS_DBOX_B2B_TRANSACTIONS_MAX_MASK	REG_GENMASK(24, 20) /* tgl+ */
#define MBUS_DBOX_B2B_TRANSACTIONS_MAX(x)	REG_FIELD_PREP(MBUS_DBOX_B2B_TRANSACTIONS_MAX_MASK, x)
#define MBUS_DBOX_B2B_TRANSACTIONS_DELAY_MASK	REG_GENMASK(19, 17) /* tgl+ */
#define MBUS_DBOX_B2B_TRANSACTIONS_DELAY(x)	REG_FIELD_PREP(MBUS_DBOX_B2B_TRANSACTIONS_DELAY_MASK, x)
#define MBUS_DBOX_REGULATE_B2B_TRANSACTIONS_EN	REG_BIT(16) /* tgl+ */
#define MBUS_DBOX_BW_CREDIT_MASK		REG_GENMASK(15, 14)
#define MBUS_DBOX_BW_CREDIT(x)			REG_FIELD_PREP(MBUS_DBOX_BW_CREDIT_MASK, x)
#define MBUS_DBOX_BW_4CREDITS_MTL		REG_FIELD_PREP(MBUS_DBOX_BW_CREDIT_MASK, 0x2)
#define MBUS_DBOX_BW_8CREDITS_MTL		REG_FIELD_PREP(MBUS_DBOX_BW_CREDIT_MASK, 0x3)
#define MBUS_DBOX_B_CREDIT_MASK			REG_GENMASK(12, 8)
#define MBUS_DBOX_B_CREDIT(x)			REG_FIELD_PREP(MBUS_DBOX_B_CREDIT_MASK, x)
#define MBUS_DBOX_I_CREDIT_MASK			REG_GENMASK(7, 5)
#define MBUS_DBOX_I_CREDIT(x)			REG_FIELD_PREP(MBUS_DBOX_I_CREDIT_MASK, x)
#define MBUS_DBOX_A_CREDIT_MASK			REG_GENMASK(3, 0)
#define MBUS_DBOX_A_CREDIT(x)			REG_FIELD_PREP(MBUS_DBOX_A_CREDIT_MASK, x)

#define MBUS_CTL					_MMIO(0x4438C)
#define   MBUS_JOIN					REG_BIT(31)
#define   MBUS_HASHING_MODE_MASK			REG_BIT(30)
#define   MBUS_HASHING_MODE_2x2				REG_FIELD_PREP(MBUS_HASHING_MODE_MASK, 0)
#define   MBUS_HASHING_MODE_1x4				REG_FIELD_PREP(MBUS_HASHING_MODE_MASK, 1)
#define   MBUS_JOIN_PIPE_SELECT_MASK			REG_GENMASK(28, 26)
#define   MBUS_JOIN_PIPE_SELECT(pipe)			REG_FIELD_PREP(MBUS_JOIN_PIPE_SELECT_MASK, pipe)
#define   MBUS_JOIN_PIPE_SELECT_NONE			MBUS_JOIN_PIPE_SELECT(7)
#define   XE3P_MBUS_TRANSLATION_THROTTLE_MIN_MASK	REG_GENMASK(16, 13)
#define   XE3P_MBUS_TRANSLATION_THROTTLE_MIN(val)	REG_FIELD_PREP(XE3P_MBUS_TRANSLATION_THROTTLE_MIN_MASK, val)
#define   MBUS_TRANSLATION_THROTTLE_MIN_MASK		REG_GENMASK(15, 13)
#define   MBUS_TRANSLATION_THROTTLE_MIN(val)		REG_FIELD_PREP(MBUS_TRANSLATION_THROTTLE_MIN_MASK, val)
#define HAS_MBUS_JOINING(__display)	((__display)->platform.alderlake_p || DISPLAY_VER(__display) >= 14)

#define  DBUF_POWER_REQUEST				REG_BIT(31)
#define  DBUF_POWER_STATE				REG_BIT(30)
#define  DBUF_TRACKER_STATE_SERVICE_MASK		REG_GENMASK(23, 19)
#define  DBUF_TRACKER_STATE_SERVICE(x)			REG_FIELD_PREP(DBUF_TRACKER_STATE_SERVICE_MASK, x)
#define  XE3P_DBUF_MIN_TRACKER_STATE_SERVICE_MASK	REG_GENMASK(20, 16)
#define  XE3P_DBUF_MIN_TRACKER_STATE_SERVICE(x)		REG_FIELD_PREP(XE3P_DBUF_MIN_TRACKER_STATE_SERVICE_MASK, x)
#define  DBUF_MIN_TRACKER_STATE_SERVICE_MASK		REG_GENMASK(18, 16) /* ADL-P+ */
#define  DBUF_MIN_TRACKER_STATE_SERVICE(x)		REG_FIELD_PREP(DBUF_MIN_TRACKER_STATE_SERVICE_MASK, x) /* ADL-P+ */
#define   DISABLE_FLR_SRC			REG_BIT(15)








#ifdef __cplusplus
//}
#endif
#endif /* _INTEL_VBT_DEFS_H_ */
