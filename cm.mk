$(call inherit-product, device/sony/nypon/full_nypon.mk)

# Inherit CM common GSM stuff.
$(call inherit-product, vendor/cm/config/gsm.mk)

# Inherit CM common Phone stuff.
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Enhanced NFC
$(call inherit-product, vendor/cm/config/nfc_enhanced.mk)


PRODUCT_BUILD_PROP_OVERRIDES += PRODUCT_NAME=ST22i_1257-4009 BUILD_FINGERPRINT=SEMC/LT22i_1257-4009/LT22i:4.1.2/6.1.A.0.452/O_5_zw:user/release-keys PRIVATE_BUILD_DESC="LT22i-user 4.1.2 6.1.A.0.452 O_5_zw test-keys"

PRODUCT_NAME := cm_nypon
PRODUCT_DEVICE := nypon
