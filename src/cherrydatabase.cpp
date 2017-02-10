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

#include "cherrydatabase.h"
#include <QErrorMessage>
#include <QDebug>
#include <QFile>
#include <QString>

CherryDatabase::CherryDatabase()
{
	
}

bool CherryDatabase::load(QString filename)
{
	int *t = new int;
	int versionFromFile = 0;
	int magicFromFile = 0;
	
	QFile file ( filename);
	if (!file.open( QIODevice::ReadOnly))
	{
		QErrorMessage m;
		m.showMessage(tr("Error while open: ") + filename);
		m.exec();
		qDebug() << tr("Error while open: ") << filename;
		return false;
	}
	else
		qDebug() << tr("File ") << filename << tr("successfull opened");
	QDataStream in (&file);
	
	in.readRawData((char*)&magicFromFile,sizeof(int));
	if (magicFromFile != FILEMAGIC)
	{
		QErrorMessage m;
		m.showMessage(filename + tr(" is not a valid CherryDatabase file!"));
		m.exec();
		qDebug() << filename <<tr(" is not a valid CherryDatabase file!");
		return false;
	}
	in.readRawData((char*)&versionFromFile,sizeof(int));
	qDebug() << "Version: " << versionFromFile;
	if ( versionFromFile < FILEVERSION )
	{
		QErrorMessage m;
		m.showMessage(tr("Can't load ") + filename +tr(": fileversion to old"));
		qDebug() << tr ( "Database file version is too old!" );
	//	return false;
	}
	else if  ( versionFromFile > (int)FILEVERSION )
	{
		QErrorMessage m;
		m.showMessage(tr("Can't load ") + filename + tr(": fileversion to \
new"));
		qDebug() << tr ( "Database file version is too new!");
		return false;
	}
	
	
	in.readRawData ( (char*)t, sizeof (int) );
	//(*t)=1;
	mDB.clear();
	for ( int i = 0; i < (*t); ++i )
	{
		DBEntry *db;
		db = new DBEntry;
		in.readRawData ( (char*)db, sizeof(DBEntry) );
		mDB.push_back( (*db) );
	}
	file.close();
	return true;
}

bool CherryDatabase::save(QString filename)
{
	QFile file ( filename );
	if (!file.open(QIODevice::WriteOnly))
	{
		QErrorMessage m;
		m.showMessage(tr("Error while open: ") + filename);
		m.exec();
		qDebug() << tr("Error while open: ") << filename;
		return false;
	}
	else
	{
		qDebug() << tr("File ") << filename << tr("successfull opened");
	}
	QDataStream out (&file);
	out.status() == QDataStream::Ok;
	int dbsize = mDB.size();
	int version=FILEVERSION;
	int magic = FILEMAGIC;
	out.writeRawData((char*)&magic,sizeof (int));
	out.writeRawData((char*)&version, sizeof (int));
	out.writeRawData((char*)&dbsize, sizeof (int));
	
	
	for ( int i = 0; i < dbsize; ++i )
	{
		out.writeRawData ( (char*) &mDB.at(i), sizeof(DBEntry) );
	}
	
	file.close ();
	unsavedChanges=false;
	return true;
}

QStringList CherryDatabase::getBrandByCategory(QString category)
{
	QStringList privateList;
	
	for (int i=0; i<mDB.size();++i)
	{
		if (QString(mDB.at(i).mCategory)==category)
		{
			privateList << mDB.at(i).mBrand;
		}
	}
	return privateList;
}

QStringList CherryDatabase::getProductByBrand(QString brand)
{
	QStringList privateList;
	
	for (int i=0; i<mDB.size(); ++i)
	{
		if (QString(mDB.at(i).mBrand)==brand)
		{
			privateList << mDB.at(i).mProductName;
		}
	}
	return privateList;
}

QStringList CherryDatabase::getProductByCategory(QString category)
{
	QStringList privateList;
	
	for (int i=0; i<mDB.size(); ++i)
	{
		if (QString(mDB.at(i).mCategory)==category)
		{
			privateList << mDB.at(i).mProductName;
		}
	}
	return privateList;
}

QStringList CherryDatabase::getAllCategory()
{
	QStringList privateList;
	for (int i=0; i<mDB.size(); ++i)
	{
		privateList << mDB.at(i).mCategory;
	}
	return privateList;
}

QStringList CherryDatabase::getAllBrand()
{
	QStringList privateList;
	for (int i=0; i<mDB.size(); ++i)
	{
		privateList << mDB.at(i).mBrand;
	}
	return privateList;
}

QStringList CherryDatabase::getAllProduct()
{
	QStringList privateList;
	for (int i=0; i<mDB.size(); ++i)
	{
		privateList << mDB.at(i).mProductName;
	}
	return privateList;
}

quint32 CherryDatabase::getKcal(QString brand, QString name)
{
	for (int i=0; i<mDB.size(); ++i)
	{
		if (QString(mDB.at(i).mBrand)==brand && 
					QString(mDB.at(i).mProductName)==name)
		{
			return mDB.at(i).mKcal;
		}
	}
	return 0;
}

quint32 CherryDatabase::getKcal(int index)
{
	return mDB[index].mKcal;
}

QList<quint32> CherryDatabase::getKcalByBrand(QString brand)
{
	QList<quint32> privateList;
	for (int i=0; i<mDB.size();++i)
	{
		if (QString(mDB.at(i).mBrand) == brand)
			privateList << mDB.at(i).mKcal;
	}
	return privateList;
}

QList<quint32> CherryDatabase::getKcalByCategory(QString category)
{
	QList<quint32> privateList;
	for (int i=0; i<mDB.size();++i)
	{
		if (QString(mDB.at(i).mCategory) == category)
			privateList << mDB.at(i).mKcal;
	}
	return privateList;
}



quint8 CherryDatabase::addEntry(DBEntry entry)
{
	bool stC,stB,stP,stK;
	if (strlen(entry.mCategory)<1 || strlen(entry.mBrand)<1 ||
		strlen(entry.mProductName)<1 || entry.mKcal<1)
	{
		return RTYPE::EntryIncomplet;
	}
	
	
	for (int i=0; i<mDB.size(); ++i)
	{
		stC=stB=stP=stK=false;
		if (strcmp(mDB.at(i).mCategory,entry.mCategory)==0)
		{
			stC = true;
			if (strcmp(mDB.at(i).mBrand,entry.mBrand)==0)
			{
				stB = true;
				if (strcmp(mDB.at(i).mProductName,entry.mProductName)==0)
				{
					stP = true;
					if (mDB.at(i).mKcal==entry.mKcal)
					{
						stK = true;
					}
				}
			}
		}
		if (stC&&stB&&stP&&stK)
			return RTYPE::EntryExist;
		if (stC&&stP&&!stB)
			return RTYPE::EntryExistWithDiffBrand;
		if (!stC&&stB&&stP)
			return RTYPE::EntryExistWithDiffCategory;
		if (stC&&stB&&stP&&!stK)
			return RTYPE::EntryExistWithDiffKcal;
	}
	
	mDB.push_back(entry);
	unsavedChanges=true;
	return RTYPE::OK;
}

void CherryDatabase::forceAddEntry(DBEntry entry, bool overwrite)
{
	bool stC,stB,stP,stK;
	bool status = false;
	if (strlen(entry.mCategory)<1 || strlen(entry.mBrand)<1 ||
		strlen(entry.mProductName)<1 || entry.mKcal<1)
	{
		return;
	}
	
	
	
	for (int i=0; i<mDB.size(); ++i)
	{
		stC=stB=stP=stK=false;
		if (strcmp(mDB.at(i).mCategory,entry.mCategory)==0)
		{
			stC = true;
			if (strcmp(mDB.at(i).mBrand,entry.mBrand)==0)
			{
				stB = true;
				if (strcmp(mDB.at(i).mProductName,entry.mProductName)==0)
				{
					stP = true;
					if (mDB.at(i).mKcal==entry.mKcal)
					{
						stK = true;
					}
				}
			}
			
		}
		if (stB&&stP&&overwrite)
		{
			strcpy(mDB[i].mCategory,entry.mCategory);
			strcpy(mDB[i].mBrand,entry.mBrand);
			strcpy(mDB[i].mProductName,entry.mProductName);
			mDB[i].mKcal = entry.mKcal;
			status = true;
			unsavedChanges=true;
		}
	}
	if (!status)
		mDB.push_back(entry);
	return;
}


DBEntry * CherryDatabase::getDBEntry(QString category, QString brand, QString product)
{
	qDebug() <<"brand: "  <<brand;
	qDebug() <<"category: " << category;
	qDebug() <<"product: " << product;
	DBEntry *entry = Q_NULLPTR;
	for (int i=0; i<mDB.size(); ++i)
	{
		if (QString(mDB[i].mProductName)==product)
		{
			if ( category == tr("All") && brand == tr("All"))
				return &mDB[i];
			else if ( category == tr("All") && QString(mDB[i].mBrand)==brand)
				return  &mDB[i];
			else if ( QString(mDB[i].mCategory)==category&& brand==tr("All"))
				return  &mDB[i];
			else if ( QString(mDB[i].mCategory)==category&&QString(mDB[i].mBrand)==brand)
				return &mDB[i];
			else
				return Q_NULLPTR;
		}
	}
}

QString CherryDatabase::renameBrand(QString oldBrand, QString newBrand)
{
	bool found = false;
	for (int i=0; i<mDB.size(); ++i)
	{
		if (QString(mDB[i].mBrand)==oldBrand)
		{
			strcpy (mDB[i].mBrand,newBrand.toLocal8Bit().constData());
			found = true;
		}
	}
	if (!found)
		return QString(tr("Unknown Brand: ") + oldBrand);
	if(found)
		unsavedChanges=true;
	return newBrand;
}

QString CherryDatabase::renameCategory(QString oldCategory, QString newCategory)
{
	bool found = false;
	for (int i=0; i<mDB.size(); ++i)
	{
		if (QString(mDB[i].mCategory)==oldCategory)
		{
			strcpy (mDB[i].mCategory,newCategory.toLocal8Bit().constData());
			found = true;
		}
	}
	if (!found)
		return QString(tr("Unknown Brand: ") + oldCategory);
	if (found)
		unsavedChanges=true;
	return newCategory;
}



































