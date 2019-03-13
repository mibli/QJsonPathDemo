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

    QStringList args = a.arguments();
    if (args.size() != 3)
    {
        throw std::logic_error("Need two arguments: PATH_TO_JSON PATH_TO_VALUE");
    }

    QFile jsonFile(args[1]);
    jsonFile.open(QIODevice::ReadOnly);
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonFile.readAll());

    QString path = args[2];

    // prove that we can read a value
    QJsonValue value = QJsonPath::getValue(jsonDocument, path);
    qDebug() << value;

    // prove that we can write a value
    QJsonPath::setValue(jsonDocument, path, "DUMMY");
    value = QJsonPath::getValue(jsonDocument, path);
    qDebug() << value;

    return 0;
}
