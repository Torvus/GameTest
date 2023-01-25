#include "item.h"

Item::Item()
    : mType(Unknown)
{
}

Item::Item(const Type &type)
    : mType(type)
{
}

bool Item::operator==(const Item &other) const
{
    return mType == other.mType;
}

bool Item::operator!=(const Item &other) const
{
    return mType != other.mType;
}

Item::Type Item::type() const
{
    return mType;
}

QString Item::name() const
{
    switch (mType) {
    case Apple: return QLatin1Literal("Apple");
    default: break;
    }
    return QString();
}

QString Item::imagePath() const
{
    switch (mType) {
    case Apple: return QLatin1Literal(":/icons/apple.jpg");
    default: break;
    }
    return QString();
}
