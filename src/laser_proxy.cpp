/* RTC:Stage
 *
 * Laser proxy source file.
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
#include <rtcstage/laser_proxy.h>

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

