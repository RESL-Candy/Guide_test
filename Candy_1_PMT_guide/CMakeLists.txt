cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(CandySIM)
#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default


find_package(Geant4 REQUIRED ui_all vis_all)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project


include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
## Candy is the 실행파일 name
add_executable(Candy Candy.cc ${sources} ${headers})
target_link_libraries(Candy ${Geant4_LIBRARIES})


set(Candy_SCRIPTS
  
  init_vis.mac
  gun.mac
  vis.mac
  )

foreach(_script ${Candy_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

add_custom_target(CandySIM DEPENDS Candy)