LOCAL_PATH := $(call my-dir)

#
## libfreerdp-utils.a
#
include $(CLEAR_VARS)
MY_LIBS_PATH=freerdp-1.0-nevo-public/libs/armeabi-v7a
LOCAL_MODULE    := freerdp-utils
LOCAL_SRC_FILES := $(MY_LIBS_PATH)/libfreerdp-utils.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/freerdp-1.0-nevo-public/include
include $(PREBUILT_STATIC_LIBRARY)

#
## libfreerdp-codec.a
#
include $(CLEAR_VARS)
MY_LIBS_PATH=freerdp-1.0-nevo-public/libs/armeabi-v7a
LOCAL_MODULE    := freerdp-codec
LOCAL_SRC_FILES := $(MY_LIBS_PATH)/libfreerdp-utils.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/freerdp-1.0-nevo-public/include
include $(PREBUILT_STATIC_LIBRARY)

#
## libfreerdp-gdi.a
#
include $(CLEAR_VARS)
MY_LIBS_PATH=freerdp-1.0-nevo-public/libs/armeabi-v7a
LOCAL_MODULE    := freerdp-gdi
LOCAL_SRC_FILES := $(MY_LIBS_PATH)/libfreerdp-gdi.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/freerdp-1.0-nevo-public/include
include $(PREBUILT_STATIC_LIBRARY)

#
## libfreerdp-core.a
#
include $(CLEAR_VARS)
MY_LIBS_PATH=freerdp-1.0-nevo-public/libs/armeabi-v7a
LOCAL_MODULE    := freerdp-core
LOCAL_SRC_FILES := $(MY_LIBS_PATH)/libfreerdp-core.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/freerdp-1.0-nevo-public/include
include $(PREBUILT_STATIC_LIBRARY)

#
## libfreerdp-rail.a
#
include $(CLEAR_VARS)
MY_LIBS_PATH=freerdp-1.0-nevo-public/libs/armeabi-v7a
LOCAL_MODULE    := freerdp-rail
LOCAL_SRC_FILES := $(MY_LIBS_PATH)/libfreerdp-rail.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/freerdp-1.0-nevo-public/include
include $(PREBUILT_STATIC_LIBRARY)

#
## libfreerdp-cache.a
#
include $(CLEAR_VARS)
MY_LIBS_PATH=freerdp-1.0-nevo-public/libs/armeabi-v7a
LOCAL_MODULE    := freerdp-chche
LOCAL_SRC_FILES := $(MY_LIBS_PATH)/libfreerdp-cache.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/freerdp-1.0-nevo-public/include
include $(PREBUILT_STATIC_LIBRARY)

#
## libfreerdp-crypto.a
#
include $(CLEAR_VARS)
MY_LIBS_PATH=freerdp-1.0-nevo-public/libs/armeabi-v7a
LOCAL_MODULE    := freerdp-crypto
LOCAL_SRC_FILES := $(MY_LIBS_PATH)/libfreerdp-crypto.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/freerdp-1.0-nevo-public/include
include $(PREBUILT_STATIC_LIBRARY)

#
## libfreerdp-sspi.a
#
include $(CLEAR_VARS)
MY_LIBS_PATH=freerdp-1.0-nevo-public/libs/armeabi-v7a
LOCAL_MODULE    := freerdp-sspi
LOCAL_SRC_FILES := $(MY_LIBS_PATH)/libfreerdp-sspi.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/freerdp-1.0-nevo-public/include
include $(PREBUILT_STATIC_LIBRARY)

#
## libfreerdp-channels.a
#
include $(CLEAR_VARS)
MY_LIBS_PATH=freerdp-1.0-nevo-public/libs/armeabi-v7a
LOCAL_MODULE    := freerdp-channels
LOCAL_SRC_FILES := $(MY_LIBS_PATH)/libfreerdp-channels.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/freerdp-1.0-nevo-public/include
include $(PREBUILT_STATIC_LIBRARY)

#
## rdpsnd_alsa.a
#
include $(CLEAR_VARS)
MY_LIBS_PATH=freerdp-1.0-nevo-public/libs/armeabi-v7a
LOCAL_MODULE    := rdpsnd_alsa
LOCAL_SRC_FILES := $(MY_LIBS_PATH)/rdpsnd_alsa.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/freerdp-1.0-nevo-public/include
include $(PREBUILT_STATIC_LIBRARY)

#
## cliprdr.a
#
include $(CLEAR_VARS)
MY_LIBS_PATH=freerdp-1.0-nevo-public/libs/armeabi-v7a
LOCAL_MODULE    := cliprdr
LOCAL_SRC_FILES := $(MY_LIBS_PATH)/cliprdr.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/freerdp-1.0-nevo-public/include
include $(PREBUILT_STATIC_LIBRARY)

#
## rdpsnd.a
#
include $(CLEAR_VARS)
MY_LIBS_PATH=freerdp-1.0-nevo-public/libs/armeabi-v7a
LOCAL_MODULE    := rdpsnd
LOCAL_SRC_FILES := $(MY_LIBS_PATH)/rdpsnd.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/freerdp-1.0-nevo-public/include
include $(PREBUILT_STATIC_LIBRARY)

#
## libfreerdp-locale.a
#
include $(CLEAR_VARS)
MY_LIBS_PATH=freerdp-1.0-nevo-public/libs/armeabi-v7a
LOCAL_MODULE    := freerdp-locale
LOCAL_SRC_FILES := $(MY_LIBS_PATH)/libfreerdp-locale.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/freerdp-1.0-nevo-public/include
include $(PREBUILT_STATIC_LIBRARY)

#
## libfreerdp_android.a
#
include $(CLEAR_VARS)
MY_LIBS_PATH=freerdp-1.0-nevo-public/libs/armeabi-v7a
LOCAL_MODULE    := freerdp_android
LOCAL_SRC_FILES := $(MY_LIBS_PATH)/libfreerdp_android.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/freerdp-1.0-nevo-public/include
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/freerdp-1.0-nevo-public/client/Android
include $(PREBUILT_STATIC_LIBRARY)

#
## libfreerdp.so
#
include $(CLEAR_VARS)
LOCAL_MODULE    := freerdp
LOCAL_SRC_FILES := freerdp.c
LOCAL_ARM_MODE := arm
NDK_TOOLCHAIN_ROOT=/opt/android-toolchain
LOCAL_STATIC_LIBRARIES := freerdp-utils
LOCAL_STATIC_LIBRARIES += freerdp-locale
LOCAL_STATIC_LIBRARIES += freerdp-crypto
LOCAL_STATIC_LIBRARIES += freerdp-sspi
LOCAL_STATIC_LIBRARIES += freerdp-codec
LOCAL_STATIC_LIBRARIES += freerdp-core
LOCAL_STATIC_LIBRARIES += freerdp-cache 
LOCAL_STATIC_LIBRARIES += freerdp-gdi
LOCAL_STATIC_LIBRARIES += freerdp-rail
LOCAL_STATIC_LIBRARIES += freerdp-channels 
LOCAL_STATIC_LIBRARIES += rdpsnd_alsa
LOCAL_STATIC_LIBRARIES += cliprdr
LOCAL_STATIC_LIBRARIES += rdpsnd
LOCAL_STATIC_LIBRARIES += freerdp_android

include $(BUILD_SHARED_LIBRARY)
