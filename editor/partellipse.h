#ifndef PART_ELLIPSE_H
#define PART_ELLIPSE_H
#include <QtGui>
#include "customelementgraphicpart.h"
class EllipseEditor;
class PartEllipse : public QGraphicsEllipseItem, public CustomElementGraphicPart {
	// constructeurs, destructeur
	public:
	PartEllipse(QETElementEditor *, QGraphicsItem * = 0, QGraphicsScene * = 0);
	virtual ~PartEllipse() {}
	
	private:
	PartEllipse(const PartEllipse &);
	
	// attributs
	private:
	EllipseEditor *informations;
	
	// methodes
	public:
	virtual void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget * = 0);
	virtual const QDomElement toXml(QDomDocument &) const;
	virtual void fromXml(const QDomElement &);
	virtual QPointF sceneTopLeft() const;
	virtual void setProperty(const QString &, const QVariant &);
	virtual QVariant property(const QString &);
	
	protected:
	QVariant itemChange(GraphicsItemChange, const QVariant &);
};
#endif