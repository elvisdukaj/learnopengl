set(proj_name "hello-triangle")

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(${proj_name} main.cpp glad.c)

target_compile_features(${proj_name} PRIVATE cxx_std_17)

target_link_libraries(${proj_name}
PRIVATE
	glfw
	glm
	glsl
)

install(TARGETS ${proj_name} DESTINATION .)