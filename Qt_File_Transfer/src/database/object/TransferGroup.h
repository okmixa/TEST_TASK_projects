/*
* Copyright (C) 2019 Veli Tasalı, created on 9/25/18
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

#include <ctime>
#include <QtCore/QString>
#include <src/config/Config.h>
#include <src/database/AccessDatabase.h>

class TransferAssignee;

class TransferGroup : public DatabaseObject {
public:
	groupid id = 0;
	time_t dateCreated = 0;
	QString savePath;

	explicit TransferGroup(groupid groupId = 0);

	SqlSelection getWhere() const override;

	DbObjectMap getValues() const override;

	void onGeneratingValues(const DbObjectMap &record) override;

	void onInsertingObject(AccessDatabase *db) override;

	void onRemovingObject(AccessDatabase *db, DatabaseObject *parent) override;
};

class TransferAssignee : public DatabaseObject {
public:
	groupid groupId = 0;
	QString deviceId;
	QString connectionAdapter;
	bool isClone = false;

	explicit TransferAssignee(groupid groupId = 0, const QString &deviceId = nullptr,
	                          const QString &connectionAdapter = nullptr);

	SqlSelection getWhere() const override;

	DbObjectMap getValues() const override;

	void onGeneratingValues(const DbObjectMap &record) override;

	void onRemovingObject(AccessDatabase *db, DatabaseObject *parent) override;
};