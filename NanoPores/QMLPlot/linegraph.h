#ifndef LINEGRAPH_H
#define LINEGRAPH_H

#include "graph.h"
#include <algorithm>

class LineGraphDataSource : public QObject {
    Q_OBJECT
private:
    int m_firstIndex = 0;
    int m_numberOfPoints = 0;
    int m_maxNumberOfPoints = 200;
    void cleanupMemory();
public:
    QVector<QPointF> m_points;

    Q_INVOKABLE void addPoint(float x, float y);
    QPointF operator[](int index) { return m_points[index+m_firstIndex]; }
    QPointF get(int index) { return m_points[index+m_firstIndex]; }
    int size() { return m_numberOfPoints; }
    void clear() {
        m_firstIndex = 0;
        m_numberOfPoints = 0;
        m_points.clear();
    }
    void SetSource(const QVector<QPointF>& p) {
        m_points = p;
        m_firstIndex = 0;
        m_numberOfPoints = m_points.size();
        emit dataChanged();
    }

    void setMaxNumberOfPoints(int i) {
        m_maxNumberOfPoints = i;
    }

signals:
    void dataChanged();
};

class LineGraph : public Graph
{
    Q_OBJECT
    Q_PROPERTY(LineGraphDataSource* dataSource READ dataSource WRITE setDataSource NOTIFY dataSourceChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(Qt::PenStyle style READ style WRITE setStyle NOTIFY styleChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
private:
    LineGraphDataSource m_defaultDataSource;
    LineGraphDataSource* m_dataSource = nullptr;
    bool isInValueRange(const QPointF &point, const float &xMin, const float &xMax, const float &yMin, const float &yMax);
    QColor m_color = "black";
    Qt::PenStyle m_style = Qt::SolidLine;
    int m_width = 1;

public:
    LineGraph();
    ~LineGraph();
    virtual void paint(Figure *figure, QPainter *painter);
    LineGraphDataSource* dataSource() const;
    QColor color() const;
    Qt::PenStyle style() const;
    int width() const;

    void clear() override {
        dataSource()->clear();
    }


    double getMaxX() override {
        if (dataSource()->size()==0)
            return 1;
        double m = -1E20;
        for(int i=0;i<dataSource()->size();i++)
            m = std::max(m, dataSource()->get(i).x());
        return m;
    }
    double getMinX() override {
        double m = 1E20;
        if (dataSource()->size()==0)
            return 0;
        for(int i=0;i<dataSource()->size();i++)
            m = std::min(m, dataSource()->get(i).x());
        return m;
    }
    double getMaxY() override {
        double m = -1E20;
        if (dataSource()->size()==0)
            return 1;
        for(int i=0;i<dataSource()->size();i++)
            m = std::max(m, dataSource()->get(i).y());
        return m;
    }
    double getMinY() override {
        double m = 1E20;
        if (dataSource()->size()==0)
            return 1;
        for(int i=0;i<dataSource()->size();i++)
            m = std::min(m, dataSource()->get(i).y());
        return m;
    }

public slots:
    void addPoint(float x, float y);
    void setDataSource(LineGraphDataSource* dataSource);
    void setColor(QColor color);
    void setStyle(Qt::PenStyle style);
    void setWidth(int width);

signals:
    void dataSourceChanged(LineGraphDataSource* dataSource);
    void colorChanged(QColor color);
    void styleChanged(Qt::PenStyle style);
    void widthChanged(int width);
};

#endif // LINEGRAPH_H
