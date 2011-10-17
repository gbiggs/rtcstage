set(stage_conf_in ${CMAKE_CURRENT_SOURCE_DIR}/stage.conf.in)
set(stage_conf ${CMAKE_CURRENT_BINARY_DIR}/stage.conf)
configure_file(${stage_conf_in} ${stage_conf})

set(example_conf_files rtc.conf
                       ${stage_conf})

install(FILES ${example_conf_files}
    DESTINATION ${SHARE_INSTALL_DIR}/examples/conf COMPONENT examples)

