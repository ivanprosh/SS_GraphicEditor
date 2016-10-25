#ifndef GLOBAL_HPP
#define GLOBAL_HPP
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

bool isDirtyChange(QGraphicsItem::GraphicsItemChange change);
bool isPowerOf2(int value);
int flagToInt(const QMetaEnum &metaEnum, int flagValue);
int intToFlag(const QMetaEnum &metaEnum, int intValue);
int enumToInt(const QMetaEnum &metaEnum, int enumValue);
bool isSubValue(int value, int subValue);
int intToEnum(const QMetaEnum &metaEnum, int intValue);

//enum Column {Zipcode, PostOffice, County, State};

#endif // GLOBAL_HPP
