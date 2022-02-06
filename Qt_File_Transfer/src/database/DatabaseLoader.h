/*
* Copyright (C) 2019 Veli Tasalı, created on 4/1/19
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#pragma once

#include <src/util/SynchronizedList.h>
#include <QtCore/QArgument>
#include <QObject>
#include <src/util/AppUtils.h>
#include "AccessDatabase.h"

class DatabaseLoader;

class DatabaseLoaderPrivate : public QObject {
Q_OBJECT

public:
	explicit DatabaseLoaderPrivate(QObject *parent, DatabaseLoader *loader);

public slots:

	void databaseChanged(const SqlSelection &change, ChangeType type);

protected:
	DatabaseLoader *m_loader;
};

class DatabaseLoader {

public:
	explicit DatabaseLoader(QObject *parent = nullptr);

public:
	void databaseChanged();

	virtual void databaseChanged(const SqlSelection &change, ChangeType type) = 0;

protected:
	DatabaseLoaderPrivate m_private;
};

