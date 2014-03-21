#!/sbin/busybox sh
set +x
_PATH="$PATH"
export PATH=/sbin

busybox cd /
busybox date >>boot.txt
exec >>boot.txt 2>&1
busybox rm /init

# include device specific vars
source /sbin/bootrec-device

# create directories
busybox mkdir -m 755 -p /dev/block
busybox mkdir -m 755 -p /dev/input
busybox mkdir -m 555 -p /proc
busybox mkdir -m 755 -p /sys

# create device nodes
busybox mknod -m 600 /dev/block/mmcblk0 b 179 0
busybox mknod -m 600 ${BOOTREC_EVENT_NODE}
busybox mknod -m 666 /dev/null c 1 3

# mount filesystems
busybox mount -t proc proc /proc
busybox mount -t sysfs sysfs /sys

# trigger LED
busybox chmod 755 /sbin/bootrec-led
./sbin/bootrec-led

# keycheck
busybox cat ${BOOTREC_EVENT} > /dev/keycheck&
busybox sleep 3

# android ramdisk
load_image=/sbin/ramdisk.gz

# boot decision
if [ -s /dev/keycheck ] || busybox grep -q warmboot=0x5502 /proc/cmdline ; then
	busybox echo 'RECOVERY BOOT' >>boot.txt
	busybox mount -o remount,rw /
	# recovery ramdisk
	load_image=/sbin/ramdisk-recovery.gz
else
	busybox echo 'ANDROID BOOT' >>boot.txt
fi

# kill the keycheck process
busybox pkill -f "busybox cat ${BOOTREC_EVENT}"

busybox umount /proc
busybox umount /sys

busybox rm -fr /dev/*

# unpack the ramdisk image
# -u should be used to replace the static busybox with dynamically linked one.
busybox gunzip < ${load_image} | busybox cpio -uid

export PATH="${_PATH}"
exec /init
