project(testOpus)
set(CMAKE_OSX_ARCHITECTURES "i386")
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_library(opus_encode SHARED pdopus.c stream.cpp)

if (APPLE)

    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined suppress -flat_namespace")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -undefined suppress -flat_namespace")
    SET_TARGET_PROPERTIES(opus_encode PROPERTIES SUFFIX .pd_darwin)
    SET_TARGET_PROPERTIES(opus_encode PROPERTIES PREFIX "")
endif(APPLE)

if (APPLE)
    option(FAT_BINARY "Build a fat binary (32bit and 64bit)" OFF)
endif(APPLE)

find_package(Boost 1.40 COMPONENTS system REQUIRED)
if(Boost_FOUND)
    #target_link_libraries(pdopus ${Boost_LIBRARIES})
endif()

find_path(OPUS_INCLUDE NAME opus/opus.h PATHS /usr/local/include)
if(OPUS_INCLUDE)
    message(STATUS "opus.h path: ${OPUS_INCLUDE}")
    include_directories(${OPUS_INCLUDE})
    find_library(OPUS_LIBRARY opus)
    if(OPUS_LIBRARY)
        message(STATUS "opus lib: ${OPUS_LIBRARY}")
        target_link_libraries(pdopus)
    else()
        message(FATAL_ERROR "opus lib not found")
    endif()
else()
   message(FATAL_ERROR "opus.h not found")
endif()


find_path(PD_INCLUDE NAMES m_pd.h
    PATHS "/usr/local/include"
          "/usr/include"
          "/Applications/Pd-extended.app/Contents/Resources/include/"
          "/Applications/Pd-extended.app/Contents/Resources/include/pdextended"
          "/Applications/Pd.app/Contents/Resources/include/")

if(PD_INCLUDE)
    message(STATUS "m_pd.h path: ${PD_INCLUDE}")
    include_directories(${PD_INCLUDE})
else()
    message(FATAL_ERROR "m_pd.h not found")
endif()


