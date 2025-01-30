#include "qballswidget.h"
#include "ccolorball.h"

#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

QBallsWidget::QBallsWidget(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_OpaquePaintEvent);
    connect(&world(), SIGNAL(updated(int)), this, SLOT(onBallsUpdated()));
}

qreal QBallsWidget::xToLocal(qreal x) const
{
    return worldRect.width() * x / world().size().width();
}

qreal QBallsWidget::yToLocal(qreal y) const
{
    return worldRect.height() * y / world().size().height();
}

QPointF QBallsWidget::pntToLocal(const QPointF pnt) const
{
    return QPointF(xToLocal(pnt.x()), yToLocal(pnt.y()));
}

QRectF QBallsWidget::rectToLocal(const QRectF rc) const
{
    QRectF ret;
    ret.setTopLeft(pntToLocal(rc.topLeft()));
    ret.setBottomRight(pntToLocal(rc.bottomRight()));
    return ret;
}

qreal QBallsWidget::xToWorld(qreal x) const
{
    return world().size().width() * x / worldRect.width();
}

qreal QBallsWidget::yToWorld(qreal y) const
{
    return world().size().height() * y / worldRect.height();
}

QPointF QBallsWidget::pntToWorld(const QPointF pnt) const
{
    return QPointF(xToWorld(pnt.x()), yToWorld(pnt.y()));
}

void QBallsWidget::onBallsUpdated()
{
    update();
}

int QBallsWidget::hitTest(const QPointF &pos)
{
    for(int i = 0; i < world().numBalls(); ++i)
    {
        QPointF ballPos = pntToLocal(world().ballByIdx(i).position());

        qreal ballSize = xToLocal(world().ballByIdx(i).size());
        qreal ballSizeSqr = ballSize * ballSize / 4;
        QRectF rcBall(0, 0, ballSize, ballSize);
        rcBall.moveCenter(ballPos);

        if(rcBall.contains(pos))
        {
            qreal dx = pos.x() - ballPos.x();
            qreal dy = pos.y() - ballPos.y();
            if(ballSizeSqr > (dx * dx + dy * dy))
                return i;
        }
    }

    return -1;
}

void QBallsWidget::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF rectWorld;
    rectWorld.setBottomRight(pntToLocal(QPointF(world().size().width(), world().size().height())));
    painter.setPen(Qt::black);
    painter.setBrush(QColor(255, 255, 255, 250));
    painter.drawRect(rectWorld);

    painter.setPen(Qt::transparent);

    qreal sqrMaxSpeed = CColorBall::maxSpeed() * CColorBall::maxSpeed();
    foreach(const CColorBall& ball, world().balls())
    {
        qreal ballsSize = xToLocal(ball.size());
        QRectF rcBall(0, 0, ballsSize, ballsSize);
        rcBall.moveCenter(pntToLocal(ball.position()));
        qreal sqrSpeed = ball.speed().x() * ball.speed().x() + ball.speed().y() * ball.speed().y();
        QColor clr;
        clr.setRgbF(qMin(qSqrt(sqrSpeed / sqrMaxSpeed) / 2, 1.), 0., 0.);
        painter.setBrush(clr);
        painter.drawEllipse(rcBall);
    }
}


void QBallsWidget::mousePressEvent(QMouseEvent *event)
{
    int ballHit = hitTest(event->pos());
    if(ballHit == -1)
    {
        if(event->button() == Qt::LeftButton)
            world().addBall(pntToWorld(event->pos()));
    }
    else
        world().removeBall(ballHit);
}


void QBallsWidget::resizeEvent(QResizeEvent* /*event*/)
{
    worldRect = QRectF(0, 0, width(), width() / world().sizeRatio());
    if(worldRect.height() > height())
        worldRect = QRectF(0, 0, height() * world().sizeRatio(), height());
}
