add_library(tiny-avrlib INTERFACE)
target_include_directories(tiny-avrlib INTERFACE ${CMAKE_CURRENT_LIST_DIR}/lib)

add_subdirectory(ir-receiver)
