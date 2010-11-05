#include "rtcstage.h"
#include "proxies/actuator_proxy.h"
#include "proxies/camera_proxy.h"
#include "proxies/fiducial_proxy.h"
#include "proxies/gripper_proxy.h"
#include "proxies/laser_proxy.h"
#include "proxies/position_proxy.h"
#include "proxies/ranger_proxy.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>


// Functor used to update each proxy in a vector
template<class T>
struct UpdateProxy : public std::unary_function<T, void>
{
    UpdateProxy(double time)
        : _time(time)
    {}

    void operator() (T &rhs)
    {
        rhs->Update(_time);
    }

    double _time;
};


RTCStage::RTCStage(RTC::Manager* manager)
    : RTC::DataFlowComponentBase(manager)
{
}


RTCStage::~RTCStage()
{
    if (_world != NULL)
        delete _world;
}


RTC::ReturnCode_t RTCStage::onInitialize()
{
    bindParameter("world_file", _world_file,
            "/usr/share/stage/worlds/simple.world");
    bindParameter("gui_x", _gui_x, "400");
    bindParameter("gui_y", _gui_y, "300");
    bindParameter("limit_models", _only_models, "");
    bindParameter("plugins", _plugins, "");
    std::string active_set =
        m_properties.getProperty("configuration.active_config", "default");
    m_configsets.update(active_set.c_str());

    lt_dlinit();

    // Initialise Stage
    int argc = 0;
    Stg::Init(&argc, NULL);
    _world = new Stg::WorldGui(_gui_x, _gui_y, "RTC::Stage");
    std::cerr << "Loading world file " << _world_file << '\n';
    _world->Load(_world_file.c_str());

    try
    {
        // Initialise the model map
        ConsMapT cons_map = InitConsMap();
        cons_map = LoadPlugins(cons_map);
        // Create the model proxies
        InitProxies(cons_map);
    }
    catch (std::exception& e)
    {
        std::cerr << "Error initialising model proxies: " << e.what() <<
            std::endl;
        return RTC::RTC_ERROR;
    }

    return RTC::RTC_OK;
}

RTC::ReturnCode_t RTCStage::onFinalize()
{
    delete _world;
    _world = NULL;

    lt_dlexit();

    return RTC::RTC_OK;
}


RTC::ReturnCode_t RTCStage::onActivated(RTC::UniqueId ec_id)
{
    _world->Start();

    return RTC::RTC_OK;
}


RTC::ReturnCode_t RTCStage::onDeactivated(RTC::UniqueId ec_id)
{
    _world->Stop();

    return RTC::RTC_OK;
}


RTC::ReturnCode_t RTCStage::onExecute(RTC::UniqueId ec_id)
{
    // One round of FLTK's update loop.
    Fl::wait();

    // Update the proxies
    double current_time = _world->SimTimeNow();
    std::for_each(_proxies.begin(), _proxies.end(),
            UpdateProxy<ModelProxies::ModelProxy*>(current_time));

    return RTC::RTC_OK;
}


ConsMapT RTCStage::InitConsMap() const
{
    ConsMapT cons_map;
    // Add the default models
    cons_map["actuator"] =
        DefaultModelConstructor<ModelProxies::ActuatorProxy>;
    cons_map["camera"] =
        DefaultModelConstructor<ModelProxies::CameraProxy>;
    cons_map["fiducial"] =
        DefaultModelConstructor<ModelProxies::FiducialProxy>;
    cons_map["gripper"] =
        DefaultModelConstructor<ModelProxies::GripperProxy>;
    cons_map["laser"] =
        DefaultModelConstructor<ModelProxies::LaserProxy>;
    cons_map["position"] =
        DefaultModelConstructor<ModelProxies::PositionProxy>;
    cons_map["ranger"] =
        DefaultModelConstructor<ModelProxies::RangerProxy>;

    return cons_map;
}


void RTCStage::InitProxies(ConsMapT &cons_map)
{
    std::stringstream ss(_only_models);
    std::string search;
    std::vector<std::string> searches;
    if (!_only_models.empty())
    {
        while (std::getline(ss, search, ','))
        {
            searches.push_back(search);
        }
    }

    const std::set<Stg::Model*> models = _world->GetAllModels();
    for (std::set<Stg::Model*>::const_iterator itr = models.begin();
            itr != models.end(); itr++)
    {
        // Check if this model should be included
        if (!searches.empty())
        {
            std::string name((*itr)->Token());
            if (!AcceptModel(searches, name))
                continue;
        }
        // Search for the model type in the constructor map
        std::string type = (*itr)->GetModelType();
        ConsMapT::const_iterator cons = cons_map.find(type);
        // If not found, ignore it
        if (cons == cons_map.end())
            continue;
        // If found, use the constructor.
        ModelProxies::ModelProxy *proxy = (*cons).second(*itr);
        proxy->AddPorts(*this);
        _proxies.push_back(proxy);
    }
}


bool RTCStage::AcceptModel(std::vector<std::string> &searches,
        std::string &name) const
{
    for (std::vector<std::string>::const_iterator ii =
            searches.begin(); ii != searches.end(); ii++)
    {
        if ((*ii)[0] == '*')
        {
            if ((*ii)[(*ii).size() -1] == '*')
            {
                // Search in the middle of the name
                if (name.find(ii->substr(1, ii->size() - 2)) != std::string::npos)
                    return true;
            }
            else
            {
                // Search at the end of the name
                if (name.compare(name.size() - ii->size() + 1,
                            std::string::npos, *ii, 1,
                            std::string::npos) == 0)
                    return true;
            }
        }
        else
        {
            if ((*ii)[(*ii).size() -1] == '*')
            {
                // Search at the start of the name
                if (name.compare(0, ii->size() - 1, *ii, 0, ii->size() - 1) == 0)
                    return true;
            }
            else
            {
                // Match the whole name
                if (name.compare(*ii) == 0)
                    return true;
            }
        }
    }

    return false;
}


ConsMapT RTCStage::LoadPlugins(const ConsMapT &cons_map)
{
    ConsMapT new_cons_map(cons_map);

    std::stringstream ss(_plugins);
    std::string plugin;
    std::vector<std::string> plugins;
    if (_plugins.empty())
        return cons_map;

    // Split the list of plugins
    while (std::getline(ss, plugin, ','))
    {
        plugins.push_back(plugin);
    }

    for (std::vector<std::string>::const_iterator ii = plugins.begin();
            ii != plugins.end(); ii++)
    {
        // Open the plugin shared object
        lt_dlhandle handle = lt_dlopenext(ii->c_str());
        if (handle == NULL)
        {
            std::stringstream ss;
            ss << ii->c_str() << ": " << lt_dlerror();
            throw PluginLoadFailed(ss.str());
        }

        // Get the function pointers
        ModelProxies::GetModelTypeFn type_func =
            reinterpret_cast<ModelProxies::GetModelTypeFn>(lt_dlsym(handle,
                        "GetProxyType"));
        if (type_func == NULL)
        {
            lt_dlclose(handle);
            std::stringstream ss;
            ss << ii->c_str() << ": GetProxyType: " << lt_dlerror();
            throw MissingSymbol(ss.str());
        }
        ModelProxies::ModelConstructor get_cons =
            reinterpret_cast<ModelProxies::ModelConstructor>(lt_dlsym(handle,
                        "ProxyFactory"));
        if (get_cons == NULL)
        {
            lt_dlclose(handle);
            std::stringstream ss;
            ss << ii->c_str() << ": ProxyFactory: " << lt_dlerror();
            throw MissingSymbol(ss.str());
        }

        // Retrieve the type and constructor, and place them into the map
        new_cons_map[(*type_func)()] = get_cons;
    }

    return new_cons_map;
}


static const char* spec[] =
{
    "implementation_id", "RTC_Stage",
    "type_name",         "RTC_Stage",
    "description",       "Stage simulator interface",
    "version",           "1.0",
    "vendor",            "Geoffrey Biggs, AIST",
    "category",          "Simulation",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.world_file", "",
    "conf.default.gui_x", "400",
    "conf.default.gui_y", "300",
    "conf.default.limit_models", "",
    "conf.default.plugins", "",
    // Widget
    "conf.__widget__.world_file", "text",
    "conf.__widget__.gui_x", "spin",
    "conf.__widget__.gui_y", "spin",
    // Constraints
    ""
};

extern "C"
{
    void rtcstage_init(RTC::Manager* manager)
    {
        coil::Properties profile(spec);
        manager->registerFactory(profile, RTC::Create<RTCStage>,
                RTC::Delete<RTCStage>);
    }
};

