cmake_minimum_required(VERSION 3.10)
project(GSCommunication CXX)

add_subdirectory("${CMAKE_SOURCE_DIR}/deps/gs_examples_serialization/avrocppexample")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

# Specify the paths to the Avro include directory and library
set(INC_DIR
    "${PROJECT_SOURCE_DIR}/include"
)

# Find cppzmq wrapper
find_package(cppzmq REQUIRED)

list(APPEND LIBS 
    cppzmq
)

# Add the executables
add_executable(prodEx src/example/prodEx.cpp)
add_executable(consEx src/example/consEx.cpp)

# Add the executables
add_executable(AvroCppProd src/avrocpp/AvroProducer.cpp)
add_executable(AvroCppCons src/avrocpp/AvroConsumer.cpp)

# Link the libraries to the executables
target_include_directories(prodEx PUBLIC ${INC_DIR})
target_include_directories(consEx PUBLIC ${INC_DIR})
target_link_libraries(prodEx PUBLIC ${LIBS})
target_link_libraries(consEx PUBLIC ${LIBS})

# Link the libraries to the executables
target_include_directories(AvroCppProd PUBLIC ${INC_DIR})
target_include_directories(AvroCppCons PUBLIC ${INC_DIR})
target_link_libraries(AvroCppProd LINK_PUBLIC ${LIBS} WFTestLib)
target_link_libraries(AvroCppCons LINK_PUBLIC ${LIBS} WFTestLib)


# Configuration File:
set(SOURCE_FILE "${PROJECT_SOURCE_DIR}/config.txt")
set(DESTINATION_FILE "${CMAKE_BINARY_DIR}/config.txt")
file(COPY "${SOURCE_FILE}" DESTINATION "${CMAKE_BINARY_DIR}")