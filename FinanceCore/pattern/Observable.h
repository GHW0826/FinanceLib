#pragma once
#include "Singleton.h"

class Observable
{
public:
    using SetType = std::set<Observer*>;
public:
	void NotifyObservers();
private:
    SetType _observers;
};

class Observer
{
public:
    virtual void Update() = 0;
};

class ObservableSettings : public Singleton<ObservableSettings>
{
public:
    bool GetUpdatesEnabled() const { return _updatesEnabled; }
    bool GetUpdatesDeferred() const { return _updatesDeferred; }
    inline void RegisterDeferredObservers(const Observable::SetType& observers) {
        if (GetUpdatesDeferred()) {
            _deferredObservers.insert(observers.begin(), observers.end());
        }
    }

private:
    bool _updatesEnabled = true;
    bool _updatesDeferred = false;
    Observable::SetType _deferredObservers;
};