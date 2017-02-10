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

#include "editentry.h"
#include "ui_editentry.h"
#include "cherrydatabase.h"

#include <QString>
#include <QDebug>
#include <QStringList>
#include <QCompleter>

QString* EditEntry::lastBrand = new QString;
//if (lastCategory==Q_NULLPTR)
QString* EditEntry::lastCategory = new QString;


EditEntry::EditEntry(QWidget* parent, DBEntry* entry) :
QDialog(parent),
ui(new Ui::EditEntry)
{
	ui->setupUi(this);
	qDebug() <<"constructor";
	mLiquid = false;
	connect(ui->buttonAbort,&QPushButton::clicked,this,&EditEntry::abortClicked);
	connect(ui->buttonOK,&QPushButton::clicked,this,&EditEntry::okClicked);
	connect(ui->lineEditLiquid,&QLineEdit::textChanged,this,&EditEntry::convertLiquid);
	connect(ui->checkBoxLiquid,&QCheckBox::stateChanged,this,&EditEntry::checkBoxLiquidChanged);

	comboBoxSetup();	
	ui->lineEditLiquid->setReadOnly(true);
	ui->lineEditKcal->setText(QString::number(entry->mKcal));
	ui->lineEditProductName->setText(entry->mProductName);
	ui->comboBoxBrand->setCurrentText(entry->mBrand);
	ui->comboBoxCategory->setCurrentText(entry->mCategory);
	mEntry = entry;

}

void EditEntry::comboBoxSetup()
{
	qDebug() <<"comboboxsetup";
	QCompleter *completerBrand;
	completerBrand = new QCompleter(CherryDatabase::instance().
	getAllBrand(),this);
	QCompleter *completerCategory;
	completerCategory = new QCompleter(CherryDatabase::instance().
	getAllCategory(),this);
	ui->comboBoxBrand->lineEdit()->setCompleter(completerBrand);
	ui->comboBoxCategory->lineEdit()->setCompleter(completerCategory);
	
	QStringList brand = CherryDatabase::instance().getAllBrand();
	brand.removeDuplicates();
	QStringList category = CherryDatabase::instance().getAllCategory();
	category.removeDuplicates();
	ui->comboBoxBrand->addItems(brand);
	ui->comboBoxCategory->addItems(category);
	qDebug() <<"comboboxend";
}

void EditEntry::abortClicked()
{
	qDebug() <<"abortClicked";
	this->close();
}

void EditEntry::okClicked()
{
	qDebug() <<"okClicked";
	if (!ui->comboBoxCategory->currentText().isEmpty() &&
		!ui->comboBoxBrand->currentText().isEmpty() &&
		!ui->lineEditKcal->text().isEmpty() &&
		!ui->lineEditProductName->text().isEmpty()
	)
	{
		int kcal = ui->lineEditKcal->text().toInt();
		
		strcpy(mEntry->mBrand,ui->comboBoxBrand->currentText().
		toLocal8Bit().constData());
		strcpy(mEntry->mCategory,ui->comboBoxCategory->currentText().
		toLocal8Bit().constData());
		strcpy(mEntry->mProductName,ui->lineEditProductName->text().
		toLocal8Bit().constData());
		if (ui->checkBoxLiquid->isChecked())
		{
			/*if (ui->lineEditLiquid->text().isEmpty())
			 *			return;
			 *		else
			 *		{*/
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
		
		mEntry->mKcal = kcal;
		lastBrand->clear();
		lastBrand = new QString (ui->comboBoxBrand->currentText());
		lastCategory = new QString( ui->comboBoxCategory->currentText());
		this->close();
	}
	
}

void EditEntry::convertLiquid()
{
	qDebug() <<"convertLiquid";
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

void EditEntry::checkBoxLiquidChanged()
{
	qDebug() <<"checkBoxLiquidChanged";
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
