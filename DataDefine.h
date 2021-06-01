// #pragma once
#ifndef DATADEFINE_H
#define DATADEFINE_H


/*
问题：如何定义全局函数(非inline)？（多个地方包含本头文件时，提示重复定义）
*/

#include <QVector>
#include <QPair>
#include <QVariant>
#include <QString>
#include <QDateTime>
#include <qmath.h>
#include <QJsonArray>


#define UserData_Category Qt::UserRole+1
#define UserData_Quantity Qt::UserRole+2
#define UserData_Function Qt::UserRole+3

#define UserData_Range_Min Qt::UserRole+4
#define UserData_Range_Min_Unit Qt::UserRole+5
#define UserData_Range_Max Qt::UserRole+6
#define UserData_Range_Max_Unit Qt::UserRole+7

#define UserData_Condition Qt::UserRole+8
#define UserData_GoalValue Qt::UserRole+9
#define UserData_ToleranceValue Qt::UserRole+10

inline double GetDBData(double re, double im)
{
    return 20.0 * log10(sqrt((re) * (re) + (im) * (im)));
}

enum Condition
{
	NoCnd = 0,
	LessAndEqual,
	Equal,
	GrantAndEqual,
	Minimum,
	Maximum
};

// get describe
inline QString getConditionDes(const Condition &eCnd)
{
	QString desStr;
	switch (eCnd)
	{
	case LessAndEqual:
		desStr = "<=";
		break;
	case Equal:
		desStr = "=";
		break;
	case GrantAndEqual:
		desStr = ">=";
		break;
	case Minimum:
		desStr = "Minimum";
		break;
	case Maximum:
		desStr = "Maximum";
		break;
	default:
		break;
	}

	return desStr;
}

enum Solution
{
	NoSolution = 0,
	Setup1Sweep,
	Setup1LastAdaptive
};

inline QString getSolutionDes(const Solution &eSolu)
{
	QString desStr;
	switch (eSolu)
	{
	case Setup1Sweep:
		desStr = "Setup1:Sweep";
		break;
	case Setup1LastAdaptive:
		desStr = "Setup1:LastAdaptive";
		break;
	default:
		break;
	}

	return desStr;
}

enum Category
{
	NoCategory = 0,
	SParameter,
	Zo,
	Phase,
	GroupDelay
};

inline QString getCategoryDes(const Category &eCategory)
{
	QString desStr;
	switch (eCategory)
	{
	case SParameter:
		desStr = "S";
		break;
	case Zo:
		desStr = "Zo";
		break;
	case Phase:
		desStr = "Ph";
		break;
	default:
		break;
	}

	return desStr;
}

inline QVector<QPair<QString, QVariant>> getSerializedCategory()
{
	QVector<QPair<QString, QVariant>> items;
	items.push_back(QPair<QString, QVariant>("S Parameter", SParameter));
	items.push_back(QPair<QString, QVariant>("Zo", Zo));
	items.push_back(QPair<QString, QVariant>("Phase", Phase));
	items.push_back(QPair<QString, QVariant>("Group Delay", GroupDelay));

	return items;
}

enum Quantity
{
	NoQuantity = 0,
	Quantity_11,
	Quantity_12,
	Quantity_13,
	Quantity_21,
	Quantity_22,
	Quantity_23,
	Quantity_31,
	Quantity_32,
	Quantity_33
};

inline QString getQuantityDes(const Quantity &eQuantity)
{
	QString desStr;
	switch (eQuantity)
	{
	case Quantity_11:
		desStr = "(1,1)";
		break;
	case Quantity_12:
		desStr = "(1,2)";
		break;
	case Quantity_13:
		desStr = "(1,3)";
		break;
	case Quantity_21:
		desStr = "(2,1)";
		break;
	case Quantity_22:
		desStr = "(2,2)";
		break;
	case Quantity_23:
		desStr = "(2,3)";
		break;
	case Quantity_31:
		desStr = "(3,1)";
		break;
	case Quantity_32:
		desStr = "(3,2)";
		break;
	case Quantity_33:
		desStr = "(3,3)";
		break;
	default:
		break;
	}

	return desStr;
}

inline QString getQuantityDes_S(const Quantity &eQuantity)
{
	QString desStr;
	switch (eQuantity)
	{
	case Quantity_11:
		desStr = "S11";
		break;
	case Quantity_12:
		desStr = "S12";
		break;
	case Quantity_13:
		desStr = "S13";
		break;
	case Quantity_21:
		desStr = "S21";
		break;
	case Quantity_22:
		desStr = "S22";
		break;
	case Quantity_23:
		desStr = "S23";
		break;
	case Quantity_31:
		desStr = "S31";
		break;
	case Quantity_32:
		desStr = "S32";
		break;
	case Quantity_33:
		desStr = "S33";
		break;
	default:
		break;
	}

	return desStr;
}

inline QVector<QPair<QString, QVariant>> getSerializedQuantity(const QString &prefix, int portNum)
{
	QVector<QPair<QString, QVariant>> items;
	if (portNum == 1)
	{
		items.push_back(QPair<QString, QVariant>(prefix + "(1,1)", Quantity_11));
	}
	else if (portNum == 2)
	{
		items.push_back(QPair<QString, QVariant>(prefix + "(1,1)", Quantity_11));
		items.push_back(QPair<QString, QVariant>(prefix + "(1,2)", Quantity_12));
		items.push_back(QPair<QString, QVariant>(prefix + "(2,1)", Quantity_21));
		items.push_back(QPair<QString, QVariant>(prefix + "(2,2)", Quantity_22));
	}
	else if (portNum == 3)
	{
		items.push_back(QPair<QString, QVariant>(prefix + "(1,1)", Quantity_11));
		items.push_back(QPair<QString, QVariant>(prefix + "(1,2)", Quantity_12));
		items.push_back(QPair<QString, QVariant>(prefix + "(1,3)", Quantity_13));
		items.push_back(QPair<QString, QVariant>(prefix + "(2,1)", Quantity_21));
		items.push_back(QPair<QString, QVariant>(prefix + "(2,2)", Quantity_22));
		items.push_back(QPair<QString, QVariant>(prefix + "(2,3)", Quantity_23));
		items.push_back(QPair<QString, QVariant>(prefix + "(3,1)", Quantity_31));
		items.push_back(QPair<QString, QVariant>(prefix + "(3,2)", Quantity_32));
		items.push_back(QPair<QString, QVariant>(prefix + "(3,3)", Quantity_33));
	}

	return items;
}

inline QVector<QPair<QString, QVariant>> getQuantityItems(const Category &eCate, int portNum)
{
	QVector<QPair<QString, QVariant>> quantityItems; 

	switch (eCate)
	{
	case NoCategory:
		break;
	case SParameter:
		quantityItems = getSerializedQuantity("S", portNum);
		break;
	case Zo:
		quantityItems = getSerializedQuantity("Zo", portNum);
		break;
	case Phase:
		quantityItems = getSerializedQuantity("Ph", portNum);
		break;
	case GroupDelay:
		quantityItems = getSerializedQuantity("GD", portNum);
		break;
	default:
		break;
	}

	return quantityItems;
}

enum Function
{
	NoFunc = 0,
	Db,
	Deg,
	Ns
};

inline QString getFunctionDes(const Function &eFunc)
{
	QString desStr;
	switch (eFunc)
	{
	case Db:
		desStr = "dB";
		break;
	case Deg:
		desStr = "deg";
		break;
	case Ns:
		desStr = "ns";
		break;
	default:
		break;
	}

	return desStr;
}

inline QVector<QPair<QString, QVariant>> getSerializedFunc()
{
	QVector<QPair<QString, QVariant>> items;
	//items.push_back(QPair<QString, QVariant>("None", None));
	items.push_back(QPair<QString, QVariant>("dB", Db));
	items.push_back(QPair<QString, QVariant>("deg", Deg));
	items.push_back(QPair<QString, QVariant>("ns", Ns));

	return items;
}

inline QString getCalculatonDes(const Category &eCategory, const Quantity &eQuantity, const Function & eFunc)
{
	if (eCategory == NoCategory || eQuantity == NoQuantity || eFunc == NoFunc)
		return "";

	QString desStr = QString("%1(%2%3)").arg(getFunctionDes(eFunc))
		.arg(getCategoryDes(eCategory))
		.arg(getQuantityDes(eQuantity));

	return desStr;
}

enum RangeUnit
{
	NoUnit = 0,
	Hz,
	Khz,
	Mhz,
	Ghz,
	Thz,
	Rps,
	PerSec
};

inline QVector<QPair<QString, QVariant>> getSerializedRaneUnit()
{
	QVector<QPair<QString, QVariant>> items;
	//items.push_back(QPair<QString, QVariant>("", NoUnit));
	//items.push_back(QPair<QString, QVariant>("Hz", Hz));
	//items.push_back(QPair<QString, QVariant>("KHz", Khz));
	items.push_back(QPair<QString, QVariant>("MHz", Mhz));
	items.push_back(QPair<QString, QVariant>("GHz", Ghz));
	//items.push_back(QPair<QString, QVariant>("THz", Thz));
	//items.push_back(QPair<QString, QVariant>("rps", Rps));
	//items.push_back(QPair<QString, QVariant>("per_sec", PerSec));
	return items;
}

inline QString getRangeUnitDes(const RangeUnit &eRangeUnit)
{
	QString desStr;
	switch (eRangeUnit)
	{
	case Hz:
		desStr = "Hz";
		break;
	case Khz:
		desStr = "kHz";
		break;
	case Mhz:
		desStr = "MHz";
		break;
	case Ghz:
		desStr = "GHz";
		break;
	case Thz:
		desStr = "THz";
		break;
	case Rps:
		desStr = "rps";
		break;
	case PerSec:
		desStr = "per_sec";
		break;
	default:
		break;
	}

	return desStr;
}

inline int transRangeUnit(const RangeUnit &eRangeUnit)
{
	double count;
	switch (eRangeUnit)
	{
	case Hz:
		count = 1.0;
		break;
	case Khz:
		count = 1.0e+3;
		break;
	case Mhz:
		count = 1.0e+6;
		break;
	case Ghz:
		count = 1.0e+9;
		break;
	case Thz:
	case Rps:
	case PerSec:
	default:
		break;
	}

	return count;
}

typedef struct tag_calcRange
{
	tag_calcRange()
	{
		_min = 0;
		_eMinUnit = NoUnit;
		_max = 0;
		_eMaxUnit = NoUnit;
	}

	tag_calcRange(float min, RangeUnit eMinUnit, float max, RangeUnit eMaxUnit)
	{
		_min = min;
		_eMinUnit = eMinUnit;
		_max = max;
		_eMaxUnit = eMaxUnit;
	}

	float _min;
	RangeUnit _eMinUnit;
	float _max;
	RangeUnit _eMaxUnit;
}CalcRange;

inline QString getCalcRangeDes(const CalcRange & calcRange)
{
	if (calcRange._eMaxUnit == NoUnit || calcRange._eMinUnit == NoUnit)
		return "";

	QString desStr = QString("Freq(%1%2:%3%4)")
		.arg(calcRange._min).arg(getRangeUnitDes(calcRange._eMinUnit))
		.arg(calcRange._max).arg(getRangeUnitDes(calcRange._eMaxUnit));
	return desStr;
}

inline QJsonArray getCalcRangeVal(const CalcRange& calcRange)
{
	QJsonArray range;
	float minVal,maxVal;

	if (calcRange._eMaxUnit == NoUnit || calcRange._eMinUnit == NoUnit)
		return range;

	switch (calcRange._eMinUnit)
	{
	case Mhz:
		minVal=calcRange._min;
		break;
	case Ghz:
		minVal = calcRange._min*qPow(10, 3);
		break;
	default:
		break;
	}
	switch (calcRange._eMaxUnit)
	{
	case Mhz:
		maxVal = calcRange._max;
		break;
	case Ghz:
		maxVal = calcRange._max*qPow(10, 3);
		break;
	default:
		break;
	}

	range.append(minVal);
	range.append(maxVal);

	return range;
}

typedef float GoalValue;
typedef float WeightValue;
typedef float ToleranceValue;

typedef struct tag_CostFunction
{
	tag_CostFunction()
	{
		_eCategory = NoCategory;
		_eQuantity = NoQuantity;
		_eFunc = NoFunc;
		_eCnd = NoCnd;
		_goal = 1;
		_tolerance = 1;
		_rowid = 1;
	}

	tag_CostFunction( Category eCategory, Quantity eQuantity,
		Function eFunc, Condition eCnd, CalcRange range, GoalValue goal, ToleranceValue tolerance,int rowid=1)
	{
		_eCategory = eCategory;
		_eQuantity = eQuantity;
		_eFunc = eFunc;
		_eCnd = eCnd;
		_range = range;
		_goal = goal;
		_tolerance = tolerance;
		_rowid = rowid;
	}

	tag_CostFunction(const tag_CostFunction &other)
	{
		_eCategory = other._eCategory;
		_eQuantity = other._eQuantity;
		_eFunc = other._eFunc;
		_eCnd = other._eCnd;
		_range = other._range;
		_goal = other._goal;
		_tolerance = other._tolerance;
		_rowid = other._rowid;
	}

	Category _eCategory;
	Quantity _eQuantity;
	Function _eFunc;

	CalcRange _range;
	Condition _eCnd;
	GoalValue _goal;
	ToleranceValue _tolerance;
	int _rowid;
}CostFunc, *PCostFunc;

enum VisaChannel
{
	NoChannel = 0x0000,
	Channel_11 = 0x0001,
	Channel_12 = 0x0002,
	Channel_13 = 0x0004,
	Channel_21 = 0x0008,
	Channel_22 = 0x0010,
	Channel_23 = 0x0020,
	Channel_31 = 0x0040,
	Channel_32 = 0x0080,
	Channel_33 = 0x0100
};
Q_FLAGS(VisaChannels)
Q_DECLARE_FLAGS(VisaChannels, VisaChannel)


typedef struct tag_Visa
{
	tag_Visa()
	{
		_portNum = 0;
		_pointCnt = 0;
		_start = 0;
		_startUnit = NoUnit;
		_stop = 0;
		_stopUnit = NoUnit;
		_channels = NoChannel;
	}

	tag_Visa(int portNum, int pointCnt, float start, RangeUnit startUnit, float stop, RangeUnit stopUnit, int channels, QString modelFile)
	{
		_portNum = portNum;
		_pointCnt = pointCnt;
		_start = start;
		_startUnit = startUnit;
		_stop = stop;
		_stopUnit = stopUnit;
		_channels = channels;
		_modelFile = modelFile;
	}

	tag_Visa(const tag_Visa & other)
	{
		_portNum = other._portNum;
		_pointCnt = other._pointCnt;
		_start = other._start;
		_startUnit = other._startUnit;
		_stop = other._stop;
		_stopUnit = other._stopUnit;
		_channels = other._channels;
		_modelFile = other._modelFile;
	}

	int _portNum;
	int _pointCnt;
	double _start;
	RangeUnit _startUnit;
	double _stop;
	RangeUnit _stopUnit;
	int _channels;
	QString _modelFile;
}Visa, *PVisa;

typedef struct tag_ProductInfo
{
	tag_ProductInfo()
	{
	}
	~tag_ProductInfo()
	{
	}

	tag_ProductInfo(const tag_ProductInfo& other)
	{
		_dateTime = other._dateTime;
		_proName = other._proName;
		_visa = other._visa;
		_costFunc = other._costFunc;
		_imageFile = other._imageFile;
	}

	tag_ProductInfo(QString proName, Visa visa, QVector<CostFunc> costFunc, QString imageFile, QString modelFile)
	{
		_proName = proName;
		_visa = visa;
		_costFunc = costFunc;
		_imageFile = imageFile;
	}

	// 写复制

	QString _proName;
	QDateTime _dateTime;
	Visa _visa;
	QVector<CostFunc> _costFunc;
	QString _imageFile;
}ProductInfo, *PProductInfo;

enum VisaType
{
	emE5071C = 0,
	emE5063A,
	emZNLE6,
	em8720ES
};

struct ParameterInfo
{
	ParameterInfo() = default;

	ParameterInfo(const ParameterInfo& other)
	{
		variable = other.variable;
		override = other.override;
		startingValue = other.startingValue;
		startingValueUnits = other.startingValueUnits;
		include = other.include;
		min = other.min;
		max = other.max;
        includeAble = other.includeAble;
	}

	QString variable{};
	bool override{ false };
    QString startingValue{};
	QString startingValueUnits{};
	bool include{ false };
    bool includeAble{ true };
    QString min{};
    QString max{};
};

struct DEA_Info
{
    DEA_Info() = default;

    DEA_Info(const DEA_Info& other)
    {
        company_name = other.company_name;
        product_type = other.product_type;
        product_model = other.product_model;
        product_order = other.product_order;
        bw = other.bw;
        fo = other.fo;
        Standard_HFSS_Model_path = other.Standard_HFSS_Model_path;
        CollectData_tmpHFSS_path = other.CollectData_tmpHFSS_path;
        Data_for_optiAI_path = other.Data_for_optiAI_path;
        Data_for_preAI_path = other.Data_for_preAI_path;
        optiAI_agent_path = other.optiAI_agent_path;
        preAI_agent_path = other.preAI_agent_path;
		zero_num = other.zero_num;
    }

    QString company_name{};
    QString product_type{};
    QString product_model{};
    QString product_order{};
    QString bw{};
    QString fo{};
    QString Standard_HFSS_Model_path{};
    QString CollectData_tmpHFSS_path{};
    QString Data_for_optiAI_path{};
    QString Data_for_preAI_path{};
    QString optiAI_agent_path{};
    QString preAI_agent_path{};
	int zero_num;
};
#endif // !DATADEFINE
