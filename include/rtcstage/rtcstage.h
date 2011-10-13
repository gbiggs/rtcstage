/* RTC:Stage
 *
 * Header file for the component.
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


#ifndef STAGE_H__
#define STAGE_H__

#include <rtcstage/model_proxy.h>

#include <ltdl.h>
#include <map>
#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <stage.hh>
#include <vector>

using namespace RTC;


typedef std::map<std::string, ModelProxies::ModelConstructor> ConsMapT;


// Failed to load plugin exception
class PluginLoadFailed : public std::runtime_error
{
    public:
        PluginLoadFailed(const std::string &arg)
            : std::runtime_error(std::string("Failed to load plugin ") + arg)
        {}
};


// Missing symbol exception
class MissingSymbol : public std::runtime_error
{
    public:
        MissingSymbol(const std::string &arg)
            : std::runtime_error(std::string("Missing symbol in plugin ") + arg)
        {}
};


class RTCStage
: public RTC::DataFlowComponentBase
{
    public:
        RTCStage(RTC::Manager* manager);
        ~RTCStage();

        virtual RTC::ReturnCode_t onInitialize();
        virtual RTC::ReturnCode_t onFinalize();
        virtual RTC::ReturnCode_t onActivated(RTC::UniqueId ec_id);
        virtual RTC::ReturnCode_t onDeactivated(RTC::UniqueId ec_id);
        virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

    private:
        std::string _world_file;
        unsigned int _gui_x;
        unsigned int _gui_y;
        std::string _only_models;
        std::string _plugins;
        Stg::WorldGui *_world;
        std::vector<ModelProxies::ModelProxy*> _proxies;

        ConsMapT InitConsMap() const;
        void InitProxies(ConsMapT &cons_map);
        bool AcceptModel(std::vector<std::string> &searches,
                std::string &name) const;
        ConsMapT LoadPlugins(const ConsMapT &cons_map);
};


extern "C"
{
    DLL_EXPORT void rtcstage_init(RTC::Manager* manager);
};

#endif // STAGE_H__

