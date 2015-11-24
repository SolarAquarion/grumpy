//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2015

#ifndef SCROLLBACKFRAME_H
#define SCROLLBACKFRAME_H

#include <QFrame>
#include <QStandardItemModel>
#include <QThread>
#include <QMutex>
#include <QTimer>
#include "stextbox.h"
#include "../libirc2htmlcode/parser.h"
#include "../libcore/scrollback.h"

#define GRUMPY_H_UNKNOWN 0
#define GRUMPY_H_YES     1
#define GRUMPY_H_NOT     2

#define GRUMPY_SCROLLER_TIME_WAIT 200

namespace Ui
{
    class ScrollbackFrame;
}

namespace libircclient
{
    class Network;
    class User;
}

namespace GrumpyIRC
{
    class InputBox;
    class Scrollback;
    class ScrollbackList_Node;
    class ScrollbackList_Window;
    class UserFrame;
    class ScrollbackFrame;
    class NetworkSession;

    /*!
     * \brief The ScrollbackFrame_WorkerThread class processes CPU intensive items on background in order to speed up grumpy
     */
    class ScrollbackFrame_WorkerThread : public QThread
    {
            Q_OBJECT
        public:
            ScrollbackFrame_WorkerThread();
        protected:
            void run();
    };

    class ScrollbackFrame : public QFrame
    {
            Q_OBJECT

        public:
            static void InitializeThread();
            static irc2htmlcode::Parser parser;

            explicit ScrollbackFrame(ScrollbackFrame *parentWindow = NULL, QWidget *parent = NULL, Scrollback *_scrollback = NULL, bool is_system = false);
            ~ScrollbackFrame();
            QString GetWindowName() const;
            void InsertText(QString text, ScrollbackItemType item = ScrollbackItemType_System);
            void InsertText(ScrollbackItem item);
            void SetWindowName(QString title);
            bool IsConnectedToIRC();
            bool IsChannel();
            bool IsNetwork();
            bool IsGrumpy();
            bool IsVisible();
            bool IsDead();
			ScrollbackFrame *GetParent();
            unsigned long GetID();
            NetworkSession *GetSession();
            Scrollback *GetScrollback();
            UserFrame *GetUserFrame();
            QString GetTitle();
            void UpdateColor();
            void Focus();
            void RequestClose();
            void UpdateIcon();
            void EnableState(bool enable);
            void RequestPart();
            void ToggleSecure();
            void RequestJoin();
            void Reconnect();
            void RequestDisconnect();
            void RequestMore(unsigned int count);
            void RefreshHtml();
            void RefreshHtmlIfNeeded();
            libircclient::Network *GetNetwork();
            void TransferRaw(QString data);
            libircclient::User *GetIdentity();
            scrollback_id_t GetItems();
            QList<QString> GetUsers();
            QList<QString> GetChannels();
            QString GetLocalUserMode();
            int GetSynced();
            //void SetParent(ScrollbackFrame* parentWindow);
            bool IsDeletable;
            bool Highlighting;
            void SetVisible(bool is_visible);
            ScrollbackList_Node *TreeNode;
            bool IsSystem;
        protected:
            static ScrollbackFrame_WorkerThread *WorkerThread;
        private slots:
            void _insertText_(ScrollbackItem item);
            void UserList_Insert(libircclient::User *ux, bool bulk);
            void UserList_Refresh(libircclient::User *ux);
            void OnState();
            void UserList_Remove(QString user, bool bulk);
            void UserList_Rename(QString old, libircclient::User *us);
            void OnFinishSortBulk();
            void OnDead();
            void OnLink(QString url);
            void OnScroll();
            void Refresh();
            void Menu(QPoint pn);
            void OnClosed();
            void NetworkChanged(libircclient::Network *network);
        private:
            friend class ScrollbackFrame_WorkerThread;
            void clearItems();
            void writeText(ScrollbackItem item, int highlighted = 0);
            bool isVisible;
            QTimer scroller;
            bool isClean;
            int maxItems;
            int currentScrollbar;
            QList<ScrollbackItem> unwritten;
            libircclient::Network *precachedNetwork;
            STextBox *textEdit;
            bool needsRefresh;
            Scrollback *scrollback;
			//QStandardItem *treeNode;
            QString buffer;
            UserFrame *userFrame;
            QString _name;
            InputBox *inputBox;
            Ui::ScrollbackFrame *ui;
            ScrollbackFrame *_parent;
    };
}

#endif // SCROLLBACKFRAME_H
