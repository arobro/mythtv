// Qt
#include <QMetaProperty>

// MythTV
#include "mythdate.h"
#include "http/serialisers/mythcborserialiser.h"

#if QT_VERSION >= QT_VERSION_CHECK(5,12,0)
#include <QCborStreamWriter>
MythCBORSerialiser::MythCBORSerialiser(const QString& Name, const QVariant& Value)
{
    m_writer = new QCborStreamWriter(&m_buffer);
    AddObject(Name, Value);
}

void MythCBORSerialiser::AddObject(const QString& Name, const QVariant& Value)
{
    m_writer->startMap();
    auto name = Name.toUtf8();
    m_writer->appendTextString(name.constData(), name.size());
    AddValue(Value);
    m_writer->endMap();
}

/*! \brief MythCBORSerialiser::AddValue
 *
 * \note Cbor is a superset of JSON and hence supports additional types. We do
 * not attempt to use those here however to maintain consistency with the other
 * serialisers - and hence lossless conversion between formats using different
 * (de)serialisers.
 */
void MythCBORSerialiser::AddValue(const QVariant& Value)
{
    if (Value.isNull() || !Value.isValid())
    {
        m_writer->append(QCborSimpleType::Null);
        return;
    }

    if (Value.value<QObject*>())
    {
        AddQObject(Value.value<QObject*>());
        return;
    }

    bool ok = false;
    switch (Value.type())
    {
        case QVariant::Int:
        case QVariant::LongLong:
            if (auto value = Value.toLongLong(&ok); ok)
                m_writer->append(value);
            break;
        case QVariant::UInt:
        case QVariant::ULongLong:
            if (auto value = Value.toULongLong(&ok); ok)
                m_writer->append(value);
            break;
        case QVariant::Double:     m_writer->append(Value.toDouble()); break;
        case QVariant::Bool:       m_writer->append(Value.toBool()); break;
        case QVariant::StringList: AddStringList(Value); break;
        case QVariant::List:       AddList(Value); break;
        case QVariant::Map:        AddMap(Value.toMap()); break;
        // TODO This
        case QVariant::DateTime:
            {
                QDateTime dt(Value.toDateTime());
                if (dt.isNull())
                {
                    m_writer->append(QCborSimpleType::Null);
                }
                else
                {
                    auto name = MythDate::toString(dt, MythDate::ISODate).toUtf8();
                    m_writer->appendTextString(name.constData(), name.size());
                }
            }
            break;
        default:
            auto name = Value.toString().toUtf8();
            m_writer->appendTextString(name.constData(), name.size());
    }
}

void MythCBORSerialiser::AddQObject(const QObject* Object)
{
    if (!Object)
        return;
    const auto * metaobject = Object->metaObject();
    int count = metaobject->propertyCount();

    m_writer->startMap();
    for (int index = 0; index  < count; ++index  )
    {
        QMetaProperty metaProperty = metaobject->property(index);
        if (metaProperty.isUser(Object))
        {
            const char *rawname = metaProperty.name();
            QString name(rawname);
            if (name.compare("objectName") == 0)
                continue;
            auto utf8 = name.toUtf8();
            m_writer->appendTextString(utf8.constData(), utf8.size());
            AddValue(Object->property(rawname));
        }
    }
    m_writer->endMap();
}

void MythCBORSerialiser::AddStringList(const QVariant &Values)
{
    m_writer->startArray();
    QSequentialIterable values = Values.value<QSequentialIterable>();
    for (const auto & value : values)
    {
        auto utf8 = value.toString().toUtf8();
        m_writer->appendTextString(utf8.constData(), utf8.size());
    }
    m_writer->endArray();
}

void MythCBORSerialiser::AddList(const QVariant& Values)
{
    m_writer->startArray();
    QSequentialIterable values = Values.value<QSequentialIterable>();
    for (const auto & value : values)
        AddValue(value);
    m_writer->endArray();
}

void MythCBORSerialiser::AddMap(const QVariantMap& Map)
{
    m_writer->startMap();
    for (auto it = Map.cbegin(); it != Map.cend(); ++it)
    {
        auto utf8 = it.key().toUtf8();
        m_writer->appendTextString(utf8.constData(), utf8.size());
        AddValue(it.value());
    }
    m_writer->endMap();
}

#endif
