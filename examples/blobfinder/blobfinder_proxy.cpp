#include <comp/rtcstage.h>
#include "blobfinder_proxy.h"

//////////////////////////////////////////////////////////////////////////////
// Blobfinder model proxy class
//
// Provides a proxy to a blobfinder model in the simulation.

BlobfinderProxy::BlobfinderProxy(Stg::Model *model)
    : ModelProxies::ModelProxy(model),
    _blobs_port("blobs", _blobs),
    _bf_model(reinterpret_cast<Stg::ModelBlobfinder*>(model))
{
    _blobs_port.setName(PrefixName("blobs", model).c_str());
}


BlobfinderProxy::~BlobfinderProxy()
{
}


void BlobfinderProxy::AddPorts(RTCStage &comp)
{
    comp.addOutPort(_blobs_port.getName(), _blobs_port);
    ModelProxies::ModelProxy::AddPorts(comp);
}


void BlobfinderProxy::Update(double &time)
{
    if ((time - _last_time) / 1e6 >= 0.1)
    {
        std::vector<Stg::ModelBlobfinder::Blob> blobs = _bf_model->GetBlobs();
        _blobs.length(blobs.size());
        for (std::vector<Stg::ModelBlobfinder::Blob>::size_type ii = 0;
                ii < blobs.size(); ii++)
        {
            _blobs[ii].colour.r = 0.0;
            _blobs[ii].colour.g = 0.0;
            _blobs[ii].colour.b = 0.0;
            _blobs[ii].colour.a = 0.0;
            _blobs[ii].left = 0;
            _blobs[ii].right = 0;
            _blobs[ii].top = 0;
            _blobs[ii].bottom = 0;
            _blobs[ii].range = 0.0;
        }
        _blobs_port.write();

        _last_time = time;
    }
}


std::string GetProxyType()
{
    return "blobfinder";
}


ModelProxies::ModelProxy* ProxyFactory(Stg::Model* model)
{
    return new BlobfinderProxy(model);
}

