#------------- 变量设置 ------------------------------
SET (LIB_SRC vfh.cxx )
SET (LIB_NAME vfh)

INCLUDE_DIRECTORIES ( 
					  ${PROJECT_SOURCE_DIR}/gridmap
					  ${PROJECT_SOURCE_DIR}/mapbuilder 
					  ${PROJECT_SOURCE_DIR}/misc
					  ${PROJECT_SOURCE_DIR}/vfh
				    )

LINK_DIRECTORIES ( 
				   ${CMAKE_BINARY_DIR}/gridmap 
				   ${CMAKE_BINARY_DIR}/mapbuilder 
				 )

ADD_LIBRARY (${LIB_NAME} STATIC ${LIB_SRC})
ADD_DEPENDENCIES( ${LIB_NAME} gridmap )
ADD_DEPENDENCIES( ${LIB_NAME} mapbuilder )

TARGET_LINK_LIBRARIES ( ${LIB_NAME} gridmap )
TARGET_LINK_LIBRARIES ( ${LIB_NAME} mapbuilder )

