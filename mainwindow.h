#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "game.h"
#include <QTimer>
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Game game;
    QTimer timer;

public slots:
    void start();
    void draw();
//    void nextpage();
    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
