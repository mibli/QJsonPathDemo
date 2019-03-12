#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <string>
#include <QDebug>

enum State {
    NONE,
    PARSING_ARRAY,
    PARSING_OBJECT
};

QJsonValue valueFromPath(QJsonDocument const &root, std::string const &path)
{
    State state = NONE;
    QJsonValue new_root;
    size_t pos = 0;
    if (path[0] == '[')
    {
        state = PARSING_ARRAY;
        if (not root.isArray())
            throw std::logic_error("Parsing error: expected array, but document is not an array");
        new_root = root.array();
        pos++;
    }
    else if (path[0] == '.')
    {
        if (not root.isObject())
            throw std::logic_error("Parsing error: expected object, but document is not an object");
        state = PARSING_OBJECT;
        new_root = root.object();
        pos++;
    }
    else
    {
        return new_root;
    }

    size_t lpos = pos;
    size_t key_size = 0;
    pos = path.find_first_of("[].", pos);
    std::string key = "";
    while (pos != std::string::npos) {
        switch (path[pos])
        {
            case '.':
            case '[':
                if (state == PARSING_OBJECT)
                {
                    state = NONE;
                    key_size = pos - lpos;
                    if (key_size == 0)
                        throw std::logic_error("Parsing error: zero-length keyword");
                    key = path.substr(lpos, key_size);
                    if (not new_root.isObject())
                        throw std::logic_error("Parsing error: expected object, but document is not an object");
                    new_root = new_root[QString(key.c_str())];
                }
            default:
                if (state == PARSING_ARRAY)
                    throw std::logic_error("Parsing error: unclosed brackets - control character encountered");
                break;
            case ']':
                if (state == PARSING_ARRAY)
                {
                    state = NONE;
                    key_size = pos - lpos;
                    if (key_size == 0)
                        throw std::logic_error("Parsing error: zero-length index");
                    if (not new_root.isArray())
                        throw std::logic_error("Parsing error: expected object, but document is not an object");
                    key = path.substr(lpos, key_size);
                    new_root = new_root[std::stoi(key)];
                }
                else
                    throw std::logic_error("Parsing error: unexpected closing bracket");
                break;
        }
        switch (path[pos])
        {
            case '.':
                state = PARSING_OBJECT;
                break;
            case '[':
                state = PARSING_ARRAY;
                break;
            default:
                break;
        }
        lpos = pos + 1;

        pos = path.find_first_of("[].", lpos);
    }
    pos = path.size();
    if (state == PARSING_ARRAY)
        throw std::logic_error("Path parsing error: unclosed brackets - end of path encountered");
    if (state == PARSING_OBJECT)
    {
        key_size = pos - lpos;
        if (key_size == 0)
            throw std::logic_error("Parsing error: zero-length keyword");
        if (not new_root.isObject())
            throw std::logic_error("Parsing error: expected object, but document is not an object");
        key = path.substr(lpos, key_size);
        new_root = new_root[QString(key.c_str())];
    }
    return new_root;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile jsonFile("/ecs/rpq/workspace/sp/system-config/cp5/ds/21/ARCHIVE.json");
    jsonFile.open(QIODevice::ReadOnly);
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonFile.readAll());
    QJsonValue value = valueFromPath(jsonDocument, a.arguments()[1].toStdString());

    qDebug() << value;

    return 0;
}
