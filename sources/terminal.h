/*
	Copyright 2006-2012 Xavier Guerrin
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
#ifndef TERMINAL_H
#define TERMINAL_H
#include <QtGui>
#include <QtXml>
#include "qet.h"
class Conductor;
class Diagram;
class Element;
/**
	This class represents a terminal of an electrical element, i.e. a possible
	plug point for conductors.
*/
class Terminal : public QGraphicsItem {
	
	// constructors, destructor
	public:
	Terminal(QPointF,      QET::Orientation, Element * = 0, Diagram * = 0);
	Terminal(qreal, qreal, QET::Orientation, Element * = 0, Diagram * = 0);
	virtual ~Terminal();
	
	private:
	Terminal(const Terminal &);
	
	// methods
	public:
	/**
		Enable the use of qgraphicsitem_cast to safely cast a QGraphicsItem into a
		Terminal
		@return the QGraphicsItem type
	*/
	virtual int type() const { return Type; }
	
	// implementation of QGraphicsItem pure virtual methods
	void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
	QRectF boundingRect() const;
	
	// methods to manage conductors attached to the terminal
	bool addConductor(Conductor *);
	void removeConductor(Conductor *);
	int conductorsCount() const;
	Diagram *diagram() const;
	Element *parentElement() const;
	
	QList<Conductor *> conductors() const;
	QET::Orientation orientation() const;
	QPointF dockConductor() const;
	void updateConductor();
	bool isLinkedTo(Terminal *);
	bool canBeLinkedTo(Terminal *);
	
	// methods related to XML import/export
	static bool valideXml(QDomElement  &);
	bool fromXml (QDomElement &);
	QDomElement toXml (QDomDocument &) const;
	
	protected:
	// methods related to events management
	void hoverEnterEvent  (QGraphicsSceneHoverEvent *);
	void hoverMoveEvent   (QGraphicsSceneHoverEvent *);
	void hoverLeaveEvent  (QGraphicsSceneHoverEvent *);
	void mousePressEvent  (QGraphicsSceneMouseEvent *);
	void mouseMoveEvent   (QGraphicsSceneMouseEvent *);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
	
	// attributes
	public:
	enum { Type = UserType + 1002 };
	/// terminal length
	static const qreal terminalSize;
	
	// Various static colors used for hover effects
	/// default color
	static QColor neutralColor;
	/// color for legal actions
	static QColor allowedColor;
	/// color for allowed but fuzzy or not recommended  actions
	static QColor warningColor;
	/// color for forbidden actions
	static QColor forbiddenColor;
	
	private:
	/// Parent electrical element
	Element *parent_element_;
	/// docking point for conductors
	QPointF dock_conductor_;
	/// docking point for parent element
	QPointF dock_elmt_;
	/// terminal orientation
	QET::Orientation ori_;
	/// List of conductors attached to the terminal
	QList<Conductor *> conductors_;
	/// Pointer to a rectangle representing the terminal bounding rect;
	/// used to calculate the bounding rect once only;
	/// used a pointer because boundingRect() is supposed to be const.
	QRectF *br_;
	/// Last terminal seen through an attached conductor
	Terminal *previous_terminal_;
	/// Whether the mouse pointer is hovering the terminal
	bool hovered_;
	/// Color used for the hover effect
	QColor hovered_color_;
	
	private:
	void init(QPointF, QET::Orientation);
};

/**
	@return the number of conductors attached to the terminal.
*/
inline int Terminal::conductorsCount() const {
	return(conductors_.size());
}

/**
	@return the position, relative to the scene, of the docking point for
	conductors.
*/
inline QPointF Terminal::dockConductor() const {
	return(mapToScene(dock_conductor_));
}

#endif
