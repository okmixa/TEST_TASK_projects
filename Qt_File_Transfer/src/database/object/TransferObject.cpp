/*
* Copyright (C) 2019 Veli Tasalı
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

#include <src/util/TransferUtils.h>

TransferObject::TransferObject(requestid id, const QString &deviceId, const Type &type)
		: DatabaseObject()
{
	this->id = id;
	this->deviceId = deviceId;
	this->type = type;
}

bool TransferObject::isDivisionObject() const
{
	return deviceId == nullptr;
}

DbObjectMap TransferObject::getValues() const
{
	return DbObjectMap{
			{DB_FIELD_TRANSFER_ACCESSPORT,   accessPort},
			{DB_FIELD_TRANSFER_DIRECTORY,    directory},
			{DB_FIELD_TRANSFER_FILE,         file},
			{DB_FIELD_TRANSFER_FLAG,         flag},
			{DB_FIELD_TRANSFER_GROUPID,      groupId},
			{DB_FIELD_TRANSFER_ID,           id},
			{DB_FIELD_TRANSFER_MIME,         fileMimeType},
			{DB_FIELD_TRANSFER_NAME,         friendlyName},
			{DB_FIELD_TRANSFER_SIZE,         (uint) fileSize},
			{DB_FIELD_TRANSFER_SKIPPEDBYTES, (uint) skippedBytes},
			{DB_FIELD_TRANSFER_TYPE,         type},
			{DB_FIELD_TRANSFER_DEVICEID,     deviceId}
	};
}

SqlSelection TransferObject::getWhere() const
{
	SqlSelection selection;

	if (isDivisionObject()) {
		selection.setTableName(DB_DIVIS_TRANSFER);
		selection.setWhere(QString("`%1` = ? AND `%2` = ?")
				                   .arg(DB_FIELD_TRANSFER_ID)
				                   .arg(DB_FIELD_TRANSFER_TYPE));
	} else {
		selection.setTableName(DB_TABLE_TRANSFER);
		selection.setWhere(QString("`%1` = ? AND `%2` = ? AND `%3` = ?")
				                   .arg(DB_FIELD_TRANSFER_ID)
				                   .arg(DB_FIELD_TRANSFER_TYPE)
				                   .arg(DB_FIELD_TRANSFER_DEVICEID));
	}

	selection.whereArgs << this->id
	                    << this->type
	                    << this->deviceId;

	return selection;
}

void TransferObject::onGeneratingValues(const DbObjectMap &record)
{
	accessPort = record.value(DB_FIELD_TRANSFER_ACCESSPORT).toInt();
	file = record.value(DB_FIELD_TRANSFER_FILE).toString();
	directory = record.value(DB_FIELD_TRANSFER_DIRECTORY).toString();
	flag = (Flag) record.value(DB_FIELD_TRANSFER_FLAG).toInt();
	groupId = record.value(DB_FIELD_TRANSFER_GROUPID).toUInt();
	deviceId = record.value(DB_FIELD_TRANSFER_DEVICEID).toString();
	id = record.value(DB_FIELD_TRANSFER_ID).toUInt();
	fileMimeType = record.value(DB_FIELD_TRANSFER_MIME).toString();
	friendlyName = record.value(DB_FIELD_TRANSFER_NAME).toString();
	fileSize = record.value(DB_FIELD_TRANSFER_SIZE).toUInt();
	skippedBytes = record.value(DB_FIELD_TRANSFER_SKIPPEDBYTES).toUInt();
	type = (Type) record.value(DB_FIELD_TRANSFER_TYPE).toInt();
}

void TransferObject::onRemovingObject(AccessDatabase *db, DatabaseObject *parent)
{
	DatabaseObject::onRemovingObject(db, parent);

	if (type != Type::Incoming || flag != Flag::Interrupted)
		return;

	{
		TransferGroup *castGroup = parent == nullptr ? nullptr : dynamic_cast<TransferGroup *>(parent);
		TransferGroup group = castGroup == nullptr ? TransferGroup(groupId) : *castGroup;

		if (castGroup == nullptr)
			if (!db->reconstructSilently(group))
				return;

		QFileInfo incomingFile = TransferUtils::getIncomingFilePath(group, *this);

		if (incomingFile.isFile())
			QFile::remove(incomingFile.filePath());
	}
}
