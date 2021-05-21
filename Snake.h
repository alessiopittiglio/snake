#pragma once

#include <QWidget>
#include <QPoint>
#include <QTimer>
#include <list>
#include <set>

class Snake : public QWidget
{
	Q_OBJECT

private:
	enum class GameState { READY, RUNNING, PAUSED, GAME_OVER };

	QTimer _engine;
	GameState _state;
	int _rows, _cols;
	std::list<QPoint> _snake;
	std::list<int> _freeCells;
	QPoint _dir;
	QPoint _food;
	bool _canChangeDir;

	void reset();

public:
	Snake(int rows = 20, int cols = 30, int scale = 20);

	// @override 
	virtual void paintEvent(QPaintEvent* e);
	virtual void keyPressEvent(QKeyEvent* e);

	// setter
	void setDir(QPoint newDir) { _dir = newDir; _canChangeDir = false; }

public slots:
	void advance();
};
