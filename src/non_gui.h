#ifndef NON_GUI_H
#define NON_GUI_H

#include <QObject>
#include <QCoreApplication>

#include "json_manager.h"

class NonGuiRunner : public QObject
{
    Q_OBJECT

public:
    NonGuiRunner(const QString& conf, const QString& scheme, double panic, int algo)
    {
        m_engine.reset(new Engine());

        JsonManager::setConfPath(conf);

        m_engine->loadPlan(scheme);
        m_engine->getCalculator().changePanicLevelSlot(panic);

        if (!algo)
            m_engine->getCalculator().setUsePathFinding(false);
        else
            m_engine->getCalculator().pathAlgorithmChangedSlot(algo - 1);

        m_engine->setDelay(0);
        m_engine->resume();

        connect(m_engine.get(), &Engine::sendStatReportSignal, this, &NonGuiRunner::simulationFinishSlot);
        //connect(m_engine.get(), &Engine::updateAgentInRoomSignal, this, &NonGuiRunner::updateAgentsInRoomSlot);
    }

public slots:
    void simulationFinishSlot(QString report)
    {
        qDebug() << "Result:\n" << report;
        QCoreApplication::exit();
    }

//    void updateAgentsInRoomSlot(int n)
//    {
//        qDebug() << n;
//    }

private:
    std::unique_ptr<Engine> m_engine;
};

#endif // NON_GUI_H

