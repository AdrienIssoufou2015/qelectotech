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
#ifndef EDITOR_COMMANDS_H
#define EDITOR_COMMANDS_H
#include "customelementpart.h"
#include "partpolygon.h"
#include "elementview.h"
#include "elementscene.h"
#include "elementcontent.h"
#include "qgimanager.h"
#include <QtGui>
/**
	This command deletes one or several primitives/parts when editing an
	electrical element.
*/
class DeletePartsCommand : public QUndoCommand {
	// constructors, destructor
	public:
	DeletePartsCommand(ElementScene *, const QList<QGraphicsItem *>, QUndoCommand * = 0);
	virtual ~DeletePartsCommand();
	private:
	DeletePartsCommand(const DeletePartsCommand &);
	
	// methods
	public:
	virtual void undo();
	virtual void redo();
	
	// attributes
	private:
	/// Deleted primitives
	QList<QGraphicsItem *> deleted_parts;
	/// Element editor/view/scene the command should take place on
	ElementScene *editor_scene;
};

/**
	This command pastes primitives when editing an electrical element.
*/
class PastePartsCommand : public QUndoCommand {
	// constructors, destructor
	public:
	PastePartsCommand(ElementView *, const ElementContent &, QUndoCommand * = 0);
	virtual ~PastePartsCommand();
	private:
	PastePartsCommand(const PastePartsCommand &);
	
	// methods
	public:
	virtual void undo();
	virtual void redo();
	virtual void setOffset(int, const QPointF &, int, const QPointF &);
	
	// attributes
	private:
	/// Pasted content
	ElementContent content_;
	/// Element editor/view/scene the command should take place on
	ElementView *editor_view_;
	ElementScene *editor_scene_;
	/// Data required to undo a copy/paste with offset
	int old_offset_paste_count_;
	QPointF old_start_top_left_corner_;
	int new_offset_paste_count_;
	QPointF new_start_top_left_corner_;
	bool uses_offset;
	/// Prevent the first call to redo()
	bool first_redo;
};

/**
	This command cut primitives when editing an electrical element.
*/
class CutPartsCommand : public DeletePartsCommand {
	// constructors, destructor
	public:
	CutPartsCommand(ElementScene *, const QList<QGraphicsItem *>, QUndoCommand * = 0);
	virtual ~CutPartsCommand();
	private:
	CutPartsCommand(const CutPartsCommand &);
};

/**
	This command moves primitives when editing an electrical element.
*/
class MovePartsCommand : public QUndoCommand {
	// constructors, destructor
	public:
	MovePartsCommand(const QPointF &, ElementScene *, const QList<QGraphicsItem *>, QUndoCommand * = 0);
	virtual ~MovePartsCommand();
	private:
	MovePartsCommand(const MovePartsCommand &);
	
	// methods
	public:
	virtual void undo();
	virtual void redo();
	
	// attributes
	private:
	/// List of moved primitives
	QList<QGraphicsItem *> moved_parts;
	/// Element editor/view/scene the command should take place on
	ElementScene *editor_scene;
	/// applied movement
	QPointF movement;
	/// Prevent the first call to redo()
	bool first_redo;
};

/**
	This command adds a primitive when editing an electrical element.
*/
class AddPartCommand : public QUndoCommand {
	// constructors, destructor
	public:
	AddPartCommand(const QString &, ElementScene *, QGraphicsItem *, QUndoCommand * = 0);
	virtual ~AddPartCommand();
	private:
	AddPartCommand(const AddPartCommand &);
	
	// methods
	public:
	virtual void undo();
	virtual void redo();
	
	// attributes
	private:
	/// Added primitive
	QGraphicsItem *part;
	/// Element editor/view/scene the command should take place on
	ElementScene *editor_scene;
	/// Prevent the first call to redo()
	bool first_redo;
};

/**
	This command changes a property of a primitive when editing an electrical
	element.
*/
class ChangePartCommand : public QUndoCommand {
	// constructors, destructor
	public:
	ChangePartCommand(const QString &, CustomElementPart *, const QString &, const QVariant &, const QVariant &, QUndoCommand * = 0);
	virtual ~ChangePartCommand();
	private:
	ChangePartCommand(const ChangePartCommand &);
	
	// methods
	public:
	virtual void undo();
	virtual void redo();
	
	// attributes
	private:
	/// Changed primitive
	CustomElementPart *cep;
	/// Changed property
	QString property;
	/// Former value
	QVariant old_value;
	/// New value
	QVariant new_value;
};

/**
	This command changes the points of a polygon when editing an electrical
	element.
*/
class ChangePolygonPointsCommand : public QUndoCommand {
	// constructors, destructor
	public:
	ChangePolygonPointsCommand(PartPolygon *, const QVector<QPointF> &, const QVector<QPointF> &, QUndoCommand * = 0);
	virtual ~ChangePolygonPointsCommand();
	private:
	ChangePolygonPointsCommand(const ChangePolygonPointsCommand &);
	
	// methods
	public:
	virtual void undo();
	virtual void redo();
	
	// attributes
	/// Changed polygon
	PartPolygon *polygon;
	/// Former points
	QVector<QPointF> old_points;
	/// New points
	QVector<QPointF> new_points;
};

/**
	This command changes the dimensions and/or the hotspot of an electrical
	element.
*/
class ChangeHotspotCommand : public QUndoCommand {
	// constructors, destructor
	public:
	ChangeHotspotCommand(ElementScene *, const QSize &, const QSize &, const QPoint &, const QPoint &, const QPoint & = QPoint(), QUndoCommand * = 0);
	virtual ~ChangeHotspotCommand();
	private:
	ChangeHotspotCommand(const ChangeHotspotCommand &);
	
	// methods
	public:
	virtual void undo();
	virtual void redo();
	
	private:
	void applyOffset(const QPointF &);
	
	// attributes
	/// Element editor/view/scene the command should take place on
	ElementScene *element;
	/// Former dimensions
	QSize size_before;
	/// new dimensions
	QSize size_after;
	/// Former hotspot
	QPoint hotspot_before;
	/// New hotspot
	QPoint hotspot_after;
	/// Offset to be applied to primitives
	QPoint offset;
};

/**
	This command changes the translated names of an electrical element.
*/
class ChangeNamesCommand : public QUndoCommand {
	// constructors, destructor
	public:
	ChangeNamesCommand(ElementScene *, const NamesList &, const NamesList &, QUndoCommand * = 0);
	virtual ~ChangeNamesCommand();
	private:
	ChangeNamesCommand(const ChangeNamesCommand &);
	
	// methods
	public:
	virtual void undo();
	virtual void redo();
	
	// attributes
	private:
	/// List of former names
	NamesList names_before;
	/// List of new names
	NamesList names_after;
	/// Element editor/view/scene the command should take place on
	ElementScene *element;
};

/**
	This command changes the allowed orientations of an electrical element.
*/
class ChangeOrientationsCommand : public QUndoCommand {
	// constructors, destructor
	public:
	ChangeOrientationsCommand(ElementScene *, const OrientationSet &, const OrientationSet &, QUndoCommand * = 0);
	virtual ~ChangeOrientationsCommand();
	private:
	ChangeOrientationsCommand(const ChangeOrientationsCommand &);
	
	// methods
	public:
	virtual void undo();
	virtual void redo();
	
	// attributes
	private:
	/// Former orientations
	OrientationSet ori_before;
	/// New orientations
	OrientationSet ori_after;
	/// Element editor/view/scene the command should take place on
	ElementScene *element;
};

/**
	This command changes the zValue of a set of primitives when editing an
	electrical element.
*/
class ChangeZValueCommand : public QUndoCommand {
	// constructors, destructor
	public:
	/// List the various kind of changes for the zValue
	enum Option {
		BringForward, ///< Bring primitives to the foreground so they have the highest zValue
		Raise,        ///< Raise primitives one layer above their current one; zValues are incremented
		Lower,        ///< Send primitives one layer below their current one; zValues are decremented
		SendBackward  ///< Send primitives to the background so they have the lowest zValue
	};
	ChangeZValueCommand(ElementScene *, Option, QUndoCommand * = 0);
	virtual ~ChangeZValueCommand();
	private:
	ChangeZValueCommand(const ChangeZValueCommand &);
	
	// methods
	public:
	virtual void undo();
	virtual void redo();
	
	private:
	void applyBringForward(const QList<QGraphicsItem *> &);
	void applyRaise(const QList<QGraphicsItem *> &);
	void applyLower(const QList<QGraphicsItem *> &);
	void applySendBackward(const QList<QGraphicsItem *> &);
	
	// attributes
	private:
	/// associates impacted primitives with their former zValues
	QHash<QGraphicsItem *, qreal> undo_hash;
	/// associates impacted primitives with their new zValues
	QHash<QGraphicsItem *, qreal> redo_hash;
	/// Element editor/view/scene the command should take place on
	ElementScene *element;
	/// kind of treatment to apply
	Option option;
};

/**
	This command enables or disables internal connections for an electrical
	element.
*/
class AllowInternalConnectionsCommand : public QUndoCommand {
	// constructors, destructor
	public:
	AllowInternalConnectionsCommand(ElementScene *, bool, QUndoCommand * = 0);
	virtual ~AllowInternalConnectionsCommand();
	private:
	AllowInternalConnectionsCommand(const AllowInternalConnectionsCommand &);
	
	// methods
	public:
	virtual void undo();
	virtual void redo();
	
	// attributes
	private:
	/// Element editor/view/scene the command should take place on
	ElementScene *element;
	/// whether internal connections are allowed afterward
	bool ic;
};

/**
	This command changes extra information carried by an electrical element.
*/
class ChangeInformationsCommand : public QUndoCommand {
	// constructors, destructor
	public:
	ChangeInformationsCommand(ElementScene *, const QString &, const QString &, QUndoCommand * = 0);
	virtual ~ChangeInformationsCommand();
	private:
	ChangeInformationsCommand(const ChangeInformationsCommand &);
	
	// methods
	public:
	virtual void undo();
	virtual void redo();
	
	// attributes
	private:
	/// Element editor/view/scene the command should take place on
	ElementScene *element;
	/// Former information
	QString old_informations_;
	/// New information
	QString new_informations_;
};
#endif
