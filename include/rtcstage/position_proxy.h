/* RTC:Stage
 *
 * Position proxy header file.
 *
 * Copyright 2010-2011 Geoffrey Biggs geoffrey.biggs@aist.go.jp
 *     RT-Synthesis Research Group
 *     Intelligent Systems Research Institute,
 *     National Institute of Advanced Industrial Science and Technology (AIST),
 *     Japan
 *     All rights reserved.
 *
 * This file is part of RTCStage.
 *
 * RTCStage is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License,
 * or (at your option) any later version.
 *
 * RTCStage is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with RTCStage. If not, see
 * <http://www.gnu.org/licenses/>.
 */


#if !defined(__POSITION_PROXY_H)
#define __POSITION_PROXY_H

#include <rtcstage/model_proxy.h>
#include <rtcstage/idl/stage_services.hh>

#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>

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

