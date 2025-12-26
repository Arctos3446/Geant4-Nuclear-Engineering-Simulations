cmake_minimum_required(VERSION 3.16)
project(RadShield)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(RadShield RadShield.cc ${sources} ${headers})

target_link_libraries(RadShield ${Geant4_LIBRARIES})

set(RADSHIELD_SCRIPTS
    vis.mac
  )

foreach(_script ${RADSHIELD_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
  )
endforeach()
