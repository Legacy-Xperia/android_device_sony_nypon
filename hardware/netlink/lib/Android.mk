LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
cache_mngr.c \
nl.c \
cache.c \
error.c \
data.c \
addr.c \
socket.c \
msg.c \
attr.c \
object.c \
doc.c \
cache_mngt.c \
utils.c \
handlers.c \
fib_lookup/lookup.c \
fib_lookup/request.c \
genl/ctrl.c \
genl/mngt.c \
genl/family.c \
genl/genl.c \
netfilter/log_obj.c \
netfilter/log.c \
netfilter/queue.c \
netfilter/log_msg_obj.c \
netfilter/queue_obj.c \
netfilter/netfilter.c \
netfilter/log_msg.c \
netfilter/queue_msg.c \
netfilter/nfnl.c \
netfilter/queue_msg_obj.c \
netfilter/ct.c \
netfilter/ct_obj.c \
route/tc.c \
route/class_api.c \
route/qdisc_obj.c \
route/nexthop.c \
route/route_utils.c \
route/cls_api.c \
route/cls.c \
route/qdisc_api.c \
route/link/vlan.c \
route/link/api.c \
route/link.c \
route/sch/dsmark.c \
route/sch/red.c \
route/sch/netem.c \
route/sch/cbq.c \
route/sch/blackhole.c \
route/sch/sfq.c \
route/sch/tbf.c \
route/sch/htb.c \
route/sch/prio.c \
route/sch/fifo.c \
route/addr.c \
route/neightbl.c \
route/rtnl.c \
route/route.c \
route/class.c \
route/rule.c \
route/class_obj.c \
route/qdisc.c \
route/route_obj.c \
route/cls/u32.c \
route/cls/cgroup.c \
route/cls/fw.c \
route/cls/police.c \
route/cls/basic.c \
route/cls/ematch.c \
route/neigh.c \
route/cls_obj.c


# We need to include out own set of
# linux header files before the bionic ones,
# so we turn off the default compiler flags
# and add them ourselves after our own
# include directory.

LOCAL_NO_DEFAULT_COMPILER_FLAGS := true

LOCAL_C_INCLUDES := \
	external/netlink/include \
	$(TARGET_PROJECT_INCLUDES) \
	$(TARGET_C_INCLUDES)

LOCAL_CFLAGS := \
	$(TARGET_GLOBAL_CFLAGS)

ifeq ($(TARGET_ARCH),arm)
	LOCAL_CFLAGS += $(normal_objects_cflags)
endif



LOCAL_PRELINK_MODULE := false
LOCAL_SHARED_LIBRARIES := libc libcutils

LOCAL_MODULE := libnl
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)

