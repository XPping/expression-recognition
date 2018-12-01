#ifndef MYAREACHART_H
#define MYAREACHART_H

#include <QtCharts/QChart>
#include <QtCore/QTimer>

QT_CHARTS_BEGIN_NAMESPACE
class QSplineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

class MyAreaChart: public QChart
{
    Q_OBJECT
public:
    MyAreaChart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~MyAreaChart();

    DataTable generateRandomData(int listCount, int valueMax, int valueCount);
    QChart *createAreaChart();
public slots:

private:
};


#endif // MYAREACHART_H
