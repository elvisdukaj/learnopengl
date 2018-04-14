set(proj_name "2.3.1_hello-window")

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