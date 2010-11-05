#if !defined(__CAMERA_PROXY_H)
#define __CAMERA_PROXY_H

#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

#include "model_proxy.h"

namespace ModelProxies
{

class CameraProxy : public ModelProxy
{
    public:
        CameraProxy(Stg::Model *model);
        ~CameraProxy();

        void AddPorts(RTCStage &comp);
        void Update(double &time);

    private:
        RTC::TimedPoint2D _control;
        RTC::InPort<RTC::TimedPoint2D> _control_port;
        RTC::CameraImage _image;
        RTC::OutPort<RTC::CameraImage> _image_port;
        RTC::CameraImage _depth;
        RTC::OutPort<RTC::CameraImage> _depth_port;
        Stg::ModelCamera *_camera_model;
}; // class CameraProxy

}; // namespace ModelProxies

#endif // __CAMERA_PROXY_H

