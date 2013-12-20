#
# Copyright (C) 2013 The Unofficial CyanogenMod Project For Sony Xperia P [Percy Gahlot]
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

USE_CAMERA_STUB := true

# inherit from the proprietary version
-include vendor/sony/nypon/BoardConfigVendor.mk

TARGET_SPECIFIC_HEADER_PATH := device/sony/nypon/include
TARGET_SPECIFIC_HEADER_PATH += device/sony/nypon/hardware

TARGET_NO_BOOTLOADER := true
TARGET_NO_RADIOIMAGE := true
BOARD_HAS_NO_MISC_PARTITION := true

# Platform
TARGET_BOARD_PLATFORM := montblanc
TARGET_BOOTLOADER_BOARD_NAME := montblanc
BOARD_USES_STE_HARDWARE := true

# Architecture
TARGET_ARCH := arm
TARGET_CPU_VARIANT := cortex-a9
TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_ARCH_VARIANT_CPU := cortex-a9
TARGET_ARCH_VARIANT := armv7-a-neon
ARCH_ARM_HAVE_TLS_REGISTER := true
TARGET_CPU_SMP := true

# Flags
TARGET_GLOBAL_CFLAGS += -mfpu=neon -mfloat-abi=softfp
TARGET_GLOBAL_CPPFLAGS += -mfpu=neon -mfloat-abi=softfp

# Bluetooth
BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_STE := true
COMMON_GLOBAL_CFLAGS += -DSTE_BT

# Audio
BOARD_USES_GENERIC_AUDIO := false
BOARD_USES_ALSA_AUDIO := true
COMMON_GLOBAL_CFLAGS += -DMR1_AUDIO_BLOB
COMMON_GLOBAL_CFLAGS += -DMR0_AUDIO_BLOB
BOARD_USES_LIBMEDIA_WITH_AUDIOPARAMETER := true
COMMON_GLOBAL_CFLAGS += -DSTE_AUDIO
BOARD_HAVE_PRE_KITKAT_AUDIO_BLOB := true 

# WIFI
BOARD_WLAN_DEVICE := cw1200
WPA_SUPPLICANT_VERSION := VER_0_8_X
CONFIG_DRIVER_NL80211 := true
BOARD_WPA_SUPPLICANT_DRIVER := NL80211
BOARD_HOSTAPD_DRIVER := NL80211
USES_TI_MAC80211 := true
COMMON_GLOBAL_CFLAGS += -DUSES_TI_MAC80211 -DCONFIG_DRIVER_NL80211
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := private_lib_nl80211_cmd
BOARD_HOSTAPD_PRIVATE_LIB := private_lib_nl80211_cmd

# Graphics
USE_OPENGL_RENDERER := true
BOARD_EGL_CFG := device/sony/nypon/config/egl.cfg
COMMON_GLOBAL_CFLAGS += -DSTE_HARDWARE
BOARD_HAVE_PIXEL_FORMAT_INFO := true
TARGET_RUNNING_WITHOUT_SYNC_FRAMEWORK := true

#Backward compatibility for hals
COMMON_GLOBAL_CFLAGS += -DNEEDS_VECTORIMPL_SYMBOLS

#Jb camera
COMMON_GLOBAL_CFLAGS += -DICS_CAMERA_BLOB

# surfaceflinger support for Xperia Sola,Go,P,U
BOARD_EGL_NEEDS_LEGACY_FB := true
COMMON_GLOBAL_CFLAGS += -DEGL_NEEDS_FNW

# Fm Radio
#BOARD_USES_STE_FM := true
#COMMON_GLOBAL_CFLAGS += -DSTE_FM

# Lights
TARGET_PROVIDES_LIBLIGHTS := true

# ril implementation
BOARD_RIL_CLASS := ../../../device/sony/nypon/telephony-common/

# Custom boot
TARGET_PROVIDES_INIT_RC := true
TARGET_RECOVERY_PRE_COMMAND := "touch /cache/recovery/boot;sync;"
BOARD_CUSTOM_BOOTIMG_MK := device/sony/nypon/custombootimg.mk
TARGET_RELEASETOOL_OTA_FROM_TARGET_SCRIPT := device/sony/nypon/releasetools/semc_ota_from_target_files
BOARD_CUSTOM_RECOVERY_KEYMAPPING := ../../device/sony/nypon/recovery/recovery-keys.c
BOARD_USE_CUSTOM_RECOVERY_FONT := \"lucidaconsole_10x18.h\"
BOARD_USES_MMCUTILS := true

#CWM Recovery TOUCH (All Rights Reserved By munjeni)
#RECOVERY_NAME := CWM-NYPON
#COMMON_GLOBAL_CFLAGS += -DXPERIA_CWM_TOUCH

#CWM recovery fix
TARGET_RECOVERY_FSTAB = device/sony/nypon/config/root/fstab.st-ericsson
RECOVERY_FSTAB_VERSION := 2

#Vold
TARGET_USE_CUSTOM_LUN_FILE_PATH := "/sys/devices/platform/musb-ux500.0/musb-hdrc/gadget/lun%d/file"

#HDMI
COMMON_GLOBAL_CFLAGS += -DSTE_HDMI
 
# Kernel information
TARGET_KERNEL_CONFIG := xperiap_defconfig
TARGET_KERNEL_SOURCE := kernel/sony/u8500
TARGET_KERNEL_CUSTOM_TOOLCHAIN := arm-eabi-4.4.3
BOARD_KERNEL_BASE := 0x40200000
BOARD_RECOVERY_BASE := 0x40200000
BOARD_KERNEL_PAGESIZE := 2048
BOARD_MKBOOTIMG_ARGS := --ramdisk_offset 0x01000000

# Partition information
BOARD_VOLD_MAX_PARTITIONS := 16

BOARD_BOOTIMAGE_PARTITION_SIZE := 0x01400000
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 0x01400000
BOARD_CMDLINE := device/sony/nypon/config/cmdline.txt

BOARD_SYSTEMIMAGE_PARTITION_SIZE := 1056964608
BOARD_USERDATA_PARTITION_SIZE := 2147483648
BOARD_FLASH_BLOCK_SIZE := 131072
BOARD_FORCE_KERNEL_ADDRESS  := 0x00008000
BOARD_MKBOOTIMG_ARGS := 0x01000000

BOARD_HAS_NO_SELECT_BUTTON := true
TARGET_USERIMAGES_USE_EXT4 := true

BOARD_SDCARD_INTERNAL_DEVICE := /dev/block/mmcblk0p14

TARGET_OTA_ASSERT_DEVICE := LT22i,LT22a,nypon
