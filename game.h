#ifndef GAME_H
#define GAME_H
#include <QVector2D>
#include <QLabel>
#include <QPixmap>
#include <QVector>

class Ball {
public:
    Ball(QVector2D pos, QVector2D vel, int rad) {
        Ball::pos = pos;
        Ball::vel = vel;
        Ball::rad = rad;
    }
    Ball() { }
    QVector2D pos, vel;
    int rad;
};

class Rect {
public:
    int h, w, x1, y1;
    QVector2D vel;
    Rect (int h, int w, int x1, int y1){
        Rect::h = h;
        Rect::w = w;
        Rect::x1 = x1;
        Rect::y1 = y1;
        QVector2D vel(0, 0);
        Rect::vel = vel;
    }
    Rect () { };
};

struct Player {
    Rect rect;
    int score;
    int shoot, freeze;
    bool big, small;
};

class Game
{
    int width, height;
    QPixmap pixmap, ball_pixmap, rect_pixmap;
    Ball ball, freezeBall;
    Player player1, player2;
    int timeout;
    int freezetime;
public:
    Game();
    void draw(QLabel *label);
    void step();
    void freeze(Player&);
    void regame();
    void border(Ball&, Rect&, Rect&); // fizika движения шара
 //   void collision(Ball&, Ball&);
    void key_move(int);
    void rect_move(Rect&);
    void upload_map(QString);
};

#endif // GAME_H
