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


#find cppzmq wrapper, installed by make of cppzmq
find_package(cppzmq)

list(APPEND LIBS 
	avrocpp_s
	)

link_directories(${AVRO_LIBRARY_DIR})


# Add the source files (include all relevant .cpp files here)
set(ProdEx
    # src/main.cpp
    src/producer.cpp
    # Add other .cpp files if necessary
)
set(ConsEx
    src/consumer.cpp
    # Add other .cpp files if necessary
)

# Add the executable
add_executable(prodEx ${ProdEx})
add_executable(consEx ${ConsEx})

target_include_directories (prodEx PUBLIC ${INC_DIR})
target_link_libraries( prodEx LINK_PUBLIC ${LIBS})
target_link_libraries(prodEx  PUBLIC cppzmq)

target_include_directories (consEx PUBLIC ${INC_DIR})
target_link_libraries( consEx LINK_PUBLIC ${LIBS})
target_link_libraries(consEx  PUBLIC cppzmq)

# Link the Avro library to the executable
