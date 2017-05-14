#include "crowd_parameters.h"
#include "ui_crowdparameters.h"
#include <QDebug>

CrowdParameters::CrowdParameters(const QJsonObject &data, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CrowdParameters)
{
    ui->setupUi(this);
    readJson(data);
}

CrowdParameters::~CrowdParameters()
{
    delete ui;
}

void CrowdParameters::on_saveParamsButton_clicked()
{
    QJsonObject data = createJson();
    emit sendCorwdParamsJson(data);
    this->close();
}

void CrowdParameters::readJson(const QJsonObject &file)
{
    QJsonObject data = file["agents"].toObject();
    QJsonObject children = data.value(QString("children")).toObject();
    QJsonObject women = data.value(QString("women")).toObject();
    QJsonObject men = data.value(QString("men")).toObject();
    QJsonObject old = data.value(QString("old")).toObject();
    QJsonObject other = data.value(QString("other")).toObject();

    QJsonObject wish_speed;
    QJsonObject mass;

    wish_speed = children.value(QString("wish_speed")).toObject();
    mass = children.value(QString("mass")).toObject();
    this->ui->child_min_speed->setValue(wish_speed.value(QString("min")).toDouble());
    this->ui->child_max_speed->setValue(wish_speed.value(QString("max")).toDouble());
    this->ui->child_min_mass->setValue(mass.value(QString("min")).toDouble());
    this->ui->child_max_mass->setValue(mass.value(QString("max")).toDouble());
    this->ui->child_part->setValue(children.value(QString("part")).toDouble()*100);

    wish_speed = women.value(QString("wish_speed")).toObject();
    mass = women.value(QString("mass")).toObject();
    this->ui->women_min_speed->setValue(wish_speed.value(QString("min")).toDouble());
    this->ui->women_max_speed->setValue(wish_speed.value(QString("max")).toDouble());
    this->ui->women_min_mass->setValue(mass.value(QString("min")).toDouble());
    this->ui->women_max_mass->setValue(mass.value(QString("max")).toDouble());
    this->ui->women_part->setValue(women.value(QString("part")).toDouble()*100);

    wish_speed = men.value(QString("wish_speed")).toObject();
    mass = men.value(QString("mass")).toObject();
    this->ui->men_min_speed->setValue(wish_speed.value(QString("min")).toDouble());
    this->ui->men_max_speed->setValue(wish_speed.value(QString("max")).toDouble());
    this->ui->men_min_mass->setValue(mass.value(QString("min")).toDouble());
    this->ui->men_max_mass->setValue(mass.value(QString("max")).toDouble());
    this->ui->men_part->setValue(men.value(QString("part")).toDouble()*100);

    wish_speed = old.value(QString("wish_speed")).toObject();
    mass = old.value(QString("mass")).toObject();
    this->ui->old_min_speed->setValue(wish_speed.value(QString("min")).toDouble());
    this->ui->old_max_speed->setValue(wish_speed.value(QString("max")).toDouble());
    this->ui->old_min_mass->setValue(mass.value(QString("min")).toDouble());
    this->ui->old_max_mass->setValue(mass.value(QString("max")).toDouble());
    this->ui->old_part->setValue(old.value(QString("part")).toDouble()*100);

    wish_speed = other.value(QString("wish_speed")).toObject();
    mass = old.value(QString("mass")).toObject();
    this->ui->other_min_speed->setValue(wish_speed.value(QString("min")).toDouble());
    this->ui->other_max_speed->setValue(wish_speed.value(QString("max")).toDouble());
    this->ui->other_min_mass->setValue(mass.value(QString("min")).toDouble());
    this->ui->other_max_mass->setValue(mass.value(QString("max")).toDouble());
    this->ui->other_part->setValue(other.value(QString("part")).toDouble()*100);
}

QJsonObject CrowdParameters::createJson()
{
    QJsonObject data;
    QJsonObject children;
    QJsonObject women;
    QJsonObject men;
    QJsonObject old;
    QJsonObject other;
    QJsonObject wish_speed;
    QJsonObject mass;

    wish_speed["min"] = this->ui->child_min_speed->value();
    wish_speed["max"] = this->ui->child_max_speed->value();
    mass["min"] = this->ui->child_min_mass->value();
    mass["max"] = this->ui->child_max_mass->value();
    children["part"] = this->ui->child_part->value()/100;
    children["wish_speed"] = wish_speed;
    children["mass"] = mass;

    wish_speed["min"] = this->ui->women_min_speed->value();
    wish_speed["max"] = this->ui->women_max_speed->value();
    mass["min"] = this->ui->women_min_mass->value();
    mass["max"] = this->ui->women_max_mass->value();
    women["part"] = this->ui->women_part->value()/100;
    women["wish_speed"] = wish_speed;
    women["mass"] = mass;

    wish_speed["min"] = this->ui->men_min_speed->value();
    wish_speed["max"] = this->ui->men_max_speed->value();
    mass["min"] = this->ui->men_min_mass->value();
    mass["max"] = this->ui->men_max_mass->value();
    men["part"] = this->ui->men_part->value()/100;
    men["wish_speed"] = wish_speed;
    men["mass"] = mass;

    wish_speed["min"] = this->ui->old_min_speed->value();
    wish_speed["max"] = this->ui->old_max_speed->value();
    mass["min"] = this->ui->old_min_mass->value();
    mass["max"] = this->ui->old_max_mass->value();
    old["part"] = this->ui->old_part->value()/100;
    old["wish_speed"] = wish_speed;
    old["mass"] = mass;

    wish_speed["min"] = this->ui->other_min_speed->value();
    wish_speed["max"] = this->ui->other_max_speed->value();
    mass["min"] = this->ui->other_min_mass->value();
    mass["max"] = this->ui->other_max_mass->value();
    other["part"] = this->ui->other_part->value()/100;
    other["wish_speed"] = wish_speed;
    other["mass"] = mass;


    data["children"] = children;
    data["women"] = women;
    data["men"] = women;
    data["old"] = women;
    data["other"] = women;

    return data;
}
