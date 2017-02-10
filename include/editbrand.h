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

#ifndef EDITBRAND_H
#define EDITBRAND_H

#include <QObject>
#include <QWidget>
#include <QDialog>

namespace Ui
{
class EditBrand;
}

class EditBrand : public QDialog
{
    Q_OBJECT
    
public:
	EditBrand(QWidget *parent);
	
private slots:
	/**
	 * @brief is executed if buttonOK is klicked
	 */
	void okClicked();
	
	/**
	 * @brief is executed if buttonAbort is klicked
	 */
	void exitClicked();
	
	/**
	 * @brief setup the brand list
	 */
	void initListWidgetBrand();
	
	/**
	 * @brief brand is changed
	 */
	void onClick();

private:
    Ui::EditBrand* ui;
	QString oldName, newName;
};

#endif // EDITBRAND_H
