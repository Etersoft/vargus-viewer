#include "videowidgetdragdata.h"

VideoWidgetDragData::VideoWidgetDragData(Camera *_camera, VideoWidget *_videowidget)
{
    camera = _camera;
    videowidget = _videowidget;
    typeDrag = DRAG_FROM_VIDEO_WIDGET;
}

VideoWidgetDragData::VideoWidgetDragData(Camera *_camera)
{
    camera = _camera;
    typeDrag = DRAG_FROM_LIST;
}


VideoWidgetDragData::~VideoWidgetDragData()
{
}

Camera *VideoWidgetDragData::getCamera()
{
    return camera;
}

VideoWidget *VideoWidgetDragData::getVideoWidget()
{
    return videowidget;
}

VideoWidgetDragData::TypeDrag VideoWidgetDragData::getTypeDrag ()
{
    return typeDrag;
}
