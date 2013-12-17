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
 
# Torch
PRODUCT_PACKAGES := \
    Torch

# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

#charging animation
$(call inherit-product, device/sony/nypon/prebuilt/resources-540x960.mk)

# Inherit from nypon device
$(call inherit-product, device/sony/nypon/nypon.mk)

# Set those variables here to overwrite the inherited values.
PRODUCT_NAME := full_nypon
PRODUCT_DEVICE := nypon
PRODUCT_BRAND := Sony
PRODUCT_MANUFACTURER := Sony
PRODUCT_MODEL := Xperia P
