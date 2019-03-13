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
    QString path = a.arguments()[1];
    QJsonValue value = QJsonPath::getValue(jsonDocument, path);
    qDebug() << value;
    QJsonPath::setValue(jsonDocument, path, a.arguments()[2]);
    value = QJsonPath::getValue(jsonDocument, path);
    qDebug() << value;

    return 0;
}
