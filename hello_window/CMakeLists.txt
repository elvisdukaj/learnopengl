find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(hello-window main.cpp glad.c)

target_link_libraries(hello-window 
PRIVATE
	glfw
	glm
)

install(TARGETS hello-window DESTINATION .)