cmake_minimum_required (VERSION 3.1)

project(Shield_Guardian)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-Wall -Wextra -pedantic -Werror=return-type -Werror=parentheses -Werror=vla -Werror=array-bounds -Wcast-qual -Wmaybe-uninitialized )

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(
        -Wall 
        -Wextra 
        -Wpedantic 
        -Werror=return-type 
        -Werror=parentheses 
        -Werror=vla 
        -Werror=array-bounds 
        -Wcast-qual 
        -Wmaybe-uninitialized
    )
endif()

# Windows specific config
# You can change it if you install SFML in different directory (or have different version of it)
IF (WIN32)
    # Include local sfml cmake config
    set(SFML_DIR "C:/lib/SFML-2.5.1/lib/cmake/SFML")
    # Link sfml statically (Optional)
    set(SFML_STATIC_LIBRARIES TRUE)
ELSEIF (UNIX AND NOT APPLE)
    # Linux-specific config:
    # Linux system-installed SFML (e.g., via apt) installs CMake files into standard 
    # paths (/usr/lib/x86_64-linux-gnu/cmake/SFML or /usr/lib/cmake/SFML).
    # CMake finds it automatically—no SFML_DIR variable needed!
ENDIF()

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)


add_executable(Shield_Guardian
		main.cpp
        Menu.cpp
        Menu.h
        GuardianLogic.cpp
        GuardianLogic.h
        SGSFMLView.cpp
        SGSFMLView.h
        SGSFMLController.cpp
        SGSFMLController.h)
target_link_libraries(Shield_Guardian sfml-graphics)

file(COPY ${CMAKE_SOURCE_DIR}/Press_Start_2P/PressStart2P-Regular.ttf DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/pngs/wall.png DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/pngs/grass.png DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/pngs/cannon1.png DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/pngs/cannon2.png DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/pngs/cannon3.png DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/pngs/cannon4.png DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/pngs/shield.png DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/pngs/cannonBall.png DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/pngs/redGuardian.png DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/pngs/blueGuardian.png DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/pngs/laserOrb.png DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/pngs/red.png DESTINATION ${CMAKE_BINARY_DIR})
