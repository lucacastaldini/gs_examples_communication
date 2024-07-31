cmake_minimum_required(VERSION 3.10)
project(AvroSerialize CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)

option(CPPKAFKA_BUILD_SHARED OFF)
option(CPPKAFKA_DISABLE_TESTS ON)
option(CPPKAFKA_DISABLE_EXAMPLES ON)

# Specify the paths to the Avro include directory and library
set(AVRO_CPP_PATH "/usr/local/include/avro")
set(INC_DIR
    "${CMAKE_SOURCE_DIR}/include"
    "${AVRO_CPP_PATH}"
)

find_library(AVRO_CPP_LIBRARY avrocpp_s REQUIRED PATHS ${AVRO_CPP_PATH}/../lib)

# Find cppzmq wrapper
find_package(cppzmq REQUIRED)

list(APPEND LIBS 
    ${AVRO_CPP_LIBRARY}
    cppzmq
)

link_directories(${AVRO_CPP_PATH}/../lib)

# Set source files
set(PRODUCER_SRC
    src/prodEx.cpp
)

set(CONSUMER_SRC
    src/consEx.cpp
)

# Include directories
include_directories(${INC_DIR})

# Add the executables
add_executable(prodEx ${PRODUCER_SRC})
add_executable(consEx ${CONSUMER_SRC})

# Link the libraries to the executables
target_link_libraries(prodEx PRIVATE ${LIBS})
target_link_libraries(consEx PRIVATE ${LIBS})
