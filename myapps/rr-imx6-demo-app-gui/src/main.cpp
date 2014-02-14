#include <QtGui/QApplication>
#include "rr_imx6_demo.h"

int main(int argc, char *argv[])
{

    // Initialize Display
    Tools* tools = new Tools();

    // Set fbo and fb1 with the same resolution
    int x,y;
    tools->get_display_resolution(0,x,y);
    tools->set_display_resolution(1,x,y);
    tools->wake_fb(0); // wake fb up
    tools->wake_fb(1); // wake overlay up

    // Start application
    QApplication a(argc, argv);
    rr_imx6_demo w;
    w.show();

    return a.exec();
}
