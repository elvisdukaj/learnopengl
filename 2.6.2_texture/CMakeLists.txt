set(proj_name "2.6.2_texture")

find_package(glfw3 REQUIRED)

set(SOURCES "main.cpp")

if (WIN32)
	add_executable(${proj_name} WIN32 ${SOURCES})
else()
	add_executable(${proj_name} ${SOURCES})
endif(WIN32)


target_link_libraries(${proj_name}
PRIVATE
	glfw
	common_libs
)

install(TARGETS ${proj_name} DESTINATION .)
install(
FILES
	"../resources/shaders/2.6.2_texture.vs"
	"../resources/shaders/2.6.2_texture.fs"
DESTINATION
	"resources/shaders"
)

install(
FILES
	"../resources/textures/wall.jpeg"
DESTINATION
	"resources/textures"
)