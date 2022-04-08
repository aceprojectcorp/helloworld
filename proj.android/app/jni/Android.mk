LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../../cocos2dx-3x/cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../../cocos2dx-3x/cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../../cocos2dx-3x/cocos2d/external/sqlite3/include)
$(call import-add-path,$(LOCAL_PATH)/../../../../cocos2dx-3x/cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../../cocos2dx-3x/cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

FILE_LIST := $(wildcard $(LOCAL_PATH)/../../../Classes/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/**/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/**/**/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/**/**/**/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/**/**/**/**/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/**/**/**/**/**/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/**/**/**/**/**/**/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/**/**/**/**/**/**/**/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/**/**/**/**/**/**/**/**/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/**/**/**/**/**/**/**/**/**/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/**/**/**/**/**/**/**/**/**/**/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/**/**/**/**/**/**/**/**/**/**/**/*.cpp)


LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(LOCAL_PATH)/hellocpp/main.cpp \
                   $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
                   $(LOCAL_PATH)/../../../Classes/HelloWorldScene.cpp


HEADER_LIST := $(wildcard $(LOCAL_PATH)/../../../Classes/*.h)
HEADER_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/**/*.h)
HEADER_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/**/**/*.h)
HEADER_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/**/**/**/*.h)
HEADER_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/**/**/**/**/*.h)
HEADER_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/**/**/**/**/**/*.h)
HEADER_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/**/**/**/**/**/**/*.h)
HEADER_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/**/**/**/**/**/**/**/*.h)
HEADER_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/**/**/**/**/**/**/**/**/*.h)
HEADER_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/**/**/**/**/**/**/**/**/**/*.h)
HEADER_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/**/**/**/**/**/**/**/**/**/**/*.h)
HEADER_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/**/**/**/**/**/**/**/**/**/**/**/*.h)


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../../../../cocos2dx-3x/cocos2d)

$(call import-module, cocos)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
