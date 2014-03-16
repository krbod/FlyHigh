LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := hellocpp_shared

LOCAL_MODULE_FILENAME := libhellocpp

FILE_LIST := $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)

LOCAL_SRC_FILES := hellocpp/main.cpp $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../../../../external/Box2D

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static box2d_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx) \
$(call import-module,CocosDenshion/android) \
$(call import-module,extensions) \
$(call import-module,external/Box2D)
