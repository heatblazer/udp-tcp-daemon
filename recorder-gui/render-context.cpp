#include "render-context.h"
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QPainter>

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
    p_scene->setSceneRect(QRect(0, 0, 320, 240));
    QGraphicsLineItem* line = p_scene->addLine(0 , 240/2 , p_scene->width(), 240/2);
    m_globalTick.setInterval(30);
    connect(&m_globalTick, SIGNAL(timeout()),
            this, SLOT(renderScene()));
    m_globalTick.start();

//    line->setPos(22, 44);
}

void RenderScene::renderScene()
{
}


} // iz

