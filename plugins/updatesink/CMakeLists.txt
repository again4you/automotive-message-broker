include_directories(${CMAKE_SOURCE_DIR}/lib ${include_dirs})

set(updatesink_headers updatesink.h)
set(updatesink_sources updatesink.cpp)

add_library(updatesink MODULE ${updatesink_sources})
set_target_properties(updatesink PROPERTIES PREFIX "")
target_link_libraries(updatesink amb -L${CMAKE_CURRENT_BINARY_DIR}/lib ${link_libraries})

install(TARGETS updatesink LIBRARY DESTINATION ${PLUGIN_INSTALL_PATH})
