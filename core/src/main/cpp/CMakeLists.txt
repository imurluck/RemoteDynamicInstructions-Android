# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)
project("core")

include_directories(include)

#third part
include_directories(third-part/enhance)
set(ENHANCE_SOURCES third-part/enhance/EnhanceDlsym.cpp)

add_library(rdi SHARED
        art_env.cpp
        rdi_jni.cpp
        rdi_base.cpp
        rdi_agent.cpp
        jvmti_registry.cpp
        jvmti_utils.cpp
        ${ENHANCE_SOURCES})

target_link_libraries(
        rdi
        PUBLIC log)

