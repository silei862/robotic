SET (TEST_BINARY_SRC main.cxx)
SET (TEST_BINARY go)

LINK_DIRECTORIES (	
					${CMAKE_BINARY_DIR}/bridge 
					${CMAKE_BINARY_DIR}/gridmap
					${CMAKE_BINARY_DIR}/playerwrapper
					${CMAKE_BINARY_DIR}/mapbuilder
					${CMAKE_BINARY_DIR}/vfh
					${CMAKE_BINARY_DIR}/planner
					${CMAKE_BINARY_DIR}/misc
					${PLAYERCXX_LIBRARY_DIRS}
				) 


INCLUDE_DIRECTORIES (	
						${PROJECT_SOURCE_DIR}/bridge
						${PROJECT_SOURCE_DIR}/gridmap
						${PROJECT_SOURCE_DIR}/playerwrapper
						${PROJECT_SOURCE_DIR}/mapbuilder
						${PROJECT_SOURCE_DIR}/vfh
						${PROJECT_SOURCE_DIR}/planner
						${PROJECT_SOURCE_DIR}/misc
						${PLAYERCXX_INCLUDE_DIRS}
						${OPENCV_INCLUDE_DIRS}
					)


############## Mapping #############
ADD_EXECUTABLE( mapping mapping.cxx )
TARGET_LINK_LIBRARIES( mapping mapbuilder
							   playerwrapper
							   bridge
							   gridmap
							   planner
							   misc
					 		   ${PLAYERCXX_LIBRARIES}
					 )

############## Goto Position #########
ADD_EXECUTABLE( goto_pos gotopos_test.cxx )
TARGET_LINK_LIBRARIES( goto_pos mapbuilder
							   playerwrapper
							   bridge
							   gridmap
							   planner
							   misc
					 		   ${PLAYERCXX_LIBRARIES}
					 )


############## MapWin #############
ADD_EXECUTABLE( mapwin mapwin.cxx )
TARGET_LINK_LIBRARIES( mapwin mapbuilder
							  playerwrapper
							  bridge
							  gridmap
							  planner
							  misc
					 		  ${PLAYERCXX_LIBRARIES}
					 )

############## Vector Filed Histogram #############
ADD_EXECUTABLE( vfhisto vfhisto.cxx )
TARGET_LINK_LIBRARIES( vfhisto mapbuilder
							   gridmap
							   playerwrapper
							   planner
							   misc
							   vfh
					 		  ${PLAYERCXX_LIBRARIES}
					 )

############## Binary Vector Filed Histogram #############
ADD_EXECUTABLE( vfhbinhist vfhbinhist.cxx )
TARGET_LINK_LIBRARIES( vfhbinhist mapbuilder
							   playerwrapper
							   gridmap
							   misc
							   planner
							   vfh
					 		  ${PLAYERCXX_LIBRARIES}
					 )

################# Direction Finder ########################
ADD_EXECUTABLE( directionfind directionfind.cxx )
TARGET_LINK_LIBRARIES( directionfind mapbuilder
							   playerwrapper
							   gridmap
							   vfh
							   planner
					 		  ${PLAYERCXX_LIBRARIES}
					 )

################# gone  ########################
ADD_EXECUTABLE( gone main.cxx )
TARGET_LINK_LIBRARIES( gone mapbuilder
							   playerwrapper
							   gridmap
							   misc
							   vfh
							   planner
					 		  ${PLAYERCXX_LIBRARIES}
					 )


