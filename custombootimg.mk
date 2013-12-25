LOCAL_PATH := $(call my-dir)

MKELF := device/sony/nypon/releasetools/mkelf.py
INSTALLED_BOOTIMAGE_TARGET := $(PRODUCT_OUT)/boot.img
$(INSTALLED_BOOTIMAGE_TARGET): $(PRODUCT_OUT)/kernel $(recovery_ramdisk) $(INSTALLED_RAMDISK_TARGET) $(PRODUCT_OUT)/utilities/busybox $(MKBOOTIMG) $(MINIGZIP) $(INTERNAL_BOOTIMAGE_FILES)
	$(call pretty,"Boot image: $@")
	$(hide) mkdir -p $(PRODUCT_OUT)/combinedroot/
	$(hide) cp -R $(PRODUCT_OUT)/root/* $(PRODUCT_OUT)/combinedroot/
	$(hide) cp -R $(PRODUCT_OUT)/../../../../device/sony/nypon/config/root/default.prop $(PRODUCT_OUT)/combinedroot/
	$(hide) sed -i 's/18D1/0FCE/g' $(PRODUCT_OUT)/recovery/root/init.rc
	$(hide) sed -i 's/D001/617E/g' $(PRODUCT_OUT)/recovery/root/init.rc
	$(hide) $(MKBOOTFS) $(PRODUCT_OUT)/recovery/root > $(PRODUCT_OUT)/recoveryforkexec.cpio
	$(hide) cat $(PRODUCT_OUT)/recoveryforkexec.cpio | gzip > $(PRODUCT_OUT)/recoveryforkexec.fs
	$(hide) cp -R $(PRODUCT_OUT)/recoveryforkexec.fs $(PRODUCT_OUT)/combinedroot/sbin/recovery_ramdisk.gz
	$(hide) cp -R $(PRODUCT_OUT)/recovery/root/sbin/* $(PRODUCT_OUT)/combinedroot/sbin/
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
