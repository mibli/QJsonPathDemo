#pragma once

#include <stack>
#include <string>
#include <QDebug>

namespace QJsonPath {

std::pair<size_t, std::string> getKey(std::string const &path, size_t lpos)
{
    if (path[lpos] != '.')
        throw std::logic_error("Path parsing error: getIndex, should be used with position of dot");
    size_t pos = path.find_first_of("[].", ++lpos);
    if (pos == std::string::npos)
        pos = path.size();
    int size = pos - lpos;
    if (size <= 0)
        throw std::logic_error("Parsing error: zero-length index");
    return {pos, path.substr(lpos, size)};
}

std::pair<size_t, std::string> getIndex(std::string const &path, size_t lpos)
{
    if (path[lpos] != '[')
        throw std::logic_error("Path parsing error: getIndex, should be used with position of left bracket");
    size_t pos = path.find_first_of("[].", ++lpos);
    if (pos == std::string::npos)
        throw std::logic_error("Path parsing error: unclosed brackets - end of path encountered");
    if (path[pos] != ']')
        throw std::logic_error("Parsing error: unexpected closing bracket");
    int size = pos - lpos;
    if (size <= 0)
        throw std::logic_error("Parsing error: zero-length index");
    return {++pos, path.substr(lpos, size)};
}

QJsonValue getValue(QJsonValue const &root, std::string const &path)
{
    QJsonValue new_root = root;
    size_t pos = 0;
    std::string key;
    while (pos != path.size()) {
        switch (path[pos])
        {
            case '.':
                std::tie(pos, key) = getKey(path, pos);
                new_root = new_root[QString(key.c_str())];
                break;
            case '[':
                std::tie(pos, key) = getIndex(path, pos);
                new_root = new_root[std::stoi(key)];
                break;
            default:
                throw std::logic_error("Parsing error: expected key or index");
        }
    }
    return new_root;
}

QJsonValue getValue(const QJsonDocument &root, const std::string &path)
{
    QJsonValue new_root;
    if (path[0] == '[')
    {
        if (not root.isArray())
            throw std::logic_error("Parsing error: expected array, but document is not an array");
        new_root = root.array();
    }
    else if (path[0] == '.')
    {
        if (not root.isObject())
            throw std::logic_error("Parsing error: expected object, but document is not an object");
        new_root = root.object();
    }
    else {
        return new_root;
    }
    return getValue(new_root, path);
}

void setValue(QJsonValue &root, std::string const &path, QJsonValue const &value)
{
    std::stack<QJsonValue> nodes;
    nodes.push(root);
    std::stack<std::string> keys {};
    size_t pos = 0;
    std::string key;
    while (pos != path.size()) {
        switch (path[pos])
        {
            case '.':
                std::tie(pos, key) = getKey(path, pos);
                nodes.push(nodes.top()[QString(key.c_str())]);
                keys.push(key);
                break;
            case '[':
                std::tie(pos, key) = getIndex(path, pos);
                nodes.push(nodes.top()[std::stoi(key)]);
                keys.push(key);
                break;
            default:
                throw std::logic_error("Parsing error: expected key or index");
        }
    }
    QJsonValue node = value;
    nodes.pop();
    while (nodes.size() >= 1)
    {
        if (nodes.top().isArray())
        {
            QJsonArray array = nodes.top().toArray();
            array[std::stoi(keys.top())] = node;
            node = array;
        }
        else if (nodes.top().isObject())
        {
            QJsonObject object = nodes.top().toObject();
            object[QString(keys.top().c_str())] = node;
            node = object;
        }
        nodes.pop();
        keys.pop();
    }
    root = node;
}

void setValue(QJsonDocument &root, const std::string &path, QJsonValue const &value)
{
    QJsonValue new_root;
    if (path[0] == '[')
    {
        if (not root.isArray())
            throw std::logic_error("Parsing error: expected array, but document is not an array");
        new_root = root.array();
    }
    else if (path[0] == '.')
    {
        if (not root.isObject())
            throw std::logic_error("Parsing error: expected object, but document is not an object");
        new_root = root.object();
    }
    else {
        new_root = value;
    }
    setValue(new_root, path, value);
    if (new_root.isArray())
        root = QJsonDocument(new_root.toArray());
    else
    if (new_root.isObject())
        root = QJsonDocument(new_root.toObject());
}

}
