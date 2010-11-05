#if !defined(__MODEL_PROXY_H)
#define __MODEL_PROXY_H

#include <rtm/InPortBase.h>
#include <rtm/OutPortBase.h>
#include <rtm/CorbaPort.h>
#include <stage.hh>
#include <stdexcept>
#include <string>

#include "idl/stage_services.hh"

class RTCStage;

namespace ModelProxies
{

// GetGeometry2D service provider
class GetGeomProvider
    : public virtual POA_RTC::GetGeometry2D,
    public virtual PortableServer::RefCountServantBase
{
    public:
        GetGeomProvider(Stg::Model* model);
        virtual ~GetGeomProvider();

        RTC::Geometry2D GetGeometry() throw (CORBA::SystemException);

    private:
        Stg::Model* _model;
};


// ModelProxy base class
class ModelProxy
{
    public:
        ModelProxy(Stg::Model* model);
        virtual ~ModelProxy();

        virtual void AddPorts(RTCStage &comp);
        virtual void Update(double &time) = 0;

        std::string GetName() const;

    protected:
        GetGeomProvider _get_geom_prov;
        RTC::CorbaPort _svc_port;
        Stg::Model* _model;
        double _last_time;

        std::string PrefixName(std::string port_name, Stg::Model* model);
}; // class ModelProxy


// Model type retriever type
typedef std::string (*GetModelTypeFn)();
// Model constructor type
typedef ModelProxy* (*ModelConstructor)(Stg::Model* model);

}; // namespace ModelProxies


// Default model constructor
template<class T>
ModelProxies::ModelProxy* DefaultModelConstructor(Stg::Model* model)
{
    return new T(model);
}

#endif // __MODEL_PROXY_H

