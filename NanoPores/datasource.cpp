#include "datasource.h"
#include <QDebug>

bool DataSource::isValid()
{
    if(m_xValuesRaw.size() != m_yValuesRaw.size()) return false;
    if(m_xValues.size() != m_yValues.size()) return false;
    return true;
}

DataSource::DataSource()
{

}

QVariantList DataSource::xValues() const
{
    return m_xValues;
}

QVariantList DataSource::yValues() const
{
    return m_yValues;
}

float DataSource::xMin()
{
    float xMin = 1e30;
    for(const float &x : m_xValuesRaw) {
        xMin = std::min(x, xMin);
    }
}

float DataSource::xMax()
{
    float xMax = -1e30;
    for(const float &x : m_xValuesRaw) {
        xMax = std::max(x, xMax);
    }
}

float DataSource::yMin()
{
    float yMin = 1e30;
    for(const float &y : m_yValuesRaw) {
        yMin = std::min(y, yMin);
    }
}

float DataSource::yMax()
{
    float yMax = -1e30;
    for(const float &y : m_yValuesRaw) {
        yMax = std::max(y, yMax);
    }
}

void DataSource::setPoints(QVector<QPointF> points, bool normalized)
{
    int numberOfPoints = points.size();
    m_xValuesRaw.resize(numberOfPoints);
    m_yValuesRaw.resize(numberOfPoints);
    for(int i=0; i<numberOfPoints; i++) {
        m_xValuesRaw[i] = points[i].x();
        m_yValuesRaw[i] = points[i].y();
    }
    if(normalized) {
        normalizeArea();
        qDebug() << "Did normalize.";
    }
    qDebug() << "Did set points.";
    update();
    qDebug() << "Did update.";
}

void DataSource::normalizeArea()
{
    if(!isValid()) {
        qDebug() << "Could not normalize area in data source. x.size() != y.size(). Aborting!";
        return;
    }

    double integralSum = 0;
    int numberOfPoints = m_xValuesRaw.size();
    for(int i=0; i<numberOfPoints-1; i++) {
        float x1 = m_xValuesRaw[i];
        float x2 = m_xValuesRaw[i+1];
        float y1 = m_yValuesRaw[i];
        float y2 = m_yValuesRaw[i+1];
        double dx = x2 - x1;
        double dy = y1 + y2;
        integralSum += dx*dy;
    }
    integralSum *= 0.5;
    for(int i=0; i<numberOfPoints-1; i++) {
        m_yValuesRaw[i] /= integralSum;
    }
}

void DataSource::update()
{
    if(!isValid()) {
        qDebug() << "Could not update data source. x.size() != y.size(). Aborting!";
        return;
    }

    m_xValues.clear();
    m_yValues.clear();
    m_xValues.reserve(m_xValuesRaw.size());
    m_yValues.reserve(m_yValuesRaw.size());

    for(int i=0; i<m_xValuesRaw.size(); i++) {
        m_xValues.push_back(QVariant::fromValue<float>(m_xValuesRaw[i]));
        m_yValues.push_back(QVariant::fromValue<float>(m_yValuesRaw[i]));
    }

    emit updated();
}

void DataSource::setXValues(QVariantList xValues)
{
    if (m_xValues == xValues)
        return;

    m_xValues = xValues;
    emit xValuesChanged(xValues);
}

void DataSource::setYValues(QVariantList yValues)
{
    if (m_yValues == yValues)
        return;

    m_yValues = yValues;
    emit yValuesChanged(yValues);
}
