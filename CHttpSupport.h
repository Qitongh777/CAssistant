/************************************************************
 *              http数据处理接口类
 * **********************************************************
 *
 *          Project Name : CHttpSupport
 *
 *              FileName : C_HTTP_SUPPORT_H
 *
 *            Programmer :
 *
 *            Start Date : 2021-07-30
 *
 *           Last Update :
 *
 ***********************************************************/
#ifndef C_HTTP_SUPPORT_H
#define C_HTTP_SUPPORT_H

#include <QNetworkAccessManager>

class CHttpSupport : public QObject
{
Q_OBJECT
private:
    explicit CHttpSupport(QObject* parent = nullptr);

    ~CHttpSupport() override;

private:
    QNetworkAccessManager* m_NetAccessManager;
    QMap<QString, bool> m_ProcessingRq;                // 当前正在处理的请求url 和是否正在处理
    QMap<QString, QByteArray> m_DownloadDataCache;     // 数据缓存url -> data | postMD5 -> data
    QMap<QString, QString> m_RedirectMap;              // 重定向关系

signals:
    /// \brief httpGetRspReady http get 请求得到回复
    /// \param url             请求的地址
    /// \param data            回复的数据（如果isEmpty则表示请求出错了）
    void httpGetRspReady(QString url, QByteArray data);

    /// \brief httpPostRspReady http post 请求得到回复
    /// \param url              请求的地址
    /// \param postMD5          是post的时候url+数据的md5
    /// \param data             data是回复的数据（如果isEmpty则表示请求出错了）
    void httpPostRspReady(QString url, QString postMD5, QByteArray data);

private slots:
    /**
    * @brief:清除post成功之后返回的信息
    * @creator:turbo
    * @param:*rp,网络返回的内同
    * @return:
    */
    void clearRp(QNetworkReply* rp);

    /**
    * @brief:get方式请求成功，监听到返回值
    * @creator:turbo
    * @param:bytesReceived
    *        bytesTotal
    * @return:
    */
    void onHttpGetRspProgress(qint64 bytesReceived, qint64 bytesTotal); //* http get 回复进度

    /**
    * @brief:get方式请求结束
    * @creator:turbo
    * @param:
    * @return:
    */
    void onHttpGetRspFinished(); //* http get 处理完毕

    /**
    * @brief:POST方式请求成功，监听到返回值
    * @creator:turbo
    * @param:
    * @return:
    */
    void onHttpPostRspProgress(qint64 bytesReceived, qint64 bytesTotal); //* http post 回复进度

    /**
    * @brief:POST方式请求结束
    * @creator:turbo
    * @param:
    * @return:
    */
    void onHttpPostRspFinished(); //* http get 处理完毕

public:
    static CHttpSupport& instance();

    /**
    * @brief:根据url get方式调用接口
    * @creator:turbo
    * @param:url
    * @return:
    */
    void httpGet(const QString& url);

    /**
    * @brief:根据url post方式调用接口
    * @creator:turbo
    * @param:url
    *        data：post参数
    * @return:
    */
    QByteArray httpPost(const QString& url, const QByteArray& data);

};

#endif // C_HTTP_SUPPORT_H