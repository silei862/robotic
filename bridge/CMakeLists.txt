#------------- 变量设置 ------------------------------
SET ( LIB_SRC bridge.cxx )
SET (LIB_NAME bridge )

INCLUDE_DIRECTORIES ( 
						${PROJECT_SOURCE_DIR}/bridge 
						${PROJECT_SOURCE_DIR}/misc
					)

LINK_DIRECTORIES (
					${CMAKE_BINARY_DIR}/misc
				 )

ADD_LIBRARY (${LIB_NAME} STATIC ${LIB_SRC})

ADD_DEPENDENCIES( ${LIB_NAME} misc )

TARGET_LINK_LIBRARIES( ${LIB_NAME} misc )

