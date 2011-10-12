/* RTC:Stage
 *
 * Position proxy source file.
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


#include <rtcstage/rtcstage.h>
#include <rtcstage/position_proxy.h>

#include <iostream>

using namespace ModelProxies;

//////////////////////////////////////////////////////////////////////////////
// SetOdometry2D service provider class

PositionSetOdomProvider::PositionSetOdomProvider(Stg::ModelPosition *model)
    : _pos_model(model)
{
}


PositionSetOdomProvider::~PositionSetOdomProvider()
{
}


void PositionSetOdomProvider::SetOdometry(const RTC::Pose2D &new_odometry)
    throw (CORBA::SystemException)
{
    Stg::Pose odom;
    odom.x = new_odometry.position.x;
    odom.y = new_odometry.position.y;
    odom.z = 0.0;
    odom.a = new_odometry.heading;
    _pos_model->SetOdom(odom);
}


//////////////////////////////////////////////////////////////////////////////
// Position model proxy class
//
// Provides a proxy to a position model in the simulation.

PositionProxy::PositionProxy(Stg::Model *model)
    : ModelProxy(model),
    _vel_control_port("vel_control", _vel_control),
    _pose_control_port("pose_control", _pose_control),
    _current_vel_port("current_vel", _current_vel),
    _odometry_port("odometry", _odometry),
    _set_odom_prov(reinterpret_cast<Stg::ModelPosition*>(model)),
    _pos_model(reinterpret_cast<Stg::ModelPosition*>(model))
{
    _vel_control_port.setName(PrefixName("vel_control", model).c_str());
    _pose_control_port.setName(PrefixName("pose_control", model).c_str());
    _current_vel_port.setName(PrefixName("current_vel", model).c_str());
    _odometry_port.setName(PrefixName("odometry", model).c_str());
}


PositionProxy::~PositionProxy()
{
}


void PositionProxy::AddPorts(RTCStage &comp)
{
    comp.addInPort(_vel_control_port.getName(), _vel_control_port);
    comp.addInPort(_pose_control_port.getName(), _pose_control_port);
    comp.addOutPort(_current_vel_port.getName(), _current_vel_port);
    comp.addOutPort(_odometry_port.getName(), _odometry_port);
    _svc_port.registerProvider("SetOdometry2D", "SetOdometry2D",
            _set_odom_prov);

    ModelProxy::AddPorts(comp);
}


void PositionProxy::Update(double &time)
{
    if (_vel_control_port.isNew())
    {
        _vel_control_port.read();
        _pos_model->SetSpeed(_vel_control.data.vx, _vel_control.data.vy,
                _vel_control.data.va);
    }

    if (_pose_control_port.isNew())
    {
        _pose_control_port.read();
        _pos_model->GoTo(_pose_control.data.position.x,
                _pose_control.data.position.y, _pose_control.data.heading);
    }

    if ((time - _last_time) / 1e6 >= 0.1)
    {
        _odometry.tm.sec = static_cast<int>(floor(time / 1e6));
        _odometry.tm.nsec = static_cast<int>(rint(fmod(time, 1e9) * 1e9));
        _odometry.data.position.x = _pos_model->est_pose.x;
        _odometry.data.position.y = _pos_model->est_pose.y;
        _odometry.data.heading = _pos_model->est_pose.a;

        _current_vel.tm.sec = static_cast<int>(floor(time / 1e6));
        _current_vel.tm.nsec = static_cast<int>(rint(fmod(time, 1e9) * 1e9));
        _current_vel.data.vx = _pos_model->GetGlobalVelocity().x;
        _current_vel.data.vy = _pos_model->GetGlobalVelocity().y;
        _current_vel.data.va = _pos_model->GetGlobalVelocity().a;

        _odometry_port.write();
        _current_vel_port.write();

        _last_time = time;
    }
}

