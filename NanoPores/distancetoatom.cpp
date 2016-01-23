#include "distancetoatom.h"
#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_histogram.h>
#include <gsl/gsl_errno.h>
#include <cmath>
#include <QDebug>
#include <QElapsedTimer>

float floatRandom(const int & min, const int & max) {
#if defined(__clang__)
    static std::mt19937 generator;
#else
    static thread_local std::mt19937 generator;
#endif
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}

DistanceToAtom::DistanceToAtom(int size)
{
    resize(size);
}

float DistanceToAtom::periodicDistanceSquared(const QVector3D &p1, const QVector3D &p2, float systemSize)
{
    float dx = p1[0] - p2[0];
    float dy = p1[1] - p2[1];
    float dz = p1[2] - p2[2];
    if(dx < -0.5*systemSize) dx += systemSize;
    else if(dx > 0.5*systemSize) dx -= systemSize;
    if(dy < -0.5*systemSize) dy += systemSize;
    else if(dy > 0.5*systemSize) dy -= systemSize;
    if(dz < -0.5*systemSize) dz += systemSize;
    else if(dz > 0.5*systemSize) dz -= systemSize;
    return dx*dx + dy*dy + dz*dz;
}

CellList DistanceToAtom::buildCellList(const QVector<QVector3D> &points, float size, float cutoff, float &cellSize)
{
    CellList cellList;
    cellSize = cutoff;
    int numCells = size / cutoff;
    cellSize = size / numCells;
    cellList.resize(numCells, vector<vector<vector<int> > >(numCells, vector<vector<int> >(numCells)));
    for(int i=0; i<points.size(); i++) {
        const QVector3D &p = points[i];
        int ci = p[0] / cellSize;
        int cj = p[1] / cellSize;
        int ck = p[2] / cellSize;
        if(!checkRange<int>(ci, 0, numCells-1) || !checkRange<int>(cj, 0, numCells-1) || !checkRange<int>(ck, 0, numCells-1)) {
            qFatal("DistanceToAtom::buildCellList() error: particle %d is out of cell list bounds.", i);
            exit(1);
        }
        cellList[ci][cj][ck].push_back(i);
    }
    return cellList;
}

void DistanceToAtom::compute(const QVector<QVector3D> &pointsOriginal, float cutoff)
{
    if(pointsOriginal.size() == 0) {
        qDebug() << "DistanceToAtom::compute WARNING: input vector is empty.";
        return;
    }

    QElapsedTimer timer;
    timer.start();

    float min = 1e90;
    float max = -1e90;
    for(const QVector3D &point : pointsOriginal) {
        min = std::min(min, point[0]);
        min = std::min(min, point[1]);
        min = std::min(min, point[2]);

        max = std::max(max, point[0]);
        max = std::max(max, point[1]);
        max = std::max(max, point[2]);
    }
    max += 1e-5;
    const float systemSize = max - min;

    // Now translate all points
    QVector<QVector3D> points = pointsOriginal;
    for(QVector3D &point : points) {
        point[0] -= min;
        point[1] -= min;
        point[2] -= min;
    }

    float cellSize;
    CellList cellList = buildCellList(points, systemSize, cutoff, cellSize);
    const int numCells = cellList.size(); // Each index should be identical

    m_values.clear();
    const int numberOfRandomVectors = 1e4;
    m_values.resize(numberOfRandomVectors);
    int numCompares = 0;
#pragma omp parallel for num_threads(4)
    for(int i=0; i<numberOfRandomVectors; i++) {
        float x = floatRandom(0, systemSize);
        float y = floatRandom(0, systemSize);
        float z = floatRandom(0, systemSize);
//        float x = systemSize*(rand() / float(RAND_MAX));
//        float y = systemSize*(rand() / float(RAND_MAX));
//        float z = systemSize*(rand() / float(RAND_MAX));
        QVector3D randomPosition(x,y,z);
        const int cx = randomPosition[0] / cellSize;
        const int cy = randomPosition[1] / cellSize;
        const int cz = randomPosition[2] / cellSize;
        float minimumDistanceSquared0 = 1e10;
        float minimumDistanceSquared = 1e10;

        // Loop through all 27 cells with size=cutoff
        for(int dx=-1; dx<=1; dx++) {
            for(int dy=-1; dy<=1; dy++) {
                for(int dz=-1; dz<=1; dz++) {
                    const vector<int> &pointsInCell = cellList[periodic(cx+dx, numCells)][periodic(cy+dy, numCells)][periodic(cz+dz, numCells)];
                    for(const int &pointIndex : pointsInCell) {
                        const QVector3D &point = points[pointIndex];

                        const float distanceSquared = periodicDistanceSquared(point, randomPosition, systemSize);
                        minimumDistanceSquared = std::min(minimumDistanceSquared, distanceSquared);
                    }
                }
            }
        }

        if(minimumDistanceSquared == minimumDistanceSquared0) {
            minimumDistanceSquared = -1;
        }

        m_values[i] = float(minimumDistanceSquared);
    }
    qDebug() << "DAO finished after " << timer.elapsed() << " ms with " << numCompares << " compares.";
    m_isValid = true;

    //    if(pointsOriginal.size() == 0) {
    //        qDebug() << "DistanceToAtom::compute WARNING: input vector is empty.";
    //        return;
    //    }

    //    float min = 1e90;
    //    float max = -1e90;
    //    for(const QVector3D &point : pointsOriginal) {
    //        min = std::min(min, point[0]);
    //        min = std::min(min, point[1]);
    //        min = std::min(min, point[2]);

    //        max = std::max(max, point[0]);
    //        max = std::max(max, point[1]);
    //        max = std::max(max, point[2]);
    //    }
    //    max += 1e-5;
    //    const float systemSize = max - min;

    //    // Now translate all points
    //    QVector<QVector3D> points = pointsOriginal;
    //    for(QVector3D &point : points) {
    //        point[0] -= min;
    //        point[1] -= min;
    //        point[2] -= min;
    //    }

    //    float cellSize;
    //    CellList cellList = buildCellList(points, systemSize, cutoff, cellSize);
    //    const int numCells = cellList.size(); // Each index should be identical

    //    const float voxelSize = systemSize / m_size;
    //    for(int i=0; i<m_size; i++) {
    //        for(int j=0; j<m_size; j++) {
    //            for(int k=0; k<m_size; k++) {
    //                const QVector3D voxelCenter((i+0.5)*voxelSize, (j+0.5)*voxelSize, (k+0.5)*voxelSize);
    //                float minimumDistanceSquared0 = 1e10;
    //                float minimumDistanceSquared = 1e10;
    //                // Find the cell list where this position belongs and loop through all cells around
    //                const int cx = voxelCenter[0] / cellSize;
    //                const int cy = voxelCenter[1] / cellSize;
    //                const int cz = voxelCenter[2] / cellSize;

    //                // Loop through all 27 cells with size=cutoff
    //                for(int dx=-1; dx<=1; dx++) {
    //                    for(int dy=-1; dy<=1; dy++) {
    //                        for(int dz=-1; dz<=1; dz++) {
    //                            const vector<int> &pointsInCell = cellList[periodic(cx+dx, numCells)][periodic(cy+dy, numCells)][periodic(cz+dz, numCells)];
    //                            for(const int &pointIndex : pointsInCell) {
    //                                const QVector3D &point = points[pointIndex];

    //                                const float distanceSquared = periodicDistanceSquared(point, voxelCenter, systemSize);
    //                                minimumDistanceSquared = std::min(minimumDistanceSquared, distanceSquared);
    //                            }
    //                        }
    //                    }
    //                }
    //                if(minimumDistanceSquared == minimumDistanceSquared0) {
    //                    minimumDistanceSquared = -1;
    //                }
    //                setValue(i,j,k,float(minimumDistanceSquared));
    //            }
    //        }
    //    }

    //    m_isValid = true;
}

bool DistanceToAtom::isValid()
{
    return m_isValid;
}

void DistanceToAtom::resize(int size)
{
    m_size = size;
    m_values.resize(size*size*size);
    for(float &v : m_values) { v = 0; }
    m_isValid = false;
}

float DistanceToAtom::getValue(int i, int j, int k)
{
    int idx = index(i,j,k);
    if(idx < 0 || idx >= m_size) {
        qFatal("Error, DistanceToAtom::getValue(%d,%d,%d) is out of bounds", i,j,k);
        exit(1);
    }

    return m_values[idx];
}

void DistanceToAtom::setValue(int i, int j, int k, float value)
{
    m_values[index(i,j,k)] = value;
}

QVector<QPointF> DistanceToAtom::histogram(int bins) {
    QVector<QPointF> histogramVector;

    if(!m_isValid) {
        qFatal("DistanceToAtom is not valid. Run compute() first.");
        exit(1);
    }
    float minValue = 1e90;
    float maxValue = 0;

    for(const float &val : m_values) {
        if(val >= 0) {
            minValue = std::min(minValue, (float)sqrt(val));
            maxValue = std::max(maxValue, (float)sqrt(val));
        }
    }
    gsl_histogram *hist = gsl_histogram_alloc (bins);
    gsl_histogram_set_ranges_uniform (hist, minValue, maxValue);
    for(const float &value : m_values) {
        if(value >= 0) {
            gsl_histogram_increment (hist, sqrt(value));
        }
    }

    histogramVector.resize(bins);
    for(int i=0; i<bins; i++) {
        double upper, lower;
        gsl_histogram_get_range(hist, i, &lower, &upper);
        float middle = 0.5*(upper+lower);
        histogramVector[i].setX(middle);
        histogramVector[i].setY(gsl_histogram_get(hist,i));
    }

    gsl_histogram_free (hist);

    return histogramVector;
}
