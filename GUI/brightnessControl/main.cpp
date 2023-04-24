#include "plusminus.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    PlusMinus window;

    window.resize(480, 272);
    window.setWindowTitle("Brightness Control");
    window.show();

    return app.exec();
}
