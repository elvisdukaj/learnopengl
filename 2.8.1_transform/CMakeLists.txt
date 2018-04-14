set(proj_name "transform_1")

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

if (WIN32)
	add_executable(${proj_name} WIN32 main.cpp glad.c)
else()
	add_executable(${proj_name} main.cpp glad.c)
endif(WIN32)

set_compiler_options(${proj_name})

target_compile_options(${proj_name} PRIVATE -DSTB_IMAGE_IMPLEMENTATION)
target_compile_features(${proj_name} PRIVATE cxx_std_17)

target_link_libraries(${proj_name}
PRIVATE
	glfw
	glm
	glsl
)

install(TARGETS ${proj_name} DESTINATION .)
install(FILES "8.1_transform.vs" "8.1_transform.fs" "wall.jpeg" "awesomeface.png" DESTINATION .)