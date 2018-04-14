set(proj_name common_libs)

find_package(glm REQUIRED)

set(SOURCES "src/glad.c" "src/glsl.cpp")
add_library(${proj_name} STATIC ${SOURCES})

target_include_directories(${proj_name}
PUBLIC
	include
#	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#	$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set_compiler_options(${proj_name})

target_compile_definitions(${proj_name} PUBLIC "STB_IMAGE_IMPLEMENTATION")
target_compile_features(${proj_name} PUBLIC cxx_std_17)
target_link_libraries(${proj_name} PUBLIC glm)