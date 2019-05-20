#include <QCoreApplication>
#include <TestRunner.hpp>

int main(int ac, char **av) {
    QCoreApplication app(ac, av);
    TestRunner TestRun;
    QObject::connect(&TestRun, &TestRunner::finished, &app, &QCoreApplication::quit);
    QTimer::singleShot(1000, &TestRun, SLOT(start()));
    return app.exec();
}
