#ifndef C_ASSISTANT_EVENT_MANAGER_H
#define C_ASSISTANT_EVENT_MANAGER_H
#include <QString>
#include <QMap>
#include <QMutexLocker>
#include <QQueue>
#include <QDebug>
#include <thread>
#include <QThread>
#include "Event.h"
#include "QCoreApplication"
#include "LogHelper.h"

class EventHandler
{
public:
	EventHandler() = default;

    virtual ~EventHandler() = default;

    virtual void PostEvent(Event* evt)
    {

    }
};

// 事件处理句柄
template<class T, class E>
class ClassEventHandler : public EventHandler
{
public:
    typedef void (T::*EventFuncConst)(const E&);

    typedef void (T::*EventFunc)(E);

public:
    ClassEventHandler(T* ptr, EventFunc funcPtr) :
            m_ptr(ptr),
            m_funcPtr(funcPtr)
    {

    }

    ClassEventHandler(T* ptr, EventFuncConst funcPtr) :
            m_ptr(ptr),
            m_constFuncPtr(funcPtr)
    {

    }

    ~ClassEventHandler() override = default;
	// 执行对应的回调函数
    void PostEvent(Event* event) override
    {
        if (m_ptr == nullptr)
        {
            return;
        }

        if (m_funcPtr != nullptr)
        {
            auto p = static_cast<E*>(event);
            return (m_ptr->*m_funcPtr)(*p);
        }
        else if (m_constFuncPtr != nullptr)
        {
            auto p = static_cast<E*>(event);
            return (m_ptr->*m_constFuncPtr)(*p);
        }
    }

private:
    T* m_ptr{nullptr};
    EventFunc m_funcPtr{nullptr};
    EventFuncConst m_constFuncPtr{nullptr};
};

// 事件管理类，增加锁
class EventManager:public QObject
{
Q_OBJECT
public:
    typedef QMap<void*, EventHandler*> OnEventListMap;
    typedef QMap<QString, OnEventListMap> OnEventMap;

public:
    EventManager(const EventManager&) = delete;
	// 析构的时候停止线程的运行
	~EventManager() override = default;

    static EventManager& Instance()
    {
        static EventManager instance;
        return instance;
    }
	// 绑定事件
    template<class T, class C>
    void BindEvent(T* ptr, void (T::*HandlerFunc)(const C&))
    {
        if (ptr == nullptr)
        {
            return;
        }

        auto eventName = typeid(C).name();
        auto delegate = new ClassEventHandler<T, C>(ptr, HandlerFunc);
        if (functionListMap.contains(eventName))
        {
            functionListMap[eventName].insert(static_cast<void*>(ptr), delegate);
        }
        else
        {
            auto it = functionListMap.insert(eventName, OnEventListMap());
            it->insert(static_cast<void*>(ptr), delegate);
        }
    }
	// 绑定事件
    template<class T, class C>
    void BindEvent(T* ptr, void (T::*HandlerFunc)(C))
    {
        if (ptr == nullptr)
        {
            return;
        }

        auto eventName = typeid(C).name();
        auto delegate = new ClassEventHandler<T, C>(ptr, HandlerFunc);
        if (functionListMap.contains(eventName))
        {

            functionListMap[eventName].insert(static_cast<void*>(ptr), delegate);
        }
        else
        {
            auto it = functionListMap.insert(eventName, OnEventListMap());
            it->insert(static_cast<void*>(ptr), delegate);
        }
    }

	// 事件解除绑定
    template<class T, class C>
    void UnBindEvent(T* ptr, C event)
    {
        auto eventType = typeid(event).name();
        if (!functionListMap.contains(eventType))
        {
            return;
        }

        auto key = static_cast<void*>(ptr);
        auto& map = functionListMap[eventType];
        if (map.contains(key))
        {
            delete map[key];
            map.remove(key);
        }
    }
	// 事件解除绑定
    template<class T>
    void UnBindEvent(T* ptr)
    {
        auto key = static_cast<void*>(ptr);
        auto it = functionListMap.begin();
        while (it != functionListMap.end())
        {
            delete it->value(key);
            it->remove(key);
            ++it;
        }
    }

	// 执行关联的回调函数
    template<class T>
    void PostEvent(T event)
    {
        if (!functionListMap.contains(typeid(event).name()))
        {
            return;
        }

        auto& map = functionListMap[typeid(event).name()];

        auto eventPtr = new T();
        *eventPtr = event;  // 新增指针。接收处理函数的地址
		
		{
			QMutexLocker locker(&mutex);
			for (auto& value : map)
			{
				if (value == nullptr)
				{
					continue;
				}
				eventPtr->Flag();
				Event* e = eventPtr;
				m_eventQueue.append(QPair<EventHandler*, Event *>(value, e));
			}
		}
        LOGI(typeid(event).name())
        emit sig_postEvent();
    }

signals:
    void sig_postEvent();
public slots:
    void slot_runPostEvent()
    {
        while (!m_eventQueue.isEmpty())
        {
            QPair<EventHandler*, Event *> func;
            {
                QMutexLocker locker(&mutex);
                func = m_eventQueue.front();
                m_eventQueue.pop_front();
            }
            func.first->PostEvent(func.second);
            func.second->Release();
        }
    };
private:
    EventManager()
    {
        connect(this,&EventManager::sig_postEvent,this,&EventManager::slot_runPostEvent);
    }
	QMutex mutex;
	QQueue<QPair<EventHandler*, Event *>> m_eventQueue;
    OnEventMap functionListMap;
};

#define EVENT_BIND(ptr, func) EventManager::Instance().BindEvent((ptr),(func));
#endif //C_ASSISTANT_EVENT_MANAGER_H