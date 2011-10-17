set(install_dir ${SHARE_INSTALL_DIR}/examples/new_position)

set(example_cmake_in ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt.example.in)
set(example_cmake ${CMAKE_CURRENT_BINARY_DIR}/CMakeLists.txt.example)
configure_file(${example_cmake_in} ${example_cmake} @ONLY)
install(FILES ${example_cmake} DESTINATION ${install_dir}
    RENAME CMakeLists.txt COMPONENT examples)

set(example_srcs new_position_proxy.h
                 new_position_proxy.cpp)
install(FILES ${example_srcs} DESTINATION ${install_dir} COMPONENT examples)

