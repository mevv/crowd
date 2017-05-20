#include "general_builder.h"

#include <unistd.h>
#include <time.h>

#include <QTime>

#include "spawn_zone.h"

GeneralBuilder::GeneralBuilder()
{

}

static bool CalculatorBuilder(const QJsonObject& settings, Calculator& calculator)
{

}

double GeneralBuilder::getRandomNumber(double a, double b)
{
    //usleep(100);

    if (a == b)
        return a;

    double hack = 1000;

    a *= hack;
    b *= hack;

    return (qrand() % ((int)b - (int)a) + (int)a) / hack;
}

bool GeneralBuilder::buildCalculator(const QJsonObject& settings, Calculator& calculator)
{
    MathParams param;

    QJsonObject tmp = settings.value("calculator").toObject();

    param.deltaT = tmp.value("delta_t").toDouble();
    param.A = tmp.value("A").toDouble();
    param.B = tmp.value("B").toDouble();
    param.K = tmp.value("K").toDouble();
    param.Awall = tmp.value("A_wall").toDouble();
    param.Bwall = tmp.value("B_wall").toDouble();
    param.Kwall = tmp.value("K_wall").toDouble();

    calculator.setMathParams(param);

    return true;
}

bool GeneralBuilder::buildAgents(const QJsonObject& settings, ObjectsPool& pool)
{
    int id = 0;

    QTime t = QTime::currentTime();
    qsrand((uint)t.msec());

    qDebug() << "GeneralBuilder::buildAgents()" << settings;

    int totalNumber = settings.value("agent").toObject().value("number").toInt();
    double panic = settings.value("agent").toObject().value("number").toDouble();
    QPair<double, double> entryPeriod(settings.value("agent").toObject().value("entry_period").toObject().value("min").toDouble(),
                                    settings.value("agent").toObject().value("entry_period").toObject().value("max").toDouble());
    qDebug() << "fron conf" << entryPeriod;
    QVector<QJsonObject> agentTypesConfig;
    QVector<AgentType> agentTypes;

    agentTypesConfig.push_back(settings.value("agent").toObject().value("children").toObject());
    agentTypesConfig.push_back(settings.value("agent").toObject().value("men").toObject());
    agentTypesConfig.push_back(settings.value("agent").toObject().value("women").toObject());
    agentTypesConfig.push_back(settings.value("agent").toObject().value("old").toObject());
    agentTypesConfig.push_back(settings.value("agent").toObject().value("custom").toObject());

    agentTypes.push_back(AgentType::Child);
    agentTypes.push_back(AgentType::Man);
    agentTypes.push_back(AgentType::Woman);
    agentTypes.push_back(AgentType::Old);
    agentTypes.push_back(AgentType::Custom);

    for (auto& i : pool.getEntries())
    {
        i.setPeriod(entryPeriod);
    }

    double totalSpawnArea = 0;
    for (auto i : pool.getSpawnZones())
            totalSpawnArea += (i.getSize().x() * i.getSize().y());

    for (auto i : pool.getSpawnZones())
    {
        int curNum = i.getSize().x() * i.getSize().y() / totalSpawnArea * totalNumber;

        double x = i.getPos().x();
        double y = i.getPos().y();

        int type = 0;

        for (auto agentType : agentTypesConfig)
        {

            int typeNum = int(curNum * agentType.value("part").toDouble());
            double size;

            for (int count = 0; count < typeNum; count++)
            {
                size = getRandomNumber(agentType.value("size").toObject().value("min").toDouble(),
                                       agentType.value("size").toObject().value("max").toDouble());
                //qDebug() << agentType.value("size").toObject().value("min").toDouble() << agentType.value("size").toObject().value("max").toDouble() << size;
                x += 2 * agentType.value("size").toObject().value("max").toDouble();
                if (x > i.getPos().x() + i.getSize().x())
                {
                    x = i.getPos().x();
                    y += 2 * agentType.value("size").toObject().value("max").toDouble();
                    if (y > i.getPos().y() + i.getSize().y())
                        break;
                }

                pool.addAgent(Agent(id,
                                    size,
                                    getRandomNumber(agentType.value("mass").toObject().value("min").toDouble(),
                                                    agentType.value("mass").toObject().value("max").toDouble()),
                                    QVector2D(x, y),
                                    QVector2D(0, 0),
                                    QColor(agentType.value("color").toObject().value("R").toInt(),
                                           agentType.value("color").toObject().value("G").toInt(),
                                           agentType.value("color").toObject().value("B").toInt(),
                                           agentType.value("color").toObject().value("A").toDouble()),
                                    getRandomNumber(agentType.value("wish_speed").toObject().value("min").toDouble(),
                                                    agentType.value("wish_speed").toObject().value("max").toDouble()),
                                    agentTypes[type]));
                id++;
            }
            type++;
        }
    }

    return true;
}

Agent GeneralBuilder::buildSingleAgent(const QJsonObject& settings, QVector2D pos, QVector2D speedDir)
{
    QVector<QJsonObject> agentTypesConfig;
    QVector<AgentType> agentTypes;

    agentTypesConfig.push_back(settings.value("agent").toObject().value("children").toObject());
    agentTypesConfig.push_back(settings.value("agent").toObject().value("men").toObject());
    agentTypesConfig.push_back(settings.value("agent").toObject().value("women").toObject());
    agentTypesConfig.push_back(settings.value("agent").toObject().value("old").toObject());
    agentTypesConfig.push_back(settings.value("agent").toObject().value("custom").toObject());

    agentTypes.push_back(AgentType::Child);
    agentTypes.push_back(AgentType::Man);
    agentTypes.push_back(AgentType::Woman);
    agentTypes.push_back(AgentType::Old);
    agentTypes.push_back(AgentType::Custom);

    QJsonObject agentType = agentTypesConfig[1];
    AgentType type;
    int count = 0;
    double random;
    double sum = 0;

    random = getRandomNumber(0, 1);
    qDebug() <<  random;
    for (auto i : agentTypesConfig)
    {
        if (random >= sum && random < sum + i.value("part").toDouble())
        {
            type = agentTypes[count];
            agentType = i;
            break;
        }

        sum += i.value("part").toDouble();
        count++;
    }

    double wishSpeed = getRandomNumber(agentType.value("wish_speed").toObject().value("min").toDouble(),
                                       agentType.value("wish_speed").toObject().value("max").toDouble());
    QVector2D speed = speedDir * wishSpeed;

    return Agent(getRandomNumber(1000000, 10000000),
                    getRandomNumber(agentType.value("size").toObject().value("min").toDouble(),
                                      agentType.value("size").toObject().value("max").toDouble()),
                    getRandomNumber(agentType.value("mass").toObject().value("min").toDouble(),
                                    agentType.value("mass").toObject().value("max").toDouble()),
                    pos,
                    speed,
                    QColor(agentType.value("color").toObject().value("R").toInt(),
                           agentType.value("color").toObject().value("G").toInt(),
                           agentType.value("color").toObject().value("B").toInt(),
                           agentType.value("color").toObject().value("A").toDouble()),
                    wishSpeed,
                    type);
}
