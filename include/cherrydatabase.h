/*
 * <one line to give the program's name and a brief idea of what it does.>
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

#ifndef CHERRYDATABASE_H
#define CHERRYDATABASE_H

#include <QObject>
#include <QStringList>
#include <QString>
#include <QVector>
#include <QList>


#define FILEVERSION 1
#define FILEMAGIC 0xCBAFB
typedef struct DBEntry {
	char mCategory[128];
	char mBrand[128];
	char mProductName[128];
	int   mKcal;
} DBEntry;


namespace RTYPE {
const quint8 UNKNOWNERROR = 0;
const quint8 OK = 1;
const quint8 EntryExist = 2;
const quint8 EntryIncomplet =3;
const quint8 EntryExistWithDiffBrand = 4;
const quint8 EntryExistWithDiffCategory = 5;
const quint8 EntryExistWithDiffKcal = 6;
};

class CherryDatabase : public QObject
{
	Q_OBJECT
	
public:
	static CherryDatabase& instance()
	{
		static CherryDatabase singleton;
		return singleton;
	}
	/**
	 * @brief Load a databasefile
	 * 
	 * @param file filename with path to open
	 * @return false on error
	 */
	bool load(QString file);

	/**
	 * @brief Save the database to a file
	 * 
	 * @param file filename with path
	 * @return false on error
	 */
	bool save(QString file);
	
	/**
	 * @brief Get all brand by the given category
	 * 
	 * @param category Name of the category for search brands
	 * @return A list of brands or a clean list if no matches found
	 */
	QStringList getBrandByCategory(QString category);

	/**
	 * @brief Get all products by the given brand
	 * 
	 * @param brand Name of the brand for search
	 * @return A list of products or a clean list if no match found
	 */
	QStringList getProductByBrand(QString brand);

	/**
	 * @brief Get all products by the given category
	 * 
	 * @param category Name of the category for search
	 * @return A list of products or a clean list if no match found
	 */
	QStringList getProductByCategory(QString category);

	/**
	 * @brief Get a list of all categorys
	 * 
	 * @return List of a categorys
	 */
	QStringList getAllCategory();

	/**
	 * @brief Get a list of all brands
	 * 
	 * @return List of all brands
	 */
	QStringList getAllBrand();

	/**
	 * @brief Get a list of all products
	 * 
	 * @return List of all products
	 */
	QStringList getAllProduct();
	
	/**
	 * @brief Get the Kcal/100g of a product
	 * 
	 * @param brand The brand of the product
	 * @param name The Name of the product
	 * @return The Kcal/100g
	 */
	quint32 getKcal(QString brand, QString name);
	
	/**
	 * @brief Get Kcal of a entry by index
	 * 
	 * @param index The index of the entry
	 * @return The Kcal/100g
	 */
	quint32 getKcal(int index);
	
	/**
	 * @brief Get list of Kcal from a brand
	 * 
	 * @param brand the brand for the kcal
	 * @return list of kcals
	 */
	QList<quint32> getKcalByBrand(QString brand);
	
	/**
	 * @brief Get list of Kcal from a category
	 * 
	 * @param category the category for the kcal
	 * @return list of kcals
	 */
	QList<quint32> getKcalByCategory(QString category);
	
	/**
	 * @brief Add a new entry to the database, of no such entry exist
	 * 
	 * @param entry the new entry
	 * @return see enum::RTYPE
	 */
	quint8 addEntry(DBEntry entry);
	
	/**
	 * @brief Force add a new entry to the database
	 * 
	 * @param entry the new entry
	 * @param overwrite if true (default) it will overwrite an existing (same) entry
	 */
	void forceAddEntry(DBEntry entry, bool overwrite = true);
	
	/**
	 * @brief return number of entrys
	 * 
	 * @return number of entrys
	 */
	int size(){return mDB.size();}
	
	/**
	 * @brief get a specific database entry by metadata
	 * 
	 * @param category the category of the product
	 * @param brand the brand of the product
	 * @param product the name of the product
	 * @param kcal the Kcal of the product
	 * 
	 * @return the databaseentry or Q_NULL_PTR
	 */
	DBEntry* getDBEntry(QString category, QString brand, QString product);
	
	/**
	 * @brief rename a brand in the whole database
	 * 
	 * @param oldBrand the actuel brand name
	 * @param newBrand the new name for the brand
	 * @return nothing on fail or the new brand name
	 */
	QString renameBrand(QString oldBrand, QString newBrand);
	
	/**
	 * @brief rename a category in the whole database
	 * 
	 * @param oldCategory the actuel category name
	 * @param newCategory the new name for the category
	 * @return nothing on fail or the new category name
	 */
	QString renameCategory(QString oldCategory, QString newCategory);
	
	/**
	 * @brief check if the database is modify since last save
	 * @return true if the database is isChanged
	 */
	bool isChanged(){return unsavedChanges;}
	
private:
	
	CherryDatabase();
	
	
	bool unsavedChanges;
	
	QVector<DBEntry> mDB;
	
	///return values for addEntry

};

#endif // CHERRYDATABASE_H
