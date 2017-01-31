#ifndef ITEMTYPES_HPP
#define ITEMTYPES_HPP
/*
    Copyright (c) 2009-10 Qtrac Ltd. All rights reserved.

    This program or module is free software: you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version. It is provided
    for educational purposes and is distributed in the hope that it will
    be useful, but WITHOUT ANY WARRANTY; without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
    the GNU General Public License for more details.
*/

#include <QGraphicsItem>
class QtVariantProperty;

const int BoxItemType = QGraphicsItem::UserType + 1;
const int SmileyItemType = QGraphicsItem::UserType + 2;
const int TextItemType = QGraphicsItem::UserType + 3;

const int SSAnparItemType = QGraphicsItem::UserType + 4;
const int SSDynTextItemType = QGraphicsItem::UserType + 5;
const int SSIndItemType = QGraphicsItem::UserType + 6;
const int SSTransitionButtonType = QGraphicsItem::UserType + 7;
const int SSControlButtonType = QGraphicsItem::UserType + 8;

typedef QPair<QString, QString> SScommandProperty;
Q_DECLARE_METATYPE(SScommandProperty)

//const int SSCommandType = QVariant::UserType + 1;

#endif // ITEMTYPES_HPP
