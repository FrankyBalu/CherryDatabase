/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2017  <copyright holder> <email>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "editbrand.h"
#include "ui_editbrand.h"

#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QMessageBox>

#include "cherrydatabase.h"

EditBrand::EditBrand(QWidget* parent):
	QDialog(parent),
	ui(new Ui::EditBrand)
{
	ui->setupUi(this);
	
	connect(ui->buttonExit,&QPushButton::clicked,this,&EditBrand::exitClicked);
	connect(ui->listWidgetBrand,&QListWidget::itemClicked,this,&EditBrand::onClick);
	connect(ui->buttonOK,&QPushButton::clicked,this,&EditBrand::okClicked);
	
	initListWidgetBrand();
}

void EditBrand::exitClicked()
{
	this->close();
}

void EditBrand::initListWidgetBrand()
{
	ui->listWidgetBrand->clear();
	QStringList privateList = CherryDatabase::instance().getAllBrand();
	privateList.removeDuplicates();
	qDebug() << "privateList: " << privateList;
	ui->listWidgetBrand->addItems(privateList);
}

void EditBrand::onClick()
{
	ui->lineEditNewName->setText(ui->listWidgetBrand->currentItem()->text());
	oldName =ui->listWidgetBrand->currentItem()->text();
}

void EditBrand::okClicked()
{
	newName= ui->lineEditNewName->text();
	if (ui->lineEditNewName->text().isNull())
		return;
	QMessageBox::StandardButton ret;
	ret = QMessageBox::question(this, tr("Rename"), tr("Relay Rename ")+oldName+tr(" in ")+newName);
	if (ret==QMessageBox::Yes)
	{
		CherryDatabase::instance().renameBrand(oldName,newName);
		initListWidgetBrand();
	}
	else
		return;
}



