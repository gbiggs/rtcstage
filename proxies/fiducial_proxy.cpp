#include "comp/rtcstage.h"
#include "fiducial_proxy.h"

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
            _fiducials.fiducialsList[ii].pose.position.x = fids[ii].pose_rel.x;
            _fiducials.fiducialsList[ii].pose.position.y = fids[ii].pose_rel.y;
            _fiducials.fiducialsList[ii].pose.position.z = fids[ii].pose_rel.z;
            _fiducials.fiducialsList[ii].pose.orientation.r = 0.0;
            _fiducials.fiducialsList[ii].pose.orientation.p = 0.0;
            _fiducials.fiducialsList[ii].pose.orientation.y =
                fids[ii].pose_rel.a;
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

