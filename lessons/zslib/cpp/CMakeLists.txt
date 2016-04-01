project(testOpus)
set(CMAKE_OSX_ARCHITECTURES "i386")
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_library(opus_encode SHARED pdopus.c stream.cpp resample.cpp encoder.cpp)
add_library(udp_send SHARED udp_send~.c)
add_library(udp_send_broadcast SHARED udp_send_broadcast~.c stream.cpp)

add_executable(udp_receieve udp_receive.cpp)

if (APPLE)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined suppress -flat_namespace")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -undefined suppress -flat_namespace")
    SET_TARGET_PROPERTIES(opus_encode PROPERTIES SUFFIX .pd_darwin)
    SET_TARGET_PROPERTIES(opus_encode PROPERTIES PREFIX "")

    SET_TARGET_PROPERTIES(udp_send PROPERTIES SUFFIX ~.pd_darwin)
    SET_TARGET_PROPERTIES(udp_send PROPERTIES PREFIX "")
    SET_TARGET_PROPERTIES(udp_send_broadcast PROPERTIES SUFFIX ~.pd_darwin)
    SET_TARGET_PROPERTIES(udp_send_broadcast PROPERTIES PREFIX "")
endif()

if(UNIX AND NOT APPLE)
    SET_TARGET_PROPERTIES(opus_encode PROPERTIES SUFFIX .pd_linux)
    SET_TARGET_PROPERTIES(opus_encode PROPERTIES PREFIX "")

    SET_TARGET_PROPERTIES(udp_send PROPERTIES SUFFIX ~.pd_linux)
    SET_TARGET_PROPERTIES(udp_send PROPERTIES PREFIX "")
    SET_TARGET_PROPERTIES(udp_send_broadcast PROPERTIES SUFFIX ~.pd_linux)
    SET_TARGET_PROPERTIES(udp_send_broadcast PROPERTIES PREFIX "")
endif()


if (APPLE)
    option(FAT_BINARY "Build a fat binary (32bit and 64bit)" OFF)
endif(APPLE)

find_package(Boost 1.40 COMPONENTS system thread REQUIRED)
if(Boost_FOUND)
    target_link_libraries(opus_encode ${Boost_LIBRARIES})
    target_link_libraries(udp_send_broadcast ${Boost_LIBRARIES})
    target_link_libraries(udp_receieve ${Boost_LIBRARIES})
endif()

find_path(OPUS_INCLUDE NAME opus/opus.h PATHS /usr/local/include)
if(OPUS_INCLUDE)
    message(STATUS "opus.h path: ${OPUS_INCLUDE}")
    include_directories(${OPUS_INCLUDE})
    find_library(OPUS_LIBRARY opus)
    if(OPUS_LIBRARY)
        message(STATUS "opus lib: ${OPUS_LIBRARY}")
        target_link_libraries(opus_encode ${OPUS_LIBRARY})
    else()
        message(FATAL_ERROR "opus lib not found")
    endif()
else()
   message(FATAL_ERROR "opus.h not found")
endif()

find_path(SR_INCLUDE NAME samplerate.h PATHS /usr/local/include)
if(SR_INCLUDE)
    message(STATUS "samplerate.h path: ${SR_INCLUDE}")
    include_directories(${SR_INCLUDE})
    find_library(SR_LIBRARY samplerate)
    if(SR_LIBRARY)
        message(STATUS "samplerate lib: ${SR_LIBRARY}")
        target_link_libraries(opus_encode ${SR_LIBRARY})
    else()
        message(FATAL_ERROR "samplerate lib not found")
    endif()
else()
   message(FATAL_ERROR "samplerate.h not found")
endif()


find_path(PD_INCLUDE NAMES m_pd.h
    PATHS "/usr/local/include"
          "/usr/include"
          "/usr/include/pdextended"
          "/Applications/Pd-extended.app/Contents/Resources/include/"
          "/Applications/Pd-extended.app/Contents/Resources/include/pdextended"
          "/Applications/Pd.app/Contents/Resources/include/")

if(PD_INCLUDE)
    message(STATUS "m_pd.h path: ${PD_INCLUDE}")
    include_directories(${PD_INCLUDE})
else()
    message(FATAL_ERROR "m_pd.h not found")
endif()

add_custom_target(run
    COMMAND open udp_send~-help.pd;)


