cmake_minimum_required(VERSION 3.19)

set(HEADERS
	Container.h
	GroupContainer.h
	GroupList.h
	Mem.h
	MemoryManager.h
	TreeAbstract.h
)

add_library(${PROJECT_NAME}.abstract INTERFACE ${HEADERS})
set_target_properties(${PROJECT_NAME}.abstract
	PROPERTIES INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
)
