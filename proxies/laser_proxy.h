#if !defined(__LASER_PROXY_H)
#define __LASER_PROXY_H

#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

#include "model_proxy.h"

namespace ModelProxies
{

class LaserProxy : public ModelProxy
{
    public:
        LaserProxy(Stg::Model *model);
        ~LaserProxy();

        void AddPorts(RTCStage &comp);
        void Update(double &time);

    private:
        RTC::RangeData _ranges;
        RTC::OutPort<RTC::RangeData> _ranges_port;
        RTC::IntensityData _intensities;
        RTC::OutPort<RTC::IntensityData> _intensities_port;
        Stg::ModelLaser *_laser_model;
}; // class LaserProxy

}; // namespace ModelProxies

#endif // __LASER_PROXY_H

