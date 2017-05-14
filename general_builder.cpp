#include "general_builder.h"

#include <unistd.h>

#include <QTime>

#include "spawn_zone.h"

GeneralBuilder::GeneralBuilder()
{

}

static bool CalculatorBuilder(const QJsonObject& settings, Calculator& calculator)
{

}

int GeneralBuilder::getRandomNumber(int a, int b)
{
    usleep(10);

    QTime midnight(0, 0, 0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    return qrand() % (b - a) + a;
}

bool GeneralBuilder::buildCalculator(const QJsonObject& settings, Calculator& calculator)
{
    MathParams param;

    QJsonObject tmp = settings.value("calculator").toObject();

    param.deltaT = tmp.value("delta_t").toDouble();
    param.A = tmp.value("A").toDouble();
    param.B = tmp.value("B").toDouble();
    param.K = tmp.value("K").toDouble();
    param.Awall = tmp.value("Awall").toDouble();
    param.Bwall = tmp.value("Bwall").toDouble();
    param.Kwall = tmp.value("Kwall").toDouble();

    calculator.setMathParams(param);

    return true;
}

bool GeneralBuilder::buildAgents(const QJsonObject& settings, ObjectsPool& pool)
{
    int id = 0;


    int totalNumber = settings.value("agent").toObject().value("number").toInt();
    double panic = settings.value("agent").toObject().value("number").toDouble();
    QPair<double, double> entryFreq(settings.value("agent").toObject().value("entry_period").toObject().value("min").toDouble(),
                                    settings.value("agent").toObject().value("entry_period").toObject().value("max").toDouble());

//    QJsonObject childrenConf = settings.value("agent").toObject().value("children").toObject();
//    QJsonObject manConf = settings.value("agent").toObject().value("man").toObject();
//    QJsonObject womenConf = settings.value("agent").toObject().value("women").toObject();
//    QJsonObject oldConf = settings.value("agent").toObject().value("old").toObject();
//    QJsonObject customConf = settings.value("agent").toObject().value("custom").toObject();

    QVector<QJsonObject> agentTypes;

    agentTypes.push_back(settings.value("agent").toObject().value("children").toObject());
    agentTypes.push_back(settings.value("agent").toObject().value("man").toObject());
    agentTypes.push_back(settings.value("agent").toObject().value("women").toObject());
    agentTypes.push_back(settings.value("agent").toObject().value("old").toObject());
    agentTypes.push_back(settings.value("agent").toObject().value("custom").toObject());

    for (auto i : pool.getEntries())
    {
        i.setFreq(entryFreq);
    }

    double totalSpawnArea = 0;
    for (auto i : pool.getSpawnZones())
            totalSpawnArea += (i.getSize().x() * i.getSize().y());

    for (auto i : pool.getSpawnZones())
    {
        int curNum = (int)i.getSize().x() * i.getSize().y() / totalSpawnArea * totalNumber;

        for (auto agentType : agentTypes)
        {
            int typeNum = int(curNum * agentType.value("part").toDouble());
            bool flag = false;

            QVector2D pos;
            int size;

            for (int count = 0 ; count < typeNum; count++)
            {
                do
                {
                    pos.setX(getRandomNumber(i.getPos().x(), i.getPos().x() + i.getSize().x()));
                    pos.setY(getRandomNumber(i.getPos().y(), i.getPos().y() + i.getSize().y()));

                    size = getRandomNumber(agentType.value("size").toObject().value("min").toDouble(),
                                               agentType.value("size").toObject().value("max").toDouble());

                    for (auto j : pool.getAgents())
                    {
                        if (distanceBetweenPoints(pos.toPoint(), j.getCenter().toPoint()) < (j.getSize() + size + MIN_DISTANCE_BETWEEN_SPAWN_AGENTS))
                        {
                            flag = true;
                            break;
                        }
                    }
                }
                while(flag);

                pool.addAgent(Agent(id,
                                    size,
                                    getRandomNumber(agentType.value("mass").toObject().value("min").toDouble(),
                                                    agentType.value("mass").toObject().value("max").toDouble()),
                                    pos,
                                    QVector2D(0, 0),
                                    QColor(0, 0, 0, 1),
                                    (double)getRandomNumber(agentType.value("wish_speed").toObject().value("min").toDouble(),
                                                    agentType.value("wish_speed").toObject().value("max").toDouble())));


                id++;
            }
        }


    }


}
