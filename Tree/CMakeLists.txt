cmake_minimum_required(VERSION 3.19)

get_target_property(CONTEST_INCLUDES ${PROJECT_NAME}.abstract INCLUDE_DIRECTORIES)

set(SOURCES
	Tree.h
	Tree.cpp
)
add_library(${PROJECT_NAME}.Tree STATIC ${SOURCES})
set_target_properties(${PROJECT_NAME}.Tree
	PROPERTIES INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR};${CONTEST_INCLUDES}")

target_link_libraries(${PROJECT_NAME}.Tree PUBLIC ${PROJECT_NAME}.abstract)
