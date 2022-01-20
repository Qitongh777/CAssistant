#ifndef C_ASSISTANT_EVENT_H
#define C_ASSISTANT_EVENT_H

#include "DataDefine.h"
#include <QJsonObject>
#include "Json.h"
struct Event {
    virtual void Flag()
    {
        ++count;
    }

    virtual void Release()
    {
        --count;
        if (count == 0)
        {
            delete this;
        }
    }

    int count{0};
};

struct Event_TreeEnable : Event
{
    bool enable{false};
};

// 连接矢网完成事件
struct Event_VisaConnectFinish : Event
{
    bool isSuccess{false};
    QString msg{};
    Visa visaInfo{};
};

struct Event_LoginPostBack :Event
{
    QString UserId{};
    QString Token{};
    QString company{};
    int Code;
    bool isOk{false};
    QString Message{};
};

struct Event_Regist :Event
{
	int Code;
    bool isOk{false};
	QString Message{};
};

struct Event_RegistCode :Event
{
	int Code;
    bool isOk{false};
	QString Message{};
};

struct Event_ForgetCode :Event
{
	int Code;
    bool isOk{false};
	QString Message{};
};

struct Event_ResetPass :Event
{
	int Code;
    bool isOk{false};
	QString Message{};
}; 

struct Event_ProInfo :Event
{
	int Code;
    bool isOk{false};
	QString Message{};
	QVector<ProductInfoNet> proNetVec;
}; 

struct Event_NewPro :Event
{
	int Code;
    bool isOk{false};
	QString Message{};
};

struct Event_ProInfoDetails :Event
{
	int Code;
	bool isOk{false};
	QString Message{};
	CreateProInfo proInfo;
	QString userName{};
	VisaInfo vInfo;
	QString resonance{};
	QString coupling{};
	QString otherParams{};
	QString crtTs{};
	QString status{};
	QString jsonUrl{};
    QJsonArray curve{};
};

struct Event_BarData : Event
{
    QVector<float> data{};
};

struct Event_RobotInfo : Event
{
    bool isOk{false};
    int Code{};
    QString Message{};
    QVector<ProductInfoNet> proInfoVec{};
    QVector<VisaProInfo> visaInfoVec{};
    QVector<MachineInfo> machineInfoVec{};
};

struct Event_RobotInfoDetails : Event
{
    int Code;
    bool isOk{false};
    QString Message{};
    CreateProInfo proInfo;
    QString userName{};
    VisaInfo vInfo;
    QString resonance{};
    QString coupling{};
    QString otherParams{};
    QString crtTs{};
    QString status{};
    QString jsonUrl{};
    QJsonArray curve{};
    RobotInfoDetails robotInfo;
    int robotId{};
};

struct Event_PythonEvent :Event
{
    QJsonObject pythonObj;
};

struct Event_LogFromPy : Event
{
    QString log{};
    int errorType{-1};
};

struct Event_RunningModel : Event
{
    QString runningModel{};
};
struct Event_OptimizeFinished : Event
{
    QJsonObject optimizeObj;
};

struct Event_OptVar : Event
{
    QJsonObject optVarObj;
};

struct Event_StartProject : Event
{
};

struct Event_ViewError : Event
{
    QJsonObject viewErrorObj;
};

struct Event_NetClose : Event
{
    bool isAlive{};
};

struct Event_ProjectStatus : Event
{
    bool isRunning{false};
};

struct Event_ParameterInfo : Event
{
    Json ParameterInfoObj;
};

struct Event_CheckFilePostBack : Event
{
    bool isOk{false};
    int Code;
    QHash<QString, int> hashMap{};
};

struct Event_ParameterToOPTIM : Event
{
    QMap<QString, ParameterInfo> mapParameter{};
};

struct Event_GoalToOPTIM : Event
{
    QMap<QString,QString> map;
};

struct Event_BaseJson : Event
{
    QJsonObject obj;
};

struct Event_LineChart : Event
{
    QMap<QString,QVector<LineCostFunc>> list;
    QMap<QString,QString> map;
};

struct Event_FreqGoal : Event
{
    float start;
    float end;
    QString unit;
};

struct Event_TuningFinished : Event
{
    QJsonObject obj;
};

struct Event_OptHistory : Event
{
    QJsonObject obj;
};

struct Event_PythonFinish : Event
{
    QString FunctionName{};
};

struct Event_PythonStart : Event
{
};

struct Event_CMDList : Event
{
    QStringList cmdList{};
};

struct Event_NeuralNet : Event
{
    QJsonObject obj;
};

struct Event_ParameterJson : Event
{
    QJsonObject obj;
};

struct Event_SaveProject : Event
{
    QString path;
};

struct Event_FilterIndexExtract : Event
{
    QJsonObject obj;
};

struct Event_dealPhaseLinearity : Event
{
    QJsonObject obj;
};

struct Event_dealSampling : Event
{
    QJsonObject obj;
};

struct Event_GrpcCallBack : Event
{
    std::string str;
};

struct Event_dealPythonConsoleInfo : Event
{
    QJsonObject obj;
};

struct Event_dealParameter : Event
{
    QJsonObject obj;
};

struct Event_dealOPTIMFinished : Event
{
    
};

struct Event_dealDrawLine : Event
{
    QJsonObject obj;
};

struct Event_dealFilterSimulate : Event
{
    QJsonObject obj;
};
#endif //C_ASSISTANT_EVENT_H