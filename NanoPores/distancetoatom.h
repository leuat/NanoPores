#pragma once
#include <vector>
#include <QVector>
#include <QVector3D>
using std::vector;

typedef vector<vector<vector<vector<int> > > > CellList;
template<class T>
bool checkRange(T value, T min, T max) {
    return (value >= min) && (value <= max);
}

class DistanceToAtom
{
private:
    QVector<float> m_values;
    int m_size = 0;
    bool m_isValid = false;
    inline int index(int i, int j, int k) { return i*m_size*m_size + j*m_size + k; }
    inline int periodic(int idx, int count) { return (idx + count) % count; }
    float periodicDistanceSquared(const QVector3D &p1, const QVector3D &p2, float systemSize);
    CellList buildCellList(const QVector<QVector3D> &points, float size, float cutoff, float &cellSize);
public:
    DistanceToAtom(int size);
    void compute(const QVector<QVector3D> &points, float cutoff);
    bool isValid();
    void resize(int size);
    float getValue(int i, int j, int k);
    void setValue(int i, int j, int k, float value);
    QVector<QPointF> histogram(int bins);
};
