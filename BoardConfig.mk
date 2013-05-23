USE_CAMERA_STUB := true

# inherit from the proprietary version
-include vendor/sony/nypon/BoardConfigVendor.mk

#inherit from the common montblanc definitions
-include device/sony/montblanc-common/BoardConfigCommon.mk

TARGET_SPECIFIC_HEADER_PATH += device/sony/nypon/include

TARGET_KERNEL_CONFIG := cm_montblanc_nypon_defconfig

COMMON_GLOBAL_CFLAGS += -DSTE_HDMI

# Partition information
BOARD_VOLD_MAX_PARTITIONS := 16

BOARD_BOOTIMAGE_PARTITION_SIZE := 0x01400000
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 0x01400000
BOARD_CMDLINE := device/sony/nypon/config/cmdline.txt

BOARD_SYSTEMIMAGE_PARTITION_SIZE := 1056964608
BOARD_USERDATA_PARTITION_SIZE := 2147483648
BOARD_FLASH_BLOCK_SIZE := 131072
BOARD_FORCE_KERNEL_ADDRESS  := 0x00008000
BOARD_FORCE_RAMDISK_ADDRESS := 0x01000000

BOARD_HAS_NO_SELECT_BUTTON := true
TARGET_USERIMAGES_USE_EXT4 := true

BOARD_SDCARD_INTERNAL_DEVICE := /dev/block/mmcblk0p14

TARGET_OTA_ASSERT_DEVICE := LT22i,LT22a,nypon
