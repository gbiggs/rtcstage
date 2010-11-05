#include "comp/rtcstage.h"
#include "actuator_proxy.h"

#include <iostream>

using namespace ModelProxies;

//////////////////////////////////////////////////////////////////////////////
// Actuator model proxy class
//
// Provides a proxy to a position model in the simulation.

ActuatorProxy::ActuatorProxy(Stg::Model *model)
    : ModelProxy(model),
    _vel_control_port("vel_control", _vel_control),
    _pos_control_port("pos_control", _pos_control),
    _state_port("state", _state),
    _current_vel_port("current_vel", _current_vel),
    _act_model(reinterpret_cast<Stg::ModelActuator*>(model))
{
    _vel_control_port.setName(PrefixName("vel_control", model).c_str());
    _pos_control_port.setName(PrefixName("pos_control", model).c_str());
    _state_port.setName(PrefixName("state", model).c_str());
    _current_vel_port.setName(PrefixName("current_vel", model).c_str());
}


ActuatorProxy::~ActuatorProxy()
{
}


void ActuatorProxy::AddPorts(RTCStage &comp)
{
    comp.addInPort(_vel_control_port.getName(), _vel_control_port);
    comp.addInPort(_pos_control_port.getName(), _pos_control_port);
    comp.addOutPort(_state_port.getName(), _state_port);
    comp.addOutPort(_current_vel_port.getName(), _current_vel_port);
    ModelProxy::AddPorts(comp);
}


void ActuatorProxy::Update(double &time)
{
    if (_vel_control_port.isNew())
    {
        _vel_control_port.read();
        _act_model->SetSpeed(_vel_control.data);
    }

    if (_pos_control_port.isNew())
    {
        _pos_control_port.read();
        _act_model->SetSpeed(_pos_control.data);
    }

    if ((time - _last_time) / 1e6 >= 0.1)
    {
        _state.tm.sec = static_cast<int>(floor(time / 1e6));
        _state.tm.nsec = static_cast<int>(rint(fmod(time, 1e9) * 1e9));
        _state.actuators.length(1);
        _state.actuators[0].position = _act_model->GetPosition();
        _state.actuators[0].speed = _act_model->GetSpeed();
        _state.actuators[0].accel = 0.0;
        _state.actuators[0].current = 0.0;
        if (_act_model->GetSpeed() != 0.0)
            _state.actuators[0].status = RTC::ACTUATOR_STATUS_MOVING;
        else
            _state.actuators[0].status = RTC::ACTUATOR_STATUS_IDLE;

        _current_vel.tm.sec = static_cast<int>(floor(time / 1e6));
        _current_vel.tm.nsec = static_cast<int>(rint(fmod(time, 1e9) * 1e9));
        _current_vel.data = _act_model->GetSpeed();

        _current_vel_port.write();
        _state_port.write();

        _last_time = time;
    }
}

