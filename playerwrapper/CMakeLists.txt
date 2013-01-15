#------------- 变量设置 ------------------------------
SET (LIB_SRC playerwrapper.cxx)
SET (LIB_NAME playerwrapper)

INCLUDE_DIRECTORIES ( ${PLAYERCXX_INCLUDE_DIRS} 
					  ${PROJECT_SOURCE_DIR}/bridge 
					  ${PROJECT_SOURCE_DIR}/misc
					)
LINK_DIRECTORIES ( ${PLAYERCXX_LIBRARY_DIRS} 
				   ${CMAKE_BINARY_DIR}/bridge 
				   ${CMAKE_BINARY_DIR}/misc )

ADD_LIBRARY (${LIB_NAME} STATIC ${LIB_SRC})
ADD_DEPENDENCIES( ${LIB_NAME} bridge )
TARGET_LINK_LIBRARIES ( ${LIB_NAME} ${PLAYERCXX_LIBRARIES} bridge misc )

