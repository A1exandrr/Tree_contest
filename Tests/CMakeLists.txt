cmake_minimum_required(VERSION 3.19)

get_target_property(TREE_INCLUDES ${PROJECT_NAME}.Tree INCLUDE_DIRECTORIES)

set(SOURCES
    TestAsContainer.h
    TestAsContainer.cpp
    TestAsTree.h
    TestAsTree.cpp
    TestData.h
    main.cpp
)
add_executable(${PROJECT_NAME}.tests ${SOURCES})
target_link_libraries(${PROJECT_NAME}.tests PRIVATE ${PROJECT_NAME}.Tree)
set_target_properties(${PROJECT_NAME}.tests
	PROPERTIES INCLUDE_DIRECTORIES "${TREE_INCLUDES}")
