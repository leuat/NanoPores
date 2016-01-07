#ifndef IO_H
#define IO_H

#include <fstream>
#include <cstdlib>
#include <iostream>

#include <QVector>
#include <QVector3D>

using std::ofstream;
using namespace std;

class IO
{
private:
public:
    IO();
    ~IO();

    static void save(const char *filename, QVector<QVector3D>& positions);
    static void open(const char *filename, QVector<QVector3D>& positions);

};
#endif
