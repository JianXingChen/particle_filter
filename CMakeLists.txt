cmake_minimum_required(VERSION 2.8)
project( particle——filter )
find_package( OpenCV 3.0 REQUIRED )

include_directories(
    include
    ${OpenCV_INCLUDE_DIRS} )

file(GLOB particle
    "src/*.cpp"
#    "include/mono-vo/*.h"
)
SET(CMAKE_BUILD_TYPE Debug CACHE STRING "set build type to debug")
add_executable( particle ${particle} )
target_link_libraries( particle ${OpenCV_LIBS} )
