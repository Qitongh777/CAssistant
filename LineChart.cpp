#include "LineChart.h"
#include "qtooltip.h"
#include "qcustomplot.h"

LineChart::LineChart(int size, const QString& title, QWidget* parent)
        : QWidget(parent)
{
    ui.setupUi(this);

    for (int i = 0; i < size; i++)
    {
        AddSeries();
    }

    if (!title.isEmpty())
    {
        ui.groupBox->setTitle(title);
    }

    InitUi();
}

LineChart::LineChart(const QString& title, QWidget* parent)
        : QWidget(parent)
{
    ui.setupUi(this);

    if (!title.isEmpty())
    {
        ui.groupBox->setTitle(title);
    }

    InitUi();
}

LineChart::~LineChart()
{
	for (auto& p : m_series)
	{
		if (!p.isNull())
		{
			p->deleteLater();
		}
	}
	for (auto& srt : m_Goal)
	{
		srt.Release();
	}
}

void LineChart::clearGoal()
{
	for (auto& srt : m_Goal)
	{
		srt.Clear();
	}
	//m_Goal.clear();
	ui.chartView->replot();
}

void LineChart::mouseDoubleClick(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (ui.chartView->axisRect()->rect().contains(event->pos()) && (ui.chartView->axisRect()->insetLayout()->selectTest(event->pos(), false) < 0))
		{
            ui.chartView->xAxis->setRange(m_xMin, m_xMax);
            ui.chartView->yAxis->setRange(m_yMin, m_yMax);
		}
		ui.chartView->replot();
	}
}

void LineChart::UpdateData(QVector<QVector<float>> vecData)
{
	if (isHidden())
	{
		return;
	}

	if (vecData.isEmpty())
	{
		return;
	}
	int pointNum = vecData.first().size();
	double kx = (m_xMax - m_xMin) / (float)pointNum;

	QVector<QPair<QVector<double>, QVector<double>>> pointF;
	for (const auto& vec : vecData)
	{
        double x = m_xMin;
		QVector<double> xData;
		QVector<double> yData;
		for (const auto& v : vec)
		{
			xData.push_back(x += kx);
			yData.push_back(v);
		}
		pointF.push_back(QPair<QVector<double>, QVector<double>>(xData, yData));
	}

    while(pointF.size() > m_series.size())
    {
        AddSeries();
    }

    for (int i = 0; i < m_series.size(); i++)
    {
        if (i >= pointF.size())
        {
            m_series[i]->clearData();
        }
        else
        {
            m_series[i]->setData(pointF.at(i).first, pointF.at(i).second);
        }
    }
	ui.chartView->replot();
}

int LineChart::AddGoal(float start, float end, Condition cnd, double value)
{
	LineGoal goal;
	goal.line = ui.chartView->addGraph();
	goal.line->setPen(QPen(Qt::red));

	goal.lineTop = ui.chartView->addGraph();
	goal.lineTop->setPen(QPen(Qt::transparent));
	goal.lineTop->setBrush(QBrush(Qt::red, Qt::FDiagPattern));

	goal.lineButtom = ui.chartView->addGraph();
	goal.lineButtom->setPen(QPen(Qt::transparent));
	goal.lineButtom->setBrush(QBrush(Qt::red, Qt::BDiagPattern));

	goal.lineTop->setChannelFillGraph(goal.line);
	goal.lineButtom->setChannelFillGraph(goal.line);

	m_Goal.push_back(goal);
	int index = m_Goal.size() - 1;
	SetGoalByIndex(index, start, end, cnd, value);
	ui.chartView->replot();
	return index;

}

void LineChart::SetGoalByIndex(int index, float start, float end, Condition cnd, double value)
{
	while (m_Goal.size() <= index)
	{
		AddGoal(start, end, cnd, value);
	}

	auto& goal = m_Goal[index];
	if (goal.line != nullptr)
	{
		QVector<double> x{ start ,end }, y{ value ,value };
		goal.line->setData(x, y);
		goal.line->setVisible(true);
	}
	if (goal.lineTop != nullptr)
	{
		QVector<double> x{ start ,end }, y{ value + 5.0 ,value + 5.0 };
		goal.lineTop->setData(x, y);
		goal.lineTop->setVisible(true);
	}
	if (goal.lineButtom != nullptr)
	{
		QVector<double> x{ start ,end }, y{ value - 5.0 ,value - 5.0 };
		goal.lineButtom->setData(x, y);
		goal.lineButtom->setVisible(true);
	}

	switch (cnd)
	{
	case NoCnd:
		break;
	case LessAndEqual:
	case Maximum:
		if (goal.lineTop != nullptr)
		{
			goal.lineTop->setVisible(false);
		}
		break;
	case Equal:
		break;
	case GrantAndEqual:
	case Minimum:
		if (goal.lineButtom != nullptr)
		{
			goal.lineButtom->setVisible(false);
		}
		break;
	default:
		break;
	}
	ui.chartView->replot();
}

void LineChart::SetAxisMaxMin(float xMax, float xMin, float yMax, float yMin)
{
    m_xMax = xMax;
    m_xMin = xMin;
    m_yMax = yMax;
    m_yMin = yMin;

	ui.chartView->xAxis->setRange(xMin, xMax);
	ui.chartView->yAxis->setRange(yMin, yMax);
}

void LineChart::setTitle(const QString & title)
{
	ui.groupBox->setTitle(title);
}

void LineChart::clearSplines()
{
	for (auto& p : m_series)
	{
		p->clearData();
	}
	ui.chartView->replot();
}

void LineChart::InitUi()
{
    setContentsMargins(0, 0, 0, 0);
    layout()->setContentsMargins(0, 0, 0, 0);

	ui.chartView->setInteraction(QCP::iRangeDrag, true);
	ui.chartView->setInteraction(QCP::iRangeZoom, true);

    m_xMin = 2270;
    m_xMax = 3375;

	ui.chartView->xAxis->setRange(m_xMin, m_xMax);
	ui.chartView->yAxis->setRange(-80, 0);

    connect(ui.chartView, &QCustomPlot::mouseDoubleClick, this, &LineChart::mouseDoubleClick);
    connect(ui.chartView, &QCustomPlot::mouseMove, this, &LineChart::mouseMoveEvent);
}

void LineChart::AddSeries()
{
    // 紫 蓝 绿 黄 橙 粉
    static QList<QColor> color{QColor(170, 0, 255), QColor(0, 0, 255),
                               QColor(0, 255, 0), QColor(255, 255, 0),
                               QColor(255, 170, 0), QColor(255, 0, 170)};
    while (m_series.size() >= color.size())
    {
        // 默认蓝色
        color << QColor(170, 0, 255);
    }
    auto p = ui.chartView->addGraph();
    p->setPen(QPen(color.at(m_series.size())));
    m_series.push_back(p);
}

void LineChart::mouseMoveEvent(QMouseEvent* event)
{
    if (ui.chartView->graphCount() == 0)
    {
        return;
    }
    int xPos = event->pos().x();
    int yPos = event->pos().y();

    double xVal = ui.chartView->xAxis->pixelToCoord(xPos);
    double yVal = ui.chartView->yAxis->pixelToCoord(yPos);

    auto tooltip = QString::number(xVal, 10, 2) + ',' + QString::number(yVal, 10, 2);
    QToolTip::showText(cursor().pos(), tooltip, ui.chartView);

    return QWidget::mouseMoveEvent(event);
}

void LineChart::SetAxisLabels(const QString& xLabel, const QString& yLabel)
{
    ui.chartView->xAxis->setLabel(xLabel);
    ui.chartView->yAxis->setLabel(yLabel);
}