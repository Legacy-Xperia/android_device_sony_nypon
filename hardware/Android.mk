# Copyright (C) 2012 The Android Open Source Project
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

ifeq ($(BOARD_USES_STE_HARDWARE), true)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_PRELINK_MODULE := false
LOCAL_SRC_FILES := display/b2r2lib/src/blt_b2r2.c
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/display/b2r2lib/include
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_MODULE := libblt_hw
LOCAL_MODULE_TAGS := optional
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libomxil-bellagio
LOCAL_MODULE_TAGS := optional
SRC := media/libomxil-bellagio/src
BASE := media/libomxil-bellagio/src/base
LOCAL_SRC_FILES := \
   $(SRC)/st_static_component_loader.c \
   $(SRC)/omxcore.c \
   $(SRC)/omx_create_loaders_linux.c \
   $(SRC)/tsemaphore.c \
   $(SRC)/queue.c \
   $(SRC)/utils.c \
   $(SRC)/common.c \
   $(SRC)/content_pipe_inet.c \
   $(SRC)/content_pipe_file.c \
   $(SRC)/omx_reference_resource_manager.c \
   $(SRC)/getline.c \
   $(BASE)/omx_base_component.c \
   $(BASE)/omx_base_port.c \
   $(BASE)/omx_base_filter.c \
   $(BASE)/omx_base_sink.c \
   $(BASE)/omx_base_source.c \
   $(BASE)/omx_base_audio_port.c \
   $(BASE)/omx_base_video_port.c \
   $(BASE)/omx_base_image_port.c \
   $(BASE)/omx_base_clock_port.c \
   $(BASE)/OMXComponentRMExt.c

LOCAL_C_INCLUDES := \
   $(LOCAL_PATH)/media/libomxil-bellagio/include \
   $(LOCAL_PATH)/$(BASE) \
   $(LOCAL_PATH)/$(SRC)

LOCAL_CFLAGS += -DRELEASE -D__RELEASE
LOCAL_SHARED_LIBRARIES := \
   libdl \
   liblog

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
SRC := liblights
LOCAL_SRC_FILES := $(SRC)/lights.c
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_MODULE := lights.$(TARGET_BOARD_PLATFORM)
LOCAL_MODULE_TAGS := optional
include $(BUILD_SHARED_LIBRARY)

LOG_TO_ANDROID_LOGCAT := true
include $(CLEAR_VARS)
SRC := dbus
LOCAL_SRC_FILES:= \
   $(SRC)/dbus-address.c \
   $(SRC)/dbus-auth.c \
   $(SRC)/dbus-bus.c \
   $(SRC)/dbus-connection.c \
   $(SRC)/dbus-credentials.c \
   $(SRC)/dbus-dataslot.c \
   $(SRC)/dbus-errors.c \
   $(SRC)/dbus-file.c \
   $(SRC)/dbus-file-unix.c \
   $(SRC)/dbus-hash.c \
   $(SRC)/dbus-internals.c \
   $(SRC)/dbus-keyring.c \
   $(SRC)/dbus-list.c \
   $(SRC)/dbus-mainloop.c \
   $(SRC)/dbus-marshal-basic.c \
   $(SRC)/dbus-marshal-byteswap.c \
   $(SRC)/dbus-marshal-header.c \
   $(SRC)/dbus-marshal-recursive.c \
   $(SRC)/dbus-marshal-validate.c \
   $(SRC)/dbus-mempool.c \
   $(SRC)/dbus-memory.c \
   $(SRC)/dbus-message.c \
   $(SRC)/dbus-nonce.c \
   $(SRC)/dbus-pending-call.c \
   $(SRC)/dbus-pipe.c \
   $(SRC)/dbus-pipe-unix.c \
   $(SRC)/dbus-resources.c \
   $(SRC)/dbus-server.c \
   $(SRC)/dbus-server-socket.c \
   $(SRC)/dbus-server-unix.c \
   $(SRC)/dbus-sha.c \
   $(SRC)/dbus-shell.c \
   $(SRC)/dbus-signature.c \
   $(SRC)/dbus-spawn.c \
   $(SRC)/dbus-string.c \
   $(SRC)/dbus-string-util.c \
   $(SRC)/dbus-sysdeps.c \
   $(SRC)/dbus-sysdeps-pthread.c \
   $(SRC)/dbus-sysdeps-unix.c \
   $(SRC)/dbus-sysdeps-util-unix.c \
   $(SRC)/dbus-timeout.c \
   $(SRC)/dbus-threads.c \
   $(SRC)/dbus-transport.c \
   $(SRC)/dbus-transport-socket.c \
   $(SRC)/dbus-transport-unix.c \
   $(SRC)/dbus-object-tree.c \
   $(SRC)/dbus-userdb.c \
   $(SRC)/dbus-userdb-util.c \
   $(SRC)/dbus-watch.c \
   $(SRC)/sd-daemon.c

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/dbus
LOCAL_MODULE:=libdbus
LOCAL_CFLAGS+= \
	-DDBUS_COMPILATION \
	-DANDROID_MANAGED_SOCKET \
	-DANDROID_ATOMIC \
	-DDBUS_MACHINE_UUID_FILE=\"/etc/machine-id\" \
	-DDBUS_SYSTEM_CONFIG_FILE=\"/system/etc/dbus.conf\" \
	-DDBUS_SESSION_CONFIG_FILE=\"/system/etc/session.conf\"
ifeq ($(LOG_TO_ANDROID_LOGCAT),true)
LOCAL_CFLAGS+= -DDBUS_ANDROID_LOG
LOCAL_SHARED_LIBRARIES+= libcutils
endif
include $(BUILD_SHARED_LIBRARY)

#
# Copyright (C) 2012 Google, inc.
#
# $Id: Android.mk,v 2.6 2009-05-07 18:25:15 hharte Exp $
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
# SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
# OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
# CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
#
# lib_net_iface_cmd
#
include $(CLEAR_VARS)
SRC := wifi/bcmdhd_net_iface
LOCAL_SRC_FILES := \
    ${SRC}/bcmdhd_net_iface.c
LOCAL_MODULE := libnetcmdiface
LOCAL_CFLAGS := -mabi=aapcs-linux
LOCAL_MODULE_TAGS := optional
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
include $(BUILD_SHARED_LIBRARY)

# iw
#
include $(CLEAR_VARS)
SRC := wifi/iw_util
NO_PKG_CONFIG=y
LOCAL_SRC_FILES := \
    ${SRC}/bitrate.c \
    ${SRC}/coalesce.c \
    ${SRC}/connect.c \
    ${SRC}/cqm.c \
    ${SRC}/event.c \
    ${SRC}/genl.c \
    ${SRC}/hwsim.c \
    ${SRC}/ibss.c \
    ${SRC}/info.c \
    ${SRC}/interface.c \
    ${SRC}/iw.c \
    ${SRC}/link.c \
    ${SRC}/mesh.c \
    ${SRC}/mpath.c \
    ${SRC}/offch.c \
    ${SRC}/p2p.c \
    ${SRC}/phy.c \
    ${SRC}/ps.c \
    ${SRC}/reason.c \
    ${SRC}/reg.c \
    ${SRC}/roc.c \
    ${SRC}/scan.c \
    ${SRC}/sections.c \
    ${SRC}/station.c \
    ${SRC}/status.c \
    ${SRC}/survey.c \
    ${SRC}/util.c \
    ${SRC}/wowlan.c \
    ${SRC}/version.c \
    ${SRC}/android-nl.c

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/wifi/iw_util \
	external/libnl-headers

LOCAL_CFLAGS := -O2 -g -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs
LOCAL_CFLAGS += -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -DCONFIG_LIBNL20
LOCAL_LDFLAGS := -Wl,--no-gc-sections
LOCAL_MODULE_PATH := $(TARGET_OUT_EXECUTABLES)
LOCAL_MODULE_TAGS := optional
LOCAL_STATIC_LIBRARIES := libnl_2 libc libstdc++ libm
LOCAL_FORCE_STATIC_EXECUTABLE := true
LOCAL_MODULE:=iw
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
SRC := wifi/hostapd_patcher
LOCAL_SRC_FILES := $(SRC)/hostapd_patcher.c
LOCAL_CFLAGS := -O2 -g -Wall -Wundef -Werror-implicit-function-declaration
LOCAL_MODULE_PATH := $(TARGET_OUT_EXECUTABLES)
LOCAL_MODULE_TAGS := optional
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_STATIC_LIBRARIES := libcrecovery
LOCAL_MODULE := hostapdpatcher
include $(BUILD_EXECUTABLE)

endif # BOARD_USES_STE_HARDWARE
