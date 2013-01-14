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
					${CMAKE_BINARY_DIR}/datatrans
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
						${PROJECT_SOURCE_DIR}/datatrans
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
							   datatrans
					 		   ${PLAYERCXX_LIBRARIES}
					 )

############## Mappingtime #############
ADD_EXECUTABLE( mappingtime mappingtime.cxx )
TARGET_LINK_LIBRARIES( mappingtime 	mapbuilder
							   		playerwrapper
							   		bridge
							   		gridmap
							   		planner
							   		misc
							   		datatrans
					 		   		${PLAYERCXX_LIBRARIES}
					 )

ADD_EXECUTABLE( astarplan astarplan.cxx )
TARGET_LINK_LIBRARIES( astarplan mapbuilder
							   playerwrapper
							   bridge
							   gridmap
							   planner
							   misc
							   datatrans
					 		   ${PLAYERCXX_LIBRARIES}
					 )


############## PPlan #############
ADD_EXECUTABLE( pplan pplan.cxx )
TARGET_LINK_LIBRARIES( pplan   gridmap
							   planner
							   misc
					 		   ${PLAYERCXX_LIBRARIES}
					 )

############## showmap #############
ADD_EXECUTABLE( showmap showmap.cxx )
TARGET_LINK_LIBRARIES( showmap datatrans 
							   gridmap
							   misc
					 		   ${PLAYERCXX_LIBRARIES}
					 )

############## dmtest  #############
ADD_EXECUTABLE( dmtest dmtest.cxx )
TARGET_LINK_LIBRARIES( dmtest mapbuilder
							   playerwrapper
							   bridge
							   gridmap
							   planner
							   misc
					 		   ${PLAYERCXX_LIBRARIES}
					 )

############## addsingle  #############
ADD_EXECUTABLE( addsingle addsingle.cxx )
TARGET_LINK_LIBRARIES( addsingle mapbuilder
							   	 playerwrapper
							   	 bridge
							   	 gridmap
							   	 planner
							   	 misc
					 		   	 ${PLAYERCXX_LIBRARIES}
					 )

############## cleartest #############
ADD_EXECUTABLE( cleartest cleartest.cxx )
TARGET_LINK_LIBRARIES( cleartest mapbuilder
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

############## Function Wrapper ################
ADD_EXECUTABLE( funcwrapper funcwrapper.cxx )

############## Debug Test ######################
ADD_EXECUTABLE( dtest dtest.cxx )
