/*
* Copyright (C) 2019 Veli Tasalı, created on 9/28/18
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
#include <QtSql/QSqlError>
#include "NetworkDeviceLoader.h"

QString NetworkDeviceLoader::convertToInet4Address(const QHostAddress &hostAddress, bool parentOnly)
{
	return convertToInet4Address(hostAddress.toIPv4Address(), parentOnly);
}


QString NetworkDeviceLoader::convertToInet4Address(quint32 ipv4Address, bool parentOnly)
{
	QString inet4Address = QString("%1.%2.%3")
			.arg(ipv4Address >> 24 & 0xff)
			.arg(ipv4Address >> 16 & 0xff)
			.arg(ipv4Address >> 8 & 0xff);


	if (!parentOnly) {
		inet4Address = inet4Address.append(".%1")
				.arg(ipv4Address & 0xff);
	}

	return inet4Address;
}

DeviceConnection NetworkDeviceLoader::processConnection(NetworkDevice &device,
                                                        const QHostAddress &hostAddress)
{
	DeviceConnection connection(hostAddress);

	processConnection(device, connection);

	return connection;
}

void NetworkDeviceLoader::processConnection(NetworkDevice &device,
                                            DeviceConnection &connection)
{
	if (!AppUtils::applyAdapterName(connection) && !gDbSignal->reconstruct(connection))
		connection.adapterName = KEYWORD_UNKNOWN_INTERFACE;

	time(&connection.lastCheckedDate);
	connection.deviceId = device.id;

	SqlSelection selection;

	selection.setTableName(DB_TABLE_DEVICECONNECTION);
	selection.setWhere(QString("`%1` = ? AND (`%2` = ? OR `%3` = ?)")
			                   .arg(DB_FIELD_DEVICECONNECTION_DEVICEID)
			                   .arg(DB_FIELD_DEVICECONNECTION_ADAPTERNAME)
			                   .arg(DB_FIELD_DEVICECONNECTION_IPADDRESS));

	selection.whereArgs << QVariant(connection.deviceId)
	                    << QVariant(connection.adapterName)
	                    << QVariant(NetworkDeviceLoader::convertToInet4Address(connection.hostAddress));

	gDbSignal->remove(selection);
	gDbSignal->publish(connection);
}

LoaderResult* NetworkDeviceLoader::loadAsynchronously(const QHostAddress &hostAddress,
                                             const std::function<void(const NetworkDevice &)> &listener)
{
	auto* loaderResult = new LoaderResult;
	QObject::connect(loaderResult, &LoaderResult::deviceLoaded, loaderResult, &QObject::deleteLater);

	GThread::startIndependent([hostAddress, listener, loaderResult](GThread *thisThread) {
		auto *object = new QObject;
		object->moveToThread(thisThread);

		const NetworkDevice &device = load(object, hostAddress);

		if (device.id != nullptr) {
			if (listener != nullptr)
				listener(device);

			emit loaderResult->deviceLoaded(device);
		}
		else
			delete loaderResult;

		delete object;
	});

	return loaderResult;
}

NetworkDevice NetworkDeviceLoader::load(QObject *sender, const QHostAddress &hostAddress)
{
	try {
		auto *bridge = new CommunicationBridge(sender);
		auto device = bridge->loadDevice(hostAddress);

		if (device.id != nullptr) {
			const NetworkDevice &localDevice = AppUtils::getLocalDevice();

			processConnection(device, hostAddress);

			if (localDevice.id != device.id) {
				time(&device.lastUsageTime);
				gDbSignal->publish(device);
			}
		}

		delete bridge;
		return device;
	} catch (...) {
		// do nothing
	}

	return NetworkDevice();
}

NetworkDevice NetworkDeviceLoader::loadFrom(const QJsonObject &jsonIndex)
{
	QJsonObject deviceInfo = jsonIndex.value(KEYWORD_DEVICE_INFO).toObject();
	QJsonObject appInfo = jsonIndex.value(KEYWORD_APP_INFO).toObject();

	NetworkDevice networkDevice(deviceInfo.value(KEYWORD_DEVICE_INFO_SERIAL).toString());

	gDbSignal->reconstruct(networkDevice);

	time(&networkDevice.lastUsageTime);
	networkDevice.brand = deviceInfo.value(KEYWORD_DEVICE_INFO_BRAND).toString();
	networkDevice.model = deviceInfo.value(KEYWORD_DEVICE_INFO_MODEL).toString();
	networkDevice.nickname = deviceInfo.value(KEYWORD_DEVICE_INFO_USER).toString();
	networkDevice.versionNumber = appInfo.value(KEYWORD_APP_INFO_VERSION_CODE).toInt();
	networkDevice.versionName = appInfo.value(KEYWORD_APP_INFO_VERSION_NAME).toString();

	if (networkDevice.nickname.length() > NICKNAME_LENGTH_MAX)
		networkDevice.nickname = networkDevice.nickname.left(NICKNAME_LENGTH_MAX - 1);

	return networkDevice;
}