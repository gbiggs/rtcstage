#include "ranger_proxy.h"
#include "comp/rtcstage.h"

#include <iostream>

using namespace ModelProxies;

//////////////////////////////////////////////////////////////////////////////
// Ranger model proxy class
//
// Provides a proxy to a ranger model in the simulation.

RangerProxy::RangerProxy(Stg::Model *model)
    : ModelProxy(model),
    _ranges_port("ranger:ranges", _ranges),
    _ranger_model(reinterpret_cast<Stg::ModelRanger*>(model))
{
    _ranges_port.setName(PrefixName("ranges", model).c_str());
}


RangerProxy::~RangerProxy()
{
}


void RangerProxy::AddPorts(RTCStage &comp)
{
    comp.addOutPort(_ranges_port.getName(), _ranges_port);
    ModelProxy::AddPorts(comp);
}


void RangerProxy::Update(double &time)
{
    if ((time - _last_time) / 1e6 >= 0.1)
    {
        _last_time = time;
    }
}

