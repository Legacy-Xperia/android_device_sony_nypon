android_device_sony_nypon
=========================

CM-11.0 device tree for sony xperia p


Patch android source code :

    patch -p1 < device/sony/nypon/patches/framework_av.patch
    patch -p1 < device/sony/nypon/patches/framework_native.patch
    patch -p1 < device/sony/nypon/patches/frameworks_base.patch
    patch -p1 < device/sony/nypon/patches/hardware_libhardware.patch
    patch -p1 < device/sony/nypon/patches/hardware_libhardware_legacy.patch
    patch -p1 < device/sony/nypon/patches/system_core.patch
    patch -p1 < device/sony/nypon/patches/system_netd.patch
    patch -p1 < device/sony/nypon/patches/bionic.patch
    patch -p1 < device/sony/nypon/patches/bootable_recovery.patch

Our step is optional!!! Use only if you going to sync CM 11 source code daily, than simple revert each patch before you sync CM 11 source code :

    patch -p1 -R < device/sony/nypon/patches/framework_av.patch
    patch -p1 -R < device/sony/nypon/patches/framework_native.patch
    patch -p1 -R < device/sony/nypon/patches/frameworks_base.patch
    patch -p1 -R < device/sony/nypon/patches/hardware_libhardware.patch
    patch -p1 -R < device/sony/nypon/patches/hardware_libhardware_legacy.patch
    patch -p1 -R < device/sony/nypon/patches/system_core.patch
    patch -p1 -R < device/sony/nypon/patches/system_netd.patch
    patch -p1 -R < device/sony/nypon/patches/bionic.patch
    patch -p1 -R < device/sony/nypon/patches/bootable_recovery.patch
    repo forall -p -c 'git checkounypont -f'
    repo sync
    patch -p1 < device/sony/nypon/patches/framework_av.patch
    patch -p1 < device/sony/nypon/patches/framework_native.patch
    patch -p1 < device/sony/nypon/patches/frameworks_base.patch
    patch -p1 < device/sony/nypon/patches/hardware_libhardware.patch
    patch -p1 < device/sony/nypon/patches/hardware_libhardware_legacy.patch
    patch -p1 < device/sony/nypon/patches/system_core.patch
    patch -p1 < device/sony/nypon/patches/system_netd.patch
    patch -p1 < device/sony/nypon/patches/bionic.patch
    patch -p1 < device/sony/nypon/patches/bootable_recovery.patch
