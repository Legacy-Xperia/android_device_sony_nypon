LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
genl-ctrl-list.c \
nf-ct-list.c \
nf-log.c \
nf-monitor.c \
nf-queue.c \
nl-addr-add.c \
nl-addr-delete.c \
nl-addr-list.c \
nl-cls-add.c \
nl-cls-delete.c \
nl-cls-list.c \
nl-fib-lookup.c \
nl-link-ifindex2name.c \
nl-link-list.c \
nl-link-name2ifindex.c \
nl-link-set.c \
nl-link-stats.c \
nl-list-caches.c \
nl-list-sockets.c \
nl-monitor.c \
nl-neigh-add.c \
nl-neigh-delete.c \
nl-neigh-list.c \
nl-neightbl-list.c \
nl-qdisc-delete.c \
nl-qdisc-list.c \
nl-route-add.c \
nl-route-delete.c \
nl-route-get.c \
nl-route-list.c \
nl-rule-list.c \
nl-tctree-list.c \
nl-util-addr.c \
lib/link.c \
lib/neigh.c \
lib/addr.c \
lib/ct.c \
lib/qdisc.c \
lib/route.c \
lib/rule \
lib/utils.c \
cls/basic.c \
cls/cgroup.c \
cls/utils.c

LOCAL_C_INCLUDES += \
	external/netlink/include

#LOCAL_SHARED_LIBRARIES += libnl

LOCAL_MODULE:= libnlsrc

include $(BUILD_SHARED_LIBRARY)
