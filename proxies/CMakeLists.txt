set(proxy_headers actuator_proxy.h
                  camera_proxy.h
                  fiducial_proxy.h
                  gripper_proxy.h
                  laser_proxy.h
                  model_proxy.h
                  position_proxy.h
                  ranger_proxy.h)

install(FILES ${proxy_headers} DESTINATION ${RTCS_INCLUDE_INSTALL_DIR}/proxies
    COMPONENT headers)

