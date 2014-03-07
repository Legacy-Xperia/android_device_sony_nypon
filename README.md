android_device_sony_nypon
=========================


Copyright (C) 2014 The Unofficial CyanogenMod Project For Sony Xperia P [Percy Gahlot]

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


LINK FOR BUILDS:- https://sites.google.com/site/androdevlinux/xperia-p/xperia-p-roms/cm11-download

Getting Started :

    mkdir cm-11.0
    cd cm-11.0
    repo init -u git://github.com/CyanogenMod/android.git -b cm-11.0
    repo sync -j16


Make a local_manifests dir in .repo folder and add a default.xml manifest file containing below local manifest , then repo sync .

Local manifest for NYPON http://pastebin.com/ASSLkXQ7


Download CM prebuilts [Needed Only once] :

   cd vendor/cm && ./get-prebuilts && cd ../..
   
You are ready to build :

    . build/envsetup.sh
    brunch cm_nypon-userdebug

Credits:- Munjeni , Joise87 , 95A31 , mike(semc-2011 DEV) , Gava97 ,Andrewasth ,areshinakya ,FXP Team , CyanogenMod , Sony , Devswift1 and Paul.L for early help in cm-10.1 and Users ;)


ENJOY! 


