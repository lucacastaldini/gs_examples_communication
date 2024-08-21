cmake_minimum_required(VERSION 3.10)
project(GSCommunication CXX)

# Ensure that we are using static libraries, including Protocol Buffers
set(protobuf_BUILD_SHARED_LIBS OFF)  # Disable shared libraries for protobuf
set(BUILD_SHARED_LIBS OFF)  # Globally disable shared libraries

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
# Find required protobuf package
find_package(protobuf CONFIG REQUIRED)

add_subdirectory("${CMAKE_SOURCE_DIR}/deps/gs_examples_serialization/avrocppexample")
add_subdirectory("${CMAKE_SOURCE_DIR}/deps/gs_examples_serialization/protocol_buffer")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Specify the paths to the Avro include directory and library
set(INC_DIR
    "${PROJECT_SOURCE_DIR}/include"
)

# Find cppzmq wrapper
find_package(cppzmq REQUIRED)
find_package(absl REQUIRED)

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
target_link_libraries(AvroCppProd LINK_PUBLIC ${LIBS} GSAvroSerialization)
target_link_libraries(AvroCppCons LINK_PUBLIC ${LIBS} GSAvroSerialization)

# http://www.cmake.org/Wiki/CMake_FAQ#How_can_I_build_my_MSVC_application_with_a_static_runtime.3F
if(MSVC AND protobuf_MSVC_STATIC_RUNTIME)
  foreach(flag_var
      CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    if(${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MD")
  endforeach()
endif()

# Add the executables
add_executable(PBCppProd src/protobuf/ProtoProducer.cpp )
add_executable(PBCppCons src/protobuf/ProtoConsumer.cpp )

set(GSProtoclBufferSerialization_DIR "${PROJECT_SOURCE_DIR}/deps/gs_examples_serialization/protocol_buffer/build")
find_library(GSProtoclBufferSerialization_LIB
             NAMES GSProtoclBufferSerialization
             PATHS ${GSProtoclBufferSerialization_DIR})
# Link the libraries to the executables
target_include_directories(PBCppProd PUBLIC ${INC_DIR} ${PROTOBUF_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/deps/gs_examples_serialization/protocol_buffer/include" "${PROJECT_SOURCE_DIR}/build/deps/gs_examples_serialization/protocol_buffer")
target_include_directories(PBCppCons PUBLIC ${INC_DIR} "${CMAKE_SOURCE_DIR}/deps/gs_examples_serialization/protocol_buffer/include" "${CMAKE_SOURCE_DIR}/build/deps/gs_examples_serialization/protocol_buffer")
target_link_libraries(PBCppProd PUBLIC ${LIBS} protobuf::libprotobuf GSProtoclBufferSerialization)
target_link_libraries(PBCppCons LINK_PUBLIC ${LIBS} protobuf::libprotobuf GSProtoclBufferSerialization)

add_dependencies(PBCppProd GSProtoclBufferSerialization)

# Configuration File:
set(SOURCE_FILE "${PROJECT_SOURCE_DIR}/config.txt")
set(DESTINATION_FILE "${CMAKE_BINARY_DIR}/config.txt")
file(COPY "${SOURCE_FILE}" DESTINATION "${CMAKE_BINARY_DIR}")