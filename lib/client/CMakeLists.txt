set (client_src libdbus-client.c)
set (client_headers libdbus-client.h)

add_library(ambd-client SHARED ${client_src})
target_link_libraries(ambd-client ${glib_LIBRARIES} ${gio_LIBRARIES})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ambd-client.pc.in ${CMAKE_CURRENT_BINARY_DIR}/ambd-client.pc @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/ambd-client.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
install (FILES ${client_headers} DESTINATION ${INCLUDE_INSTALL_DIR}/ambd-client COMPONENT Devel)
install (TARGETS ambd-client LIBRARY DESTINATION ${LIB_INSTALL_DIR})
