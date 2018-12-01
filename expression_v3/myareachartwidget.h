#ifndef MYAREACHARTWIDGET_H
#define MYAREACHARTWIDGET_H

#include <QAreaSeries>
#include <QWidget>
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


/*
 * MyAreaChartWidget is a QChart Widget Component inherit QWidget
 */
class MyAreaChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAreaChartWidget(QWidget *parent = nullptr);

    DataTable generateRandomData(int listCount, int valueMax, int valueCount);
    QChart *createAreaChart();
signals:

public slots:
    void addNewPoint(QVector<qreal> vector);
private:
    QChart *m_chart;
    QVector<QAreaSeries *> m_areaSeriseList;
    QAreaSeries *m_areaSeries;
    QVector<QSplineSeries *> m_splineSeriesList;
    QValueAxis *m_axisX, *m_axisY;
    qreal m_number;
    qreal m_x;
    QVector<qreal> m_y;

    QString legend[7] = {"An", "Di", "Fe", "Ha", "Ne", "Sa", "Su"};
};

#endif // MYAREACHARTWIDGET_H
