/*
	Copyright 2006-2013 The QElectroTech Team
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
#ifndef DIAGRAM_H
#define DIAGRAM_H
#include <QtGui>
#include <QtXml>
#include <QHash>
#include "bordertitleblock.h"
#include "conductorproperties.h"
#include "exportproperties.h"
#include "qgimanager.h"
#include "numerotationcontext.h"

class Conductor;
class CustomElement;
class DiagramContent;
class DiagramPosition;
class DiagramTextItem;
class Element;
class ElementsLocation;
class ElementsMover;
class ElementTextItem;
class ElementTextsMover;
class IndependentTextItem;
class QETProject;
class Terminal;
class ConductorTextItem;
class DiagramImageItem;
/**
	This class represents an electric diagram. It manages its various child
	elements, conductors and texts and handles their graphic rendering.
*/
class Diagram : public QGraphicsScene {
	Q_OBJECT
	
	// constructors, destructor
	public:
	Diagram(QObject * = 0);
	virtual ~Diagram();
	
	private:
	Diagram(const Diagram &diagram);
	
	// attributes
	public:
	/**
		Represents available options when rendering a particular diagram:
		 * EmptyBorder: display border only
		 * TitleBlock: display title block
		 * Columns: display columns
	*/
	enum BorderOptions { EmptyBorder, TitleBlock, Columns };
	/// Represents available option of Numerotation type.
	enum NumerotationType { Conductors };
	/// Default properties for new conductors
	ConductorProperties defaultConductorProperties;
	/// Diagram dimensions and title block
	BorderTitleBlock border_and_titleblock;
	/// abscissa grid step size
	static const int xGrid;
	/// ordinate grid step size
	static const int yGrid;
	/// margin around the diagram
	static const qreal margin;
	
	private:
	QGraphicsLineItem *conductor_setter_;
	ElementsMover *elements_mover_;
	ElementTextsMover *element_texts_mover_;
	bool draw_grid_;
	bool use_border_;
	QGIManager *qgi_manager_;
	QUndoStack *undo_stack_;
	bool draw_terminals_;
	bool draw_colored_conductors_;
	QDomDocument xml_document_;
	QETProject *project_;
	bool read_only_;
	qreal diagram_qet_version_;
	QHash <NumerotationType, NumerotationContext > numerotation_;
	
	// methods
	protected:
	virtual void drawBackground(QPainter *, const QRectF &);
	virtual void keyPressEvent(QKeyEvent *);
	virtual void keyReleaseEvent(QKeyEvent *);
	
	public:
	static bool clipboardMayContainDiagram();
	bool setNumerotation (NumerotationType, NumerotationContext);
	NumerotationContext getNumerotation (NumerotationType) const;
	
	// methods related to parent project
	QETProject *project() const;
	void setProject(QETProject *);
	int folioIndex() const;
	qreal declaredQElectroTechVersion(bool = true) const;
	
	// methods related to read only mode
	bool isReadOnly() const;
	void setReadOnly(bool);
	
	// methods related to conductor creation
	void setConductor(bool);
	void setConductorStart (QPointF);
	void setConductorStop(QPointF);
	QList < QSet <Conductor *> > potentials();
	
	// methods related to XML import/export
	QDomDocument toXml(bool = true);
	bool initFromXml(QDomElement &, QPointF = QPointF(), bool = true, DiagramContent * = 0);
	bool fromXml(QDomDocument &, QPointF = QPointF(), bool = true, DiagramContent * = 0);
	bool fromXml(QDomElement &, QPointF = QPointF(), bool = true, DiagramContent * = 0);
	void write();
	void write(const QDomElement &);
	bool wasWritten() const;
	QDomElement writeXml(QDomDocument &) const;
	
	// methods related to graphics items addition/removal on the diagram
	void addElement(Element *);
	void addConductor(Conductor *);
	void addIndependentTextItem(IndependentTextItem *);
	void addDiagramImageItem(DiagramImageItem *);
	
	void removeElement(Element *);
	void removeConductor(Conductor *);
	void removeIndependentTextItem(IndependentTextItem *);
	
	// methods related to graphics options
	ExportProperties applyProperties(const ExportProperties &);
	void setDisplayGrid(bool);
	bool displayGrid();
	void setUseBorder(bool);
	bool useBorder();
	void setBorderOptions(BorderOptions);
	BorderOptions borderOptions();
	DiagramPosition convertPosition(const QPointF &);
	
	bool drawTerminals() const;
	void setDrawTerminals(bool);
	bool drawColoredConductors() const;
	void setDrawColoredConductors(bool);
	
	QRectF border() const;
	QString title() const;
	bool toPaintDevice(QPaintDevice &, int = -1, int = -1, Qt::AspectRatioMode = Qt::KeepAspectRatio);
	QSize imageSize() const;
	
	bool isEmpty() const;
	
	QList<CustomElement *> customElements() const;
	QSet<DiagramTextItem *> selectedTexts() const;
	QSet<ConductorTextItem *> selectedConductorTexts() const;
	QSet<Conductor *> selectedConductors() const;
	DiagramContent content() const;
	DiagramContent selectedContent();
	bool canRotateSelection() const;
	int  beginMoveElements(QGraphicsItem * = 0);
	void continueMoveElements(const QPointF &);
	void endMoveElements();
	int  beginMoveElementTexts(QGraphicsItem * = 0);
	void continueMoveElementTexts(const QPointF &);
	void endMoveElementTexts();
	bool usesElement(const ElementsLocation &);
	bool usesTitleBlockTemplate(const QString &);
	
	QUndoStack &undoStack();
	QGIManager &qgiManager();
	
	public slots:
	void titleChanged(const QString &);
	void diagramTextChanged(DiagramTextItem *, const QString &, const QString &);
	void titleBlockTemplateChanged(const QString &);
	void titleBlockTemplateRemoved(const QString &, const QString & = QString());
	void setTitleBlockTemplate(const QString &);
	
	// methods related to graphics items selection
	void selectAll();
	void deselectAll();
	void invertSelection();
	
	signals:
	void written();
	void readOnlyChanged(bool);
	void usedTitleBlockTemplateChanged(const QString &);
	void diagramTitleChanged(Diagram *, const QString &);
};
Q_DECLARE_METATYPE(Diagram *)

/**
 * @brief Diagram::setNumerotation, store a numerotation type
 * @return true if storage is available
 */
inline bool Diagram::setNumerotation(Diagram::NumerotationType type, NumerotationContext context) {
	switch (type) {
		case Conductors:
			numerotation_.insert(type, context);
			return true;
			break;
		default:
			return false;
			break;
	}
}

/**
 * @brief Diagram::getNumerotation
 * @return the NumerotationContext associated with the key.
 * If numerotation_ contains no item with the key, the function returns a default-constructed NumerotationContext
 */
inline NumerotationContext Diagram::getNumerotation(Diagram::NumerotationType type) const {
	return numerotation_.value(type);
}

/**
	Display or hide the conductor setter, i.e. a dashed conductor stub which appears when creating a conductor between two terminals.
	@param pf true pour ajouter le poseur de conducteur, false pour l'enlever
*/
inline void Diagram::setConductor(bool adding) {
	if (adding) {
		if (!conductor_setter_ -> scene()) addItem(conductor_setter_);
	} else {
		if (conductor_setter_ -> scene()) removeItem(conductor_setter_);
	}
}

/**
	Set the start point of the conductor setter.
	@param start the point (in scene coordinates) which the newly created
	conductor should start from.
*/
inline void Diagram::setConductorStart(QPointF start) {
	conductor_setter_ -> setLine(QLineF(start, conductor_setter_ -> line().p2()));
}

/**
	Set the end point of the conductor setter.
	@param end the point (in scene coordinates) upon to which the newly created
	conductor should be drawn.
*/
inline void Diagram::setConductorStop(QPointF end) {
	conductor_setter_ -> setLine(QLineF(conductor_setter_ -> line().p1(), end));
}

/**
	Set whether the diagram grid should be drawn.
	@param dg true to render the grid, false otherwise.
*/
inline void Diagram::setDisplayGrid(bool dg) {
	draw_grid_ = dg;
}

/**
	@return true if the grid is drawn, false otherwise.
*/
inline bool Diagram::displayGrid() {
	return(draw_grid_);
}

/**
	Set whether the diagram border (including rows/colums headers and the title
	block) should be rendered along with the diagram. When set to false, the size
	of the smallest rectangle containing all items is considered as the diagram
	size.
	@param ub true to take the border into account, false otherwise
*/
inline void Diagram::setUseBorder(bool ub) {
	use_border_ = ub;
}

/**
	@return true if the border is rendered and take into account, false
	otherwise.
*/
inline bool Diagram::useBorder() {
	return(use_border_);
}

/**
	Set the rendering options for the diagram border (including rows/colums
	headers and the title block)
	@param bo Enabled options ORed together
	@see BorderOptions
*/
inline void Diagram::setBorderOptions(Diagram::BorderOptions bo) {
	border_and_titleblock.displayBorder(!(bo & EmptyBorder));
	border_and_titleblock.displayColumns(bo & Columns);
	border_and_titleblock.displayTitleBlock(bo & TitleBlock);
}

/**
	@return The rendering optios for the diagram border
	@see setBorderOptions
*/
inline Diagram::BorderOptions Diagram::borderOptions() {
	BorderOptions options = EmptyBorder;
	if (border_and_titleblock.titleBlockIsDisplayed()) options = (BorderOptions)(options|TitleBlock);
	if (border_and_titleblock.columnsAreDisplayed()) options = (BorderOptions)(options|Columns);
	return(options);
}

/// @return the diagram undo stack
inline QUndoStack &Diagram::undoStack() {
	return(*undo_stack_);
}

/// @return the diagram graphics item manager
inline QGIManager &Diagram::qgiManager() {
	return(*qgi_manager_);
}

/// @return true if terminals are rendered, false otherwise
inline bool Diagram::drawTerminals() const {
	return(draw_terminals_);
}

/// @return true if conductors colors are rendered, false otherwise.
inline bool Diagram::drawColoredConductors() const {
	return(draw_colored_conductors_);
}

#endif
