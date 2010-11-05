#ifndef STAGE_H
#define STAGE_H

#include <ltdl.h>
#include <map>
#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <stage.hh>
#include <vector>

#include "proxies/model_proxy.h"

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
        Stg::ModelLaser *_ranger_model;

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

#endif // STAGE_H
