/* RTC:Stage
 *
 * Camera proxy header file.
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


#if !defined(__CAMERA_PROXY_H)
#define __CAMERA_PROXY_H

#include <rtcstage/model_proxy.h>

#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

namespace ModelProxies
{

class CameraProxy : public ModelProxy
{
    public:
        CameraProxy(Stg::Model *model);
        ~CameraProxy();

        void AddPorts(RTCStage &comp);
        void Update(double &time);

    private:
        RTC::TimedPoint2D _control;
        RTC::InPort<RTC::TimedPoint2D> _control_port;
        RTC::CameraImage _image;
        RTC::OutPort<RTC::CameraImage> _image_port;
        RTC::CameraImage _depth;
        RTC::OutPort<RTC::CameraImage> _depth_port;
        Stg::ModelCamera *_camera_model;
}; // class CameraProxy

}; // namespace ModelProxies

#endif // __CAMERA_PROXY_H

