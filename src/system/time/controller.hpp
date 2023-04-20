#pragma once

#include "datetime.h"
#include "itime_provider.h"
#include "utils/list.h"
#include "utils/logger.h"

class SystemTimeProvider : public ITimeProvider {
  struct ProviderUpdateEvents {
    ITimeProvider *provider;
    uint32_t timeUntilUpdate;
  };

  LinkedList<ProviderUpdateEvents> providers;

 public:
  SystemTimeProvider() : providers() {}

  template <typename Tprovider>
  bool TryToRegisterTimeProvider() {
    Tprovider *p = new Tprovider();
    providers.add({p, 1});
    return true;
  }

  int countTimeProviders() { return providers.size(); }

  virtual const char *getTypeName() const { return "None"; }

  const LinkedList<const char *> getNames() {
    LinkedList<const char *> names;
    _for_each(providers, _tp, ProviderUpdateEvents) {
      names.add(_tp.provider->getTypeName());
    }
    return names;
  }

  bool init() {
    bool success = true;
    logger << LOG_INFO << "Initializing Time Providers" << EndLine;
    _for_each(providers, _tp, ProviderUpdateEvents) {
      bool success = _tp.provider->init();

      if (success) {
        _tp.timeUntilUpdate = _tp.provider->getSecondsThreshold();
        logger << LOG_INFO << "  - Init " << _tp.provider->getTypeName()
               << LOGGER_TEXT_GREEN << " Success" << EndLine
               << "    Updating each " << _tp.timeUntilUpdate << EndLine;
      } else {
        success = false;
        logger << LOG_ERROR << "  - Init " << _tp.provider->getTypeName()
               << LOGGER_TEXT_RED << " Failure!" << EndLine;
        providers.remove(i);
        delete _tp.provider;
        i--;
      }
    }
    return success;
  }

  bool update() {
    DateTime now = getLatestDateTimeFromAllSources();
    if (now.unixtime() == SECONDS_FROM_1970_TO_2000) {
      return false;
    }
    datetime = now;
    updateBackupSourcesWithDateTime(now);
    return true;
  }

 private:
  DateTime getLatestDateTimeFromAllSources() {
    DateTime now = DateTime(SECONDS_FROM_1970_TO_2000);
    _for_each(providers, _tp, ProviderUpdateEvents) {
      logger << LOG_DEBUG << "Updating " << _tp.provider->getTypeName() << EndLine;
      _tp.timeUntilUpdate--;
      if (_tp.timeUntilUpdate == 0) {
        bool status = _tp.provider->update();
        _tp.timeUntilUpdate = _tp.provider->getSecondsThreshold();
        if (status) {
          logger << LOG_DEBUG << LOGGER_TEXT_GREEN << "Success!" << EndLine;
          DateTime d = _tp.provider->get().toDateTime();
          if (now <= d) {
            now = d;
          }
          if (_tp.provider->getType() == PRIMARY) {
            return now;
          }
        } else {
          logger << LOG_ERROR << "Error while updating time provider!" << EndLine;
        }
      } else {
        logger << LOG_DEBUG << "  In " << _tp.timeUntilUpdate << "s" << EndLine;
      }
    }
    return now;
  }

  void updateBackupSourcesWithDateTime(DateTime &dateTime) {
    logger << LOG_DEBUG << "Set backups with time "
           << Time_s(dateTime).toString() << EndLine;

    _for_each(providers, _tp, ProviderUpdateEvents) {
      if (_tp.provider->getType() != BACKUP) {
        continue;
      }
      logger << LOG_DEBUG << LOGGER_TEXT_GREEN << " Backup provider "
             << _tp.provider->getTypeName() << EndLine;
      _tp.provider->set(dateTime);
    }
  }
};
