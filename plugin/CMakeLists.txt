Project(ReflexControllersPlugin)

cmake_minimum_required(VERSION 2.6)
if(COMMAND cmake_policy)
        cmake_policy(SET CMP0003 NEW)
        cmake_policy(SET CMP0005 NEW)
endif(COMMAND cmake_policy)

SET(KIT reflexes)
SET(UKIT REFLEXES)

SET(NameSpace "OpenSim_" CACHE STRING 
   "Prefix for Simbody/SimTK lib names, includes trailing '_'. Set to '' to use stock libraries.")

FILE(GLOB SOURCE_FILES *.cpp)
FILE(GLOB INCLUDE_FILES *.h)

SET(CMAKE_CONFIGURATION_TYPES "Debug;RelWithDebInfo;Release"
      CACHE STRING "Semicolon separated list of supported configuration types, only supports Debug, Release, MinSizeRel, and RelWithDebInfo, anything else will be ignored." FORCE )

SET(EXPORT_MACRO OSIM${UKIT}_EXPORTS)

SET(OPENSIM_INSTALL_DIR $ENV{OPENSIM_HOME} 
		CACHE PATH "Top-level directory of OpenSim install")

SET(PLUGIN_NAME "ReflexControllersPlugin" CACHE STRING "Name of shared library to create")

### HEADERS
SET(OPENSIM_HEADERS_DIR ${OPENSIM_INSTALL_DIR}/sdk/include)
SET(SIMTK_HEADERS_DIR ${OPENSIM_INSTALL_DIR}/sdk/include/SimTK/include)

INCLUDE_DIRECTORIES(${OPENSIM_HEADERS_DIR} ${SIMTK_HEADERS_DIR})

### LIBRARIES
SET(OPENSIM_LIBS_DIR  ${OPENSIM_INSTALL_DIR}/sdk/lib)
LINK_DIRECTORIES(${OPENSIM_LIBS_DIR})

# SimmKinematicsEngine library only needed for SimmFileWriter...
LINK_LIBRARIES(
	debug osimCommon_d optimized osimCommon
	debug osimSimulation_d optimized osimSimulation
	debug osimAnalyses_d optimized osimAnalyses
	debug osimActuators_d optimized osimActuators
	debug osimTools_d optimized osimTools
	debug  ${NameSpace}SimTKcommon${CMAKE_DEBUG_POSTFIX} optimized   ${NameSpace}SimTKcommon
	debug  ${NameSpace}SimTKmath${CMAKE_DEBUG_POSTFIX} optimized  ${NameSpace}SimTKmath
	debug ${NameSpace}SimTKsimbody${CMAKE_DEBUG_POSTFIX} optimized  ${NameSpace}SimTKsimbody
)

ADD_LIBRARY(${PLUGIN_NAME} SHARED ${SOURCE_FILES} ${INCLUDE_FILES}) 

#IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	SET(CMAKE_INSTALL_PREFIX ${OPENSIM_INSTALL_DIR}/ CACHE PATH "Install path prefix." FORCE)
#ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

MARK_AS_ADVANCED(CMAKE_INSTALL_PREFIX)
MARK_AS_ADVANCED(EXECUTABLE_OUTPUT_PATH)
MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH)

SET_TARGET_PROPERTIES(
	${PLUGIN_NAME} PROPERTIES
	DEFINE_SYMBOL ${EXPORT_MACRO}
	PROJECT_LABEL "Libraries - ${PLUGIN_NAME}")

#INSTALL_TARGETS(/plugins RUNTIME_DIRECTORY /plugins  ${PLUGIN_NAME})
INSTALL(TARGETS ${PLUGIN_NAME} 
        RUNTIME DESTINATION ${OPENSIM_INSTALL_DIR}/plugins
        LIBRARY DESTINATION ${OPENSIM_INSTALL_DIR}/plugins
        ARCHIVE DESTINATION ${OPENSIM_LIBS_DIR}
        )

INSTALL(FILES ${INCLUDE_FILES} DESTINATION ${OPENSIM_HEADERS_DIR})

