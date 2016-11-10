#include "render-context.h"
#include <QGraphicsView>
#include <QGraphicsLineItem>

namespace iz {

GuiView::GuiView()
{

}

GuiView::~GuiView()
{
    delete m_ctx;
}

void GuiView::init()
{
    m_ctx = new QGraphicsView;
}

void GuiView::addRendScene(RenderScene * const rsc)
{
    m_ctx->setScene(rsc->p_scene);
    m_ctx->show();
}

RenderScene::RenderScene()
{
}

RenderScene::~RenderScene()
{
    delete p_scene;

}

void RenderScene::init()
{
    p_scene = new QGraphicsScene;
    p_scene->addLine(10 , 10 , 100, 10);
}


} // iz

