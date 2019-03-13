#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <string>
#include <QDebug>
#include "qjsonpath.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile jsonFile("/ecs/rpq/workspace/sp/system-config/cp5/ds/21/ARCHIVE.json");
    jsonFile.open(QIODevice::ReadOnly);
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonFile.readAll());
    QJsonValue value = QJsonPath::getValue(jsonDocument, a.arguments()[1].toStdString());

    qDebug() << value;

    return 0;
}
