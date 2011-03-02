/*
	Copyright 2006-2011 Xavier Guerrin
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
#ifndef QET_TAB_BAR_H
#define QET_TAB_BAR_H
#include <QtGui>
#include <QTabBar>
/**
	Cette classe represente une barre d'onglets.
	Elle se distingue d'une QTabBar sur les points suivants :
		*elle emet un signal lorsque le dernier onglet est ferme
		*elle emet un signal lorsque le premier onglet est insere
		*elle permet de passer d'un onglet a l'autre avec la rollette
*/
class QETTabBar : public QTabBar {
	Q_OBJECT
	
	// constructeurs, destructeur
	public:
	QETTabBar(QWidget * = 0);
	virtual~QETTabBar();
	
	private:
	QETTabBar(const QETTabBar &);
	
	// methodes
	public:
	void activateNextTab();
	void activatePreviousTab();
	void setMovable(bool);
	bool isMovable() const;
	bool isVertical() const;
	bool isHorizontal() const;
	void moveTab(int, int);
	
	protected:
	virtual void tabInserted(int);
	virtual void tabRemoved(int);
	virtual void wheelEvent(QWheelEvent *);
	virtual void mousePressEvent(QMouseEvent *);
	virtual void mouseMoveEvent(QMouseEvent *);
	virtual void mouseReleaseEvent(QMouseEvent *);
	virtual void mouseDoubleClickEvent(QMouseEvent *);
	
	signals:
	void lastTabRemoved();
	void firstTabInserted();
#if QT_VERSION < 0x040500
	void tabMoved(int, int);
#endif
	void tabDoubleClicked(int);
	
	private:
	bool mustMoveTab(int, int, const QPoint &) const;
	int tabForPressedPosition(const QPoint &);
	int tabForMovedPosition(const QPoint &);
	bool posMatchesTabRect(const QRect &, const QPoint &) const;
	
	// attributs
	private:
	bool no_more_tabs_;
	bool movable_tabs_;
	int  moved_tab_;
	QPoint press_point_;
};
#endif
