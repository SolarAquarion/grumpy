//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2015

#include "../libcore/exception.h"
#include "../libcore/ircsession.h"
#include "../libcore/networksession.h"
#include "../libcore/grumpydsession.h"
#include "userconfiguration.h"
#include "databasebackend.h"
#include "grumpyd.h"
#include "virtualscrollback.h"
#include "user.h"
#include "session.h"

using namespace GrumpyIRC;

void VirtualScrollback::SetLastID(scrollback_id_t id)
{
    lastID = id;
}

VirtualScrollback::VirtualScrollback(ScrollbackType Type, Scrollback *parent) : Scrollback(Type, parent)
{
    this->owner = NULL;
}

VirtualScrollback::~VirtualScrollback()
{

}

User *VirtualScrollback::GetOwner() const
{
    return this->owner;
}

QList<QVariant> VirtualScrollback::OriginFetchBacklog(scrollback_id_t from, unsigned int size)
{
    return Scrollback::FetchBacklog(from, size);
}

QList<QVariant> VirtualScrollback::FetchBacklog(scrollback_id_t from, unsigned int size)
{
    return Grumpyd::GetBackend()->FetchBacklog(this, from, size);
}

void VirtualScrollback::Sync()
{
    if (this->owner == NULL)
        throw new NullPointerException("this->owner", BOOST_CURRENT_FUNCTION);

    // Let's get all sessions who need to be informed about creation of this scrollback
    Session *session = this->owner->GetAnyGPSession();
    if (!session)
        return;
    QHash<QString, QVariant> parameters;
    parameters.insert("name", this->GetTarget());
    if (this->parentSx)
        parameters.insert("parent_sid", QVariant(this->parentSx->GetOriginalID()));
    if (this->GetSession() && this->GetSession()->GetType() == SessionType_IRC)
        parameters.insert("network_id", QVariant(((IRCSession*)this->GetSession())->GetSID()));
    parameters.insert("scrollback", QVariant(this->ToHash()));
    session->SendToEverySession(GP_CMD_SCROLLBACK_RESYNC, parameters);
}

void VirtualScrollback::PartialSync()
{
    if (this->owner == NULL)
        throw new NullPointerException("this->owner", BOOST_CURRENT_FUNCTION);

    // Let's get all sessions who need to be informed about resync of this scrollback
    Session *session = this->owner->GetAnyGPSession();
    if (!session)
        return;
    QHash<QString, QVariant> parameters;
    if (this->GetSession() && this->GetSession()->GetType() == SessionType_IRC)
        parameters.insert("network_id", QVariant(((IRCSession*)this->GetSession())->GetSID()));
    parameters.insert("scrollback", QVariant(this->ToPartialHash()));
    session->SendToEverySession(GP_CMD_SCROLLBACK_PARTIAL_RESYNC, parameters);
}

void VirtualScrollback::SetLastItemID(scrollback_id_t id)
{
    this->_lastItemID = id;
}

void VirtualScrollback::SetOwner(User *user, bool restored)
{
    this->owner = user;
    this->owner->RegisterScrollback(this, restored);
    this->_maxItems = this->owner->GetConfiguration()->GetValueAsUInt("maximum_bsize", 2000);
}

void VirtualScrollback::ImportText(ScrollbackItem item)
{
    this->_items.append(item);
}

void VirtualScrollback::InsertText(ScrollbackItem item)
{
    //Scrollback::InsertText(item);

    //////////////////////////////////////////////////////////////////
    // Override of original function
    //
    // We need to do this nasty thing so that we get the item ID which
    // can't normally be retrieved because we are passing the copy of
    // item instead of pointer
    //
    //! \todo Fix this crap
    //////////////////////////////////////////////////////////////////
    item.SetID(this->_lastItemID++);
    this->insertSI(item);
    if (!this->IgnoreState)
    {
        switch (item.GetType())
        {
            case ScrollbackItemType_Act:
            case ScrollbackItemType_Notice:
            case ScrollbackItemType_Message:
                this->SetState(ScrollbackState_UnreadMessages);
                break;
            case ScrollbackItemType_Kick:
            case ScrollbackItemType_Nick:
            case ScrollbackItemType_Join:
            case ScrollbackItemType_Part:
            case ScrollbackItemType_Quit:
            case ScrollbackItemType_SystemWarning:
            case ScrollbackItemType_SystemError:
            case ScrollbackItemType_Mode:
            case ScrollbackItemType_System:
            case ScrollbackItemType_Topic:
                this->SetState(ScrollbackState_UnreadSystem);
                break;
            case ScrollbackItemType_Unknown:
                break;
        }
    }
    emit Event_InsertText(item);
    //////////////////////////////////////////////////////////////////

    // let's check if this window belongs to a user which has clients
    if (!this->owner)
        throw new Exception("VirtualScrollback NULL owner", BOOST_CURRENT_FUNCTION);
    Grumpyd::GetBackend()->StoreItem(this->owner, this, &item);
    Session * xx = this->owner->GetAnyGPSession();
    if (!xx)
        return;
    // Deliver information about this message to everyone
    QHash<QString, QVariant> parameters;
    parameters.insert("scrollback", QVariant(this->GetOriginalID()));
    parameters.insert("scrollback_name", QVariant(this->GetTarget()));
    if (this->GetSession() && this->GetSession()->GetType() == SessionType_IRC)
        parameters.insert("network_id", QVariant(((IRCSession*)this->GetSession())->GetSID()));
    parameters.insert("item", QVariant(item.ToHash()));
    xx->SendToEverySession(GP_CMD_SCROLLBACK_LOAD_NEW_ITEM, parameters);
}

void VirtualScrollback::Close()
{
    if (!this->owner)
        throw new Exception("VirtualScrollback NULL owner", BOOST_CURRENT_FUNCTION);
    Grumpyd::GetBackend()->RemoveScrollback(this->owner, this);
    Session *gs = this->owner->GetAnyGPSession();
    if (!gs)
        return;
    // Deliver information about this message to everyone
    QHash<QString, QVariant> parameters;
    parameters.insert("scrollback", QVariant(this->GetOriginalID()));
    parameters.insert("scrollback_name", QVariant(this->GetTarget()));
    if (this->GetSession() && this->GetSession()->GetType() == SessionType_IRC)
        parameters.insert("network_id", QVariant(((IRCSession*)this->GetSession())->GetSID()));
    gs->SendToEverySession(GP_CMD_REMOVE, parameters);
}

