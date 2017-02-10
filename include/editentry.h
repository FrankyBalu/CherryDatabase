/*
 * Part of Cherry Database - Widget to edit a database entry (by CherryDBWidget)
 * Copyright (C) 2017  <Frank Kurbatsch> <frank.kurbatsch@gmail.com>
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

#ifndef EDITENTRY_H
#define EDITENTRY_H

#include <QObject>
#include <QDialog>
#include "cherrydatabase.h"

namespace Ui
{
class EditEntry;
}

class EditEntry : public QDialog
{
    Q_OBJECT

public:
	EditEntry(QWidget* parent, DBEntry* entry);
	
private slots:
	/**
	 * @brief setup the comboBoxs
	 */
	void comboBoxSetup();
	
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
	
private:
	/**
	 * @brief convert the Kcal of 100ml to Kcal of 100g
	 */
	void convertLiquid();
	
    Ui::EditEntry* ui;
	DBEntry *mEntry;
	bool mLiquid;
	static QString *lastBrand;
	static QString *lastCategory;
};

#endif // EDITENTRY_H
