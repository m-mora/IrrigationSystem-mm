#pragma once

#include "datetime.h"
#include "itime_provider.h"
#include "utils/list.h"
#include "utils/logger.h"

class SystemTimeProvider : public ITimeProvider {
  LinkedList<ITimeProvider *> providers;

 public:
  SystemTimeProvider() : providers() {}

  template <typename Tprovider>
  bool TryToRegisterTimeProvider() {
    Tprovider *p = new Tprovider();
    providers.add(p);
    return true;
  }

  int countTimeProviders() { return providers.size(); }

  virtual const char *getTypeName() const { return "None"; }

  const LinkedList<const char *> getNames() {
    LinkedList<const char *> names;
    _for_each(providers, _tp, ITimeProvider *) {
      names.add(_tp->getTypeName());
    }
    return names;
  }

  bool init() {
    bool success = true;
    uint8_t index = 0;
    logger << LOG_INFO << "Initializing Time Providers" << EndLine;
    _for_each(providers, _tp, ITimeProvider *) {
      bool success = _tp->init();

      if (success) {
        logger << LOG_INFO << "  - Init " << _tp->getTypeName()
               << LOGGER_TEXT_GREEN << " Success" << EndLine;
        index++;
      } else {
        success = false;
        logger << LOG_ERROR << "  - Init " << _tp->getTypeName()
               << LOGGER_TEXT_RED << " Failure!" << EndLine;
        providers.remove(index);
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
    _for_each(providers, _tp, ITimeProvider *) {
      logger << LOG_DEBUG << "Updating " << _tp->getTypeName() << EndLine;
      bool status = _tp->update();
      if (status) {
        logger << LOG_DEBUG << LOGGER_TEXT_GREEN << "Success!" << EndLine;
        DateTime d = _tp->get().toDateTime();
        if (now <= d) {
          now = d;
        }
        if (_tp->getType() == PRIMARY) {
          return now;
        }
      } else {
        logger << LOG_ERROR << "Error while updating time provider!" << EndLine;
      }
    }
    return now;
  }

  void updateBackupSourcesWithDateTime(DateTime& dateTime) {
    logger << LOG_DEBUG << "Set backups with time " << Time_s(dateTime).toString() << EndLine;

    _for_each(providers, _tp, ITimeProvider *) {
      if (_tp->getType() != BACKUP) {
        continue;
      }
      logger << LOG_DEBUG << LOGGER_TEXT_GREEN << " Backup provider " << _tp->getTypeName() << EndLine;
      _tp->set(dateTime);
    }
  }
};
