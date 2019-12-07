#
# ******************************************************************************
# Copyright (c) 2019 Robert Bosch GmbH and others.
#
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v2.0
# which accompanies this distribution, and is available at
# https://www.eclipse.org/org/documents/epl-2.0/index.php
#
#  Contributors:
#      Robert Bosch GmbH - initial API and functionality
# *****************************************************************************

cmake_minimum_required(VERSION 3.1.3)
project(sensoris-invehicle)

#add_definitions(-DDEBUG -DTEST -DDEMO)

set(ADDRESS_SAN OFF CACHE STRING "Build with AddressSanitizer")
set(CMAKE_CXX_FLAGS "-std=c++11  ${CMAKE_CXX_FLAGS}")


if ("${ADDRESS_SAN}" STREQUAL "ON" AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_compile_options(-g -fsanitize=address -fno-omit-frame-pointer)
  add_link_options(-g -fsanitize=address)
endif()

add_definitions(-DGOOGLE_PROTOBUF_CMAKE_BUILD)

add_library(all-dependancies INTERFACE)
target_compile_features(all-dependancies INTERFACE cxx_relaxed_constexpr)

target_include_directories(all-dependancies INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../3rd-party-libs)

find_package(Threads REQUIRED)
target_link_libraries(all-dependancies INTERFACE ${CMAKE_THREAD_LIBS_INIT})

#Protobuf v3.6.1 required
FIND_PACKAGE(Protobuf 3.6.1 REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
target_link_libraries(all-dependancies INTERFACE ${PROTOBUF_LIBRARIES})
target_include_directories(all-dependancies INTERFACE ${PROTOBUF_INCLUDE_DIRS})

# Boost min version 1.67
find_package(Boost REQUIRED)
set(Boost_USE_STATIC_LIBS ON)
include_directories(${Boost_INCLUDE_DIRS})
message(STATUS " boost includes ${Boost_INCLUDE_DIRS} ")

find_package(Boost 1.67.0 COMPONENTS system thread REQUIRED)
target_link_libraries(all-dependancies INTERFACE ${Boost_LIBRARIES})
target_include_directories(all-dependancies INTERFACE ${Boost_INCLUDE_DIR})
message(STATUS " boost libs ${Boost_LIBRARIES} ")

#openssl
find_package(OpenSSL REQUIRED)
target_link_libraries(all-dependancies INTERFACE ${OPENSSL_LIBRARIES})
target_include_directories(all-dependancies INTERFACE ${OPENSSL_INCLUDE_DIR})
message(STATUS " openssl libraries ${OPENSSL_LIBRARIES}")

file(GLOB_RECURSE SRC_FILES
${CMAKE_CURRENT_SOURCE_DIR}/sensoris/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/sensoris/*/*/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/event-handlers/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/websocket/src/*.cpp)

 
add_executable(${PROJECT_NAME} ${SRC_FILES})


target_include_directories(${PROJECT_NAME} PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/sensoris/include ${CMAKE_CURRENT_SOURCE_DIR}/event-handlers/include ${CMAKE_CURRENT_SOURCE_DIR}/websocket/include)
target_link_libraries(${PROJECT_NAME} PRIVATE all-dependancies)


configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../examples/demo-certificates/Client.pem  ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../examples/demo-certificates/Client.key  ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../examples/demo-certificates/CA.pem  ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

