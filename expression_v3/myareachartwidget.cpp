#include "myareachartwidget.h"

#include <QAreaSeries>
#include <QChartView>
#include <QDebug>
#include <QGridLayout>
#include <QLineSeries>
#include <QSplineSeries>
#include <QTime>
#include <QValueAxis>

MyAreaChartWidget::MyAreaChartWidget(QWidget *parent) : QWidget(parent),
    m_chart(0),
    m_areaSeries(0),
    m_splineSeriesList(0),
    m_areaSeriseList(0),
    m_axisX(new QValueAxis), m_axisY(new QValueAxis),
    m_number(3),
    m_x(5), m_y(0)
{
    m_chart = new QChart();
    // Chart setting
    m_chart->setTitle("Dynamic multi spline");
    //m_chart->legend()->hide();
    m_chart->setAnimationOptions(QChart::AllAnimations);
    // XY axis setting
    m_axisX->setTickCount(5);
    m_axisX->setRange(1, 10);
    m_axisY->setRange(0, 1);

    /*
    // First SplineSeries
    QSplineSeries * qs1 = new QSplineSeries(m_chart);
    qs1->setName("An");
    QPen pen1(Qt::red); pen1.setWidth(2);
    qs1->setPen(pen1); qs1->append(5, 1);

    // Second SplineSeries
    QSplineSeries * qs2 = new QSplineSeries(m_chart);
    qs2->setName("Ha");
    QPen pen2(Qt::blue); pen2.setWidth(2);
    qs2->setPen(pen2); qs2->append(5, 1);
    // Third SplineSeries
    QSplineSeries * qs3 = new QSplineSeries(m_chart);
    qs3->setName("Sa");
    QPen pen3(Qt::green); pen3.setWidth(2);
    qs3->setPen(pen3); qs3->append(5, 1);

    // Add SplineSeries to chart;
    m_chart->addSeries(qs1);
    m_chart->addSeries(qs2);
    m_chart->addSeries(qs3);
    // Add SplineSeries to global cache
    m_splineSeriesList.append(qs1);
    m_splineSeriesList.append(qs2);
    m_splineSeriesList.append(qs3);
    */
    m_areaSeriseList.resize(0);
    for(int i=0; i<7; i++){
        // AreaSeries
        QLineSeries *lowerSeries = new QLineSeries();
        QLineSeries *upperSeries = new QLineSeries();
        lowerSeries->append(1, 0);
        upperSeries->append(1, 0);
        QAreaSeries *series = new QAreaSeries(upperSeries, lowerSeries);
        series->setColor(QColor(255/6*i, 255/6*(6-i), 255/6*i));
        series->setName(legend[i]);
        m_areaSeriseList.push_back(series);
    }
    for(int i=0; i<7; i++)
        m_chart->addSeries(m_areaSeriseList[i]);


    // Chart add XY axis
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);

    // Series bind axis
    for(int i=0; i<7; i++){
        m_areaSeriseList[i]->attachAxis(m_axisX);
        m_areaSeriseList[i]->attachAxis(m_axisY);
    }
    // Chart Align
    m_chart->legend()->setAlignment(Qt::AlignRight);
    //m_areaSeries->attachAxis(m_axisX);
    //m_areaSeries->attachAxis(m_axisY);
    //qs1->attachAxis(m_axisX);
    //qs1->attachAxis(m_axisY);
    //qs2->attachAxis(m_axisX);
    //qs2->attachAxis(m_axisY);
    //qs3->attachAxis(m_axisX);
    //qs3->attachAxis(m_axisY);
    // Add chart into a QChartView for show up
    QChartView *chartView;
    chartView = new QChartView(m_chart);
    // Any Layout.
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->addWidget(chartView);
    this->setLayout(gridLayout);

    chartView->show();
}

void MyAreaChartWidget::addNewPoint(QVector<qreal> vector)
{
    qreal dist = m_chart->plotArea().width() / m_axisX->tickCount();       // Each move distance
    qreal x = (m_axisX->max() - m_axisX->min()) / m_axisX->tickCount(); // Current x axis

    m_x += x;
    qreal sum = 0.0;
    for (int i(0); i<vector.size(); i++){

        QLineSeries *lowerSeries = m_areaSeriseList[i]->lowerSeries();
        QLineSeries *upperSeries = m_areaSeriseList[i]->upperSeries();
        lowerSeries->append(m_x, sum);
        upperSeries->append(m_x, sum+vector[i]);
        m_areaSeriseList[i]->setLowerSeries(lowerSeries);
        m_areaSeriseList[i]->setUpperSeries(upperSeries);
        sum += vector[i];
    }

    m_chart->scroll(dist, 0);
}

