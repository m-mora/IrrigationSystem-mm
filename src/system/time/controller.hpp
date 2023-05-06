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

  struct LatestDateTimeWithProviderIndex {
    int providerIndex;
    DateTime latestNow;
  };

 public:
  SystemTimeProvider() : providers() {}

  template <typename Tprovider>
  bool TryToRegisterTimeProvider() {
    Tprovider *p = new Tprovider();
    providers.add({p, 0});
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
    for (int i = 0; i < providers.size(); i++) {
      ProviderUpdateEvents& _tp = providers[i];

      if (!_tp.provider->init()) {
        success = false;
        logger << LOG_ERROR << "  - Init " << _tp.provider->getTypeName()
               << LOGGER_TEXT_RED << " Failure!" << EndLine;
        delete _tp.provider;
        providers.remove(i);
        i--;
        continue;
      }

      _tp.timeUntilUpdate = _tp.provider->getSecondsThreshold();
      logger << LOG_INFO << "  - Init " << _tp.provider->getTypeName()
              << LOGGER_TEXT_GREEN << " Success" << EndLine
              << LOG_DEBUG << "   Updating each " << _tp.timeUntilUpdate << EndLine;
    }
    logger << LOG_INFO << "Initializing Time Providers - Finished" << EndLine;
    return success;
  }

  bool update() {
    LatestDateTimeWithProviderIndex result = getLatestDateTimeFromAllSources();
    if (result.latestNow.unixtime() == SECONDS_FROM_1970_TO_2000) {
      return false;
    }
    datetime = result.latestNow;
    updateBackupSourcesWithDateTime(result);
    return true;
  }

 private:
  LatestDateTimeWithProviderIndex getLatestDateTimeFromAllSources() {
    LatestDateTimeWithProviderIndex result;
    result.latestNow = DateTime(SECONDS_FROM_1970_TO_2000);

    for (int i = 0; i < providers.size(); i++) {
      ProviderUpdateEvents& _tp = providers[i];
      logger << LOG_DEBUG << "Updating " << _tp.provider->getTypeName() << EndLine;
      if (_tp.timeUntilUpdate > 0) {
        logger << LOG_DEBUG << "  In " << _tp.timeUntilUpdate << "s" << EndLine;
        _tp.timeUntilUpdate--;
        continue;
      }

      _tp.timeUntilUpdate = _tp.provider->getSecondsThreshold();
      if (!_tp.provider->update()) {
        logger << LOG_ERROR << "Error while updating time provider!" << EndLine;
        continue;
      }

      DateTime d = _tp.provider->get().toDateTime();
      if (result.latestNow <= d) {
        result.latestNow = d;
        result.providerIndex = i;
        logger << LOG_DEBUG << LOGGER_TEXT_GREEN << "Success!" << EndLine;

        if (_tp.provider->getType() == PRIMARY) {
          break;
        }
      } else {
        logger << LOG_DEBUG << LOGGER_TEXT_YELLOW << "Not most recent data!" << EndLine;
      }
    }
    return result;
  }

  void updateBackupSourcesWithDateTime(LatestDateTimeWithProviderIndex &latestDateTimeWithProvider) {
    logger << LOG_DEBUG << "Set backups with time "
           << Time_s(latestDateTimeWithProvider.latestNow).toString() << EndLine;

    for (int i = 0; i < providers.size(); i++) {
      ProviderUpdateEvents& _tp = providers[i];
      if (_tp.provider->getType() != BACKUP || i == latestDateTimeWithProvider.providerIndex) {
        continue;
      }
      logger << LOG_DEBUG << LOGGER_TEXT_GREEN << " Backup provider "
             << _tp.provider->getTypeName() << EndLine;
      _tp.provider->set(latestDateTimeWithProvider.latestNow);
    }
  }
};
