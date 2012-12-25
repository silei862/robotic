#------------- 变量设置 ------------------------------
SET ( LIB_SRC mapserver.cxx )
SET ( LIB_NAME datatrans )

INCLUDE_DIRECTORIES ( ${PROJECT_SOURCE_DIR}/gridmap 
					)

LINK_DIRECTORIES ( ${CMAKE_BINARY_DIR}/gridmap
				   )


ADD_LIBRARY (${LIB_NAME} STATIC ${LIB_SRC})
ADD_DEPENDENCIES( ${LIB_NAME} gridmap )

TARGET_LINK_LIBRARIES ( ${LIB_NAME} gridmap )

