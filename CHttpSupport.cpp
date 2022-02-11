#include "CHttpSupport.h"
#include <QStandardPaths>
#include <QNetworkReply>
#include <QImage>
#include <iostream>

CHttpSupport::CHttpSupport(QObject *parent)
	:QObject(parent)
	, m_NetAccessManager(new QNetworkAccessManager(this))
{

}

CHttpSupport::~CHttpSupport() = default;

void CHttpSupport::clearRp(QNetworkReply *rp)
{
	if (rp)
	{
		QString url = rp->request().url().toString();
		QString postMD5 = rp->property("postMD5").toString();

		if (postMD5.isEmpty())
		{
			//清理对应缓存
			m_DownloadDataCache.remove(url);

			//解除正在处理状态
			m_ProcessingRq.remove(url);
		}
		else
		{
			//清理对应缓存
			m_DownloadDataCache.remove(postMD5);


			//解除正在处理状态
			m_ProcessingRq.remove(postMD5);
		}

		m_RedirectMap.remove(url);
		m_RedirectMap.remove(postMD5);

		qDebug() << "delete cache, url:" << url << " postMOD5:" << postMD5;
		rp->deleteLater();
	}
}

void CHttpSupport::onHttpGetRspProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	if (sender() == nullptr)
	{
		return;
	}

	auto rp = qobject_cast<QNetworkReply*>(sender());
	if (rp == nullptr)
	{
		return;
	}

	qDebug() << "http get rsp progress:" << rp->url().toString() << bytesReceived << "/" << bytesTotal;
	if (bytesTotal <= 0)
	{
		return;
	}

	QString url = rp->url().toString();

	m_DownloadDataCache[url].append(rp->readAll());

}

void CHttpSupport::onHttpGetRspFinished()
{
	if (sender() == nullptr)
	{
		return;
	}

	auto rp = qobject_cast<QNetworkReply*>(sender());
	if (rp == nullptr)
	{
		return;
	}

	QByteArray rpData;
	QString url = rp->url().toString();


	int statusCode = rp->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	QString strUrl = rp->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();

	switch (statusCode)
	{
	case 200: // ok
	{
		rpData = m_DownloadDataCache[url];

		QString redirectUrl = m_RedirectMap[url];
		if (redirectUrl.isEmpty())
		{
			qDebug() << "http get rsp ok:" << url << "total size:" << rpData.count() << "bytes";
			emit httpGetRspReady(url, rpData);
		}
		else
		{
			qDebug() << "http get rsp ok:" << url << "total size:" << rpData.count() << "bytes" << "[redirect by]" << redirectUrl;
			emit httpGetRspReady(redirectUrl, rpData);
		}

	}
	break;
	case 301:
	case 302: // redirect
	{
		if (!strUrl.isEmpty())
		{
			QString tUrl = m_RedirectMap[url];
			if (tUrl.isEmpty())
				m_RedirectMap[strUrl] = url;
			else
				m_RedirectMap[strUrl] = tUrl;

			httpGet(strUrl);
		}
	}
	break;
	default: // error
	{
		qDebug() << url << "[get error:" << statusCode << "]";
		QString redirectUrl = m_RedirectMap[url];
		if (redirectUrl.isEmpty())
		{
			emit httpGetRspReady(url, QByteArray());
		}
		else
		{
			emit httpGetRspReady(redirectUrl, QByteArray());
		}
	}
	break;
	}

	clearRp(rp);
}

void CHttpSupport::onHttpPostRspProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	if (sender() == nullptr)
	{
		return;
	}

	auto rp = qobject_cast<QNetworkReply*>(sender());
	if (rp == nullptr)
	{
		return;
	}

	std::cout << "http post rsp progress:" << rp->url().toString().toStdString() << bytesReceived << "/" << bytesTotal;
	if (bytesTotal <= 0)
	{
		return;
	}

	QString postMD5 = rp->property("postMD5").toString();

	m_DownloadDataCache[postMD5].append(rp->readAll());
}

void CHttpSupport::onHttpPostRspFinished()
{
	if (sender() == nullptr)
	{
		return;
	}

	auto rp = qobject_cast<QNetworkReply*>(sender());
	if (rp == nullptr)
	{
		return;
	}

	QByteArray rpData;

	QString url = rp->url().toString();
	QString postMD5 = rp->property("postMD5").toString();
	QByteArray postData = rp->property("data").toByteArray();

	int statusCode = rp->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	QString strUrl = rp->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();

	switch (statusCode)
	{
	case 200:
	case 201:
	{
		rpData = m_DownloadDataCache[postMD5];

		QString redirectMD5 = m_RedirectMap[postMD5];
		QString redirectUrl = m_RedirectMap[url];
		if (redirectMD5.isEmpty() || redirectUrl.isEmpty())
		{
            std::cout << rpData.toStdString() << std::endl;
			emit httpPostRspReady(url, postMD5, rpData);
		}
		else
		{
            std::cout << rpData.toStdString() << std::endl;
            emit httpPostRspReady(redirectUrl, redirectMD5, rpData);
		}

	}
	break;
	case 301:
	case 302:
	{
		if (!strUrl.isEmpty())
		{
			QString tUrl = m_RedirectMap[url];//direct by
			if (tUrl.isEmpty())
				m_RedirectMap[strUrl] = url;
			else
				m_RedirectMap[strUrl] = tUrl;


			QByteArray d;
			d.append(strUrl);
			d.append(postData);
			QString md5 = QCryptographicHash::hash(d, QCryptographicHash::Md5);
			QString tPostMD5 = m_RedirectMap[md5];//direct by
			if (tPostMD5.isEmpty())
			{
				m_RedirectMap[md5] = postMD5;
			}
			else
			{
				m_RedirectMap[md5] = tPostMD5;
			}


			httpPost(strUrl, postData);
		}
	}
	break;
	default:
		qDebug() << url << "[post error:" << statusCode << "]";
		QString redirectMD5 = m_RedirectMap[postMD5];
		QString redirectUrl = m_RedirectMap[url];
		if (redirectMD5.isEmpty() || redirectUrl.isEmpty())
		{
			emit httpPostRspReady(url, postMD5, m_DownloadDataCache[postMD5]);
		}
		else
		{
			emit httpPostRspReady(redirectUrl, redirectMD5, m_DownloadDataCache[postMD5]);
		}
		break;
	}

	clearRp(rp);
}

CHttpSupport &CHttpSupport::instance()
{
	static CHttpSupport g_instance;
	return g_instance;
}

void CHttpSupport::httpGet(const QString &url)
{
	if (m_ProcessingRq.value(url, false)) //ignore when rq processing
	{
		return;
	}

	for (auto &vv:m_RedirectMap)
	{
		if (vv == url) //ignore when redirect processing
		{
			return;
		}
	}

	m_ProcessingRq.insert(url, true);

	QNetworkRequest rq;
	QSslConfiguration config = rq.sslConfiguration();
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1SslV3);
	rq.setSslConfiguration(config);

	rq.setUrl(QUrl(url));
	QNetworkReply* rp = m_NetAccessManager->get(rq);
	connect(rp, &QNetworkReply::finished, this, &CHttpSupport::onHttpGetRspFinished);
	connect(rp, &QNetworkReply::downloadProgress, this, &CHttpSupport::onHttpGetRspProgress);
}

QByteArray CHttpSupport::httpPost(const QString &url, const QByteArray &data)
{
	QByteArray d;
	d.append(url);
	d.append(data);
	QByteArray md5 = QCryptographicHash::hash(d, QCryptographicHash::Md5);

	if (m_ProcessingRq.value(md5, false)) //ignore when rq processing
	{
		return md5;
	}
	for (auto &vv:m_RedirectMap)
	{
		if (vv == md5) //ignore when redirect processing
		{
			return md5;
		}
	}
	m_ProcessingRq.insert(md5, true);

	QNetworkRequest rq;
	QSslConfiguration config = rq.sslConfiguration();
	rq.setRawHeader("Content-Type","application/json");
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1SslV3);
	rq.setSslConfiguration(config);

	rq.setUrl(QUrl(url));
	QNetworkReply* rp = m_NetAccessManager->post(rq, data);
	rp->setProperty("postMD5", md5);
	rp->setProperty("url", url);
	rp->setProperty("data", data);

	connect(rp, &QNetworkReply::finished, this, &CHttpSupport::onHttpPostRspFinished);
	connect(rp, &QNetworkReply::downloadProgress, this, &CHttpSupport::onHttpPostRspProgress);

	return md5;
}
