LOCAL_PATH := $(call my-dir)

INITSH := device/sony/nypon/combinedroot/init.sh
BOOTREC_DEVICE := device/sony/nypon/combinedroot/bootrec-device
BOOTREC_LED := device/sony/nypon/combinedroot/bootrec-led

MKELF := device/sony/nypon/releasetools/mkelf.py
INSTALLED_BOOTIMAGE_TARGET := $(PRODUCT_OUT)/boot.img
$(INSTALLED_BOOTIMAGE_TARGET): $(PRODUCT_OUT)/kernel $(recovery_ramdisk) $(INSTALLED_RAMDISK_TARGET) $(PRODUCT_OUT)/utilities/busybox $(MKBOOTIMG) $(MINIGZIP) $(INTERNAL_BOOTIMAGE_FILES)
	$(call pretty,"Boot image: $@")
	$(hide) mkdir -p $(PRODUCT_OUT)/combinedroot/sbin
	$(hide) cp -R $(PRODUCT_OUT)/root/logo.rle $(PRODUCT_OUT)/combinedroot/logo.rle
	$(hide) cp -R $(INITSH) $(PRODUCT_OUT)/combinedroot/sbin/init.sh
	$(hide) chmod 755 $(PRODUCT_OUT)/combinedroot/sbin/init.sh
	$(hide) ln -s sbin/init.sh $(PRODUCT_OUT)/combinedroot/init
	$(hide) cp $(PRODUCT_OUT)/utilities/busybox $(PRODUCT_OUT)/combinedroot/sbin/
	$(hide) cp $(BOOTREC_DEVICE) $(PRODUCT_OUT)/combinedroot/sbin/
	$(hide) cp $(BOOTREC_LED) $(PRODUCT_OUT)/combinedroot/sbin/
	$(hide) cp -R $(PRODUCT_OUT)/recovery/root/sbin/* $(PRODUCT_OUT)/root/sbin/
	$(hide) cp -R $(PRODUCT_OUT)/../../../../device/sony/$(TARGET_DEVICE)/prebuilt/root/default.prop $(PRODUCT_OUT)/root/
	$(hide) cp -R $(PRODUCT_OUT)/../../../../device/sony/$(TARGET_DEVICE)/prebuilt/root/init.environ.rc $(PRODUCT_OUT)/root/
	$(hide) cp -R $(PRODUCT_OUT)/../../../../device/sony/nypon/recovery/init.rc $(PRODUCT_OUT)/recovery/root/
	$(hide) cp -R $(PRODUCT_OUT)/../../../../device/sony/$(TARGET_DEVICE)/recovery/runatboot.sh $(PRODUCT_OUT)/recovery/root/sbin/
	$(hide) rm -rf $(PRODUCT_OUT)/recovery/root/sbin/usbid_init.sh
	$(hide) $(MKBOOTFS) $(PRODUCT_OUT)/recovery/root | gzip > $(PRODUCT_OUT)/ramdisk-recovery.gz
	$(hide) cp -R $(PRODUCT_OUT)/ramdisk-recovery.gz $(PRODUCT_OUT)/combinedroot/sbin/ramdisk-recovery.gz
	$(hide) $(MKBOOTFS) $(PRODUCT_OUT)/root | gzip > $(PRODUCT_OUT)/ramdisk.gz
	$(hide) cp -R $(PRODUCT_OUT)/ramdisk.gz $(PRODUCT_OUT)/combinedroot/sbin/ramdisk.gz
	$(hide) $(MKBOOTFS) $(PRODUCT_OUT)/combinedroot/ > $(PRODUCT_OUT)/combinedroot.cpio
	$(hide) cat $(PRODUCT_OUT)/combinedroot.cpio | gzip > $(PRODUCT_OUT)/combinedroot.fs
	$(hide) rm -rf $(PRODUCT_OUT)/system/bin/recovery
	$(hide) python $(MKELF) -o $@ $(PRODUCT_OUT)/kernel@$(BOARD_FORCE_KERNEL_ADDRESS) $(PRODUCT_OUT)/combinedroot.fs@$(BOARD_MKBOOTIMG_ARGS),ramdisk $(BOARD_CMDLINE)@cmdline

INSTALLED_RECOVERYIMAGE_TARGET := $(PRODUCT_OUT)/recovery.img
$(INSTALLED_RECOVERYIMAGE_TARGET): $(MKBOOTIMG) \
	$(recovery_ramdisk) \
	$(recovery_kernel)
	@echo ----- Making recovery image ------
	$(hide) python $(MKELF) -o $@ $(PRODUCT_OUT)/kernel@$(BOARD_FORCE_KERNEL_ADDRESS) $(PRODUCT_OUT)/ramdisk-recovery.img@$(BOARD_MKBOOTIMG_ARGS),ramdisk  $(BOARD_CMDLINE)@cmdline
	@echo ----- Made recovery image -------- $@
