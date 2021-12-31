cmake_minimum_required(VERSION "3.21")
project(basic-ffmpeg)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED true)

# To find and use ffmpeg avcodec library
set(AVCODEC_INCLUDE_DIR "D:/src/vcpkg/packages/ffmpeg_x64-windows-static/include" )
set(AVCODEC_LIBRARY "D:/src/vcpkg/packages/ffmpeg_x64-windows-static/lib/avcodec.lib")


set(srcs 
"ctffmpeg.h"
"ctfolder.h"
"ffmpeg.cpp"
"main.cpp"
)
add_executable(main ${srcs})

target_include_directories( main PRIVATE ${AVCODEC_INCLUDE_DIR} )
target_link_libraries( main PRIVATE ${AVCODEC_LIBRARY})