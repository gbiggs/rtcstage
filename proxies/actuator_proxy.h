#if !defined(__ACTUATOR_PROXY_H)
#define __ACTUATOR_PROXY_H

#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

#include "model_proxy.h"

namespace ModelProxies
{

class ActuatorProxy : public ModelProxy
{
    public:
        ActuatorProxy(Stg::Model *model);
        ~ActuatorProxy();

        void AddPorts(RTCStage &comp);
        void Update(double &time);

    private:
        RTC::TimedDouble _vel_control;
        RTC::InPort<RTC::TimedDouble> _vel_control_port;
        RTC::TimedDouble _pos_control;
        RTC::InPort<RTC::TimedDouble> _pos_control_port;
        RTC::ActArrayState _state;
        RTC::OutPort<RTC::ActArrayState> _state_port;
        RTC::TimedDouble _current_vel;
        RTC::OutPort<RTC::TimedDouble> _current_vel_port;
        Stg::ModelActuator *_act_model;
}; // class ActuatorProxy

}; // namespace ModelProxies

#endif // __ACTUATOR_PROXY_H

