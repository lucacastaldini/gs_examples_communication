cmake_minimum_required(VERSION 3.10)
project(GSCommunication CXX)

# Ensure that we are using static libraries, including Protocol Buffers
set(protobuf_BUILD_SHARED_LIBS OFF)  # Disable shared libraries for protobuf
set(BUILD_SHARED_LIBS OFF)  # Globally disable shared libraries

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
# Find required protobuf package
find_package(protobuf CONFIG REQUIRED)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(ZMQ_DIR ${PROJECT_SOURCE_DIR}/src/zeromq)
set(SHM_DIR ${PROJECT_SOURCE_DIR}/src/shared_memory)


set(GS_SERIALIZATION_DIR ${PROJECT_SOURCE_DIR}/gs_examples_serialization)

add_subdirectory(${GS_SERIALIZATION_DIR}/avrocppexample)
add_subdirectory(${GS_SERIALIZATION_DIR}/protocol_buffer)
add_subdirectory(${GS_SERIALIZATION_DIR}/ccsds)


set(INC_DIR
    ${PROJECT_SOURCE_DIR}/include
)

set(COMMON_SRCS
    ${PROJECT_SOURCE_DIR}/include/utils.cc
)

set(ZMQ_INC_DIR
    ${INC_DIR}
    ${ZMQ_DIR}/include
)

set(ZMQ_COMMON_SRCS
    ${COMMON_SRCS}
)

# Find cppzmq wrapper
find_package(cppzmq REQUIRED)
find_package(absl REQUIRED)

list(APPEND LIBS 
    cppzmq
)

# Add the executables
add_executable(prodEx ${ZMQ_DIR}/example/prodEx.cpp ${ZMQ_COMMON_SRCS})
add_executable(consEx ${ZMQ_DIR}/example/consEx.cpp ${ZMQ_COMMON_SRCS})

# Add the executables
add_executable(AvroCppProd ${ZMQ_DIR}/avrocpp/AvroProducer.cpp ${ZMQ_COMMON_SRCS})
add_executable(AvroCppCons ${ZMQ_DIR}/avrocpp/AvroConsumer.cpp ${ZMQ_COMMON_SRCS})

# Link the libraries to the executables
target_include_directories(prodEx PUBLIC ${ZMQ_INC_DIR})
target_include_directories(consEx PUBLIC ${ZMQ_INC_DIR})
target_link_libraries(prodEx PUBLIC ${LIBS})
target_link_libraries(consEx PUBLIC ${LIBS})

set(AVRO_LIBS
  ${LIBS}
  GSAvroSerialization
)

# Link the libraries to the executables
target_include_directories(AvroCppProd PUBLIC ${ZMQ_INC_DIR})
target_include_directories(AvroCppCons PUBLIC ${ZMQ_INC_DIR})
target_link_libraries(AvroCppProd LINK_PUBLIC ${AVRO_LIBS})
target_link_libraries(AvroCppCons LINK_PUBLIC ${AVRO_LIBS})

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
add_executable(PBCppProd ${ZMQ_DIR}/protobuf/ProtoProducer.cpp ${ZMQ_COMMON_SRCS})
add_executable(PBCppCons ${ZMQ_DIR}/protobuf/ProtoConsumer.cpp ${ZMQ_COMMON_SRCS})

set(GSProtoclBufferSerialization_DIR ${GS_SERIALIZATION_DIR}/protocol_buffer/build)

find_library(GSProtoclBufferSerialization_LIB
             NAMES GSProtoclBufferSerialization
             PATHS ${GSProtoclBufferSerialization_DIR})
# Link the libraries to the executables
set(PB_DIRS
  ${ZMQ_INC_DIR}
  ${GS_SERIALIZATION_DIR}/protocol_buffer
  ${GS_SERIALIZATION_DIR}/protocol_buffer/include
)
set(PB_LIBS
  ${LIBS}
  protobuf::libprotobuf
  GSProtoclBufferSerialization
)
target_include_directories(PBCppProd PUBLIC ${PB_DIRS})
target_include_directories(PBCppCons PUBLIC ${PB_DIRS})
target_link_libraries(PBCppProd PUBLIC ${PB_LIBS})
target_link_libraries(PBCppCons LINK_PUBLIC ${PB_LIBS})

add_dependencies(PBCppProd GSProtoclBufferSerialization)
add_dependencies(PBCppCons GSProtoclBufferSerialization)

# Add the executables
add_executable(CCSDSCppProd ${ZMQ_DIR}/ccsds/CCSDSProducer.cpp ${ZMQ_COMMON_SRCS})
add_executable(CCSDSCppCons ${ZMQ_DIR}/ccsds/CCSDSConsumer.cpp ${ZMQ_COMMON_SRCS})

set(CCSDS_DIRS
  ${ZMQ_INC_DIR}
  ${GS_SERIALIZATION_DIR}/ccsds/include
)
set(CCSDS_LIBS
  ${LIBS}
  GSccsds
)
target_include_directories(CCSDSCppProd PUBLIC ${CCSDS_DIRS})
target_include_directories(CCSDSCppCons PUBLIC ${ZMQ_INC_DIR})
target_link_libraries(CCSDSCppProd PUBLIC ${CCSDS_LIBS})
target_link_libraries(CCSDSCppCons PUBLIC ${CCSDS_LIBS})


# Configuration File:
set(SOURCE_FILE ${ZMQ_DIR}/config.txt)
set(DESTINATION_FILE ${CMAKE_BINARY_DIR}/config.txt)
file(COPY ${SOURCE_FILE} DESTINATION ${CMAKE_BINARY_DIR})


###BUILD SHARED MEM APPLICATIONS
set(SOURCE_DIR
${SHM_DIR}/ProdCons.cpp
)

# Add the executable
add_executable(shmEx ${SOURCE_DIR})

# Link the Avro library to the executable