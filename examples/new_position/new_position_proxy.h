#if !defined(__NEW_POSITION_PROXY_H)
#define __NEW_POSITION_PROXY_H

#include <proxies/model_proxy.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>

class NewPositionProxy : public ModelProxies::ModelProxy
{
    public:
        NewPositionProxy(Stg::Model *model);
        ~NewPositionProxy();

        void AddPorts(RTCStage &comp);
        void Update(double &time);

    private:
        RTC::TimedVelocity3D _vel_control;
        RTC::InPort<RTC::TimedVelocity3D> _vel_control_port;
        RTC::TimedPose3D _odometry;
        RTC::OutPort<RTC::TimedPose3D> _odometry_port;
        Stg::ModelPosition *_pos_model;
}; // class NewPositionProxy


extern "C"
{
    std::string GetProxyType();
    ModelProxies::ModelProxy* ProxyFactory(Stg::Model* model);
};

#endif // __NEW_POSITION_PROXY_H

