/* RTC:Stage
 *
 * Fiducial proxy source file.
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
#include <rtcstage/fiducial_proxy.h>

#include <iostream>

using namespace ModelProxies;

//////////////////////////////////////////////////////////////////////////////
// Fiducial model proxy class
//
// Provides a proxy to a fiducial model in the simulation.

FiducialProxy::FiducialProxy(Stg::Model *model)
    : ModelProxy(model),
    _fiducials_port("fiducials", _fiducials),
    _fid_model(reinterpret_cast<Stg::ModelFiducial*>(model))
{
    _fiducials_port.setName(PrefixName("fiducials", model).c_str());
}


FiducialProxy::~FiducialProxy()
{
}


void FiducialProxy::AddPorts(RTCStage &comp)
{
    comp.addOutPort(_fiducials_port.getName(), _fiducials_port);
    ModelProxy::AddPorts(comp);
}


void FiducialProxy::Update(double &time)
{
    if ((time - _last_time) / 1e6 >= 0.1)
    {
        _fiducials.tm.sec = static_cast<int>(floor(time / 1e6));
        _fiducials.tm.nsec = static_cast<int>(rint(fmod(time, 1e9) * 1e9));
        std::vector<Stg::ModelFiducial::Fiducial> &fids =
            _fid_model->GetFiducials();
        _fiducials.fiducialsList.length(fids.size());
        for (std::vector<Stg::ModelFiducial::Fiducial>::size_type ii = 0;
                ii < fids.size(); ii++)
        {
            _fiducials.fiducialsList[ii].id = fids[ii].id;
            _fiducials.fiducialsList[ii].pose.position.x = fids[ii].pose.x;
            _fiducials.fiducialsList[ii].pose.position.y = fids[ii].pose.y;
            _fiducials.fiducialsList[ii].pose.position.z = fids[ii].pose.z;
            _fiducials.fiducialsList[ii].pose.orientation.r = 0.0;
            _fiducials.fiducialsList[ii].pose.orientation.p = 0.0;
            _fiducials.fiducialsList[ii].pose.orientation.y =
                fids[ii].pose.a;
            _fiducials.fiducialsList[ii].poseUncertainty.position.x = 0.0;
            _fiducials.fiducialsList[ii].poseUncertainty.position.y = 0.0;
            _fiducials.fiducialsList[ii].poseUncertainty.position.z = 0.0;
            _fiducials.fiducialsList[ii].poseUncertainty.orientation.r = 0.0;
            _fiducials.fiducialsList[ii].poseUncertainty.orientation.p = 0.0;
            _fiducials.fiducialsList[ii].poseUncertainty.orientation.y = 0.0;
            _fiducials.fiducialsList[ii].size.l = 0.0;
            _fiducials.fiducialsList[ii].size.w = 0.0;
            _fiducials.fiducialsList[ii].size.h = 0.0;
            _fiducials.fiducialsList[ii].sizeUncertainty.l = 0.0;
            _fiducials.fiducialsList[ii].sizeUncertainty.w = 0.0;
            _fiducials.fiducialsList[ii].sizeUncertainty.h = 0.0;
        }
        _fiducials_port.write();

        _last_time = time;
    }
}

