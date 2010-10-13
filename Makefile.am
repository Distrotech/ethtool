AM_CFLAGS = -Wall

man_MANS = ethtool.8
EXTRA_DIST = LICENSE ethtool.8 ethtool.spec.in aclocal.m4 ChangeLog autogen.sh

sbin_PROGRAMS = ethtool
ethtool_SOURCES = ethtool.c ethtool-copy.h ethtool-util.h	\
		  amd8111e.c de2104x.c e100.c e1000.c igb.c	\
		  fec_8xx.c ibm_emac.c ixgb.c ixgbe.c natsemi.c	\
		  pcnet32.c realtek.c tg3.c marvell.c vioc.c	\
		  smsc911x.c at76c50x-usb.c sfc.c stmmac.c

dist-hook:
	cp $(top_srcdir)/ethtool.spec $(distdir)

