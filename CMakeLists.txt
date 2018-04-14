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

add_subdirectory(glsl)
add_subdirectory(hello_window)
add_subdirectory(hello_triangle)
add_subdirectory(hello_triangle_2)
add_subdirectory(texture)
add_subdirectory(texture2)
add_subdirectory(texture3)
add_subdirectory(8.1_transform)