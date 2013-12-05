android_device_sony_nypon
=========================

Unoficial CM-11.0 for Sony Xperia P

Getting Started :

    mkdir cm-11.0
    cd cm-11.0
    repo init -u git://github.com/Legacy-Xperia/manifest.git -b cm-11.0
    repo sync -j16
    
Patch android source code :

    patch -p1 < device/sony/nypon/patches/framework_av.patch
    patch -p1 < device/sony/nypon/patches/framework_base.patch
    patch -p1 < device/sony/nypon/patches/bootable_recovery.patch
    patch -p1 < device/sony/nypon/patches/bionic.patch

Our step is optional!!! Use only if you going to sync CM 11 source code daily, than simple revert each patch before you sync CM 11 source code :

    patch -p1 -R < device/sony/nypon/patches/framework_av.patch
    patch -p1 -R < device/sony/nypon/patches/framework_base.patch
    patch -p1 -R < device/sony/nypon/patches/bootable_recovery.patch
    patch -p1 -R < device/sony/nypon/patches/bionic.patch
    repo forall -p -c 'git checkout -f'
    repo sync
    patch -p1 < device/sony/nypon/patches/framework_av.patch
    patch -p1 < device/sony/nypon/patches/framework_base.patch
    patch -p1 < device/sony/nypon/patches/bootable_recovery.patch
    patch -p1 < device/sony/nypon/patches/bionic.patch

Download CM prebuilts :

   cd vendor/cm
   ./get-prebuilts
   cd ../..

You are ready to build :

    . build/envsetup.sh
    brunch cm_nypon-userdebug


ENJOY! 
