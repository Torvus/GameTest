#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include <QMetaType>

class Item
{
public:
    enum Type
    {
        Apple = 0,
        Unknown
    };

    Item();
    explicit Item(const Type &type);
    Item(const Item &other) = default;

    Item &operator=(const Item &other) = default;

    bool operator==(const Item &other) const;
    bool operator!=(const Item &other) const;

    Type type() const;
    QString name() const;
    QString imagePath() const;

private:
    Type mType;
};

Q_DECLARE_METATYPE(Item);

#endif // ITEM_H
