#if !defined(__GRIPPER_PROXY_H)
#define __GRIPPER_PROXY_H

#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

#include "model_proxy.h"
#include "idl/stage_services.hh"

namespace ModelProxies
{

class GripperControlProvider
    : public virtual POA_RTC::GripperControl,
    public virtual PortableServer::RefCountServantBase
{
    public:
        GripperControlProvider(Stg::ModelGripper *model);
        virtual ~GripperControlProvider();

        void Open() throw (CORBA::SystemException);
        void Close() throw (CORBA::SystemException);

    private:
        Stg::ModelGripper *_gripper_model;
};


class GripperProxy : public ModelProxy
{
    public:
        GripperProxy(Stg::Model *model);
        ~GripperProxy();

        void AddPorts(RTCStage &comp);
        void Update(double &time);

    private:
        RTC::GripperState _state;
        RTC::OutPort<RTC::GripperState> _state_port;
        GripperControlProvider _gc_prov;
        Stg::ModelGripper *_gripper_model;
}; // class GripperProxy

}; // namespace ModelProxies

#endif // __GRIPPER_PROXY_H

