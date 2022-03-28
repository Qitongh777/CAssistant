#ifndef DATA_BINDER_H
#define DATA_BINDER_H

#include <map>
#include <set>
#include <functional>
#include <QTimer>

#ifdef QT_DEBUG
#include <QCoreApplication>
#include <QThread>
#endif

#define _ }+[=]{}
#define $attr(type, name) \
public:\
type & $##name(){ \
QDataBinder::obsever(this, #name);\
return this->name;}\
void $##name(type newValue){       \
if (newValue == this->name){       \
this->name=newValue;               \
return \
} \
QDataBinder::fireChange(this,#name); \
this->name=newValue;               \
}\
protected:         \
type name


class QExecute;

class QExecutes {
public:
    static QExecutes &get() {
        static QExecutes ret;
        return ret;
    }

    QExecute *currentExecute = nullptr;
    std::map<void *, std::map<std::string, std::set<QExecute *>>> allBindings;
    std::set<QExecute *> nextRunExecute; // 执行表，待执行任务
};

class QExecute {
public:
    QExecute(std::function<void()> changeFun) {
        this->changeFun = changeFun;
    }

    virtual ~QExecute() {};

    virtual void reBind() {
        QExecutes::get().currentExecute = this;
        try {
            unObsever();
            this->changeFun();
        } catch (...) {
            // error
        }
        QExecutes::get().currentExecute = nullptr;
    }

    void unObsever() {
        this->valid = false;
    }

    void setValid(bool valid) {
        this->valid = valid ? 1 : 0;
    }

    bool isValid() {
        if (this->valid != 0 && this->valid != 1) {
            // error
        }
        return this->valid == 1;
    }

    static QExecute *currentExecute;
protected:
    std::function<void()> changeFun = nullptr;
    int valid = 0; // 执行器是否有效
};

template<typename T>
class QWatcher : public QExecute {
protected:
    T oldValue;
    std::function<void(const T &, const T &)> watchFun = nullptr;
    std::function<T(void)> watchName = nullptr;
public:
    QWatcher(std::function<T(void)> watchName) : QExecute(nullptr) {
        this->watchName = watchName;
        this->watchFun = watchFun;
        oldValue = this->watchName();
        this->reBind();
    }

    void operator=(std::function<void(const T &, const T &)> watchFun) {
        this->watchFun = watchFun;
    }

    void reBind() override {
        QExecutes::get().currentExecute = nullptr;
        try {
            T newVal = this->watchName();
            if (!(newVal == oldValue) && nullptr != watchFun) {
                watchFun(newVal, oldValue);
                oldValue = newVal;
            }
        } catch (...) {

        }
        QExecutes::get().currentExecute = nullptr;
    }
};

class QDataBinder {
public:
    QDataBinder()= default;;
    ~QDataBinder() {
        removeBinding(excutes_);
        for(auto exec:excutes_) {
            exec->setValid(false);
        }
        if (!excutes_.empty()) {
            std::set<QExecute*> laterRmvs(excutes_);
            excutes_.clear();
            QTimer::singleShot(0,nullptr,[=]{
                for(auto exec:laterRmvs) {
                    delete exec;
                }
            });
        }
    }

    static void fireChange(void* obj, std::string name) {
        auto& nextRunExecute = QExecutes::get().nextRunExecute;
        bool needStartUpdate = nextRunExecute.size() <= 0;
        addExecute(obj, name, nextRunExecute);
        if (needStartUpdate && nextRunExecute.size()>0) {
            QTimer::singleShot(0,nullptr,[=]{
                QDataBinder::onUpdate();
            });
        }
    }

    static void obsever(void* obj, std::string name) {
        if (nullptr == QExecutes::get().currentExecute) {
            return;
        }
        auto& allBindings = QExecutes::get().allBindings;
        if (allBindings.find(obj) == allBindings.end()) {
            allBindings[obj] = std::map<std::string, std::set<QExecute*>>();
        }

        auto& name2Exes = allBindings[obj];
        if (name2Exes.find(name) == name2Exes.end()) {
            name2Exes[name] = std::set<QExecute*>();
        }
        auto& exes = name2Exes[name];
        exes.insert(QExecutes::get().currentExecute);
        QExecutes::get().currentExecute->setValid(true);
    }

    QDataBinder& operator+(std::function<void()> changeFun){
        auto exe = new QExecute(changeFun);
        excutes_.insert(exe);
        exe->reBind();
        return *this;
    }

    template<typename T>
    QWatcher<T>& watch(std::function<T& (void)> watchName) {
        QWatcher<T>* exe = new QWatcher<T>(watchName);
        excutes_.insert(exe);
        return *this;
    }

protected:
    std::set<QExecute*> excutes_;

    static void addExecute(void* obj, std::string name, std::set<QExecute*>& out) {
        auto& allBindings = QExecutes::get().allBindings;
        if (allBindings.find(obj) == allBindings.end()) {
            return;
        }
        auto& name2exes = allBindings[obj];
        if (name2exes.find(name) == name2exes.end()) {
            return;
        }
        auto& exes = name2exes[name];
        for(auto it = exes.begin(); it != exes.end();) {
            if ((*it)->isValid()) {
                out.insert(*it);
                ++it;
            } else {
                it = exes.erase(it);
            }
        }

        if (exes.empty()) {
            name2exes.erase(name);
        }

        if (name2exes.empty()) {
            allBindings.erase(obj);
        }
    }

    static void onUpdate() {
        auto& nextRunExecute = QExecutes::get().nextRunExecute;
        std::set<QExecute*> excutes(nextRunExecute);
        nextRunExecute.clear();

        for(auto& exe:excutes) {
            if (exe->isValid()) {
                exe->reBind();
            }
        }
    }

    static void removeBinding(std::set<QExecute*>& rmvs) {
        auto& nextRunExecute = QExecutes::get().nextRunExecute;
        auto& allBindings = QExecutes::get().allBindings;
        for (auto it = allBindings.begin(); it != allBindings.end();) {
            auto& name2Excs = it->second;
            for (auto it2 = name2Excs.begin(); it2 != name2Excs.end();) {
                auto& exes = it2->second;
                for (auto it3 = exes.begin(); it3 != exes.end();) {
                    if (rmvs.find(*it3) != rmvs.end()) {
                        nextRunExecute.erase(*it3);
                        it3 = exes.erase(it3);
                    } else {
                        ++it3;
                    }
                }
                if (exes.empty()) {
                    it2 = name2Excs.erase(it2);
                } else {
                    ++it2;
                }
            }
            if (name2Excs.empty()) {
                it = allBindings.erase(it);
            } else {
                ++it;
            }
        }
    }
};

template<typename V>
class AutoValue {
protected:
    void* obj;
    std::string name;
    V& val;
public:
    AutoValue(void* obj, std::string name, V& value):obj(obj),name(name),val(value) {}
    void operator=(const V& newVal) {
        if (val != newVal) {
            QDataBinder::fireChange(obj, name);
        }
        val = newVal;
    }

    operator V(){
        QDataBinder::obsever(obj, name);
        return val;
    }

    V& operator()(){
        QDataBinder::obsever(obj, name);
        return val;
    }
};

template<typename T>
AutoValue<T> createAutoValue(void* p, std::string name, T& val)
{
    return AutoValue<T>(p,name,val);
}
#define $(obj,attr) createAutoValue(obj, #attr, (obj)->attr)
#endif // DATA_BINDER_H
