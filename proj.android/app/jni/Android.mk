LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := MyGame

LOCAL_MODULE_FILENAME := libMyGame


#
#mplayball/com_aceproject_android_C2sCocos2dxActivity.cpp \
#mplayball/com_aceproject_android_AceCocos2dxActivity.cpp \
#mplayball/main.cpp \


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
LOCAL_SRC_FILES += hellocpp/main.cpp \
                   mplayball/com_aceproject_android_C2sCocos2dxActivity.cpp \
                   mplayball/com_aceproject_android_AceCocos2dxActivity.cpp


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

LOCAL_C_INCLUDES := $(dir $(HEADER_LIST))
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../libs/boost


# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static sqlite3_static


# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END


include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../../../../cocos2dx-3x/cocos2d)
$(call import-add-path, $(LOCAL_PATH)/../../../libs/scripting)

$(call import-module, cocos)
$(call import-module,external/sqlite3) 

# _COCOS_LIB_IMPORT_ANDROID_BEGIN



# _COCOS_LIB_IMPORT_ANDROID_END
