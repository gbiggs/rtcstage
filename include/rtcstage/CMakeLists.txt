set(hdrs rtcstage.h
    actuator_proxy.h
    camera_proxy.h
    fiducial_proxy.h
    gripper_proxy.h
    model_proxy.h
    position_proxy.h
    ranger_proxy.h)

install(FILES ${hdrs} DESTINATION ${INC_INSTALL_DIR}/${PROJECT_NAME_LOWER}
    COMPONENT library)

