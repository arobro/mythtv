#ifndef _WEATHERUTILS_H_
#define _WEATHERUTILS_H_

// QT headers
#include <QMap>
#include <QMultiHash>
#include <QString>
#include <QDomElement>
#include <QFile>
#include <QMetaType>

// MythTV headers
#include <mythcontext.h>

#define SI_UNITS 0
#define ENG_UNITS 1
#define DEFAULT_UPDATE_TIMEOUT (5*60*1000)
#define DEFAULT_SCRIPT_TIMEOUT (60)

class ScriptInfo;

using units_t = unsigned char;
using DataMap = QMap<QString, QString>;

class TypeListInfo
{
  public:

    TypeListInfo(const TypeListInfo& info) = default;
    explicit TypeListInfo(const QString &_name)
        : m_name(_name) {}
    TypeListInfo(const QString &_name, const QString &_location)
        : m_name(_name), m_location(_location) {}
    TypeListInfo(const QString &_name, const QString &_location,
                 ScriptInfo *_src)
        : m_name(_name), m_location(_location), m_src(_src) {}

  public:
    QString     m_name;
    QString     m_location;
    ScriptInfo *m_src      {nullptr};
};
using TypeListMap = QMultiHash<QString, TypeListInfo>;

class ScreenListInfo
{
  public:
    ScreenListInfo() = default;
    ScreenListInfo(const ScreenListInfo& info) = default;

    TypeListInfo GetCurrentTypeList(void) const;

  public:
    QString     m_name;
    QString     m_title;
    TypeListMap m_types;
    QStringList m_dataTypes;
    QString     m_helptxt;
    QStringList m_sources;
    units_t     m_units    {SI_UNITS};
    bool        m_hasUnits {false};
    bool        m_multiLoc {false};
    bool        m_updating {false};
};

Q_DECLARE_METATYPE(ScreenListInfo *);

using ScreenListMap = QMap<QString, ScreenListInfo>;

ScreenListMap loadScreens();
QStringList loadScreen(const QDomElement& ScreenListInfo);
bool doLoadScreens(const QString &filename, ScreenListMap &screens);

#endif
