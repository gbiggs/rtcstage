/* RTC:Stage
 *
 * Model proxy header file.
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


#if !defined(__MODEL_PROXY_H)
#define __MODEL_PROXY_H

#include <rtcstage/idl/stage_services.hh>

#include <rtm/InPortBase.h>
#include <rtm/OutPortBase.h>
#include <rtm/CorbaPort.h>
#include <stage.hh>
#include <stdexcept>
#include <string>

class RTCStage;

namespace ModelProxies
{

// GetGeometry2D service provider
class GetGeomProvider
    : public virtual POA_RTC::GetGeometry2D,
    public virtual PortableServer::RefCountServantBase
{
    public:
        GetGeomProvider(Stg::Model* model);
        virtual ~GetGeomProvider();

        RTC::Geometry2D GetGeometry() throw (CORBA::SystemException);

    private:
        Stg::Model* _model;
};


// ModelProxy base class
class ModelProxy
{
    public:
        ModelProxy(Stg::Model* model);
        virtual ~ModelProxy();

        virtual void AddPorts(RTCStage &comp);
        virtual void Update(double &time) = 0;

        std::string GetName() const;

    protected:
        GetGeomProvider _get_geom_prov;
        RTC::CorbaPort _svc_port;
        Stg::Model* _model;
        double _last_time;

        std::string PrefixName(std::string port_name, Stg::Model* model);
}; // class ModelProxy


// Model type retriever type
typedef std::string (*GetModelTypeFn)();
// Model constructor type
typedef ModelProxy* (*ModelConstructor)(Stg::Model* model);

}; // namespace ModelProxies


// Default model constructor
template<class T>
ModelProxies::ModelProxy* DefaultModelConstructor(Stg::Model* model)
{
    return new T(model);
}

#endif // __MODEL_PROXY_H

