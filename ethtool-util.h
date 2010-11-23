/* Portions Copyright 2001 Sun Microsystems (thockin@sun.com) */
/* Portions Copyright 2002 Intel (scott.feldman@intel.com) */
#ifndef ETHTOOL_UTIL_H__
#define ETHTOOL_UTIL_H__

#include <sys/types.h>
#include <endian.h>

/* ethtool.h expects these to be defined by <linux/types.h> */
#ifndef HAVE_BE_TYPES
typedef __uint16_t __be16;
typedef __uint32_t __be32;
#endif

#include "ethtool-copy.h"

typedef unsigned long long u64;
typedef __uint32_t u32;
typedef __uint16_t u16;
typedef __uint8_t u8;
typedef __int32_t s32;

#if __BYTE_ORDER == __BIG_ENDIAN
static inline u16 cpu_to_be16(u16 value)
{
    return value;
}
static inline u32 cpu_to_be32(u32 value)
{
    return value;
}
#else
static inline u16 cpu_to_be16(u16 value)
{
	return (value >> 8) | (value << 8);
}
static inline u32 cpu_to_be32(u32 value)
{
	return cpu_to_be16(value >> 16) | (cpu_to_be16(value) << 16);
}
#endif

/* National Semiconductor DP83815, DP83816 */
int natsemi_dump_regs(struct ethtool_drvinfo *info, struct ethtool_regs *regs);
int natsemi_dump_eeprom(struct ethtool_drvinfo *info,
	struct ethtool_eeprom *ee);

/* Digital/Intel 21040 and 21041 */
int de2104x_dump_regs(struct ethtool_drvinfo *info, struct ethtool_regs *regs);

/* Intel(R) PRO/1000 Gigabit Adapter Family */
int e1000_dump_regs(struct ethtool_drvinfo *info, struct ethtool_regs *regs);

int igb_dump_regs(struct ethtool_drvinfo *info, struct ethtool_regs *regs);

/* RealTek PCI */
int realtek_dump_regs(struct ethtool_drvinfo *info, struct ethtool_regs *regs);

/* Intel(R) PRO/100 Fast Ethernet Adapter Family */
int e100_dump_regs(struct ethtool_drvinfo *info, struct ethtool_regs *regs);

/* Tigon3 */
int tg3_dump_eeprom(struct ethtool_drvinfo *info, struct ethtool_eeprom *ee);

/* Advanced Micro Devices  AMD8111 based Adapter */
int amd8111e_dump_regs(struct ethtool_drvinfo *info, struct ethtool_regs *regs);

/* Advanced Micro Devices PCnet32 Adapter */
int pcnet32_dump_regs(struct ethtool_drvinfo *info, struct ethtool_regs *regs);

/* Motorola 8xx FEC Ethernet controller */
int fec_8xx_dump_regs(struct ethtool_drvinfo *info, struct ethtool_regs *regs);

/* PowerPC 4xx on-chip Ethernet controller */
int ibm_emac_dump_regs(struct ethtool_drvinfo *info, struct ethtool_regs *regs);

/* Intel(R) PRO/10GBe Gigabit Adapter Family */
int ixgb_dump_regs(struct ethtool_drvinfo *info, struct ethtool_regs *regs);

int ixgbe_dump_regs(struct ethtool_drvinfo *info, struct ethtool_regs *regs);

/* Broadcom Tigon3 Ethernet controller */
int tg3_dump_regs(struct ethtool_drvinfo *info, struct ethtool_regs *regs);

/* SysKonnect Gigabit (Genesis and Yukon) */
int skge_dump_regs(struct ethtool_drvinfo *info, struct ethtool_regs *regs);

/* SysKonnect Gigabit (Yukon2) */
int sky2_dump_regs(struct ethtool_drvinfo *info, struct ethtool_regs *regs);

/* Fabric7 VIOC */
int vioc_dump_regs(struct ethtool_drvinfo *info, struct ethtool_regs *regs);

/* SMSC LAN911x/LAN921x embedded ethernet controller */
int smsc911x_dump_regs(struct ethtool_drvinfo *info, struct ethtool_regs *regs);

int at76c50x_usb_dump_regs(struct ethtool_drvinfo *info, struct ethtool_regs *regs);

/* Solarflare Solarstorm controllers */
int sfc_dump_regs(struct ethtool_drvinfo *info, struct ethtool_regs *regs);

/* STMMAC embedded ethernet controller */
int st_mac100_dump_regs(struct ethtool_drvinfo *info,
			struct ethtool_regs *regs);
int st_gmac_dump_regs(struct ethtool_drvinfo *info, struct ethtool_regs *regs);
#endif
