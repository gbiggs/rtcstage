#include "comp/rtcstage.h"
#include "camera_proxy.h"

#include <iostream>

using namespace ModelProxies;

//////////////////////////////////////////////////////////////////////////////
// Camera model proxy class
//
// Provides a proxy to a camera model in the simulation.

CameraProxy::CameraProxy(Stg::Model *model)
    : ModelProxy(model),
    _control_port("control", _control),
    _image_port("image", _image),
    _depth_port("depth", _depth),
    _camera_model(reinterpret_cast<Stg::ModelCamera*>(model))
{
    _control_port.setName(PrefixName("control", model).c_str());
    _image_port.setName(PrefixName("image", model).c_str());
    _depth_port.setName(PrefixName("depth", model).c_str());

    _image.bpp = 32;
    _image.format = "bitmap";
    _depth.bpp = 24;
    _depth.format = "bitmap";
}


CameraProxy::~CameraProxy()
{
}


void CameraProxy::AddPorts(RTCStage &comp)
{
    comp.addInPort(_control_port.getName(), _control_port);
    comp.addOutPort(_image_port.getName(), _image_port);
    comp.addOutPort(_depth_port.getName(), _depth_port);
    ModelProxy::AddPorts(comp);
}


void CameraProxy::Update(double &time)
{
    if (_control_port.isNew())
    {
        _control_port.read();
        _camera_model->setPitch(_control.data.y);
        _camera_model->setYaw(_control.data.x);
    }

    if ((time - _last_time) / 1e6 >= 0.1)
    {
        _image.tm.sec = static_cast<int>(floor(time / 1e6));
        _image.tm.nsec = static_cast<int>(rint(fmod(time, 1e9) * 1e9));
        _depth.tm.sec = static_cast<int>(floor(time / 1e6));
        _depth.tm.nsec = static_cast<int>(rint(fmod(time, 1e9) * 1e9));

        _image.width = _camera_model->getWidth();
        _image.height = _camera_model->getHeight();
        unsigned int size = _image.width * _image.height * 4;
        _image.pixels.length(size);
        const GLubyte *frame = _camera_model->FrameColor();
        for (unsigned int ii = 0; ii < size; ii++)
            _image.pixels[ii] = frame[ii];

        _depth.width = _camera_model->getWidth();
        _depth.height = _camera_model->getHeight();
        size = _depth.width * _depth.height;
        _depth.pixels.length(size);
        const GLfloat *depth = _camera_model->FrameDepth();
        for (unsigned int ii = 0; ii < size; ii++)
        {
            _depth.pixels[ii] = depth[ii] * 100.0;
        }

        _image_port.write();
        _depth_port.write();

        _last_time = time;
    }
}

