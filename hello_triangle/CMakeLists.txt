find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(hello-triangle main.cpp glad.c)

target_link_libraries(hello-triangle 
PRIVATE
	glfw
	glm
)

install(TARGETS hello-triangle DESTINATION .)