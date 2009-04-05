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
#include "styleeditor.h"
#include "customelementgraphicpart.h"

/**
	Constructeur
	@param editor L'editeur d'element concerne
	@param p La partie a editer
	@param parent le Widget parent
*/
StyleEditor::StyleEditor(QETElementEditor *editor, CustomElementGraphicPart *p, QWidget *parent) : ElementItemEditor(editor, parent), part(p) {
	// couleur
	color = new QButtonGroup(this);
	color -> addButton(black_color = new QRadioButton(tr("Noir", "element part color")),  CustomElementGraphicPart::BlackColor);
	color -> addButton(white_color = new QRadioButton(tr("Blanc", "element part color")), CustomElementGraphicPart::WhiteColor);
	
	// style
	style = new QButtonGroup(this);
	style -> addButton(normal_style = new QRadioButton(tr("Normal", "element part line style")),       CustomElementGraphicPart::NormalStyle);
	style -> addButton(dashed_style = new QRadioButton(tr("Pointill\351", "element part line style")), CustomElementGraphicPart::DashedStyle);
	style -> button(part -> lineStyle()) -> setChecked(true);
	
	// epaisseur
	weight = new QButtonGroup(this);
	weight -> addButton(none_weight   = new QRadioButton(tr("Nulle", "element part weight")),   CustomElementGraphicPart::NoneWeight);
	weight -> addButton(thin_weight   = new QRadioButton(tr("Fine", "element part weight")),    CustomElementGraphicPart::ThinWeight);
	weight -> addButton(normal_weight = new QRadioButton(tr("Normale", "element part weight")), CustomElementGraphicPart::NormalWeight);
	
	// remplissage
	filling = new QButtonGroup(this);
	filling -> addButton(no_filling    = new QRadioButton(tr("Aucun", "element part filling")), CustomElementGraphicPart::NoneFilling );
	filling -> addButton(black_filling = new QRadioButton(tr("Noir", "element part filling")),  CustomElementGraphicPart::BlackFilling);
	filling -> addButton(white_filling = new QRadioButton(tr("Blanc", "element part filling")), CustomElementGraphicPart::WhiteFilling);
	
	// antialiasing
	antialiasing = new QCheckBox(tr("Antialiasing"));
	
	updateForm();
	
	main_layout = new QVBoxLayout();
	main_layout -> addWidget(antialiasing);
	
	main_layout -> addWidget(new QLabel("<u>" + tr("Trait :") + "</u> "));
	
	QHBoxLayout *color_layout = new QHBoxLayout();
	color_layout -> addWidget(new QLabel(tr("Couleur : ")));
	color_layout -> addWidget(black_color);
	color_layout -> addWidget(white_color);
	color_layout -> addStretch();
	main_layout -> addItem(color_layout);
	
	QHBoxLayout *style_layout = new QHBoxLayout();
	style_layout -> addWidget(new QLabel(tr("Style : ")));
	style_layout -> addWidget(normal_style);
	style_layout -> addWidget(dashed_style);
	style_layout -> addStretch();
	main_layout -> addItem(style_layout);
	
	QHBoxLayout *weight_layout = new QHBoxLayout();
	weight_layout -> addWidget(new QLabel(tr("\311paisseur : ")));
	weight_layout -> addWidget(none_weight);
	weight_layout -> addWidget(thin_weight);
	weight_layout -> addWidget(normal_weight);
	weight_layout -> addStretch();
	main_layout -> addItem(weight_layout);
	
	main_layout -> addWidget(new QLabel("<u>" + tr("Remplissage :") + "</u> "));
	
	QHBoxLayout *filling_layout = new QHBoxLayout();
	filling_layout -> addWidget(no_filling);
	filling_layout -> addWidget(black_filling);
	filling_layout -> addWidget(white_filling);
	filling_layout -> addStretch();
	main_layout -> addItem(filling_layout);
	
	main_layout -> addStretch();
	
	setLayout(main_layout);
}

/// Destructeur
StyleEditor::~StyleEditor() {
}

/**
	Met a jour le style de la partie a partir des donnees du formulaire
*/
void StyleEditor::updatePart() {
	// applique l'antialiasing
	part -> setAntialiased(antialiasing -> isChecked());
	
	// applique la couleur
	part -> setColor(static_cast<CEGP::Color>(color -> checkedId()));
	
	// applique le style
	part -> setLineStyle(static_cast<CEGP::LineStyle>(style -> checkedId()));
	
	// applique l'epaisseur
	part -> setLineWeight(static_cast<CEGP::LineWeight>(weight -> checkedId()));
	
	// applique le remplissage
	part -> setFilling(static_cast<CEGP::Filling>(filling -> checkedId()));
}

/// Met a jour l'antialiasing et cree un objet d'annulation
void StyleEditor::updatePartAntialiasing()   { addChangePartCommand("style antialiasing", part, "antialias",   antialiasing -> isChecked()); }
/// Met a jour la couleur du trait et cree un objet d'annulation
void StyleEditor::updatePartColor()          { addChangePartCommand("style couleur",      part, "color",       color -> checkedId());        }
/// Met a jour le style du trait et cree un objet d'annulation
void StyleEditor::updatePartLineStyle()      { addChangePartCommand("style ligne",        part, "line-style",  style -> checkedId());        }
/// Met a jour l'epaisseur du trait et cree un objet d'annulation
void StyleEditor::updatePartLineWeight()     { addChangePartCommand("style epaisseur",    part, "line-weight", weight -> checkedId());       }
/// Met a jour la couleur de fond et cree un objet d'annulation
void StyleEditor::updatePartFilling()        { addChangePartCommand("style remplissage",  part, "filling",     filling -> checkedId());      }

/**
	Met a jour le formulaire d'edition
*/
void StyleEditor::updateForm() {
	activeConnections(false);
	// lit l'antialiasing
	antialiasing -> setChecked(part -> antialiased());
	
	// lit la couleur
	color -> button(part -> color()) -> setChecked(true);
	
	// lit le style
	style -> button(part -> lineStyle()) -> setChecked(true);
	
	// lit l'epaisseur
	weight -> button(part -> lineWeight()) -> setChecked(true);
	
	// lit le remplissage
	filling -> button(part -> filling()) -> setChecked(true);
	activeConnections(true);
}

/**
	Ajoute un widget en bas de l'editeur de style
	@param w Widget a inserer
*/
void StyleEditor::appendWidget(QWidget *w) {
	main_layout -> insertWidget(7, w);
}

/**
	Active ou desactive les connexionx signaux/slots entre les widgets internes.
	@param active true pour activer les connexions, false pour les desactiver
*/
void StyleEditor::activeConnections(bool active) {
	if (active) {
		connect(color,        SIGNAL(buttonClicked(int)), this, SLOT(updatePartColor()));
		connect(style,        SIGNAL(buttonClicked(int)), this, SLOT(updatePartLineStyle()));
		connect(weight,       SIGNAL(buttonClicked(int)), this, SLOT(updatePartLineWeight()));
		connect(filling,      SIGNAL(buttonClicked(int)), this, SLOT(updatePartFilling()));
		connect(antialiasing, SIGNAL(stateChanged(int)),  this, SLOT(updatePartAntialiasing()));
	} else {
		disconnect(color,        SIGNAL(buttonClicked(int)), this, SLOT(updatePartColor()));
		disconnect(style,        SIGNAL(buttonClicked(int)), this, SLOT(updatePartLineStyle()));
		disconnect(weight,       SIGNAL(buttonClicked(int)), this, SLOT(updatePartLineWeight()));
		disconnect(filling,      SIGNAL(buttonClicked(int)), this, SLOT(updatePartFilling()));
		disconnect(antialiasing, SIGNAL(stateChanged(int)),  this, SLOT(updatePartAntialiasing()));
	}
}