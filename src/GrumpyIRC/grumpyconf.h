//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2015

#ifndef GRUMPYCONF_H
#define GRUMPYCONF_H

#ifdef GCFG
    #undef GCFG
#endif

#define GCFG CoreWrapper::GrumpyCore->GetConfiguration()

#ifdef CONF
#error "CONF is already defined, redefinig is not supported, grumpy can't be compiled with libraries that define their own CONF option"
#endif
#define CONF GrumpyIRC::GrumpyConf::Conf

#include <QVariant>

namespace GrumpyIRC
{
    class Configuration;
    class GrumpyConf
    {
        public:
            static GrumpyConf *Conf;
            GrumpyConf();
            Configuration *GetConfiguration();
            QString GetQuitMessage();
            void SetQuitMessage(QString text);
            void SetNick(QString nick);
            QString GetIdent();
            void SetIdent(QString ident);
            QString GetAlterNick();
            void SetAlterNick(QString text);
            bool WriteNoticesToSystem();
            void SetName(QString text);
            QString GetName();
            QString GetNick();
            QString GetLineFormat();
            QString GetNoticeFormat();
            QString GetMessageFormat();
            QString GetActionFormat();
            QString GetDefaultKickReason();
            void SetDefaultKickReason(QString text);
            bool FirstRun();
    };
}

#endif // GRUMPYCONF_H
