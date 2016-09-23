
# check that CAN interface is specified and use vcan0 by default                
if(NOT DEFINED ${DEFAULT_CAN_INTERFACE})
	set(DEFAULT_CAN_INTERFACE "vcan0")
endif(NOT DEFINED ${DEFAULT_CAN_INTERFACE})

pkg_check_modules(glib REQUIRED glib-2.0)
pkg_check_modules(json REQUIRED json)
#pkg_check_modules( amb REQUIRED automotive-message-broker ) 
#pkg_check_modules( amb-plugins-common REQUIRED amb-plugins-common )

#include_directories(${include_dirs} ${amb-plugins-common_INCLUDE_DIRS} ${amb_INCLUDE_DIRS} ${glib_INCLUDE_DIRS} ${gio_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${json_INCLUDE_DIRS} )
include_directories(${CMAKE_SOURCE_DIR}/lib ${include_dirs} ${CMAKE_SOURCE_DIR}/plugins/common ${glib_INCLUDE_DIRS} ${gio_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${json_INCLUDE_DIRS} )

set(samsungcan_plugin_headers samsungcan_plugin.h  samsungcan_cansignals.h samsungcan_cansignal.h)
set(samsungcan_plugin_sources samsungcan_plugin.cpp samsungcan_cansignal.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/samsungcan.in.json ${CMAKE_CURRENT_BINARY_DIR}/samsungcan @ONLY)

add_library(samsungcan_plugin MODULE ${samsungcan_plugin_sources} ${samsungcan_plugin_headers})
set_target_properties(samsungcan_plugin PROPERTIES PREFIX "")
# target_link_libraries(samsungcan_plugin ${link_libraries} ${libamb_LIBRARY} ${amb_LIBRARIES} ${amb-plugins-common_LIBRARIES})
target_link_libraries(samsungcan_plugin amb amb-plugins-common -L${CMAKE_CURRENT_BINARY_DIR}/lib ${link_libraries})

install(TARGETS samsungcan_plugin LIBRARY DESTINATION "lib/automotive-message-broker")
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/samsungcan DESTINATION ${PLUGIN_SEGMENT_INSTALL_PATH})
