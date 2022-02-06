/*
* Copyright (C) 2019 Veli Tasalı, created on 2/24/19
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

#include <iostream>
#include <QUrl>
#include <QPushButton>
#include <QtWidgets/QMessageBox>
#include <QtCore/QRandomGenerator>
#include <QtCore/QMimeDatabase>
#include <src/database/object/TransferObject.h>
#include <src/util/AppUtils.h>
#include <src/util/GThread.h>
#include <src/util/TransferUtils.h>
#include "DeviceChooserDialog.h"
#include "FileAdditionProgressDialog.h"

FileAdditionProgressDialog::FileAdditionProgressDialog(QWidget *parent, const QList<QString> &files)
		: QDialog(parent), m_ui(new Ui::FileAdditionProgressDialog)
{
	m_thread = new GThread([this, files](GThread *thread) { task(thread, files); }, true);
	m_ui->setupUi(this);

	connect(m_thread, &GThread::statusUpdate, this, &FileAdditionProgressDialog::taskProgress);
	connect(m_thread, &GThread::finished, this, &FileAdditionProgressDialog::close);
	connect(this, &QDialog::finished, m_thread, &GThread::notifyInterrupt);

	m_thread->start();
}

FileAdditionProgressDialog::~FileAdditionProgressDialog()
{
	delete m_ui;
}

void FileAdditionProgressDialog::task(GThread *thread, const QList<QString> &files)
{
	QList<TransferObject *> transferMap;

	try {
		auto groupId = QRandomGenerator::global()->bounded(static_cast<groupid>(time(nullptr)), sizeof(int));
		requestid requestId = groupId + 1;
		TransferGroup group(groupId);
		QMimeDatabase mimeDb;

		{
			int position = 0;
			for (const auto &path : files) {
				if (thread->interrupted())
					throw std::exception();

				TransferUtils::createTransferMap(thread, &transferMap, group, mimeDb, requestId, path);
				emit thread->statusUpdate(files.size(), position++, path);
			}
		}

		try {
			if (gDbSignal->transaction()) {
				int position = 0;
				for (auto transferObject : transferMap) {
					if (thread->interrupted())
						throw std::exception();

					gDbSignal->insert(*transferObject);
					emit thread->statusUpdate(transferMap.size(), position++, transferObject->friendlyName);
				}

				if (!transferMap.empty()) {
					gDbSignal->insert(group);
					emit filesAdded(group.id);
				}
			}
		} catch (...) {
			// do nothing
		}
	} catch (...) {
		// do nothing
	}

	qDeleteAll(transferMap);
	gDbSignal->commit();
}

void FileAdditionProgressDialog::taskProgress(int max, int progress, const QString &text)
{
	if (max >= 0 && progress >= 0) {
		m_ui->progressBar->setMaximum(max);
		m_ui->progressBar->setValue(progress);
	}

	m_ui->label->setText(text);
}
