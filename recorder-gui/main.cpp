#include "widget.h"
#include "render-context.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    iz::RenderScene rsc;
    rsc.init();
    iz::GuiView view;
    view.init();
    view.addRendScene(&rsc);

    return a.exec();
}
