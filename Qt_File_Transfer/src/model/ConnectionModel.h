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

#include <QtCore/QAbstractTableModel>
#include <src/database/object/NetworkDevice.h>
#include <src/database/DatabaseLoader.h>
#include <QDateTime>

class ConnectionModel : public QAbstractTableModel, public SynchronizedList<DeviceConnection>,
                        public DatabaseLoader {
Q_OBJECT

public:
	enum ColumnName {
		AdapterName,
		IpAddress,
		LastCheckedData,
		__itemCount
	};

	explicit ConnectionModel(const QString &deviceId, QObject *parent = nullptr);

	int columnCount(const QModelIndex &parent) const override;

	int rowCount(const QModelIndex &parent) const override;

	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

	QVariant data(const QModelIndex &index, int role) const override;

	void databaseChanged(const SqlSelection &change, ChangeType type) override;

protected:
	QString m_deviceId;
};

