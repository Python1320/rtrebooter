include $(TOPDIR)/rules.mk

PKG_NAME:=rtrebooter
PKG_VERSION:=1.2
PKG_RELEASE:=3


PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)
include $(INCLUDE_DIR)/package.mk


define Package/rtrebooter
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Utility to reboot wifi on ARV7510PW
  MAINTAINER:=Python1320 <python1320+rtreboot@gmail.com>
endef

define Package/rtrebooter/description
 This package contains the small rtrebooter utility which
 can be used to reboot router on certain rt-chipset WiFi 
 crashes.
endef

define Build/Configure
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS) -Wall" \
		LDFLAGS="$(TARGET_LDFLAGS)"
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/rtrebooter/install

	$(INSTALL_DIR) $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/rtrebooter $(1)/usr/sbin/
	$(INSTALL_BIN) files/wifireboot.sh $(1)/usr/sbin/wifireboot

	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) files/rtrebooter.init $(1)/etc/init.d/rtrebooter

	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) files/rtrebooter.config $(1)/etc/config/rtrebooter
	
endef

$(eval $(call BuildPackage,rtrebooter))
