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

#include "cherrydbwidget.h"
#include "ui_cherrydbwidget.h"
#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QScrollBar>
#include <QStringList>
#include "editentry.h"
#include "newentry.h"

CherryDBWidget::CherryDBWidget(QWidget *parent):
	QWidget(parent),
	ui(new Ui::CherryDBWidget)
{
	ui->setupUi(this);
	currentCategory = tr("All");
	currentBrand = tr("All");
	currentSliderPos =0;
	
	initComboBox();
	initListView();
	initSearch();
}

void CherryDBWidget::brandChanged()
{
	reloadListWidget();
}

void CherryDBWidget::categoryChanged()
{
	reloadListWidgetCategory();
}

void CherryDBWidget::defaultComboBoxes()
{
	ui->comboBoxBrand->clear();
	ui->comboBoxBrand->addItem(tr("All"));
	ui->comboBoxCategory->clear();
	ui->comboBoxCategory->addItem(tr("All"));
}

void CherryDBWidget::doubleClick()
{
	QMessageBox msgBox;
	msgBox.setWindowTitle(tr("Remove or Edit"));
	msgBox.setText(tr("Remove or Edit"));
	QAbstractButton* pButtonEdit = msgBox.addButton(tr("Edit"), QMessageBox::YesRole);
	QAbstractButton* pButtonRemove = msgBox.addButton(tr("Remove"), QMessageBox::YesRole);
	QAbstractButton* pButtonAbort = msgBox.addButton(tr("Abort"), QMessageBox::NoRole);
	msgBox.exec();
	if (msgBox.clickedButton()==pButtonEdit)
		editEntry();
	if (msgBox.clickedButton()==pButtonRemove)
		qDebug()<<"remove gedrückt";
	if (msgBox.clickedButton()==pButtonAbort)
		qDebug()<<"Abbruch gedrückt";
}

void CherryDBWidget::editEntry()
{
	EditEntry *edit;
	DBEntry *entry = CherryDatabase::instance().getDBEntry(currentCategory, currentBrand, ui->listWidgetProduct->currentItem()->text());
	edit = new EditEntry(this,  entry);
	edit->exec();
	reloadComboBoxBrand();
	reloadComboBoxCategory();
	reloadListWidget();
}

void CherryDBWidget::reloadComboBoxBrand()
{
	QString privateCurrent = currentBrand;
	ui->comboBoxBrand->clear();
	ui->comboBoxBrand->addItem(tr("All"));
	QStringList privateList;
	if (currentCategory==tr("All"))
	{
		privateList = CherryDatabase::instance().getAllBrand();
		privateList.removeDuplicates();
	}
	else
	{
		privateList = 
		CherryDatabase::instance().getBrandByCategory(currentCategory);
		privateList.removeDuplicates();
	}
	ui->comboBoxBrand->addItems(privateList);
	int i = privateList.indexOf( privateCurrent); 
	if ( i >= 0)
		ui->comboBoxBrand->setCurrentIndex(i+1);
	reloadListWidget();
}

void CherryDBWidget::reloadComboBoxCategory()
{
	QStringList privateList;
	ui->comboBoxCategory->clear();
	ui->comboBoxCategory->addItem(tr("All"));
	privateList = CherryDatabase::instance().getAllCategory();
	privateList.removeDuplicates();
	ui->comboBoxCategory->addItems(privateList);
	reloadListWidget();
}

void CherryDBWidget::reloadListWidget()
{
	if (currentBrand == tr("All") && currentCategory == tr("All"))
	{
		reloadListWidgetAll();
	}
	else if (currentBrand==tr("All") && currentCategory != tr("All"))
	{
		reloadListWidgetCategory();
	}
	else if (currentBrand!=tr("All"))
	{
		reloadListWidgetBrand();
	}
}

void CherryDBWidget::reloadListWidgetAll()
{
	ui->listWidgetProduct->clear();
	ui->listWidgetKcal->clear();
	QStringList privateListName = CherryDatabase::instance().
	getAllProduct();
	QStringList privateListBrand = CherryDatabase::instance().
	getAllBrand();
	QStringList privateListCategory = CherryDatabase::instance().
	getAllCategory();
	
	if (currentBrand == tr("All") && currentCategory == tr("All"))
	{
		for (int i=0; i <privateListName.size();++i)
		{
			ui->listWidgetKcal->addItem(QString::number(CherryDatabase::
			instance().getKcal(i)));
		}
		ui->listWidgetProduct->addItems(privateListName);
	}
}

void CherryDBWidget::reloadListWidgetBrand()
{
	ui->listWidgetProduct->clear();
	ui->listWidgetKcal->clear();
	QStringList privateList=CherryDatabase::instance().getAllBrand();
	QStringList privateListName = CherryDatabase::instance().getAllProduct();
	QStringList privateListCategory=CherryDatabase::instance().getAllCategory();
	for (int i=0; i<CherryDatabase::instance().size();++i)
	{
		if (currentCategory==tr("All") || 
			privateListCategory[i]==currentCategory)
		{
			if (privateList[i]==currentBrand)
			{
				ui->listWidgetProduct->addItem(privateListName[i]);
				ui->listWidgetKcal->addItem(QString::number(CherryDatabase::
				instance().getKcal(i)));
			}
		}
	}
}


void CherryDBWidget::reloadListWidgetCategory()
{
	ui->listWidgetProduct->clear();
	ui->listWidgetKcal->clear();
	QStringList privateList=CherryDatabase::instance().
	getAllCategory();
	QStringList privateListName = CherryDatabase::instance().
	getAllProduct();
	for (int i=0; i<CherryDatabase::instance().size();++i)
	{
		if (privateList.at(i)==currentCategory)
		{
			ui->listWidgetProduct->addItem(privateListName[i]);
			ui->listWidgetKcal->addItem(QString::number(CherryDatabase::
			instance().getKcal(i)));
		}
	}
}


void CherryDBWidget::reloadListWidgetSearch()
{
	ui->listWidgetProduct->clear();
	ui->listWidgetKcal->clear();
	QStringList privateList=CherryDatabase::instance().
	getAllCategory();
	QStringList privateListName = CherryDatabase::instance().
	getAllProduct();
	if (ui->lineEditSearch->text().isEmpty())
		return;
	for (int i=0; i<CherryDatabase::instance().size();++i)
	{
		if (privateListName.at(i).contains( ui->lineEditSearch->text(), Qt::CaseInsensitive))
		{
			ui->listWidgetProduct->addItem(privateListName[i]);
			ui->listWidgetKcal->addItem(QString::number(CherryDatabase::
			instance().getKcal(i)));
		}
	}
	this->repaint();
}

void CherryDBWidget::removeEntry()
{
}

void CherryDBWidget::search()
{
	reloadListWidgetSearch();
}

void CherryDBWidget::syncActivEntryKcal()
{
	int row=ui->listWidgetKcal->currentRow();
	ui->listWidgetProduct->setCurrentRow(row);
}

void CherryDBWidget::syncActivEntryProduct()
{
	int row=ui->listWidgetProduct->currentRow();
	ui->listWidgetKcal->setCurrentRow(row);
}

void CherryDBWidget::syncScollBarKcal(int pos)
{
	if (pos != currentSliderPos)
	{
		ui->listWidgetProduct->verticalScrollBar()->setValue(pos);
		currentSliderPos = pos;
	}
}

void CherryDBWidget::syncScrollBarProduct(int pos)
{
	if (pos != currentSliderPos)
	{
		ui->listWidgetKcal->verticalScrollBar()->setValue(pos);
		currentSliderPos = pos;
	}
}

void CherryDBWidget::addEntry()
{
	NewEntry entry(this);
	entry.exec();
	reloadComboBoxBrand();
	reloadComboBoxCategory();
	reloadListWidgetAll();
}

void CherryDBWidget::reload()
{
	reloadComboBoxBrand();
	reloadComboBoxCategory();
	reloadListWidget();
}


void CherryDBWidget::initComboBox()
{
	defaultComboBoxes();
	connect(ui->comboBoxCategory, static_cast<void(QComboBox::*)(const QString 
	&)>(&QComboBox::currentIndexChanged),
			[=](const QString &text){ currentCategory=text; categoryChanged();});
	connect(ui->comboBoxBrand, static_cast<void(QComboBox::*)(const QString 
	&)>(&QComboBox::currentIndexChanged),
			[=](const QString &text){ currentBrand=text; brandChanged();});
	reloadComboBoxBrand();
	reloadComboBoxCategory();
}

void CherryDBWidget::initListView()
{
	QScrollBar *scrollBarKcal = ui->listWidgetKcal->verticalScrollBar();
	QScrollBar *scrollBarProduct = ui->listWidgetProduct->verticalScrollBar();
	connect(scrollBarKcal,&QAbstractSlider::valueChanged,this,[=](int pos){syncScollBarKcal(pos);});
	connect(scrollBarProduct,&QAbstractSlider::valueChanged,this,[=](int pos){syncScrollBarProduct(pos);});
	connect(ui->listWidgetKcal,&QListWidget::itemClicked,this,&CherryDBWidget::syncActivEntryKcal);
	connect(ui->listWidgetProduct,&QListWidget::itemClicked,this,&CherryDBWidget::syncActivEntryProduct);
	connect(ui->listWidgetKcal,&QListWidget::itemDoubleClicked,this,&CherryDBWidget::doubleClick);
	connect(ui->listWidgetProduct,&QListWidget::itemDoubleClicked,this,&CherryDBWidget::doubleClick);
	ui->listWidgetProduct->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	reloadListWidget();
	
}

void CherryDBWidget::initSearch()
{
	connect(ui->buttonSearch,&QPushButton::clicked,this,&CherryDBWidget::search);
	connect(ui->lineEditSearch,&QLineEdit::textChanged,this,&CherryDBWidget::search);
}

int CherryDBWidget::getCurrentKcal()
{
	int i = ui->listWidgetKcal->currentItem()->text().toInt();
	qDebug () << "getCurrentKcal: " << i;
	return i;
}
