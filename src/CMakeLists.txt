set(BIN_NAME "mp4InformationAnalyzer")
	
cmake_minimum_required(VERSION 3.7)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(CMAKE_CXX_FLAGS "-Wall -g -Wno-unused-function -rdynamic -std=gnu++11 -O2 -L${PROJECT_ROOT_DIR}/target/lib -L$ENV{HOME}/release/CUR/lib")
	link_directories($ENV{HOME}/release/CUR/lib)
	add_definitions(-DBUILD_LINUX)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(CMAKE_CXX_FLAGS "-Wall -g -Wno-unused-function -rdynamic -Qunused-arguments -std=gnu++11 -O2 -L/usr/local/lib")
	add_definitions(-DBUILD_MACOS)
else()
	message(STATUS "ERROR, No Match System Name Found")
endif()


set(SRC_LIST
	mp4InformationAnalyzerMain.cpp
	ui/CMp4InformationAnalyzerUi.cpp
	ui/CMp4Binary.cpp
	ui/CMp4Structure.cpp
	ui/CMp4Information.cpp
	logger/CMp4AnalyzerLogger.cpp
	configuration/CMp4AnalyzerConfiguration.cpp
)

set(GLIB_LIB glib-2.0 gobject-2.0)
set(LOG4CPP_LIB log4cpp)
set(PTHREAD pthread)
set(JSONC_LIB json-c)

#set(UI gtk-3 gdk-3 cairo)
set(UI gtk-quartz-2.0 gdk_pixbuf-2.0)
set(LIB_LIST
	${GLIB_LIB}
	${LOG4CPP_LIB}
	${JSONC_LIB}
	${UI}
)

message(STATUS "SYSTEM : ${CMAKE_SYSTEM_NAME}")

include_directories(logger)
include_directories(ui)
include_directories(configuration)

include_directories(/usr/local/include/glib-2.0)	
include_directories(/usr/local/lib/glib-2.0/include)
include_directories(/usr/local/include)
include_directories(/usr/local/include/gtk-2.0)
include_directories(/usr/local/include/cairo)
include_directories(/usr/local/include/pango-1.0)
include_directories(/usr/local/include/gdk-pixbuf-2.0)
include_directories(/usr/local/include/atk-1.0)

add_definitions(-DUSE_COLOR_LOG)

add_executable(${BIN_NAME} ${SRC_LIST})
                  
target_link_libraries(
	${BIN_NAME}
	${LIB_LIST}
)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,${PROJECT_ROOT_DIR}/target/lib")

add_custom_command(TARGET ${BIN_NAME} 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_ROOT_DIR}/target/bin/${BIN_NAME} ${PROJECT_ROOT_DIR}/exe)
 
message(STATUS "CMAKE_SYSTEM_LIBRARY_PATH: ${CMAKE_SYSTEM_LIBRARY_PATH}")
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "CMAKE_LIBRARY_PATH: ${CMAKE_LIBRARY_PATH}")
message(STATUS "CMAKE_INCLUDE_PATH: ${CMAKE_INCLUDE_PATH}")
message(STATUS "CMAKE_CXX_FLAGS : ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS : ${CMAKE_EXE_LINKER_FLAGS}")
