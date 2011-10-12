/* RTC:Stage
 *
 * Gripper proxy header file.
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


#if !defined(__GRIPPER_PROXY_H)
#define __GRIPPER_PROXY_H

#include <rtcstage/model_proxy.h>
#include <rtcstage/idl/stage_services.hh>

#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

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

