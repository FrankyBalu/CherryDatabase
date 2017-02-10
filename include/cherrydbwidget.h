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

#ifndef CHERRYDBWIDGET_H
#define CHERRYDBWIDGET_H

#include <QObject>
#include <QWidget>

namespace Ui
{
class CherryDBWidget;
}

class CherryDBWidget : public QWidget
{
    Q_OBJECT

public:
	
	CherryDBWidget(QWidget *parent);

	void initSearch();
	void initListView();
	void initComboBox();
	void defaultComboBoxes();
	void reloadComboBoxCategory();
	void reloadComboBoxBrand();
	void reloadListWidget();
	void categoryChanged();
	void brandChanged();
	void reloadListWidgetAll();
	void reloadListWidgetSearch();
	void reloadListWidgetBrand();
	void reloadListWidgetCategory();
	void search();
	void syncScollBarKcal(int pos);
	void syncScrollBarProduct(int pos);
	void syncActivEntryKcal();
	void syncActivEntryProduct();
	void editEntry();
	void removeEntry();
	void doubleClick();
	void addEntry();
	void reload();
	
	int getCurrentKcal();
private:
    Ui::CherryDBWidget* ui;
	QString currentCategory;
	QString currentBrand;
	int currentSliderPos;
};

#endif // CHERRYDBWIDGET_H
