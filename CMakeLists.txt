cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from submodule
# note: this must happen before project()
include(pico-sdk/pico_sdk_init.cmake)

project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project

FILE(GLOB libft_files libft/src/*/*.c)
add_executable(picow_access_point_background
        ${libft_files}
        picow_access_point.c
        dhcpserver/dhcpserver.c
        dnsserver/dnsserver.c
        )

target_include_directories(picow_access_point_background PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/dhcpserver
        ${CMAKE_CURRENT_LIST_DIR}/dnsserver
        ${CMAKE_CURRENT_LIST_DIR}/libft
        )

target_link_libraries(picow_access_point_background
        pico_cyw43_arch_lwip_threadsafe_background
        pico_stdlib
        )

pico_add_extra_outputs(picow_access_point_background)

# add_executable(picow_access_point_poll
#         picow_access_point.c
#         dhcpserver/dhcpserver.c
#         dnsserver/dnsserver.c
#         )
# target_include_directories(picow_access_point_poll PRIVATE
#         ${CMAKE_CURRENT_LIST_DIR}
#         ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts
#         ${CMAKE_CURRENT_LIST_DIR}/dhcpserver
#         ${CMAKE_CURRENT_LIST_DIR}/dnsserver
#         )
# target_link_libraries(picow_access_point_poll
#         pico_cyw43_arch_lwip_poll
#         pico_stdlib
#         )
# pico_add_extra_outputs(picow_access_point_poll)
