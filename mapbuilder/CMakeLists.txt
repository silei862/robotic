#------------- 窗口刷新配置选项 ----------------------
OPTION ( UPDATE_WITHIN_WIN_SWICTH " Only refresh distance map in active window " )
IF ( UPDATE_WITHIN_WIN_SWICTH )
	ADD_DEFINITIONS( -DUPDATE_WITHIN_WIN )
ENDIF( UPDATE_WITHIN_WIN_SWICTH )	

OPTION ( IM_REFRESH_SWITCH " Refresh distance map after clear at once " )
IF ( IM_REFRESH_SWITCH )
	ADD_DEFINITIONS( -DIM_REFRESH )
ENDIF( IM_REFRESH_SWITCH )	

#------------- 变量设置 ------------------------------
SET ( LIB_SRC himmbuilder.cxx dmmbuilder.cxx himmuvgen.cxx )
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

