/*
	Copyright 2006-2009 Xavier Guerrin
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
#include <QtGui>
#include "qettabwidget.h"
#include "aboutqet.h"
#include "qet.h"

/**
	Constructeur
	@param parent Le QWidget parent de la boite de dialogue
*/
AboutQET::AboutQET(QWidget *parent) : QDialog(parent) {
	// Titre, taille, comportement...
	setWindowTitle(tr("\300 propos de QElectrotech", "window title"));
	setMinimumWidth(680);
	setMinimumHeight(350);
	setModal(true);
	
	// Trois onglets
	QETTabWidget *tabs = new QETTabWidget(this);
	tabs -> addTab(aboutTab(),   tr("\300 &propos",       "tab title"));
	tabs -> addTab(authorsTab(), tr("A&uteurs",           "tab title"));
	tabs -> addTab(licenseTab(), tr("&Accord de licence", "tab title"));
	
	// Un bouton pour fermer la boite de dialogue
	QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Close);
	connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttons, SIGNAL(rejected()), this, SLOT(accept()));
	
	// Le tout dans une disposition verticale
	QVBoxLayout *vlayout = new QVBoxLayout();
	vlayout -> addWidget(title());
	vlayout -> addWidget(tabs);
	vlayout -> addWidget(buttons);
	setLayout(vlayout);
}

/**
	Destructeur
*/
AboutQET::~AboutQET() {
}

/**
	@return Le titre QElectroTech avec son icone
*/
QWidget *AboutQET::title() const {
	QWidget *icon_and_title = new QWidget();
	// icone
	QLabel *icon = new QLabel();
	icon -> setPixmap(QIcon(":/ico/qelectrotech.png").pixmap(48, 48));
	// label "QElectroTech"
	QLabel *title = new QLabel("<span style=\"font-weight:0;font-size:16pt;\">QElectroTech v" + QET::displayedVersion + "</span>");
	title -> setTextFormat(Qt::RichText);
	// le tout dans une grille
	QGridLayout *grid_layout = new QGridLayout();
	grid_layout -> addWidget(icon, 0, 0);
	grid_layout -> addWidget(title, 0, 1);
	grid_layout -> setColumnStretch(0, 1);
	grid_layout -> setColumnStretch(1, 100);
	icon_and_title -> setLayout(grid_layout);
	return(icon_and_title);
}

/**
	@return Le widget contenu par l'onglet "A propos"
*/
QWidget *AboutQET::aboutTab() const {
	QLabel *about = new QLabel(
		tr("QElectroTech, une application de r\351alisation de sch\351mas \351lectriques.") +
		"<br><br>" +
		tr("\251 2006-2009 Les d\351veloppeurs de QElectroTech") +
		"<br><br>"
		"<a href=\"http://qelectrotech.org/\">"
		"http://qelectrotech.org/</a>"
	);
	about -> setAlignment(Qt::AlignCenter);
	about -> setOpenExternalLinks(true);
	about -> setTextFormat(Qt::RichText);
	return(about);
}

/**
	@return Le widget contenu par l'onglet "Auteurs"
*/
QWidget *AboutQET::authorsTab() const {
	QLabel *authors = new QLabel();
	addAuthor(authors, "Beno\356t Ansieau",  "benoit@qelectrotech.org",     tr("Id\351e originale"));
	addAuthor(authors, "Xavier Guerrin",     "xavier@qelectrotech.org",     tr("Programmation"));
	addAuthor(authors, "Youssef Oualmakran", "youssefsan@qelectrotech.org", tr("Traduction en espagnol"));
	authors -> setAlignment(Qt::AlignCenter);
	authors -> setOpenExternalLinks(true);
	authors -> setTextFormat(Qt::RichText);
	return(authors);
}

/**
	@return Le widget contenu par l'onglet "Accord de Licence"
*/
QWidget *AboutQET::licenseTab() const {
	QWidget *license = new QWidget();
	// label
	QLabel *title_license = new QLabel(tr("Ce programme est sous licence GNU/GPL."));
	
	// texte de la GNU/GPL dans une zone de texte scrollable non editable
	QTextEdit *text_license = new QTextEdit();
	text_license -> setPlainText(QET::license());
	text_license -> setReadOnly(true);
	
	// le tout dans une disposition verticale
	QVBoxLayout *license_layout = new QVBoxLayout();
	license_layout -> addWidget(title_license);
	license_layout -> addWidget(text_license);
	license -> setLayout(license_layout);
	return(license);
}

/**
	Ajoute une personne a la liste des auteurs
	@param label QLabel auquel sera ajoute la personne
	@param name  Nom de la personne
	@param email Adresse e-mail de la personne
	@param work  Fonction / travail effectue par la personne
*/
void AboutQET::addAuthor(QLabel *label, const QString &name, const QString &email, const QString &work) const {
	QString new_text = label -> text();
	
	QString author_template = "<span style=\"text-decoration: underline;\">%1</span> : %2 &lt;<a href=\"mailto:%3\">%3</a>&gt;<br><br>";
	
	// ajoute la fonction de la personne
	new_text += author_template.arg(work).arg(name).arg(email);
	label -> setText(new_text);
}