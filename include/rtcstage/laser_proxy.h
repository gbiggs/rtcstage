/* RTC:Stage
 *
 * Laser proxy header file.
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


#if !defined(__LASER_PROXY_H)
#define __LASER_PROXY_H

#include <rtcstage/model_proxy.h>

#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

namespace ModelProxies
{

class LaserProxy : public ModelProxy
{
    public:
        LaserProxy(Stg::Model *model);
        ~LaserProxy();

        void AddPorts(RTCStage &comp);
        void Update(double &time);

    private:
        RTC::RangeData _ranges;
        RTC::OutPort<RTC::RangeData> _ranges_port;
        RTC::IntensityData _intensities;
        RTC::OutPort<RTC::IntensityData> _intensities_port;
        Stg::ModelLaser *_laser_model;
}; // class LaserProxy

}; // namespace ModelProxies

#endif // __LASER_PROXY_H

