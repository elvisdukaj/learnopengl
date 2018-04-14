set(proj_name "2.4.2_hello-triangle")

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
	"../resources/shaders/2.4.2_hello_triangle.vs"
	"../resources/shaders/2.4.2_hello_triangle.fs"
DESTINATION
	"resources/shaders"
)