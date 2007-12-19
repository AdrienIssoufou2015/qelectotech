/*
	Copyright 2006-2007 Xavier Guerrin
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
#ifndef CONFIG_PAGES_H
#define CONFIG_PAGES_H
#include <QtGui>
#include "conductorpropertieswidget.h"
#include "insetpropertieswidget.h"

/**
	Cette classe abstraite contient les methodes que toutes les pages de
	configuration doivent implementer.
*/
class ConfigPage : public QWidget {
	Q_OBJECT
	public:
	/**
		Constructeur
		@param parent QWidget parent
	*/
	ConfigPage(QWidget *parent) : QWidget(parent) {};
	/// Destructeur
	virtual ~ConfigPage() {};
	/// Applique la configuration saisie par l'utilisateur dans la page de configuration
	virtual void applyConf() = 0;
	/// @return le titre de la page de configuration
	virtual QString title() const = 0;
	/// @return l'icone de la page de configuration
	virtual QIcon icon() const = 0;
};

/**
	Cette classe represente la page de configuration des nouveaux schemas.
*/
class NewDiagramPage : public ConfigPage {
	Q_OBJECT
	// constructeurs, destructeur
	public:
	NewDiagramPage(QWidget * = 0);
	virtual ~NewDiagramPage();
	private:
	NewDiagramPage(const NewDiagramPage &);
	
	// methodes
	public:
	void applyConf();
	QString title() const;
	QIcon icon() const;
	
	// attributs
	public:
	QSpinBox *columns_count;        ///< Widget d'edition du nombre par defaut de colonnes
	QSpinBox *columns_width;        ///< Widget d'edition de la largeur par defaut des colonnes
	QSpinBox *columns_height;       ///< Widget d'edition de la hauteur par defaut des colonnes
	InsetPropertiesWidget *ipw;     ///< Widget d'edition des proprietes par defaut du cartouche
	ConductorPropertiesWidget *cpw; ///< Widget d'edition des proprietes par defaut des conducteurs
};
#endif