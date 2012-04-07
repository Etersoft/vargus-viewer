#include "camera.h"

Camera* Camera::copy()
{
    Camera *res = new Camera();
    res->cam_name=cam_name;
    res->cam_description = cam_description;
    res->cam_source=cam_source;
    res->cam_preview = cam_preview;
    res->cam_agent = cam_agent;
    return res;
}
