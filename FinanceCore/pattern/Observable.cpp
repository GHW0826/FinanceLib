#include "pch.h"
#include "Observable.h"

void Observable::NotifyObservers()
{
    if (!ObservableSettings::Instance().GetUpdatesEnabled()) {
        // if updates are only deferred, flag this for later notification
        // these are held centrally by the settings singleton
        ObservableSettings::Instance().RegisterDeferredObservers(_observers);
    }
    else if (!_observers.empty()) {
        bool successful = true;
        std::string errMsg;
        for (auto* observer : _observers) {
            try {
                observer->Update();
            }
            catch (std::exception& e) {
                // quite a dilemma. If we don't catch the exception,
                // other observers will not receive the notification
                // and might be left in an incorrect state. If we do
                // catch it and continue the loop (as we do here) we
                // lose the exception. The least evil might be to try
                // and notify all observers, while raising an
                // exception if something bad happened.
                successful = false;
                errMsg = e.what();
            }
            catch (...) {
                successful = false;
            }
        }
        SF_ASSERT(successful, "could not notify one or more observers: " << errMsg);
    }
}
