cmake_minimum_required(VERSION 3.10)

option(USE_AVX "Enable AVX instruction sete" On)
option(USE_AVX2 "Enable AVX2 instruction sete" Off)

function(set_compiler_options the_target)
	if (WIN32)
		if (USE_AVX)
			message(STATUS "Using AVX istruction set")
			target_compile_options(${the_target} PRIVATE "/arch:AVX")
		endif()
	
		if (USE_AVX2)
			message(STATUS "Using AVX2 istruction set")
			target_compile_options(${the_target} PRIVATE "/arch:AVX2")
		endif()
	endif()
endfunction()

add_subdirectory(common_libs)
add_subdirectory(2.3.1_hello_window)
add_subdirectory(2.4.1_hello_triangle)
add_subdirectory(2.4.2_hello_triangle)
add_subdirectory(2.6.1_texture)
add_subdirectory(2.6.2_texture)
add_subdirectory(2.6.3_texture)
add_subdirectory(2.8.1_transform)
add_subdirectory(2.8.2_transform)
add_subdirectory(2.8.3_transform)
add_subdirectory(2.9.1_camera)