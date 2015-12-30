//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2015

#ifndef SESSION_H
#define SESSION_H

#include "../libcore/definitions.h"
#include <QObject>
#include <QHash>
#include <QThread>
#include <QList>
#include <QTcpSocket>
#include <QString>
#include <QMutex>
#include "../libgp/gp.h"

namespace GrumpyIRC
{
    class Scrollback;
    class User;

    class Session : public QThread
    {
            enum State
            {
                State_Login,
                State_Open,
                State_Killing,
                State_Exiting,
                State_Offline
            };

            Q_OBJECT
        public:
            static QList<Session*> Sessions();

            Session(qintptr socket_ptr, bool ssl);
            ~Session();
            void run();
            unsigned long GetSID();
            bool IsAuthorized(QString permission);
            //! Transfer data to every session this user has, so that every session connected as this user receives it
            void SendToEverySession(gp_command_t command, QHash<QString, QVariant> parameters);
            void SendToOtherSessions(gp_command_t command, QHash<QString, QVariant> parameters);
            Scrollback *GetScrollback(scrollback_id_t scrollback_id);
            void TransferError(gp_command_t source, QString description, int id);
            void PermissionDeny(gp_command_t source);
            bool IsRunning;
            int MaxScrollbackSyncItems;
            State SessionState;

        private slots:
            void OnDisconnected();
            void OnCommand(gp_command_t text, QHash<QString, QVariant> parameters);

        signals:
            void OnError(int error, QString text);

        private:
            static unsigned long lSID;
            static QList<Session*> SessionList;
            static QMutex *sessions_lock;

            void processLogin(QHash<QString, QVariant> parameters);
            void processNetworks();
            void processIrcQuit(QHash<QString, QVariant> parameters);
            void processRequest(QHash<QString, QVariant> parameters);
            void processSetup(QHash<QString, QVariant> parameters);
            void processResync(QHash<QString, QVariant> parameters);
            void processReconnect(QHash<QString, QVariant> parameters);
            void processOptions(QHash<QString, QVariant> parameters);
            void processRemove(QHash<QString, QVariant> parameters);
            void processPBResync(QHash<QString, QVariant> parameters);
            void processMessage(QHash<QString, QVariant> parameters);
            void processCommand(QHash<QString, QVariant> parameters);
            //! Called when user wants to connect to new IRC server
            void processNew(QHash<QString, QVariant> info);
            void processInfo(QHash<QString, QVariant> parameters);

            QTcpSocket *socket;
            QString peer;
            bool usingSsl;
            User *loggedUser;
            libgp::GP *protocol;
            unsigned long SID;
    };
}

#endif // SESSION_H
