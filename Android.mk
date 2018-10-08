
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := qpakman

LOCAL_CFLAGS :=  -DUNIX -Dprintf=LOGI

LOCAL_C_INCLUDES := $(TOP_DIR)/MobileTouchControls/libpng

LOCAL_SRC_FILES = 	arc_spec.cc u_assert.cc u_file.cc u_util.cc im_color.cc im_image.cc \
                  	 im_gen.cc im_mip.cc im_png.cc im_tex.cc archive.cc pakfile.cc main.cc \
                  	 jni.cpp

LOCAL_LDLIBS :=  -ldl -llog -lz

LOCAL_STATIC_LIBRARIES := libpng

include $(BUILD_SHARED_LIBRARY)






