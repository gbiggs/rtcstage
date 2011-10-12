/* RTC:Stage
 *
 * Gripper proxy source file.
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
#include <rtcstage/gripper_proxy.h>

#include <iostream>

using namespace ModelProxies;

//////////////////////////////////////////////////////////////////////////////
// Gripper control service provider class

GripperControlProvider::GripperControlProvider(Stg::ModelGripper *model)
    : _gripper_model(model)
{
}


GripperControlProvider::~GripperControlProvider()
{
}


void GripperControlProvider::Open() throw (CORBA::SystemException)
{
    _gripper_model->CommandOpen();
}


void GripperControlProvider::Close() throw (CORBA::SystemException)
{
    _gripper_model->CommandClose();
}


//////////////////////////////////////////////////////////////////////////////
// Gripper model proxy class
//
// Provides a proxy to a position model in the simulation.

GripperProxy::GripperProxy(Stg::Model *model)
    : ModelProxy(model),
    _state_port("state", _state),
    _gc_prov(reinterpret_cast<Stg::ModelGripper*>(model)),
    _gripper_model(reinterpret_cast<Stg::ModelGripper*>(model))
{
    _state_port.setName(PrefixName("state", model).c_str());
}


GripperProxy::~GripperProxy()
{
}


void GripperProxy::AddPorts(RTCStage &comp)
{
    comp.addOutPort(_state_port.getName(), _state_port);
    _svc_port.registerProvider("GripperControl", "GripperControl", _gc_prov);
    ModelProxy::AddPorts(comp);
}


void GripperProxy::Update(double &time)
{
    if ((time - _last_time) / 1e6 >= 0.1)
    {
        _state.tm.sec = static_cast<int>(floor(time / 1e6));
        _state.tm.nsec = static_cast<int>(rint(fmod(time, 1e9) * 1e9));
        switch (_gripper_model->GetConfig().paddles)
        {
            case Stg::ModelGripper::PADDLE_OPEN:
                _state.status = RTC::GRIPPER_STATE_OPEN;
                break;
            case Stg::ModelGripper::PADDLE_CLOSED:
                _state.status = RTC::GRIPPER_STATE_CLOSED;
                break;
            case Stg::ModelGripper::PADDLE_OPENING:
                _state.status = RTC::GRIPPER_STATE_MOVING;
                break;
            case Stg::ModelGripper::PADDLE_CLOSING:
                _state.status = RTC::GRIPPER_STATE_MOVING;
                break;
            default:
                _state.status = RTC::GRIPPER_STATE_UNKNOWN;
                break;
        }
        _state_port.write();

        _last_time = time;
    }
}

