#if !defined(__FIDUCIAL_PROXY_H)
#define __FIDUCIAL_PROXY_H

#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

#include "model_proxy.h"

namespace ModelProxies
{

class FiducialProxy : public ModelProxy
{
    public:
        FiducialProxy(Stg::Model *model);
        ~FiducialProxy();

        void AddPorts(RTCStage &comp);
        void Update(double &time);

    private:
        RTC::Fiducials _fiducials;
        RTC::OutPort<RTC::Fiducials> _fiducials_port;
        Stg::ModelFiducial *_fid_model;
}; // class FiducialProxy

}; // namespace ModelProxies

#endif // __FIDUCIAL_PROXY_H

