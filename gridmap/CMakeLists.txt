#------------- 变量设置 ------------------------------
SET ( LIB_SRC himmgrid.cxx )
SET (LIB_NAME gridmap )

INCLUDE_DIRECTORIES ( 
						${PROJECT_SOURCE_DIR}/gridmap 
						${PROJECT_SOURCE_DIR}/misc
					)

ADD_LIBRARY (${LIB_NAME} STATIC ${LIB_SRC})

