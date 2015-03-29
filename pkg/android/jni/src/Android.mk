LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL2

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
					$(LOCAL_PATH)/../SDL2_image \
					$(LOCAL_PATH)/../SDL2_mixer \
					$(LOCAL_PATH)/../SDL2_ttf

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
	../../../../src/animation.cpp \
	../../../../src/enemy.cpp \
	../../../../src/engines.cpp \
	../../../../src/entity.cpp \
	../../../../src/game_engine.cpp \
	../../../../src/input_engine.cpp \
	../../../../src/main.cpp \
	../../../../src/map_engine.cpp \
	../../../../src/player.cpp \
	../../../../src/powerup.cpp \
	../../../../src/render_engine.cpp \
	../../../../src/sound_engine.cpp \
	../../../../src/text.cpp \
	../../../../src/utils.cpp

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_mixer SDL2_ttf

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
