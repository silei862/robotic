# 查找opencv库
PKG_SEARCH_MODULE( OPENCV opencv >= 2.0 )
IF( OPENCV_FOUND )
	MESSAGE( STATUS ${INDENT} "OpenCV version ${OPENCV_VERSION} detected at ${OPENCV_PREFIX}" )
	MESSAGE( STATUS "	OpenCV C++ CFLAGS: ${OPENCV_CFLAGS}" )
	MESSAGE( STATUS "	OpenCV C++ LDFLAGS: ${OPENCV_LDFLAGS}" )
	MESSAGE( STATUS "	OpenCV C++ LIBS: ${OPENCV_LIBRARIES}" )
ELSE( OPENCV_FOUND )
	MESSAGE( STATUS ${INDENT} "OpenCV not detected. If OpenCV is installed but not detected , check your PKG_CONFIG_PATH." )
ENDIF( OPENCV_FOUND )

#------------- 变量设置 ------------------------------
SET (MAPVIEWER_SRC  canvas_test.cxx cvcanvas.cxx cvdraw.cxx )
SET (MAPVIEWER_BIN mapviewer )

INCLUDE_DIRECTORIES ( ${OPENCV_INCLUDE_DIRS} 
					  ${PROJECT_SOURCE_DIR}/misc 
					  ${PROJECT_SOURCE_DIR}/gridmap )


LINK_DIRECTORIES ( ${OPENCV_LIBRARY_DIRS} 
				   ${CMAKE_BINARY_DIR}/gridmap )

#ADD_EXECUTABLE (${MAPVIEWER_BIN} ${MAPVIEWER_SRC})

#TARGET_LINK_LIBRARIES ( ${MAPVIEWER_BIN} ${OPENCV_LIBRARIES})

########### mapconv ###########
ADD_EXECUTABLE( mapconv mapconv.cxx )
TARGET_LINK_LIBRARIES( mapconv gridmap )
