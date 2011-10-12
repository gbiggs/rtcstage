/* RTC:Stage
 *
 * Ranger proxy source file.
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
#include <rtcstage/ranger_proxy.h>

#include <iostream>

using namespace ModelProxies;

//////////////////////////////////////////////////////////////////////////////
// Ranger model proxy class
//
// Provides a proxy to a ranger model in the simulation.

RangerProxy::RangerProxy(Stg::Model *model)
    : ModelProxy(model),
    _ranges_port("ranger:ranges", _ranges),
    _ranger_model(reinterpret_cast<Stg::ModelRanger*>(model))
{
    _ranges_port.setName(PrefixName("ranges", model).c_str());
}


RangerProxy::~RangerProxy()
{
}


void RangerProxy::AddPorts(RTCStage &comp)
{
    comp.addOutPort(_ranges_port.getName(), _ranges_port);
    ModelProxy::AddPorts(comp);
}


void RangerProxy::Update(double &time)
{
    if ((time - _last_time) / 1e6 >= 0.1)
    {
        _last_time = time;
    }
}

