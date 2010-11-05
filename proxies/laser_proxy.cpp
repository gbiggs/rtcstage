#include "laser_proxy.h"
#include "comp/rtcstage.h"

using namespace ModelProxies;

//////////////////////////////////////////////////////////////////////////////
// Laser model proxy class
//
// Provides a proxy to a laser model in the simulation.

LaserProxy::LaserProxy(Stg::Model *model)
    : ModelProxy(model),
    _ranges_port("ranges", _ranges),
    _intensities_port("intensities", _intensities),
    _laser_model(reinterpret_cast<Stg::ModelLaser*>(model))
{
    _ranges_port.setName(PrefixName("ranges", model).c_str());
    _intensities_port.setName(PrefixName("intensities", model).c_str());
}


LaserProxy::~LaserProxy()
{
}


void LaserProxy::AddPorts(RTCStage &comp)
{
    comp.addOutPort(_ranges_port.getName(), _ranges_port);
    comp.addOutPort(_intensities_port.getName(), _intensities_port);
    ModelProxy::AddPorts(comp);
}


void LaserProxy::Update(double &time)
{
    if ((time - _last_time) / 1e6 >= 0.1)
    {
        _ranges.tm.sec = static_cast<int>(floor(time / 1e6));
        _ranges.tm.nsec = static_cast<int>(rint(fmod(time, 1e9) * 1e9));
        _intensities.tm.sec = static_cast<int>(floor(time / 1e6));
        _intensities.tm.nsec = static_cast<int>(rint(fmod(time, 1e9) * 1e9));
        const std::vector<Stg::ModelLaser::Sample> &samples =
            _laser_model->GetSamples();
        _ranges.ranges.length(samples.size());
        _intensities.ranges.length(samples.size());
        for (unsigned int ii = 0; ii < samples.size(); ii++)
        {
            _ranges.ranges[ii] = samples[ii].range;
            _intensities.ranges[ii] = samples[ii].reflectance;
        }
        Stg::ModelLaser::Config cfg = _laser_model->GetConfig();
        _ranges.config.minAngle = -cfg.fov / 2.0;
        _ranges.config.maxAngle = cfg.fov / 2.0;
        _ranges.config.angularRes = cfg.fov / cfg.sample_count;
        _ranges.config.minRange = 0.0;
        _ranges.config.maxRange = 0.0;
        _ranges.config.rangeRes = 0.0;
        _ranges.config.frequency = 0.0;
        _ranges_port.write();
        _intensities_port.write();

        _last_time = time;
    }
}

