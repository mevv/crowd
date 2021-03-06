#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>

#include <memory>

#include "engine.h"
#include "agent.h"
#include "non_gui.h"
#include "a_star_step.h"

QCoreApplication* createApplication(int &argc, char *argv[])
{
    if (argc > 1)
        return new QCoreApplication(argc, argv);

    return new QApplication(argc, argv);
}

int main(int argc, char *argv[])
{
//    std::vector<double> map = {1, 9, 1, 1,
//                               1, 9, 1, 9,
//                               1, 1, 1, 1};

//    ASTAR::AStar algo(map, 3, 4, ASTAR::Cell(0, 0), ASTAR::Cell(0, 3));

//    auto path = algo.findPath();
//    for (auto p : path)
//        qDebug() << "(" << p.first << ", " << p.second << ")";

    std::unique_ptr<QCoreApplication> a(createApplication(argc, argv));
    std::unique_ptr<MainWindow> w;
    std::unique_ptr<NonGuiRunner> runner;

    if (argc == 1)
    {
        w.reset(new MainWindow());
        w->show();
    }
    else if (argc == 5)
    {
        // ./crowd <conf> <scheme> <panic>[0 - 1] <algo>[0 - none, 1 - A*, 2 - Lee]
        QString conf(argv[1]);
        QString scheme(argv[2]);
        double panic = QString(argv[3]).toDouble();
        int algo = QString(argv[4]).toInt();

        runner.reset(new NonGuiRunner(conf, scheme, panic, algo));
    }
    else
    {
        qDebug() << "Bad argument list.";
        QCoreApplication::exit();
    }

    return a->exec();
}
