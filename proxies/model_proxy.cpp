#include "model_proxy.h"
#include "comp/rtcstage.h"

using namespace ModelProxies;


//////////////////////////////////////////////////////////////////////////////
// GetGeometry2D service provider class

GetGeomProvider::GetGeomProvider(Stg::Model *model)
    : _model(model)
{
}


GetGeomProvider::~GetGeomProvider()
{
}


RTC::Geometry2D GetGeomProvider::GetGeometry()
    throw (CORBA::SystemException)
{
    RTC::Geometry2D result;
    result.pose.position.x = _model->GetGeom().pose.x;
    result.pose.position.y = _model->GetGeom().pose.y;
    result.pose.heading = _model->GetGeom().pose.a;
    result.size.l = _model->GetGeom().size.x;
    result.size.w = _model->GetGeom().size.y;
    return result;
}


//////////////////////////////////////////////////////////////////////////////
// Model proxy class
//
// Provides a proxy to a model in the simulation, managing relevant input and
// output ports.

/// ModelProxy takes ownership of the object pointed to by @ref model.
ModelProxy::ModelProxy(Stg::Model *model)
    : _get_geom_prov(model), _svc_port("svc"),
    _model(model), _last_time(0.0)
{
    _svc_port.setName(PrefixName("svc", model).c_str());
}


ModelProxy::~ModelProxy()
{
    delete _model;
}


void ModelProxy::AddPorts(RTCStage &comp)
{
    _svc_port.registerProvider("GetGeometry2D", "GetGeometry2D",
            _get_geom_prov);
    comp.addPort(_svc_port);
}


std::string ModelProxy::PrefixName(std::string port_name, Stg::Model *model)
{
    std::string name = model->Token();
    std::string::size_type ii;
    while ((ii = name.find(".")) != std::string::npos)
        name.replace(ii, 1, "_");
    while ((ii = name.find(":")) != std::string::npos)
        name.replace(ii, 1, "_");
    return name + "_" + port_name;
}


std::string ModelProxy::GetName() const
{
    return std::string(_model->Token());
}

