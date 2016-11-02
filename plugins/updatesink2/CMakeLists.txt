include_directories(${CMAKE_SOURCE_DIR}/lib ${include_dirs})

set(updatesink2_headers updatesink2.h)
set(updatesink2_sources updatesink2.cpp)

add_library(updatesink2 MODULE ${updatesink2_sources})
set_target_properties(updatesink2 PROPERTIES PREFIX "")
target_link_libraries(updatesink2 amb amb-plugins-common -L${CMAKE_CURRENT_BINARY_DIR}/lib ${link_libraries})

install(TARGETS updatesink2 LIBRARY DESTINATION ${PLUGIN_INSTALL_PATH})
