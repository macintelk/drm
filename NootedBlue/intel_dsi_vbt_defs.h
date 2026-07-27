/* SPDX-License-Identifier: MIT */
/* Copyright © 2025 Intel Corporation */

#ifndef __INTEL_DSI_VBT_DEFS_H__
#define __INTEL_DSI_VBT_DEFS_H__

#ifdef __cplusplus
//extern "C" {
#endif

#define __packed                        __attribute__((__packed__))


typedef int8_t s8;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef uint64_t resource_size_t;

struct AGDCDPPortConfig_t { /* PlaceHolder Structure */
	__uint32_t portindex;
	__uint32_t field1;
	__uint16_t AGDCLinkConfigP;
	__uint8_t BitRate; /* Created by Rename Structure Field action */
	__uint8_t enableMST; /* Created by Rename Structure Field action */
	__uint16_t CR;
	__uint16_t EQ;
	__uint8_t field7_0x10;
	__uint8_t field8_0x11;
	__uint8_t field9_0x12;
	__uint8_t NumberOfLanes; /* Created by Rename Structure Field action */
	__uint8_t voltageSwing; /* Created by Rename Structure Field action */
	__uint8_t preEmphasis; /* Created by Rename Structure Field action */
	__uint8_t Downspread; /* Created by Rename Structure Field action */
	__uint8_t ASR; /* Created by Rename Structure Field action */
	__uint8_t BitRate2; /* Created by Rename Structure Field action */
	__uint8_t NumberOfLanes2; /* Created by Rename Structure Field action */
	__uint8_t field17_0x1a;
	__uint8_t EnhancedFraming; /* Created by Rename Structure Field action */
	__uint32_t field2;
	__uint64_t field20_0x20;
	__uint64_t field21_0x28;
	__uint64_t field22_0x30;
	__uint32_t status;
	__uint64_t linkst;
	__uint64_t field25_0x44;
	__uint32_t field26_0x4c;
};

#define REG_BIT(n) (1UL << (n))
#define __AC(X,Y)	(X##Y)
#define _AC(X,Y)	__AC(X,Y)
#define _AT(T,X)	((T)(X))
#define _UL(x)		(_AC(x, UL))
#define UL(x)		(_UL(x))
#define _ULL(x)		(_AC(x, ULL))
#define ULL(x)		(_ULL(x))
#define BIT_ULL(nr)		(ULL(1) << (nr))
#define BIT(nr)			(UL(1) << (nr))

#define BITS_PER_LONG_LONG 64
#define BITS_PER_LONG 64
#define BITS_PER_BYTE		8
#define BITS_PER_TYPE(type)	(sizeof(type) * BITS_PER_BYTE)
#define __KERNEL_DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))
#define BITS_TO_LONGS(nr)	__KERNEL_DIV_ROUND_UP(nr, BITS_PER_TYPE(long))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define _PICK_EVEN(__index, __a, __b) ((__a) + (__index) * ((__b) - (__a)))

#define REG_GENMASK(high, low) \
	((uint32_t)(((1ULL << ((high) - (low) + 1)) - 1ULL) << (low)))

#define REG_FIELD_GET(mask, val) \
	((uint32_t)(((val) & (mask)) / ((mask) & -(mask))))

#define BIT_U8(nr)  ((uint8_t)(1ULL << (nr)))
#define BIT_U16(nr) ((uint16_t)(1ULL << (nr)))
#define BIT_U32(nr) ((uint32_t)(1ULL << (nr)))
#define BIT_U64(nr) ((uint64_t)(1ULL << (nr)))

#define GENMASK(h, l) \
	((unsigned long)(((1ULL << ((h) - (l) + 1)) - 1ULL) << (l)))


#define REG_FIELD_PREP(mask, val) (((val) << (__builtin_ffsll(mask) - 1)) & (mask))

#define __bf_shf(x) (__builtin_ffsll(x) - 1)
#define FIELD_PREP(_mask, _val)                        \
({                                \
((__typeof(_mask))(_val) << __bf_shf(_mask)) & (_mask);    \
})

#define REG_MASKED_FIELD_ENABLE(mask)  (((mask) << 16) | (mask))
#define REG_MASKED_FIELD_DISABLE(mask) ((mask) << 16)

struct PACKED bdb_block_header {
	u8 vid;
	u8 _0; /* padding */
	u16 size;
};

struct list_head {
	struct list_head *next, *prev;
};

#define container_of(ptr, type, member) ({          \
	const __typeof( ((type *)0)->member ) *__mptr = (ptr);    \
	(type *)( (char *)__mptr - offsetof(type,member) );})

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)

#define list_entry_is_head(pos, head, member) \
	(&(pos)->member == (head))

#define list_next_entry(pos, member) \
	list_entry((pos)->member.next, __typeof(*(pos)), member)

#define list_for_each_entry(pos, head, member)				\
	for (pos = list_first_entry(head, __typeof(*pos), member);	\
		 !list_entry_is_head(pos, head, member);			\
		 pos = list_next_entry(pos, member))





#define UTIL_PIN_CTL			(0x48400)
#define   UTIL_PIN_POLARITY		(1 << 22)
#define PPS_BASE			0x61200
#define PCH_PPS_BASE			0xC7200
#define _MMIO_PPS(display, pps_idx, reg) \
	((display)->pps.mmio_base - PPS_BASE + (reg) + (pps_idx) * 0x100)
#define _PP_CONTROL			0x61204
#define PP_CONTROL(display, pps_idx)	_MMIO_PPS((display), (pps_idx), _PP_CONTROL)
#define _PP_STATUS			0x61200
#define PP_STATUS(display, pps_idx)	_MMIO_PPS((display), (pps_idx), _PP_STATUS)
#define _PP_ON_DELAYS			0x61208
#define PP_ON_DELAYS(display, pps_idx)	_MMIO_PPS((display), (pps_idx), _PP_ON_DELAYS)
#define _PP_OFF_DELAYS			0x6120C
#define PP_OFF_DELAYS(display, pps_idx)	_MMIO_PPS((display), (pps_idx), _PP_OFF_DELAYS)
#define _PP_DIVISOR			0x61210
#define PP_DIVISOR(display, pps_idx)	_MMIO_PPS((display), (pps_idx), _PP_DIVISOR)
#define SOUTH_CHICKEN1		(0xc2000)
#define _PIPE(pipe, a, b)		_PICK_EVEN(pipe, a, b)
#define _MMIO_PIPE(pipe, a, b)		(_PIPE(pipe, a, b))
#define _BXT_BLC_PWM_CTL1			0xC8250
#define _BXT_BLC_PWM_CTL2			0xC8350

#define _BXT_BLC_PWM_DUTY2			0xC8358
#define _BXT_BLC_PWM_DUTY1			0xC8258
#define BXT_BLC_PWM_DUTY(controller)   _MMIO_PIPE(controller, \
					_BXT_BLC_PWM_DUTY1, _BXT_BLC_PWM_DUTY2)
#define BXT_BLC_PWM_CTL(controller)    _MMIO_PIPE(controller,		\
					_BXT_BLC_PWM_CTL1, _BXT_BLC_PWM_CTL2)
#define   BXT_BLC_PWM_ENABLE			(1 << 31)
#define   BXT_BLC_PWM_POLARITY			(1 << 29)
#define  ICP_SECOND_PPS_IO_SELECT	REG_BIT(2)
#define SFUSE_STRAP			(0xc2014)
#define  SFUSE_STRAP_RAW_FREQUENCY	(1 << 8)
#define  CNP_RAWCLK_DIV(div)	((div) << 16)
#define  CNP_RAWCLK_DEN(den)	((den) << 26)
#define  ICP_RAWCLK_NUM(num)	((num) << 11)
#define PCH_RAWCLK_FREQ         (0xc6204)
#define _BXT_BLC_PWM_FREQ1			0xC8254
#define _BXT_BLC_PWM_FREQ2			0xC8354
#define BXT_BLC_PWM_FREQ(controller)   _MMIO_PIPE(controller, \
					_BXT_BLC_PWM_FREQ1, _BXT_BLC_PWM_FREQ2)

#define  PANEL_POWER_UP_DELAY_MASK	REG_GENMASK(28, 16)
#define  PANEL_LIGHT_ON_DELAY_MASK	REG_GENMASK(12, 0)
#define  PANEL_POWER_DOWN_DELAY_MASK	REG_GENMASK(28, 16)
#define  PANEL_LIGHT_OFF_DELAY_MASK	REG_GENMASK(12, 0)
#define  PP_REFERENCE_DIVIDER_MASK	REG_GENMASK(31, 8)
#define  PANEL_POWER_CYCLE_DELAY_MASK	REG_GENMASK(4, 0)
#define  BXT_POWER_CYCLE_DELAY_MASK	REG_GENMASK(8, 4)

enum ConnectorType : uint32_t {
	ConnectorZero       = 0x0,
	ConnectorDummy      = 0x1,
	ConnectorLVDS       = 0x2,
	ConnectorDigitalDVI = 0x4,
	ConnectorSVID       = 0x8,
	ConnectorVGA        = 0x10,
	ConnectorDP         = 0x400,
	ConnectorHDMI       = 0x800,
	ConnectorAnalogDVI  = 0x2000
};
struct PACKED ConnectorInfo {
	uint32_t index;
	uint32_t busId;
	uint32_t pipe;
	uint32_t pad;
	ConnectorType type;
	uint32_t flags;
};

#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))
#define DIV_ROUND_CLOSEST(x, divisor)		\
({							\
	__typeof__(x) __x = x;				\
	__typeof__(divisor) __d = divisor;		\
							\
	(((__typeof__(x))-1) > 0 ||			\
	 ((__typeof__(divisor))-1) > 0 ||		\
	 (((__x) > 0) == ((__d) > 0))) ?		\
		(((__x) + ((__d) / 2)) / (__d)) :	\
		(((__x) - ((__d) / 2)) / (__d));	\
})
#define KHz(x) (1000 * (x))
#define MHz(x) KHz(1000 * (x))

#define __round_mask(x, y) ((__typeof__(x))((y)-1))
#define round_up(x, y) ((((x)-1) | __round_mask(x, y))+1)


enum intel_backlight_type {
	INTEL_BACKLIGHT_PMIC,
	INTEL_BACKLIGHT_LPSS,
	INTEL_BACKLIGHT_DISPLAY_DDI,
	INTEL_BACKLIGHT_DSI_DCS,
	INTEL_BACKLIGHT_PANEL_DRIVER_INTERFACE,
	INTEL_BACKLIGHT_VESA_EDP_AUX_INTERFACE,
};

# define DP_TX_FFE_PRESET_VALUE_MASK        (0xf << 0)
# define DP_TRAIN_PRE_EMPHASIS_MASK	    (3 << 3)
# define DP_TRAIN_PRE_EMPH_LEVEL_0		(0 << 3)
# define DP_TRAIN_PRE_EMPH_LEVEL_1		(1 << 3)
# define DP_TRAIN_PRE_EMPH_LEVEL_2		(2 << 3)
# define DP_TRAIN_PRE_EMPH_LEVEL_3		(3 << 3)
# define DP_TRAIN_VOLTAGE_SWING_MASK	    0x3
# define DP_TRAIN_VOLTAGE_SWING_SHIFT	    0
# define DP_TRAIN_MAX_SWING_REACHED	    (1 << 2)
# define DP_TRAIN_VOLTAGE_SWING_LEVEL_0 (0 << 0)
# define DP_TRAIN_VOLTAGE_SWING_LEVEL_1 (1 << 0)
# define DP_TRAIN_VOLTAGE_SWING_LEVEL_2 (2 << 0)
# define DP_TRAIN_VOLTAGE_SWING_LEVEL_3 (3 << 0)

#define port_name(p) ((p) + '0')
#define AUX_CH_NAME_BUFSIZE	6

#define  _MMIO(x) (x)
#define SOUTH_DSPCLK_GATE_D	_MMIO(0xc2020)
#define  PCH_GMBUSUNIT_CLOCK_GATE_DISABLE (1 << 31)
#define  PCH_DPLUNIT_CLOCK_GATE_DISABLE (1 << 30)
#define  PCH_DPLSUNIT_CLOCK_GATE_DISABLE (1 << 29)
#define  PCH_DPMGUNIT_CLOCK_GATE_DISABLE (1 << 15)
#define  PCH_CPUNIT_CLOCK_GATE_DISABLE (1 << 14)
#define  CNP_PWM_CGE_GATING_DISABLE (1 << 13)
#define  PCH_LP_PARTITION_LEVEL_DISABLE  (1 << 12)

#define  PANEL_UNLOCK_MASK		REG_GENMASK(31, 16)
#define  PANEL_UNLOCK_REGS		REG_FIELD_PREP(PANEL_UNLOCK_MASK, 0xabcd)
#define  PANEL_POWER_RESET		REG_BIT(1)
#define  PANEL_POWER_ON			REG_BIT(0)

#define IS_DISPLAY_VER(__display, from, until) \
	(DISPLAY_VER(__display) >= (from) && DISPLAY_VER(__display) <= (until))


#define BDB_CHILD_DEVICE_CONFIG	40

#define OPREGION_HEADER_OFFSET 0
#define OPREGION_ACPI_OFFSET   0x100
#define OPREGION_SWSCI_OFFSET  0x200
#define OPREGION_ASLE_OFFSET   0x300
#define OPREGION_VBT_OFFSET    0x400
#define OPREGION_ASLE_EXT_OFFSET	0x1C00

#define OPREGION_SIGNATURE "IntelGraphicsMem"
#define MBOX_ACPI		(1 << 0)
#define MBOX_SWSCI		(1 << 1)
#define MBOX_ASLE		(1 << 2)
#define MBOX_ASLE_EXT		(1 << 4)
#define MBOX_BACKLIGHT		(1 << 5)

#define ASLE_ARDY_READY		(1 << 0)
#define ASLE_ARDY_NOT_READY	(0 << 0)

#define OPREGION_SIZE	(8 * 1024)

struct PACKED opregion_header {
	u8 signature[16];
	u32 size;
	struct PACKED  {
		u8 rsvd;
		u8 revision;
		u8 minor;
		u8 major;
	} over ;
	u8 bios_ver[32];
	u8 vbios_ver[16];
	u8 driver_ver[16];
	u32 mboxes;
	u32 driver_model;
	u32 pcon;
	u8 dver[32];
	u8 rsvd[124];
} ;

struct opregion_acpi {
	u32 drdy;
	u32 csts;
	u32 cevt;
	u8 rsvd1[20];
	u32 didl[8];
	u32 cpdl[8];
	u32 cadl[8];
	u32 nadl[8];
	u32 aslp;
	u32 tidx;
	u32 chpd;
	u32 clid;
	u32 cdck;
	u32 sxsw;
	u32 evts;
	u32 cnot;
	u32 nrdy;
	u32 did2[7];
	u32 cpd2[7];
	u8 rsvd2[4];
} __packed;

struct PACKED opregion_asle {
	u32 ardy;
	u32 aslc;
	u32 tche;
	u32 alsi;
	u32 bclp;
	u32 pfit;
	u32 cblv;
	u16 bclm[20];
	u32 cpfm;
	u32 epfm;
	u8 plut[74];
	u32 pfmb;
	u32 cddv;
	u32 pcft;
	u32 srot;
	u32 iuer;
	u64 fdss;
	u32 fdsp;
	u32 stat;
	u64 rvda;
	u32 rvds;
	u8 rsvd[58];
} ;

struct PACKED opregion_asle_ext {
	u32 phed;
	u8 bddc[256];
	u8 rsvd[764];
} ;

struct intel_opregion {
	struct opregion_header *header;
	struct opregion_acpi *acpi;
	struct opregion_asle *asle;
	struct opregion_asle_ext *asle_ext;
	void *rvda;
	const void *vbt;
	u32 vbt_size;
};


/*
 * MIPI Sequence Block definitions
 *
 * Note the VBT spec has AssertReset / DeassertReset swapped from their
 * usual naming, we use the proper names here to avoid confusion when
 * reading the code.
 */
enum mipi_seq {
	MIPI_SEQ_END = 0,
	MIPI_SEQ_DEASSERT_RESET,	/* Spec says MipiAssertResetPin */
	MIPI_SEQ_INIT_OTP,
	MIPI_SEQ_DISPLAY_ON,
	MIPI_SEQ_DISPLAY_OFF,
	MIPI_SEQ_ASSERT_RESET,		/* Spec says MipiDeassertResetPin */
	MIPI_SEQ_BACKLIGHT_ON,		/* sequence block v2+ */
	MIPI_SEQ_BACKLIGHT_OFF,		/* sequence block v2+ */
	MIPI_SEQ_TEAR_ON,		/* sequence block v2+ */
	MIPI_SEQ_TEAR_OFF,		/* sequence block v3+ */
	MIPI_SEQ_POWER_ON,		/* sequence block v3+ */
	MIPI_SEQ_POWER_OFF,		/* sequence block v3+ */
	MIPI_SEQ_MAX
};

enum mipi_seq_element {
	MIPI_SEQ_ELEM_END = 0,
	MIPI_SEQ_ELEM_SEND_PKT,
	MIPI_SEQ_ELEM_DELAY,
	MIPI_SEQ_ELEM_GPIO,
	MIPI_SEQ_ELEM_I2C,		/* sequence block v2+ */
	MIPI_SEQ_ELEM_SPI,		/* sequence block v3+ */
	MIPI_SEQ_ELEM_PMIC,		/* sequence block v3+ */
	MIPI_SEQ_ELEM_MAX
};

#define MIPI_DSI_UNDEFINED_PANEL_ID	0
#define MIPI_DSI_GENERIC_PANEL_ID	1

struct mipi_config {
	u16 panel_id;

	/* General Params */
	struct {
		u32 enable_dithering:1;
		u32 rsvd1:1;
		u32 is_bridge:1;

		u32 panel_arch_type:2;
		u32 is_cmd_mode:1;

#define NON_BURST_SYNC_PULSE	0x1
#define NON_BURST_SYNC_EVENTS	0x2
#define BURST_MODE		0x3
		u32 video_transfer_mode:2;

		u32 cabc_supported:1;
#define PPS_BLC_PMIC   0
#define PPS_BLC_SOC    1
		u32 pwm_blc:1;

#define PIXEL_FORMAT_RGB565			0x1
#define PIXEL_FORMAT_RGB666			0x2
#define PIXEL_FORMAT_RGB666_LOOSELY_PACKED	0x3
#define PIXEL_FORMAT_RGB888			0x4
		u32 videomode_color_format:4;

#define ENABLE_ROTATION_0	0x0
#define ENABLE_ROTATION_90	0x1
#define ENABLE_ROTATION_180	0x2
#define ENABLE_ROTATION_270	0x3
		u32 rotation:2;
		u32 bta_disable:1;
		u32 rsvd2:15;
	} __packed;

	/* Port Desc */
	struct {
#define DUAL_LINK_NOT_SUPPORTED	0
#define DUAL_LINK_FRONT_BACK	1
#define DUAL_LINK_PIXEL_ALT	2
		u16 dual_link:2;
		u16 lane_cnt:2;
		u16 pixel_overlap:3;
		u16 rgb_flip:1;
#define DL_DCS_PORT_A			0x00
#define DL_DCS_PORT_C			0x01
#define DL_DCS_PORT_A_AND_C		0x02
		u16 dl_dcs_cabc_ports:2;
		u16 dl_dcs_backlight_ports:2;
		u16 port_sync:1;				/* 219-230 */
		u16 rsvd3:3;
	} __packed;

	/* DSI Controller Parameters */
	struct {
		u16 dsi_usage:1;
		u16 rsvd4:15;
	} __packed;

	u8 rsvd5;
	u32 target_burst_mode_freq;
	u32 dsi_ddr_clk;
	u32 bridge_ref_clk;

	/* LP Byte Clock */
	struct {
#define  BYTE_CLK_SEL_20MHZ		0
#define  BYTE_CLK_SEL_10MHZ		1
#define  BYTE_CLK_SEL_5MHZ		2
		u8 byte_clk_sel:2;
		u8 rsvd6:6;
	} __packed;

	/* DPhy Flags */
	struct {
		u16 dphy_param_valid:1;
		u16 eot_pkt_disabled:1;
		u16 enable_clk_stop:1;
		u16 blanking_packets_during_bllp:1;		/* 219+ */
		u16 lp_clock_during_lpm:1;			/* 219+ */
		u16 rsvd7:11;
	} __packed;

	u32 hs_tx_timeout;
	u32 lp_rx_timeout;
	u32 turn_around_timeout;
	u32 device_reset_timer;
	u32 master_init_timer;
	u32 dbi_bw_timer;
	u32 lp_byte_clk_val;

	/*  DPhy Params */
	struct {
		u32 prepare_cnt:6;
		u32 rsvd8:2;
		u32 clk_zero_cnt:8;
		u32 trail_cnt:5;
		u32 rsvd9:3;
		u32 exit_zero_cnt:6;
		u32 rsvd10:2;
	} __packed;

	u32 clk_lane_switch_cnt;
	u32 hl_switch_cnt;

	u32 rsvd11[6];

	/* timings based on dphy spec */
	u8 tclk_miss;
	u8 tclk_post;
	u8 rsvd12;
	u8 tclk_pre;
	u8 tclk_prepare;
	u8 tclk_settle;
	u8 tclk_term_enable;
	u8 tclk_trail;
	u16 tclk_prepare_clkzero;
	u8 rsvd13;
	u8 td_term_enable;
	u8 teot;
	u8 ths_exit;
	u8 ths_prepare;
	u16 ths_prepare_hszero;
	u8 rsvd14;
	u8 ths_settle;
	u8 ths_skip;
	u8 ths_trail;
	u8 tinit;
	u8 tlpx;
	u8 rsvd15[3];

	/* GPIOs */
	u8 panel_enable;
	u8 bl_enable;
	u8 pwm_enable;
	u8 reset_r_n;
	u8 pwr_down_r;
	u8 stdby_r_n;
} __packed;

/* all delays have a unit of 100us */
struct mipi_pps_data {
	u16 panel_on_delay;
	u16 bl_enable_delay;
	u16 bl_disable_delay;
	u16 panel_off_delay;
	u16 panel_power_cycle_delay;
} __packed;


enum gmbus_gpio {
	GPIOA,
	GPIOB,
	GPIOC,
	GPIOD,
	GPIOE,
	GPIOF,
	GPIOG,
	GPIOH,
	__GPIOI_UNUSED,
	GPIOJ,
	GPIOK,
	GPIOL,
	GPIOM,
	GPION,
	GPIOO,
};

struct gmbus_pin {
	const char *name;
	enum gmbus_gpio gpio;
};

#define GMBUS_PIN_DISABLED	0
#define GMBUS_PIN_SSC		1
#define GMBUS_PIN_VGADDC	2
#define GMBUS_PIN_PANEL		3
#define GMBUS_PIN_DPD_CHV	3 /* HDMID_CHV */
#define GMBUS_PIN_DPC		4 /* HDMIC */
#define GMBUS_PIN_DPB		5 /* SDVO, HDMIB */
#define GMBUS_PIN_DPD		6 /* HDMID */
#define GMBUS_PIN_RESERVED	7 /* 7 reserved */
#define GMBUS_PIN_1_BXT		1 /* BXT+ (atom) and CNP+ (big core) */
#define GMBUS_PIN_2_BXT		2
#define GMBUS_PIN_3_BXT		3
#define GMBUS_PIN_4_CNP		4
#define GMBUS_PIN_5_MTP		5
#define GMBUS_PIN_9_TC1_ICP	9
#define GMBUS_PIN_10_TC2_ICP	10
#define GMBUS_PIN_11_TC3_ICP	11
#define GMBUS_PIN_12_TC4_ICP	12
#define GMBUS_PIN_13_TC5_TGP	13
#define GMBUS_PIN_14_TC6_TGP	14

#define GMBUS_NUM_PINS	15 /* including 0 */
static const struct gmbus_pin gmbus_pins_icp[] = {
	[GMBUS_PIN_1_BXT] = { "dpa", GPIOB },
	[GMBUS_PIN_2_BXT] = { "dpb", GPIOC },
	[GMBUS_PIN_3_BXT] = { "dpc", GPIOD },
	[GMBUS_PIN_9_TC1_ICP] = { "tc1", GPIOJ },
	[GMBUS_PIN_10_TC2_ICP] = { "tc2", GPIOK },
	[GMBUS_PIN_11_TC3_ICP] = { "tc3", GPIOL },
	[GMBUS_PIN_12_TC4_ICP] = { "tc4", GPIOM },
	[GMBUS_PIN_13_TC5_TGP] = { "tc5", GPION },
	[GMBUS_PIN_14_TC6_TGP] = { "tc6", GPIOO },
};

#define WRITE_ONCE(x, val)						\
do {									\
	*(volatile __typeof(x) *)&(x) = (val);				\
} while (0)
static inline void INIT_LIST_HEAD(struct list_head *list)
{
	WRITE_ONCE(list->next, list);
	WRITE_ONCE(list->prev, list);
}

#define DRM_DISPLAY_MODE_LEN	32
#define DRM_MODE_TYPE_PREFERRED	(1<<3)

#define for_each_dmc_id(__dmc_id) \
	for ((__dmc_id) = static_cast<decltype(__dmc_id)>(DMC_FW_MAIN); \
		 static_cast<int>(__dmc_id) < static_cast<int>(DMC_FW_MAX); \
		 (__dmc_id) = static_cast<decltype(__dmc_id)>(static_cast<int>(__dmc_id) + 1))

#define ICL_DMC_MAX_FW_SIZE		0x6000
#define DISPLAY_VER13_DMC_MAX_FW_SIZE	0x20000
#define DISPLAY_VER12_DMC_MAX_FW_SIZE	ICL_DMC_MAX_FW_SIZE


#ifdef __cplusplus
//}
#endif
#endif /* __INTEL_DSI_VBT_DEFS_H__ */
