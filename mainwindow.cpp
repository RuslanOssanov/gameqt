    #include "mainwindow.h"
    #include "ui_mainwindow.h"
    #include <QTimer>
    #include <QKeyEvent>
    #include <iostream>
    #include <QStackedWidget>
    #include <string>
    #include <cstdlib>

    MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
    {
        ui->setupUi(this);
        timer.setInterval(10);
        connect(&timer, SIGNAL(timeout()), this, SLOT(draw()));
    }

    MainWindow::~MainWindow()
    {
        delete ui;
    }

    void MainWindow::start() {
        setFocus();
        timer.start();
    }

    void MainWindow::draw() {
        game.step();
        game.draw(ui->label);
        QString s1 = "Score1: ", s2 = "Score2: ";
        QString tmp1, tmp2;
        tmp1 = QString::number(game.player1.score);
        tmp2 = QString::number(game.player2.score);
        s1.append(tmp1);
        s2.append(tmp2);
        ui->label_2->setText(s1);
        ui->label_3->setText(s2);
    }

   // void MainWindow::nextpage() {
     //   ui->stackedWidget->currentIndex(1);
   // }

    void MainWindow::keyPressEvent(QKeyEvent *event) {
        if ((event->key() == Qt::Key_Up) ||
            (event->key() == Qt::Key_Down) ||
            (event->key() == Qt::Key_W) ||
            (event->key() == Qt::Key_S))
            game.key_move(event->key());
    }
