$(call inherit-product, device/sony/nypon/full_nypon.mk)

# Inherit CM common GSM stuff.
$(call inherit-product, vendor/unkown/config/gsm.mk)

# Inherit CM common Phone stuff.
$(call inherit-product, vendor/unkown/config/common_full_phone.mk)

# Enhanced NFC
$(call inherit-product, vendor/unkown/config/nfc_enhanced.mk)


PRODUCT_BUILD_PROP_OVERRIDES += PRODUCT_NAME=LT22i_1257-4009 BUILD_FINGERPRINT=SEMC/LT22i_1257-4009/LT22i:4.4.2/6.1.A.0.452/O_5_zw:user/release-keys PRIVATE_BUILD_DESC="LT22i-user 4.4.2 6.1.A.0.452 O_5_zw test-keys"

PRODUCT_NAME := aosp_nypon
PRODUCT_DEVICE := nypon
