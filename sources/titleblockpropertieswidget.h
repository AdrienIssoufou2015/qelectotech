/*
	Copyright 2006-2010 Xavier Guerrin
	This file is part of QElectroTech.
	
	QElectroTech is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 2 of the License, or
	(at your option) any later version.
	
	QElectroTech is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with QElectroTech.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef TITLEBLOCK_PROPERTIES_WIDGET_H
#define TITLEBLOCK_PROPERTIES_WIDGET_H
#include <QtGui>
#include "titleblockproperties.h"
/**
	Ce widget permet d'editer un objet TitleBlockProperties, c'est-a-dire les
	valeurs affichees par le cartouche d'un schema.
*/
class TitleBlockPropertiesWidget : public QWidget {
	Q_OBJECT
	// constructeurs, destructeur
	public:
	TitleBlockPropertiesWidget(const TitleBlockProperties &titleblock = TitleBlockProperties(), bool = false, QWidget * = 0);
	virtual ~TitleBlockPropertiesWidget();
	private:
	TitleBlockPropertiesWidget(const TitleBlockPropertiesWidget &);
	
	// methodes
	public:
	TitleBlockProperties titleBlockProperties() const;
	void setTitleBlockProperties(const TitleBlockProperties &);
	bool displayCurrentDate() const;
	bool isReadOnly() const;
	void setReadOnly(bool);
	void setTitleBlockTemplatesList(const QList<QString> &);
	void setTitleBlockTemplatesVisible(bool);
	
	// slots:
	private slots:
	void checkTableRows();
	
	// private methods
	private:
	void initWidgets(const TitleBlockProperties &);
	void initLayouts();
	int nameLessRowsCount() const;
	
	// attributs
	private:
	QStackedLayout *stack_layout;
	QLabel       *titleblock_template_label;
	QComboBox    *titleblock_template_name;
	QLineEdit    *titleblock_title;
	QLineEdit    *titleblock_author;
	QDateEdit    *titleblock_date;
	QLineEdit    *titleblock_filename;
	QLineEdit    *titleblock_folio;
	QLabel       *folio_tip;
	QRadioButton *titleblock_no_date;
	QRadioButton *titleblock_current_date;
	QRadioButton *titleblock_fixed_date;
	bool display_current_date;
	QLabel       *additional_fields_label;
	QTableWidget *additional_fields_table;
	QTabBar      *tabbar;
};
#endif
