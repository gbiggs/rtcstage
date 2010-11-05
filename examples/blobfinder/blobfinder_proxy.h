#if !defined(__BLOBFINDER_PROXY_H)
#define __BLOBFINDER_PROXY_H

#include <proxies/model_proxy.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

#include "idl/bf_intf.hh"

class BlobfinderProxy : public ModelProxies::ModelProxy
{
    public:
        BlobfinderProxy(Stg::Model *model);
        ~BlobfinderProxy();

        void AddPorts(RTCStage &comp);
        void Update(double &time);

    private:
        RTC::BlobList _blobs;
        RTC::OutPort<RTC::BlobList> _blobs_port;
        Stg::ModelBlobfinder *_bf_model;
}; // class BlobfinderProxy


extern "C"
{
    std::string GetProxyType();
    ModelProxies::ModelProxy* ProxyFactory(Stg::Model* model);
};

#endif // __BLOBFINDER_PROXY_H

