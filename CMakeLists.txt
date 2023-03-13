cmake_minimum_required(VERSION 1.13)
project(lux)

file(GLOB_RECURSE SRC "src/*.cpp" "src/*.c")
file(GLOB_RECURSE LIB "lib/*.cpp" "lib/*.c")
file(GLOB_RECURSE FLOG "flog/*.cpp")

add_executable(lux ${SRC} ${LIB} ${FLOG})

target_include_directories(lux PRIVATE "src/")
target_include_directories(lux PRIVATE "lib/")
target_include_directories(lux PRIVATE "flog/")
target_include_directories(lux PRIVATE "lib/vendor")
target_include_directories(lux PRIVATE "lib/vendor/glad")

if (MSVC)
    # OpenGL
    find_package(OpenGL REQUIRED)
    target_link_libraries(lux PUBLIC OpenGL::GL)

    # GLFW3
    find_package(glfw3 CONFIG REQUIRED)
    target_link_libraries(lux PUBLIC glfw)

    target_compile_options(lux PUBLIC /std:c++17)

else()
    find_package(PkgConfig)
    find_package(OpenGL REQUIRED)

    pkg_check_modules(GLFW3 REQUIRED glfw3)

    target_include_directories(lux PUBLIC
        ${OPENGL_INCLUDE_DIRS}
        ${GLFW3_INCLUDE_DIRS}
    )
    
    target_link_directories(lux PUBLIC
        ${OPENGL_LIBRARY_DIRS}
        ${GLFW3_LIBRARY_DIRS}
    )

    target_link_libraries(lux PUBLIC
        ${OPENGL_LIBRARIES}
        ${GLFW3_LIBRARIES}
    )

    if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        target_link_libraries(lux PUBLIC stdc++fs)
    endif ()
endif ()