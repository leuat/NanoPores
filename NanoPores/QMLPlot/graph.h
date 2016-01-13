#ifndef GRAPH_H
#define GRAPH_H

#include <QQuickItem>
#include <QPainter>
class Figure;
class Graph : public QQuickItem
{
    Q_OBJECT
protected:
    bool m_dirty = true;

public:
    virtual void paint(Figure *figure, QPainter *painter) = 0;
    virtual double getMaxX() = 0;
    virtual double getMinX() = 0;
    virtual double getMaxY() = 0;
    virtual double getMinY() = 0;
    virtual void clear() = 0;
signals:

public slots:
    void setDirty(bool dirty);
    void markDirty();
};

#endif // GRAPH_H
