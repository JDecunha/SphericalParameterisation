#CMakeLists file for MicroTrackGenerator

cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(SphericalParameterisationExample VERSION 1.0.0)

###
### Find and include Geant4 libraries
###

find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE}) #include the Geant4 CMakeLists

###
### Configure executable and in-project libraries
###

add_executable(SphericalParameterisationExample main.cc)

#locate our project source files and make a library
	file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
	add_library(src STATIC ${sources})
	target_include_directories(src PUBLIC ${PROJECT_SOURCE_DIR}/include ${Geant4_INCLUDE_DIR})

###
### Link MicroTrackGenerator with libraries
###

target_link_libraries(${PROJECT_NAME} src ${Geant4_LIBRARIES})

#Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

