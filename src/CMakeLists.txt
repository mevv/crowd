file (GLOB HEADER_FILES *.h)
file (GLOB CPP_FILES *.cpp)

set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable (${CMAKE_PROJECT_NAME} ${HEADER_FILES} ${CPP_FILES})
target_link_libraries (${CMAKE_PROJECT_NAME} Qt5::Core Qt5::Widgets)
