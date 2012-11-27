#------------- 变量设置 ------------------------------
SET ( LIB_SRC himmbuilder.cxx )
SET ( LIB_NAME mapbuilder)

INCLUDE_DIRECTORIES ( ${PROJECT_SOURCE_DIR}/gridmap 
					  ${PROJECT_SOURCE_DIR}/bridge
					  ${PROJECT_SOURCE_DIR}/mapbuilder 
					  ${PROJECT_SOURCE_DIR}/misc
					)

LINK_DIRECTORIES ( ${CMAKE_BINARY_DIR}/gridmap
				   ${CMAKE_BINARY_DIR}/bridge )


ADD_LIBRARY (${LIB_NAME} STATIC ${LIB_SRC})
ADD_DEPENDENCIES( ${LIB_NAME} gridmap  bridge )

TARGET_LINK_LIBRARIES ( ${LIB_NAME} gridmap  bridge )

