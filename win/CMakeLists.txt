cmake_minimum_required (VERSION 3.8)

project ("Similarity")

set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR D:/opencv/build/)

find_package(OpenCV REQUIRED core highgui imgproc features2d)

include_directories(${OpenCV_INCLUDE_DIRS})

link_directories(${OpenCV_INCLUDE_DIRS})

add_executable(similarity Similarity/Similarity.cpp)

target_link_libraries(similarity  ${OpenCV_LIBS})