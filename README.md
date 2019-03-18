## QJsonPath demo

So this is basically a library that I'm too lazy to make a library, so it's just
header and source file for a simple implementation of qjsonpath.

### QJsonPath

`QJsonPath::getValue` gets value from QJsonDocument or QJsonValue, traversing
the given path to it first.

`QJsonPath::setValue` sets value from QJsonDocument or QJsonValue, traversing to
given path to it, and storing every tree element it finds, and then in reverse
reconstructing the json with the value changed. (It's a slow process, but if you
insist on using QJsonDocument, then fucking deal with it, because there's
basically no references or pointers)

### Building the demo

```
git submodule update --init
mkdir build
cd build
qmake .. && make
```

### Running the demo

Specify the path to json file and path to the variable in json

```
./qjsonpathdemo ../example.json .phoneNumbers[1].number
```
