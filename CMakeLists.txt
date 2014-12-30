cmake_minimum_required(VERSION 2.8.4)
project(Asteroids3D)

set(CMAKE_DEBUG_POSTFIX "_d")

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_MODULE_PATH "/usr/share/cmake/Modules/;${CMAKE_MODULE_PATH}")

find_package(OGRE REQUIRED)
find_package(Boost COMPONENTS thread date_time)

set(ALLEGRO_LIBRARIES
    allegro
    allegro_dialog
    )

file(GLOB HEADER_FILES *.hpp *.h)
file(GLOB SOURCE_FILES *.cpp *.c)
add_executable(Asteroids3D ${HEADER_FILES} ${SOURCE_FILES})

set_target_properties(Asteroids3D PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_link_libraries(Asteroids3D ${OGRE_LIBRARIES} ${Boost_LIBRARIES} ${ALLEGRO_LIBRARIES})

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/bin)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/media)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/dist/bin)

install(TARGETS Asteroids3D
	RUNTIME DESTINATION bin
	CONFIGURATIONS All)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/dist/media
	DESTINATION ./
	CONFIGURATIONS Release RelWithDebInfo Debug
)

install(FILES ${CMAKE_SOURCE_DIR}/dist/bin/plugins.cfg
	${CMAKE_SOURCE_DIR}/dist/bin/resources.cfg
	DESTINATION bin
	CONFIGURATIONS Release RelWithDebInfo Debug
)

