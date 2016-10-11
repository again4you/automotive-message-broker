set (client_src libamb-client.c)
set (client_headers libamb-client.h libamb-util.h libamb-objects.h libamb-common.h)

add_library(amb-client SHARED ${client_src})
set_target_properties(amb-client PROPERTIES SOVERSION ${PROJECT_VERSION} COMPILE_FLAGS "-fvisibility=hidden")
target_link_libraries(amb-client ${glib_LIBRARIES} ${gio_LIBRARIES})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/amb-client.pc.in ${CMAKE_CURRENT_BINARY_DIR}/amb-client.pc @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/amb-client.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
install (FILES ${client_headers} DESTINATION ${INCLUDE_INSTALL_DIR}/amb-client COMPONENT Devel)
install (TARGETS amb-client LIBRARY DESTINATION ${LIB_INSTALL_DIR})
