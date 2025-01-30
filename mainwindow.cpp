#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>

#include <QDebug>
#include <QLabel>

#include "ccolorball.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->statusBar->addPermanentWidget(m_numBallsLabel = new QLabel);
    ui->statusBar->addPermanentWidget(m_dtLabel = new QLabel);

    connect(&world(), SIGNAL(updated(int)), this, SLOT(onWorldUpdated(int)));
    connect(ui->pushPlay, SIGNAL(toggled(bool)), &world(), SLOT(setPlay(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onWorldUpdated(int msecTaken)
{
    if(m_dtLabel)
        m_dtLabel->setText(QString("%1 msec").arg(msecTaken));
    if(m_numBallsLabel)
        m_numBallsLabel->setText(QString("%1 balls").arg(world().numBalls()));
}


void MainWindow::on_pushAdd10_clicked()
{
    for(int i = 0; i < 10; ++i)
        world().addBall();
}

void MainWindow::on_pushAdd100_clicked()
{
    for(int i = 0; i < 100; ++i)
        world().addBall();
}

void MainWindow::on_pushClear_clicked()
{
    world().clear();
}

void MainWindow::on_pushUseQuadTree_toggled(bool checked)
{
    world().setUseQuadTree(checked);
}
