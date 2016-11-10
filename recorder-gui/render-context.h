#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

// qt stuff //
#include <QGraphicsScene>
#include <QTimer>

// experimental drawing api for the player
namespace iz {

class RenderScene;
class GuiView;

class GuiView
{
public:
    GuiView();
    ~GuiView();
    void init();
    void addRendScene(RenderScene* const rsc);

private:
    QGraphicsView* m_ctx;
};

class RenderScene
{
public:

    RenderScene();
    ~RenderScene();
    void init();

private:
    QTimer m_globalTick;
    QGraphicsScene* p_scene;

    friend class GuiView;
};


} // iz



#endif // RENDERCONTEXT_H
