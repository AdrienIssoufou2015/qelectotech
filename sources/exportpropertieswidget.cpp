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
#include "exportpropertieswidget.h"

/**
	Constructeur
	@param parent QWidget parent
*/
ExportPropertiesWidget::ExportPropertiesWidget(QWidget *parent) : QWidget(parent) {
	build();
	setExportProperties(ExportProperties());
}

/**
	Constructeur
	@param export_properties Parametres d'export a afficher / editer
	@param parent QWidget parent
*/
ExportPropertiesWidget::ExportPropertiesWidget(const ExportProperties &export_properties, QWidget *parent) : QWidget(parent) {
	build();
	setExportProperties(export_properties);
}

/// Destructeur
ExportPropertiesWidget::~ExportPropertiesWidget() {
}

/**
	@return les parametres d'export definis via le widget
*/
ExportProperties ExportPropertiesWidget::exportProperties() const {
	ExportProperties export_properties;
	
	export_properties.destination_directory = QDir(dirpath -> text());
	export_properties.format                = format -> itemData(format -> currentIndex()).toString();
	export_properties.draw_grid             = draw_grid      -> isChecked();
	export_properties.draw_border           = draw_border    -> isChecked();
	export_properties.draw_inset            = draw_inset     -> isChecked();
	export_properties.draw_terminals        = draw_terminals -> isChecked();
	export_properties.exported_area         = export_border -> isChecked() ? QET::BorderArea : QET::ElementsArea;
	
	return(export_properties);
}

/**
	@param export_properties les parametres d'export a afficher / editer via le widget
*/
void ExportPropertiesWidget::setExportProperties(const ExportProperties &export_properties) {
	dirpath -> setText(QDir::toNativeSeparators(export_properties.destination_directory.absolutePath()));
	
	int index = format -> findData(export_properties.format);
	if (index == -1) index = 0;
	format -> setCurrentIndex(index);
	
	draw_grid           -> setChecked(export_properties.draw_grid);
	draw_border         -> setChecked(export_properties.draw_border);
	draw_inset          -> setChecked(export_properties.draw_inset);
	draw_terminals      -> setChecked(export_properties.draw_terminals);
	
	if (export_properties.exported_area == QET::BorderArea) {
		export_border -> setChecked(true);
	} else {
		export_elements -> setChecked(true);
	}
}

/**
	Slot demandant a l'utilisateur de choisir un dossier
*/
void ExportPropertiesWidget::slot_chooseADirectory() {
	QString user_dir = QFileDialog::getExistingDirectory(
		this,
		tr("Exporter dans le dossier", "dialog title"),
		dirpath -> text()
	);
	if (!user_dir.isEmpty()) {
		dirpath -> setText(QDir::toNativeSeparators(user_dir));
	}
}

/**
	Cette methode construit le widget en lui-meme
*/
void ExportPropertiesWidget::build() {
	// le dialogue est un empilement vertical d'elements
	QVBoxLayout *vboxLayout = new QVBoxLayout();
	
	/* le dialogue comprend une ligne permettant d'indiquer un chemin de dossier (hboxLayout) */
	QHBoxLayout *hboxLayout = new QHBoxLayout();
	QLabel *dirpath_label = new QLabel(tr("Dossier cible :"), this);
	dirpath = new QLineEdit(this);
	QCompleter *completer = new QCompleter(this);
	completer -> setModel(new QDirModel(completer));
	dirpath -> setCompleter(completer);
	button_browse = new QPushButton(tr("Parcourir"), this);
	hboxLayout -> addWidget(dirpath_label);
	hboxLayout -> addWidget(dirpath);
	hboxLayout -> addWidget(button_browse);
	hboxLayout -> addStretch();
	
	vboxLayout -> addLayout(hboxLayout);
	
	/* une ligne permettant de choisir le format (hboxLayout1) */
	QHBoxLayout *hboxLayout1 = new QHBoxLayout();
	hboxLayout1 -> addWidget(new QLabel(tr("Format :"), this));
	hboxLayout1 -> addWidget(format = new QComboBox(this));
	format -> addItem(tr("PNG (*.png)"),    "PNG");
	format -> addItem(tr("JPEG (*.jpg)"),   "JPG");
	format -> addItem(tr("Bitmap (*.bmp)"), "BMP");
	format -> addItem(tr("SVG (*.svg)"),    "SVG");
	hboxLayout1 -> addStretch();
	
	vboxLayout -> addLayout(hboxLayout1);

	
	/* un cadre permettant de specifier les options de l'image finale */
	QGroupBox *groupbox_options = new QGroupBox(tr("Options"));
	QGridLayout *optionshlayout = new QGridLayout(groupbox_options);
	
	// Choix de la zone du schema a exporter
	exported_content_choices = new QButtonGroup(groupbox_options);
	export_border = new QRadioButton(tr("Exporter le cadre"), groupbox_options);
	optionshlayout -> addWidget(export_border, 0, 0);
	exported_content_choices -> addButton(export_border);
	export_elements = new QRadioButton(tr("Exporter les \351l\351ments"), groupbox_options);
	optionshlayout -> addWidget(export_elements, 0, 1);
	exported_content_choices -> addButton(export_elements);
	
	// dessiner la grille
	draw_grid = new QCheckBox(tr("Dessiner la grille"), groupbox_options);
	optionshlayout -> addWidget(draw_grid, 1, 1);
	
	// dessiner le cadre
	draw_border = new QCheckBox(tr("Dessiner le cadre"), groupbox_options);
	optionshlayout -> addWidget(draw_border, 1, 0);
	
	// dessiner le cartouche
	draw_inset = new QCheckBox(tr("Dessiner le cartouche"), groupbox_options);
	optionshlayout -> addWidget(draw_inset, 2, 0);
	
	// dessiner les bornes
	draw_terminals = new QCheckBox(tr("Dessiner les bornes"), groupbox_options);
	optionshlayout -> addWidget(draw_terminals, 2, 1);

	vboxLayout -> addWidget(groupbox_options);
	vboxLayout -> addStretch();
	
	setLayout(vboxLayout);

	// ordre des input selectionnes avec la touche tab
	setTabOrder(dirpath, button_browse);
	setTabOrder(button_browse, format);
	setTabOrder(format, export_border);
	setTabOrder(export_border, draw_border);
	setTabOrder(draw_border, draw_grid);
	setTabOrder(draw_grid, draw_inset);
	setTabOrder(draw_inset, draw_terminals);

	// connexion du bouton permettant le choix du repertoire
	connect(button_browse, SIGNAL(released()), this, SLOT(slot_chooseADirectory()));
	
	// emission de signaux lors du changement de format et lors du changement de zone exportee
	connect(format,                   SIGNAL(currentIndexChanged(int)),         this, SIGNAL(formatChanged()));
	connect(exported_content_choices, SIGNAL(buttonClicked(QAbstractButton *)), this, SIGNAL(exportedAreaChanged()));
}