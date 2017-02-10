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

#include "editcategory.h"
#include "ui_editcategory.h"

#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QMessageBox>

#include "cherrydatabase.h"

EditCategory::EditCategory(QWidget* parent):
QDialog(parent),
ui(new Ui::EditCategory)
{
	ui->setupUi(this);
	
	connect(ui->buttonExit,&QPushButton::clicked,this,&EditCategory::exitClicked);
	connect(ui->listWidgetCategory,&QListWidget::itemClicked,this,&EditCategory::onClick);
	connect(ui->buttonOK,&QPushButton::clicked,this,&EditCategory::okClicked);
	
        initListWidgetCategory();
}

void EditCategory::exitClicked()
{
	this->close();
}

void EditCategory::initListWidgetCategory()
{
	ui->listWidgetCategory->clear();
	QStringList privateList = CherryDatabase::instance().getAllCategory();
	privateList.removeDuplicates();
	ui->listWidgetCategory->addItems(privateList);
}

void EditCategory::onClick()
{
	ui->lineEditNewName->setText(ui->listWidgetCategory->currentItem()->text());
	oldName =ui->listWidgetCategory->currentItem()->text();
}

void EditCategory::okClicked()
{
	newName= ui->lineEditNewName->text();
	if (ui->lineEditNewName->text().isNull())
		return;
	QMessageBox::StandardButton ret;
	ret = QMessageBox::question(this, tr("Rename"), tr("Relay Rename ")+oldName+tr(" in ")+newName);
	if (ret==QMessageBox::Yes)
	{
		CherryDatabase::instance().renameCategory(oldName,newName);
                initListWidgetCategory();
	}
	else
		return;
}
