#
# Copyright (C) 2011-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=rtrebooter
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/rtrebooter
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Utility to reboot wifi on ARV7510PW
  MAINTAINER:=Python1320 <python1320@example.com>
endef

define Package/rtrebooter/description
 This package contains the small rtrebooter utility which
 can be used to reboot router on certain rt-chipset WiFi 
 crashes.
endef

define Build/Prepare
	$(INSTALL_DIR) $(PKG_BUILD_DIR)
	$(INSTALL_DATA) ./src/rtrebooter.c $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -Wall \
		-o $(PKG_BUILD_DIR)/rtrebooter $(PKG_BUILD_DIR)/rtrebooter.c
endef

define Package/rtrebooter/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/rtrebooter $(1)/usr/bin/
endef

$(eval $(call BuildPackage,rtrebooter))
