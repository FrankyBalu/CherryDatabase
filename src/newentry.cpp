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

#include "newentry.h"
#include "ui_newentry.h"
#include "cherrydatabase.h"

#include <QString>
#include <QDebug>
#include <QStringList>
#include <QCompleter>

//if (lastBrand==Q_NULLPTR)
QString* NewEntry::lastBrand = new QString;
//if (lastCategory==Q_NULLPTR)
QString* NewEntry::lastCategory = new QString;

NewEntry::NewEntry(QWidget *parent) :
QDialog(parent),
ui(new Ui::NewEntry)
{
	ui->setupUi(this);
	
	mLiquid = false;
	connect(ui->buttonAbort,&QPushButton::clicked,this,&NewEntry::abortClicked);
	connect(ui->buttonOK,&QPushButton::clicked,this,&NewEntry::okClicked);
	connect(ui->lineEditLiquid,&QLineEdit::textChanged,this,&NewEntry::convertLiquid);
	connect(ui->checkBoxLiquid,&QCheckBox::stateChanged,this,&NewEntry::checkBoxLiquidChanged);
	
	ui->lineEditLiquid->setReadOnly(true);
	
	comboBoxSetup();
}


void NewEntry::abortClicked()
{
	this->close();
}

void NewEntry::okClicked()
{
	if (!ui->comboBoxCategory->currentText().isEmpty() &&
		!ui->comboBoxBrand->currentText().isEmpty() &&
		!ui->lineEditKcal->text().isEmpty() &&
		!ui->lineEditProductName->text().isEmpty()
	)
	{
		int kcal = ui->lineEditKcal->text().toInt();
		DBEntry db;
		
		strcpy(db.mBrand,ui->comboBoxBrand->currentText().
				toLocal8Bit().constData());
		strcpy(db.mCategory,ui->comboBoxCategory->currentText().
				toLocal8Bit().constData());
		strcpy(db.mProductName,ui->lineEditProductName->text().
				toLocal8Bit().constData());
		if (ui->checkBoxLiquid->isChecked())
		{
			/*if (ui->lineEditLiquid->text().isEmpty())
				return;
			else
			{*/
				float xGPer100ml; 
				float kcalPer100g;
				float kcalPerXg;
				
				qDebug() << "xGPer100ml: " << ui->lineEditLiquid->text().toFloat();
				xGPer100ml = ui->lineEditLiquid->text().toFloat();
				qDebug() << "kcalPerXg: " << ui->lineEditKcal->text().toFloat();
				kcalPerXg = ui->lineEditKcal->text().toFloat();
				
				kcalPer100g = (100*kcalPerXg)/xGPer100ml;
				float tmp;
				tmp = kcalPer100g;
				if ( (tmp+0.5) > (int)kcalPer100g)
					kcal  = kcalPer100g + 1;
				else
					kcal = kcalPer100g;
			//}	
		}	
		
		db.mKcal = kcal;
		CherryDatabase::instance().addEntry(db);
		lastBrand->clear();
		lastBrand = new QString (ui->comboBoxBrand->currentText());
		lastCategory = new QString( ui->comboBoxCategory->currentText());
		this->close();
	}
	
}

void NewEntry::convertLiquid()
{
	int kcal;
	float xGPer100ml; 
	float kcalPer100g;
	float kcalPerXg;
	
	qDebug() << "xGPer100ml: " << ui->lineEditLiquid->text().toFloat();
	xGPer100ml = ui->lineEditLiquid->text().toFloat();
	qDebug() << "kcalPerXg: " << ui->lineEditKcal->text().toFloat();
	kcalPerXg = ui->lineEditKcal->text().toFloat();
	
	kcalPer100g = (100*kcalPerXg)/xGPer100ml;
	float tmp;
	tmp = kcalPer100g;
	if ( (tmp+0.5) > (int)kcalPer100g)
		kcal  = kcalPer100g + 1;
	else
		kcal = kcalPer100g;
	if ( xGPer100ml < 1)
		ui->labelKcalConverted->setText(tr("n/a"));
	else
		ui->labelKcalConverted->setText(QString::number(kcal)+"kcal/100g");
}

void NewEntry::checkBoxLiquidChanged()
{
	if (ui->checkBoxLiquid->isChecked())
	{
		ui->lineEditLiquid->setReadOnly(false);
		ui->labelKcal->setText("kcal/100ml");
	}
	else
	{
		ui->labelKcal->setText("kcal/100g");
		ui->lineEditLiquid->setReadOnly(true);
	}
}

void NewEntry::comboBoxSetup()
{
	QStringList brand = CherryDatabase::instance().getAllBrand();
	brand.removeDuplicates();
	QStringList category = CherryDatabase::instance().getAllCategory();
	category.removeDuplicates();
	brand.sort();
	category.sort();
	
	QCompleter *completerBrand;
	completerBrand = new QCompleter(brand,this);
	QCompleter *completerCategory;
	completerCategory = new QCompleter(category,this);
	ui->comboBoxBrand->lineEdit()->setCompleter(completerBrand);
	ui->comboBoxCategory->lineEdit()->setCompleter(completerCategory);
	
	
	ui->comboBoxBrand->addItems(brand);
	ui->comboBoxCategory->addItems(category);
	if (!lastBrand->isEmpty())
		ui->comboBoxBrand->setCurrentText(lastBrand->toLocal8Bit());
	if (!lastCategory->isEmpty())
		ui->comboBoxCategory->setCurrentText(lastCategory->toLocal8Bit());
}
