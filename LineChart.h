#pragma once

#include <QWidget>
#include <QPointer>
#include "ui_LineChart.h"
#include "DataDefine.h"

struct LineGoal
{

	void Release()
	{
		if (!line.isNull())
		{
			line->deleteLater();
		}
		if (!lineTop.isNull())
		{
			lineTop->deleteLater();
		}
		if (!lineButtom.isNull())
		{
			lineButtom->deleteLater();
		}
	}

	void Clear()
	{
		if (!line.isNull())
		{
			line->setVisible(false);
		}
		if (!lineTop.isNull())
		{
			lineTop->setVisible(false);
		}
		if (!lineButtom.isNull())
		{
			lineButtom->setVisible(false);
		}
	}

	QPointer<QCPGraph> line{ nullptr };
	QPointer<QCPGraph> lineTop{ nullptr };
	QPointer<QCPGraph> lineButtom{ nullptr };
};

class LineChart : public QWidget
{
	Q_OBJECT

public:
	LineChart(int size, const QString& title, QWidget *parent = Q_NULLPTR);

	explicit LineChart(const QString& title, QWidget *parent = Q_NULLPTR);

	~LineChart() override;

	void UpdateData(QVector<QVector<float>> vecData);

	int AddGoal(float start, float end, Condition cnd, double value);

	void SetGoalByIndex(int index, float start, float end, Condition cnd, double value);

	void SetAxisMaxMin(float xMax, float xMin, float yMax, float yMin);

	//! 设置title
	void setTitle(const QString& title);

	//! 清除曲线
	void clearSplines();
	void clearGoal();

	void SetAxisLabels(const QString& xLabel, const QString& yLabel);

	void mouseMoveEvent(QMouseEvent *e) override;

	void mouseDoubleClick(QMouseEvent * event);

protected:
    void resizeEvent(QResizeEvent *event) override
    {
        QLinearGradient p(1,1,1,this->height()*0.85);
        p.setColorAt(0,QColor(203,225,244));
        p.setColorAt(1,Qt::white);
        p.setSpread(QGradient::PadSpread);
        ui.chartView->setBackground(p);
        ui.chartView->replot();
        QWidget::resizeEvent(event);
    }

private slots:
	
private:
    void AddSeries();

	void InitUi();

	float m_xMin;
	float m_xMax;
	float m_yMin;
	float m_yMax;

	QVector<QPointer<QCPGraph>> m_series;

	// 指标
	QVector<LineGoal> m_Goal;

	Ui::LineChart ui;
};