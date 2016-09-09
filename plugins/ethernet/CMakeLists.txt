include_directories(${CMAKE_SOURCE_DIR}/lib ${include_dirs} ${gio_INCLUDE_DIRS} ${gio-unix_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/plugins/common)

set(ethernetplugin_headers ethernetplugin.h)
set(ethernetplugin_sources ethernetplugin.cpp)

add_library(ethernetplugin MODULE ${ethernetplugin_sources} ${ethernetplugin_headers})
set_target_properties(ethernetplugin PROPERTIES PREFIX "")
target_link_libraries(ethernetplugin amb amb-plugins-common -L${CMAKE_CURRENT_BINARY_DIR}/lib ${link_libraries} )

install(TARGETS ethernetplugin LIBRARY DESTINATION ${PLUGIN_INSTALL_PATH})
