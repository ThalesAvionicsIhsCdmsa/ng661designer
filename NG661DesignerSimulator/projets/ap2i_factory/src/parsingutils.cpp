/*******************************************************************************
* Copyright (c) 2015, 2016 Thales.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
* Contributors:
*    Thales Avionics - initial design and implementation
*******************************************************************************/

#include "parsingutils.h"

#include "basicobject.h"
#include "component.h"
#include <QDomElement>
#include <QDebug>

namespace AP2I
{

void ParsingUtils::parseItemProperties(Component &pOwner,
                                       BasicObject &pItem,
                                       QDomElement &pElement)
{
    QList<QPair<QString, QString> > lBindings;
    QDomNamedNodeMap lAttributes = pElement.attributes();
    for (int i = 0; i < lAttributes.count(); i++)
    {
        BasicObject::SetPropertyResult lRes =
                pItem.setPropertyValue(lAttributes.item(i).nodeName(),
                                           lAttributes.item(i).nodeValue().section(":", -1));
        switch (lRes)
        {
        case BasicObject::UnknownProperty:
            qDebug() << "Unknown property " << lAttributes.item(i).nodeName()
                     << " for object " + pItem.id() + "(" + pItem.className() + ")";
            break;
        case BasicObject::UnknownType:
            qDebug() << "Unknown property type for " << lAttributes.item(i).nodeName()
                     << " of object " + pItem.id() + "(" + pItem.className() + ")";
            break;
        case BasicObject::ValueIsABinding:
            //don't add binding here since it needs to have id property set
            lBindings.append(QPair<QString, QString>(
                                 lAttributes.item(i).nodeName(),
                                 lAttributes.item(i).nodeValue())
                             );
            break;
        case BasicObject::CorrectlySet:
        default:
            break;
        }
    }
    for (QList<QPair<QString, QString> >::iterator it = lBindings.begin();
         it != lBindings.end();
         it++)
    {
        pOwner.addBinding("Representation_" + pItem.id(), (*it).first, (*it).second);

    }
}

} /* namespace AP2I */
