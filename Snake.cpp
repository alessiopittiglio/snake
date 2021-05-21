#include "Snake.h"
#include <QPainter>
#include <QKeyEvent>

Snake::Snake(int rows, int cols, int scale) : QWidget(0)
{
	_rows = rows;
	_cols = cols;
	
	reset();
	
	setFixedSize(_cols * scale, _rows * scale);

	// populate free cells
	for (int i = 0; i < _rows * _cols; i++)
		_freeCells.push_back(i);

	connect(&_engine, SIGNAL(timeout()), this, SLOT(advance()));
}

void Snake::reset()
{
	_state = GameState::READY;
	_engine.setInterval(100);
	
	_dir = QPoint(1, 0); // right
	_snake.clear();
	_snake.push_back(QPoint(0, _rows - 1));
	_freeCells.remove((_rows - 1) * _cols);
	for (int i = 0; i < _cols / 2 - 1; i++)
	{
		_snake.push_back(_snake.back() + _dir);
		_freeCells.remove(_snake.back().y() * _cols + _snake.back().x());
	}	
	_food = QPoint(_cols / 2, _rows / 2);
	_canChangeDir = true;
	update();
}

void Snake::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);
	
	painter.setWindow(0, 0, _cols, _rows);
	
	painter.setPen(Qt::NoPen);
	painter.setBrush(QBrush(QColor(182, 196, 3)));
	painter.drawRect(painter.window());

	painter.setBrush(QBrush(QColor(101, 83, 10)));
	for (auto& p : _snake)
		painter.drawRect(p.x(), p.y(), 1, 1);

	painter.setPen(QPen(QBrush(QColor(101, 83, 10)), 0.1));
	painter.setBrush(Qt::NoBrush);
	painter.drawRoundRect(_food.x(), _food.y(), 1, 1, 100, 100);

	if (_state == GameState::GAME_OVER)
	{
		painter.setPen(Qt::NoPen);
		painter.setBrush(QBrush(QColor(255, 0, 0, 100)));
		painter.drawRect(painter.window());
	}
}

void Snake::advance()
{
	_snake.push_back(_snake.back() + _dir);
	_freeCells.remove(_snake.back().y() * _cols + _snake.back().x());
	if (_snake.back().x() >= _cols)
		_snake.back().setX(0);
	if (_snake.back().x() < 0)
		_snake.back().setX(_cols - 1);
	if (_snake.back().y() >= _rows)
		_snake.back().setY(0);
	if (_snake.back().y() < 0)
		_snake.back().setY(_rows - 1);

	if (_snake.back() == _food)
	{
		std::list<int>::iterator it = _freeCells.begin();
		std::advance(it, std::rand() % _freeCells.size());
		_food = QPoint(*it % _cols, *it / _cols);
	}
	else
	{
		if(_snake.front().y() * _cols + _snake.front().x() > 0)
			_freeCells.push_back(_snake.front().y() * _cols + _snake.front().x());
		_snake.pop_front();
	}	

	if (std::find(_snake.begin(), std::prev(_snake.end()), _snake.back()) != std::prev(_snake.end()))
	{
		_state = GameState::GAME_OVER;
		_engine.stop();
	}
	update();	// force paintEvent() call	

	_canChangeDir = true;
}

void Snake::keyPressEvent(QKeyEvent* e)
{
	if (_state == GameState::RUNNING && _canChangeDir && e->key() == Qt::Key::Key_Right && _dir.x() != -1)
		setDir(QPoint(1, 0));
	else if (_state == GameState::RUNNING && _canChangeDir && e->key() == Qt::Key::Key_Left && _dir.x() != 1)
		setDir(_dir = QPoint(-1, 0));
	else if (_state == GameState::RUNNING && _canChangeDir && e->key() == Qt::Key::Key_Down && _dir.y() != -1)
		setDir(_dir = QPoint(0, 1));
	else if (_state == GameState::RUNNING && _canChangeDir && e->key() == Qt::Key::Key_Up && _dir.y() != 1)
		setDir(_dir = QPoint(0, -1));
	else if (_state == GameState::READY && e->key() == Qt::Key::Key_S)
	{
		_engine.start();
		_state = GameState::RUNNING;
	}
	else if (_state == GameState::RUNNING && e->key() == Qt::Key::Key_P)
	{
		_engine.stop();
		_state = GameState::PAUSED;
	}
	else if (_state == GameState::PAUSED && e->key() == Qt::Key::Key_R)
	{
		_engine.start();
		_state = GameState::RUNNING;
	}
	else if (_state == GameState::GAME_OVER && e->key() == Qt::Key::Key_N)
		reset();
}
