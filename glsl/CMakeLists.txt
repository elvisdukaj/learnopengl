set(proj_name glsl)

add_library(${proj_name} INTERFACE)

#message(FATAL_ERROR ${CMAKE_CURRENT_SOURCE_DIR})
 
 target_include_directories(${proj_name}
 INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
