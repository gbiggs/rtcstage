#if !defined(__RANGER_PROXY_H)
#define __RANGER_PROXY_H

#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

#include "model_proxy.h"

namespace ModelProxies
{

class RangerProxy : public ModelProxy
{
    public:
        RangerProxy(Stg::Model *model);
        ~RangerProxy();

        void AddPorts(RTCStage &comp);
        void Update(double &time);

    private:
        RTC::RangeData _ranges;
        RTC::OutPort<RTC::RangeData> _ranges_port;
        Stg::ModelRanger *_ranger_model;
}; // class RangerProxy

}; // namespace ModelProxies

#endif // __RANGER_PROXY_H

