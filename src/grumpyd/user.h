//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2015

#ifndef GDUSER_H
#define GDUSER_H

#include <QList>
#include <QString>
#include "../libirc/libirc/serveraddress.h"

namespace GrumpyIRC
{
    class Session;
    class Role;
    class IRCSession;

    class User
    {
        public:
            static QList<User*> UserInfo;
            /*!
             * \brief Login try to login user using the provided credentials, if login is successful a pointer is returned
             * \param user
             * \param pw
             * \return
             */
            static User *Login(QString user, QString pw);

            User(QString Name, QString Password);
            void InsertSession(Session *sx);
            void RemoveSession(Session *sx);
            IRCSession *ConnectToIRCServer(libirc::ServerAddress info);
            bool IsAuthorized(QString perm);
            QList<Session*> GetGPSessions();
            QList<IRCSession*> GetSessions();
            void SetRole(Role *rx);
            QString DefaultNick;

        private:
            Role *role;
            QList<Session*> sessions_gp;
            QList<IRCSession*> sessions;
            QString username;
            QString password;
    };
}

#endif // USER_H