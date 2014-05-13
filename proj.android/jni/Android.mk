LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/BulletLayer.cpp \
                   ../../Classes/ControlLayer.cpp \
                   ../../Classes/Enemy.cpp \
                   ../../Classes/EnemyLayer.cpp \
                   ../../Classes/GameLayer.cpp \
                   ../../Classes/GameOverLayer.cpp \
                   ../../Classes/GameOverScene.cpp \
                   ../../Classes/GameScene.cpp \
                   ../../Classes/MutiBulletsLayer.cpp \
                   ../../Classes/PlaneLayer.cpp \
                   ../../Classes/UFOLayer.cpp \
                   ../../Classes/WelcomeLayer.cpp \
                   ../../Classes/WelcomeScene.cpp 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,2d)
$(call import-module,audio/android)
$(call import-module,Box2D)
