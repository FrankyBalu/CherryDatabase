/*
 * Part of Cherry Database - Widget to add a new entry
 * Copyright (C) 2017  <Frank Kurbatsch> <frank.kurbatsch@gmail.coml>
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

#ifndef NEWENTRY_H
#define NEWENTRY_H

#include <QDialog>
#include <QString>


namespace Ui
{
class NewEntry;
}

class NewEntry : public QDialog
{
    Q_OBJECT
public:
    NewEntry(QWidget* parent);
	
private slots:
	/**
	 * @brief is executed if buttonAbort is klicked
	 */
	void abortClicked();
	
	/**
	 * @brief is executed if buttonOK is klicked
	 */
	void okClicked();
	
	/**
	 * @brief is executed if checkBoxLiquid is toggled
	 */
	void checkBoxLiquidChanged();
	
	/**
	 * @brief setup the comboBoxs
	 */
	void comboBoxSetup();
	
private:
	/**
	 * @brief convert the Kcal of 100ml to Kcal of 100g
	 */
	void convertLiquid();
	
    Ui::NewEntry* ui;
	bool mLiquid;
	static QString *lastBrand;
	static QString *lastCategory;
};

#endif // NEWENTRY_H
