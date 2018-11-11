#include "general_builder.h"

#include <unistd.h>
#include <time.h>


#include <QTime>

#include "spawn_zone.h"

GeneralBuilder::GeneralBuilder()
{

}

double GeneralBuilder::getRandomNumber(double a, double b)
{
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

    calculator.setGridStep(tmp.value("grid_step").toDouble());
    calculator.setCheckpointRadius(tmp.value("checkpoint_radius").toDouble());
    calculator.setRepathCoef(tmp.value("repath_coef").toDouble());

    return true;
}

bool GeneralBuilder::buildAgents(const QJsonObject& settings, ObjectsPool& pool, double panicLevel)
{
    int id = 0;

    QTime t = QTime::currentTime();
    qsrand((uint)t.msec());

    int totalNumber = settings.value("agent").toObject().value("number").toInt();
    //double panic = settings.value("agent").toObject().value("number").toDouble();
    QPair<double, double> entryPeriod(settings.value("agent").toObject().value("entry_period").toObject().value("min").toDouble(),
                                    settings.value("agent").toObject().value("entry_period").toObject().value("max").toDouble());

    QMap<AgentType, QJsonObject> agentTypesConfig;
    QVector<AgentType> agentTypes;
    QMap<AgentType, int> numAgents;

    agentTypesConfig[AgentType::Child] = settings.value("agent").toObject().value("children").toObject();
    agentTypesConfig[AgentType::Man] = settings.value("agent").toObject().value("men").toObject();
    agentTypesConfig[AgentType::Woman] = settings.value("agent").toObject().value("women").toObject();
    agentTypesConfig[AgentType::Old] = settings.value("agent").toObject().value("old").toObject();
    agentTypesConfig[AgentType::Custom] = settings.value("agent").toObject().value("custom").toObject();

    agentTypes.push_back(AgentType::Child);
    agentTypes.push_back(AgentType::Man);
    agentTypes.push_back(AgentType::Woman);
    agentTypes.push_back(AgentType::Old);
    agentTypes.push_back(AgentType::Custom);

    numAgents[AgentType::Child] = totalNumber * agentTypesConfig[AgentType::Child].value("part").toDouble();
    numAgents[AgentType::Man] = totalNumber * agentTypesConfig[AgentType::Man].value("part").toDouble();
    numAgents[AgentType::Woman] = totalNumber * agentTypesConfig[AgentType::Woman].value("part").toDouble();
    numAgents[AgentType::Old] = totalNumber * agentTypesConfig[AgentType::Old].value("part").toDouble();
    numAgents[AgentType::Custom] = totalNumber * agentTypesConfig[AgentType::Custom].value("part").toDouble();

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

        double size;

        for (int count = 0; count < curNum; count++)
        {
            AgentType agentTypeID = static_cast<AgentType>((int)getRandomNumber(0, 4));

            while (numAgents[static_cast<AgentType>(agentTypeID)] <= 0)
            {
                if (curNum < 10)
                {
                    agentTypeID = static_cast<AgentType>(count % 5);
                    break;
                }

                agentTypeID = static_cast<AgentType>((int)getRandomNumber(0, 4));
            }

           numAgents[static_cast<AgentType>(agentTypeID)]--;

           QJsonObject agentType = agentTypesConfig[static_cast<AgentType>(agentTypeID)];

            size = getRandomNumber(agentType.value("size").toObject().value("min").toDouble(),
                                   agentType.value("size").toObject().value("max").toDouble());
            x += DISTANCE_BETWEEN_AGENTS * agentType.value("size").toObject().value("max").toDouble();
            if (x > i.getPos().x() + i.getSize().x())
            {
                x = i.getPos().x();
                y += DISTANCE_BETWEEN_AGENTS * agentType.value("size").toObject().value("max").toDouble();
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
                                agentType.value("wish_speed").toObject().value("min").toDouble() +
                                (agentType.value("wish_speed").toObject().value("max").toDouble() -
                                 agentType.value("wish_speed").toObject().value("min").toDouble()) * panicLevel,
                                agentTypeID));
            id++;
        }

    }

    return true;
}

Agent GeneralBuilder::buildSingleAgent(const QJsonObject& settings, QVector2D pos, QVector2D speedDir, double panicLevel)
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

    double wishSpeed = agentType.value("wish_speed").toObject().value("min").toDouble() +
                      (agentType.value("wish_speed").toObject().value("max").toDouble() -
                       agentType.value("wish_speed").toObject().value("min").toDouble()) * panicLevel;
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


bool GeneralBuilder::buildCheckPoints(QJsonObject& settings,
                                      ObjectsPool& pool,
                                      Calculator& calculator,
                                      const int pathAlgorithmIndex)
{
    int height, width;
    auto matrix = calculator.buildAStarMatrix(height, width);
    std::vector<std::pair<double, double>> backupPath;

    for (auto agent : pool.getAgents())
    {
        QVector2D nearestExit = calculator.getNearestExit(agent);

        int agentMatrixX = floor(agent.getCenter().x() / calculator.getGridStep());
        if(agentMatrixX >= width)
            agentMatrixX--;
        int agentMatrixY = floor(agent.getCenter().y() / calculator.getGridStep());
        if(agentMatrixY >= height)
            agentMatrixY--;
        int exitMatrixX = floor(nearestExit.x() / calculator.getGridStep());
        if(exitMatrixX >= width)
            exitMatrixX--;
        int exitMatrixY = floor(nearestExit.y() / calculator.getGridStep());
        if(exitMatrixY >= height)
            exitMatrixY--;

        std::vector<std::pair<double, double>> path;

        switch(pathAlgorithmIndex)
        {
            case 0:
                path = Astar(matrix.toStdVector(),
                             width,
                             height,
                             std::make_pair(agentMatrixX, agentMatrixY),
                             std::make_pair(exitMatrixX, exitMatrixY));
                break;
            case 1:
                path = Lee(matrix.toStdVector(),
                           width,
                           height,
                           std::make_pair(agentMatrixX, agentMatrixY),
                           std::make_pair(exitMatrixX, exitMatrixY));
                break;

        }

        std::vector<Checkpoint> checkpoints;
        for (auto i : path)
            checkpoints.push_back(Checkpoint(0,
                                  QVector2D(i.first * calculator.getGridStep() + calculator.getGridStep() / 2.0,
                                            i.second * calculator.getGridStep() + calculator.getGridStep() / 2.0),
                                  QColor(),
                                  settings.value("calculator").toObject().value("checkpoint_radius").toDouble()));

        pool.getCheckpoints()[agent.getID()] = checkpoints;
    }

    return true;
}

bool GeneralBuilder::buildCheckPointsForSingleAgent(QJsonObject& settings,
                                                    ObjectsPool& pool,
                                                    Calculator& calculator,
                                                    const Agent& agent,
                                                    const int pathAlgorithmIndex)
{
    int height, width;
    auto matrix = calculator.buildAStarMatrix(height, width);
    QVector2D nearestExit = calculator.getNearestExit(agent);

    int agentMatrixX = floor(agent.getCenter().x() / calculator.getGridStep());
    if(agentMatrixX >= width)
        agentMatrixX--;
    int agentMatrixY = floor(agent.getCenter().y() / calculator.getGridStep());
    if(agentMatrixY >= height)
        agentMatrixY--;
    int exitMatrixX = floor(nearestExit.x() / calculator.getGridStep());
    if(exitMatrixX >= width)
        exitMatrixX--;
    int exitMatrixY = floor(nearestExit.y() / calculator.getGridStep());
    if(exitMatrixY >= height)
        exitMatrixY--;

    std::vector<std::pair<double, double>> path;

    switch(pathAlgorithmIndex)
    {
        case 0:
            path = Astar(matrix.toStdVector(),
                         width,
                         height,
                         std::make_pair(agentMatrixX, agentMatrixY),
                         std::make_pair(exitMatrixX, exitMatrixY));
            break;

        case 1:
            path = Lee(matrix.toStdVector(),
                       width,
                       height,
                       std::make_pair(agentMatrixX, agentMatrixY),
                       std::make_pair(exitMatrixX, exitMatrixY));
            break;

    }

    std::vector<Checkpoint> checkpoints;

    for (auto i : path)
        checkpoints.push_back(Checkpoint(0,
                              QVector2D(i.first * calculator.getGridStep() + calculator.getGridStep() / 2.0,
                                        i.second * calculator.getGridStep() + calculator.getGridStep() / 2.0),
                              QColor(),
                              settings.value("calculator").toObject().value("checkpoint_radius").toDouble()));

    pool.getCheckpoints()[agent.getID()] = checkpoints;

    return true;
}
