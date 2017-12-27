        #include "mainwindow.h"
    #include "ui_mainwindow.h"
    #include <QTimer>
    #include <QKeyEvent>
    #include <iostream>
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
    }

    void MainWindow::keyPressEvent(QKeyEvent *event) {
        if ((event->key() == Qt::Key_Up) ||
            (event->key() == Qt::Key_Down) ||
            (event->key() == Qt::Key_W) ||
            (event->key() == Qt::Key_S))
            game.key_move(event->key());
    }
