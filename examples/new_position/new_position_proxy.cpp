#include <comp/rtcstage.h>
#include "new_position_proxy.h"

using namespace ModelProxies;

//////////////////////////////////////////////////////////////////////////////
// New position model proxy class

NewPositionProxy::NewPositionProxy(Stg::Model *model)
    : ModelProxy(model),
    _vel_control_port("vel_control", _vel_control),
    _odometry_port("odometry", _odometry),
    _pos_model(reinterpret_cast<Stg::ModelPosition*>(model))
{
    _vel_control_port.setName(PrefixName("vel_control", model).c_str());
    _odometry_port.setName(PrefixName("odometry", model).c_str());
}


NewPositionProxy::~NewPositionProxy()
{
}


void NewPositionProxy::AddPorts(RTCStage &comp)
{
    comp.addInPort(_vel_control_port.getName(), _vel_control_port);
    comp.addOutPort(_odometry_port.getName(), _odometry_port);
    ModelProxy::AddPorts(comp);
}


void NewPositionProxy::Update(double &time)
{
    if (_vel_control_port.isNew())
    {
        _vel_control_port.read();
        _pos_model->SetSpeed(_vel_control.data.vx, _vel_control.data.vy,
                _vel_control.data.va);
    }

    if ((time - _last_time) / 1e6 >= 0.1)
    {
        _odometry.tm.sec = static_cast<int>(floor(time / 1e6));
        _odometry.tm.nsec = static_cast<int>(rint(fmod(time, 1e9) * 1e9));
        _odometry.data.position.x = _pos_model->est_pose.x;
        _odometry.data.position.y = _pos_model->est_pose.y;
        _odometry.data.position.z = 0.0;
        _odometry.data.orientation.r = 0.0;
        _odometry.data.orientation.p = 0.0;
        _odometry.data.orientation.y = _pos_model->est_pose.a;
        _odometry_port.write();

        _last_time = time;
    }
}


std::string GetProxyType()
{
    return "position";
}


ModelProxies::ModelProxy* ProxyFactory(Stg::Model* model)
{
    return new NewPositionProxy(model);
}

