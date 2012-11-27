#------------- 变量设置 ------------------------------
SET (LIB_SRC vfhplanner.cxx  localplanner.cxx )
SET (LIB_NAME planner)

INCLUDE_DIRECTORIES ( 
					  ${PROJECT_SOURCE_DIR}/gridmap 
					  ${PROJECT_SOURCE_DIR}/bridge
					  ${PROJECT_SOURCE_DIR}/misc
					  ${PROJECT_SOURCE_DIR}/vfh
				    )

LINK_DIRECTORIES ( ${CMAKE_BINARY_DIR}/gridmap 
				   ${CMAKE_BINARY_DIR}/bridge 
				   ${CMAKE_BINARY_DIR}/vfh
				   ${CMAKE_BINARY_DIR}/misc )

ADD_LIBRARY (${LIB_NAME} STATIC ${LIB_SRC})

ADD_DEPENDENCIES( ${LIB_NAME} gridmap bridge misc vfh )

TARGET_LINK_LIBRARIES ( ${LIB_NAME} gridmap bridge misc vfh )

