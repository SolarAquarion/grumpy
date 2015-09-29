//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2015

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QList>
#include <QString>
#include "libcore_global.h"

namespace GrumpyIRC
{
    class LIBCORESHARED_EXPORT Exception
    {
        public:
            Exception();
            Exception(QString Message);

            virtual ~Exception() {}
            virtual int ErrorCode() { return errc; }
            virtual QString GetMessage();
        private:
            int errc;
            QString message;
            QString stack_tree;
    };

    class LIBCORESHARED_EXPORT NullPointerException : public Exception
    {
        public:
            NullPointerException(QString pointer, QString signature);
    };
}

#endif // EXCEPTION_H
