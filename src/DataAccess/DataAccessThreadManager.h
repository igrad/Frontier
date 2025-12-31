#pragma once

#include <QThread>

namespace Settings
{
   class SettingsService;
}

class DataAccessThreadManager: public QObject
{
   Q_OBJECT

public:
   DataAccessThreadManager();
   ~DataAccessThreadManager() = default;

   void AssignToThread(QThread* thread);

   void Start();

   Settings::SettingsService* GetTheSettingsService();

signals:
   void DataAccessThreadStarted();

public slots:
   void HandleUIConnectedToComponents();

private slots:
   void HandleDataAccessThreadStarted();

private:
   Settings::SettingsService* TheSettingsService;
};
