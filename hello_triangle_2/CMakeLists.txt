set(proj_name "hello-triangle-2")

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

if (WIN32)
	add_executable(${proj_name} WIN32 main.cpp glad.c)
else()
	add_executable(${proj_name} main.cpp glad.c)
endif(WIN32)

target_compile_features(${proj_name} PRIVATE cxx_std_17)

target_link_libraries(${proj_name}
PRIVATE
	glfw
	glm
	glsl
)

install(TARGETS ${proj_name} DESTINATION .)
install(FILES "hello_triangle2.vs" "hello_triangle2.fs" DESTINATION .)