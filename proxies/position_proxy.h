#if !defined(__POSITION_PROXY_H)
#define __POSITION_PROXY_H

#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>

#include "model_proxy.h"
#include "idl/stage_services.hh"

namespace ModelProxies
{

class PositionSetOdomProvider
    : public virtual POA_RTC::SetOdometry2D,
    public virtual PortableServer::RefCountServantBase
{
    public:
        PositionSetOdomProvider(Stg::ModelPosition *model);
        virtual ~PositionSetOdomProvider();

        void SetOdometry(const RTC::Pose2D &new_odometry)
            throw (CORBA::SystemException);

    private:
        Stg::ModelPosition *_pos_model;
};


class PositionProxy : public ModelProxy
{
    public:
        PositionProxy(Stg::Model *model);
        ~PositionProxy();

        void AddPorts(RTCStage &comp);
        void Update(double &time);

    private:
        RTC::TimedVelocity2D _vel_control;
        RTC::InPort<RTC::TimedVelocity2D> _vel_control_port;
        RTC::TimedPose2D _pose_control;
        RTC::InPort<RTC::TimedPose2D> _pose_control_port;
        RTC::TimedVelocity2D _current_vel;
        RTC::OutPort<RTC::TimedVelocity2D> _current_vel_port;
        RTC::TimedPose2D _odometry;
        RTC::OutPort<RTC::TimedPose2D> _odometry_port;
        PositionSetOdomProvider _set_odom_prov;
        Stg::ModelPosition *_pos_model;
}; // class PositionProxy

}; // namespace ModelProxies

#endif // __POSITION_PROXY_H

