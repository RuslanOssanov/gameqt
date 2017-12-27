#include "game.h"
#include <QColor>
#include <QLabel>
#include <QPainter>
#include <QVector2D>
#include <iostream>
#include <QKeyEvent>
#include <QFile>
#include <QTextStream>
#include <cstdlib>

Game::Game() : width(1100), height(500)
{
    int rand_num_y = rand() % 2;
    int rand_num_x = rand() % 2 + 1;
    QVector2D pos(width / 2, height / 2);
    QVector2D vel(rand_num_x, rand_num_y);
    int rad = width / 110;
    pixmap = QPixmap(width, height);
    ball_pixmap = QPixmap(":/pictures/ball50.png");
  //  rect_pixmap = QPixmap(":/pictures/rect.png");
    if (ball_pixmap.width() > 0)
        rad = ball_pixmap.width() / 2;
    timeout = 50;
    ball.pos = pos;
    ball.vel = vel;
    ball.rad = rad;

    freezetime = rand() % 1000 + 300;
    freezeBall.pos = QVector2D(1.5 * width, 1.5 * height);
    freezeBall.vel = QVector2D(0, 0);
    freezeBall.rad = rad;

    player1.score = 0;
    player2.score = 0;
    int h = 0.05 * width;
    int w = 0.02 * height;
    int x1 = 0.01 * width;
    int y1 = 0.4 * height;

    player1.rect.h = h;
    player1.rect.w = w;
    player1.rect.x1 = x1;
    player1.rect.y1 = y1;
    player1.freeze = 0;

    player2.rect.h = h;
    player2.rect.w = w;
    player2.rect.x1 = width - 1.8 * x1;
    player2.rect.y1 = y1;
    player2.freeze = 0;

//    upload_map(":/maps/map.txt");
 //   std::cout << balls.size() << std::endl;
    /*
    balls.append(Ball(QVector2D(rad, rad), QVector2D(1, 1), rad));
    balls.append(Ball(QVector2D(3 * rad, rad), QVector2D(-1, 3), rad));
    balls.append(Ball(QVector2D(5 * rad, rad), QVector2D(2, -1), rad));
    */
}

void Game::regame() {
    int k = rand() % 3 - 1;
    if (k == 0){
        k++;
    }
    int rand_num_y = rand() % 5;
    int rand_num_x = rand() % 3 + 2;
    QVector2D pos(width / 2, height / 2);
    QVector2D vel(k * rand_num_x, rand_num_y);
    int rad = width / 110;
    if (ball_pixmap.width() > 0)
        rad = ball_pixmap.width() / 2;

    freezetime = rand() % 1000 + 300;
    freezeBall.pos = QVector2D(1.5 * width, 1.5 * height);
    freezeBall.vel = QVector2D(0, 0);
    freezeBall.rad = rad;

    ball.pos = pos;
    ball.vel = vel;
    ball.rad = rad;

}

// рисуем
void Game::draw(QLabel *label) {
    pixmap.fill(QColor(Qt::white));
    QPainter painter(&pixmap);

    painter.drawPixmap(ball.pos.x() - ball.rad, ball.pos.y() - ball.rad,
                        ball.rad, ball.rad, ball_pixmap);
    //painter.drawPixmap(player1.rect.x1, player1.rect.y1, player1.rect.x1 + player1.rect.w, player1.rect.y1 + player1.rect.h, rect_pixmap);
   // painter.drawPixmap(player2.rect.x1, player2.rect.y1, player2.rect.x1 + player2.rect.w, player2.rect.y1 + player2.rect.h, rect_pixmap);

    painter.drawRect(player1.rect.x1, player1.rect.y1, player1.rect.w, player1.rect.h);
    painter.drawRect(player2.rect.x1, player2.rect.y1, player2.rect.w, player2.rect.h);
    if (freezetime == 0) {
        freezeBall.pos.setX(0.3 * width + rand () % (width / 3));
        freezeBall.pos.setY(0.4 * height + rand() % (height / 4));
        painter.drawPixmap(freezeBall.pos.x() - freezeBall.rad, freezeBall.pos.y() - freezeBall.rad,
                            freezeBall.rad, freezeBall.rad, ball_pixmap);
    }
    else if (freezetime == -1) {
        painter.drawPixmap(freezeBall.pos.x() - freezeBall.rad, freezeBall.pos.y() - freezeBall.rad,
                            freezeBall.rad, freezeBall.rad, ball_pixmap);
    }
    label->setPixmap(pixmap);
}

void Game::step() {
    border(ball, player1.rect, player2.rect);
    /*for (int i = 0; i < balls.size(); i++)
        for (int j = i + 1; j < balls.size(); j++)
            collision(balls[i], balls[j]);*/
}

// физика движения шара
void Game::border(Ball &ball, Rect &rect1, Rect &rect2) {
    if (timeout < 50)
        timeout++;
    ball.pos = ball.pos + ball.vel;
    QVector2D norm(0, 0), dist_freeze(freezeBall.pos.x() - ball.pos.x(), freezeBall.pos.y() - ball.pos.y());
    if (timeout == 50 && ball.pos.x() + ball.rad > 1.02 * rect2.x1 &&
        ball.pos.y() - 0.2 * ball.rad <= rect2.y1 + rect2.h &&
        ball.pos.y() + 0.2 * ball.rad >= rect2.y1)
    {
        ball.vel.setY(ball.vel.y() + 0.4 * rect2.vel.y());
        ball.vel.setX(ball.vel.x() * 1.05);
        norm += QVector2D(-1, 0);
        timeout = 0;
    }
    if (timeout == 50 && ball.pos.x() - ball.rad < rect1.x1 + rect1.w &&
        ball.pos.y() - 0.2 * ball.rad <= rect1.y1 + rect1.h &&
        ball.pos.y() + 0.2 * ball.rad >= rect1.y1)
    {
        ball.vel.setY(ball.vel.y() + 0.4 * rect1.vel.y());
        ball.vel.setX(ball.vel.x() * 1.05);
        norm += QVector2D(1, 0);
        timeout = 0;
    }
    if (ball.pos.y() + ball.rad > height)
        norm += QVector2D(0, -1);
    if (ball.pos.y() - ball.rad < 0)
        norm += QVector2D(0, 1);
    if (norm.length() > 1)
        norm.normalize();
    if (ball.pos.x() - ball.rad < -50)
    {
        player2.score++;
        regame();
    }
    if (ball.pos.x() + ball.rad > width + 50)
    {

        player1.score++;
        regame();
    }
    if (freezeBall.rad + ball.rad >= dist_freeze.length()) {
        freezeBall.pos = QVector2D(1.5 * width, 1.5 * height);
        if (ball.vel.x() > 0) {
            player2.freeze = 200;
        }
        else {
            player1.freeze = 200;
        }
    }

    ball.vel = ball.vel - 2 * QVector2D::dotProduct(ball.vel, norm) * norm;

    if (freezetime >= 0)
        freezetime--;
    if (player1.freeze == 0)
        rect_move(rect1);
    else
        player1.freeze--;

    if (player2.freeze == 0)
        rect_move(rect2);
    else
        player2.freeze--;
 //   std :: cout << player1.freeze << ' ' << player2.freeze << std::endl;
}


void Game::rect_move(Rect &rect) {
    if (rect.y1 >= ball.rad * 1.4 && rect.y1 <= height - 100) {
        rect.x1 += rect.vel.x();
        rect.y1 += rect.vel.y();
    }
    if (rect.y1 < ball.rad * 1.4 || rect.y1 > height - 100) {
        if (rect.y1 < ball.rad * 1.4)
            rect.y1 += 5;
        else
            rect.y1 = rect.y1 - 5;
        rect.vel.setX(0);
        rect.vel.setY(0);
    }
}

void Game::key_move(int key) {
    if (key == Qt::Key_Up || key == Qt::Key_Down) {
        if (key == Qt::Key_Down) {
            player2.rect.vel = QVector2D(0, 2);
        }
        else {
            player2.rect.vel = QVector2D(0, -2);
        }
        rect_move(player2.rect);
    }
    if (key == Qt::Key_W || key == Qt::Key_S) {
        if (key == Qt::Key_S) {
            player1.rect.vel = QVector2D(0, 2);
        }
        else {
            player1.rect.vel = QVector2D(0, -2);
        }
        rect_move(player1.rect);
    }
}


/*void Game::upload_map(QString name) {
    QFile data(name);
    int h, w;

    if (data.open(QFile::ReadOnly)) {
          QTextStream in(&data);
          in >> h >> w;
          int r = qMin(height / 3 / h, width / 3 / w);
          for (int i = 0; i < h; i++)
              for (int j = 0; j < w; j++) {
                  char a;
                  in >> a;
                  if (a == '0')
                      balls.append(Ball(QVector2D(height * i / h + r, width * j / w + r),
                                        QVector2D(1, 1), r));
                  else if (a == '1')
                      ball = Ball(QVector2D(height * i / h + r, width * j / w + r),
                                       QVector2D(1, 1), r);
              }
    }
}*/
