android_device_sony_nypon
=========================


Copyright (C) 2013 The Unofficial CyanogenMod Project For Sony Xperia P [Percy Gahlot]

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


LINK FOR BUILDS:- https://sites.google.com/site/androdevlinux/xperia-p/xperia-p-roms/cm10-download

Getting Started :

    mkdir cm-10.2
    cd cm-10.2
    repo init -u git://github.com/Legacy-Xperia/manifest.git -b cm-10.2
    repo sync -j16



Patch android source code :

    patch -p1 < device/sony/nypon/patches/external_bluetooth_bluedroid.patch
    patch -p1 < device/sony/nypon/patches/framework_av.patch
    patch -p1 < device/sony/nypon/patches/framework_native.patch
    patch -p1 < device/sony/nypon/patches/hardware_libhardware.patch
    patch -p1 < device/sony/nypon/patches/hardware_libhardware_legacy.patch
    patch -p1 < device/sony/nypon/patches/system_netd.patch
    patch -p1 < device/sony/nypon/patches/system_core.patch

Our step is optional!!! Use only if you going to sync CM source code daily, than simple revert each patch before you sync CM source code :

    patch -p1 -R < device/sony/nypon/patches/external_bluetooth_bluedroid.patch
    patch -p1 -R < device/sony/nypon/patches/framework_av.patch
    patch -p1 -R < device/sony/nypon/patches/framework_native.patch
    patch -p1 -R < device/sony/nypon/patches/hardware_libhardware.patch
    patch -p1 -R < device/sony/nypon/patches/hardware_libhardware_legacy.patch
    patch -p1 -R < device/sony/nypon/patches/system_netd.patch
    patch -p1 -R < device/sony/nypon/patches/system_core.patch
    repo forall -p -c 'git checkout -f'
    repo sync
    patch -p1 < device/sony/nypon/patches/external_bluetooth_bluedroid.patch
    patch -p1 < device/sony/nypon/patches/framework_av.patch
    patch -p1 < device/sony/nypon/patches/framework_native.patch
    patch -p1 < device/sony/nypon/patches/hardware_libhardware.patch
    patch -p1 < device/sony/nypon/patches/hardware_libhardware_legacy.patch
    patch -p1 < device/sony/nypon/patches/system_netd.patch
    patch -p1 < device/sony/nypon/patches/system_core.patch

Download CM prebuilts [only Needed once] :

   cd vendor/cm
   ./get-prebuilts
   cd ../..

You are ready to build :

    . build/envsetup.sh
    brunch cm_nypon-userdebug

Credits:- Munjeni , Joise87 , 95A31 , Gava97 ,Andrewasth ,areshinakya ,FXP Team , CyanogenMod , Sony , Devswift1 and Paul.L for early help in cm-10.1 and Users ;)


ENJOY! 

