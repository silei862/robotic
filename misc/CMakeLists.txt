#------------- 变量设置 ------------------------------
SET ( LIB_SRC coord.cxx  
			  misc.cxx 
			  exception.cxx )

SET (LIB_NAME misc )

INCLUDE_DIRECTORIES ( 
					   ${PROJECT_SOURCE_DIR}/misc	
					)


ADD_LIBRARY (${LIB_NAME} STATIC ${LIB_SRC})

