#include "ccolorball.h"
#include "cballsquadtree.h"

#include <QTimerEvent>
#include <QTime>
#include <QRandomGenerator>
#include <QPainter>
#include <QtMath>

#include <QDebug>

QWorld* g_ballsStorage = nullptr;

QWorld &world()
{
    if(!g_ballsStorage)
        g_ballsStorage = new QWorld;
    return *g_ballsStorage;
}

const QSizeF QWorld::m_size = QSizeF(1900, 1000);

QWorld::QWorld(QObject *pParent) : QObject(pParent)
{
    addBall(QPointF(100, 100));
    addBall();

    startTimer(0);
}

void QWorld::addBall(QPointF pos /* = QPointF()*/)
{
    if(pos.isNull())
        pos = randPos();
    CColorBall ball(pos, randSize());
    ball.setSpeed(randSpeed());
    m_vBalls << ball;
    emit updated(0);
}

void QWorld::removeBall(int idx)
{
    m_vBalls.remove(idx);
    emit updated(0);
}

void QWorld::calc(int dT)
{
    int start = QTime::currentTime().msecsSinceStartOfDay();

    if(m_bUseQuadTree)
        checkCollisionsQuadTree();
    else
        checkCollisions();

    for(CColorBall& ball : m_vBalls)
        ball.move(dT);
    checkWorldBoundaries();

    int finish = QTime::currentTime().msecsSinceStartOfDay();
    emit updated(finish - start);
}

void QWorld::checkWorldBoundaries()
{
    for(CColorBall& ball : m_vBalls)
    {
        qreal radius = ball.size() / 2;
        qreal x = ball.position().x();
        qreal Vx = ball.speed().x();
        qreal y = ball.position().y();
        qreal Vy = ball.speed().y();
        if((x < radius && Vx < 0) ||    // Залетели за лево и летим
            (x > m_size.width() - radius && Vx > 0))    // Залетели за право и летим
            ball.setSpeed(QPointF(-Vx, Vy));
        if((y < radius && Vy < 0) ||    // Залетели за верх и летим
            (y > m_size.height() - radius && Vy > 0))    // Залетели за низ и летим
            ball.setSpeed(QPointF(Vx, -Vy));
    }
}

void QWorld::checkCollisions()
{
    for(int i = 0; i < numBalls(); ++i)
    {
        QRectF firstRcBall(0, 0, m_vBalls[i].size(), m_vBalls[i].size());
        QPointF firstBallPos = m_vBalls[i].position();
        firstRcBall.moveCenter(firstBallPos);


        for(int j = i + 1; j < numBalls(); ++j)
        {
            QRectF secondRcBall(0, 0, m_vBalls[j].size(), m_vBalls[j].size());
            QPointF secondBallPos = m_vBalls[j].position();
            secondRcBall.moveCenter(secondBallPos);

            if(firstRcBall.intersects(secondRcBall))
                collide(&m_vBalls[i], &m_vBalls[j]);
        }
    }
}

void QWorld::checkCollisionsQuadTree()
{
    QRectF range(0,0, CColorBall::maxSize() * 2., CColorBall::maxSize() * 2.);

    CBallsQuadTree m_quadTree(QRectF(QPointF(0, 0), m_size));

    for(CColorBall& ball : m_vBalls)
        m_quadTree.insert(&ball);

    for(CColorBall& ball : m_vBalls)
    {
        range.moveCenter(ball.position());
        QVector<CColorBall *> neighbors = m_quadTree.queryRange(range);
        for(CColorBall* neighbor : neighbors)
        {
            if(neighbor == &ball)
                continue;
            collide(&ball, neighbor);
        }
    }
}

void QWorld::collide(CColorBall *b1, CColorBall *b2)
{
    qreal minDist = b1->size() / 2. + b2->size() / 2.;
    qreal minDistSqr = minDist * minDist;

    qreal ax = b1->position().x();
    qreal ay = b1->position().y();
    qreal bx = b2->position().x();
    qreal by = b2->position().y();
    qreal distanceSqr = (ax - bx) * (ax - bx) + (ay - by) * (ay - by);

    if(distanceSqr > minDistSqr)
        return;

    QPointF normal = b2->position() - b1->position();
    normal /= qSqrt(normal.x() * normal.x() + normal.y() * normal.y());

    QPointF relVelocity = b2->speed() - b1->speed();

    qreal relVelValue = QPointF::dotProduct(relVelocity, normal);

    if(relVelValue >= 0)
        return;

    qreal invMass1 = 1. / b1->mass();
    qreal invMass2 = 1. / b2->mass();

    qreal impulseModule = -2 * relVelValue / (invMass1 + invMass2);

    QPointF impulse = normal * impulseModule;
    b1->setSpeed(b1->speed() - impulse * invMass1);
    b2->setSpeed(b2->speed() + impulse * invMass2);
}

void QWorld::timerEvent(QTimerEvent *event)
{
    CColorBall b;
    int curTime = QTime::currentTime().msecsSinceStartOfDay();
    if(m_bPlay && m_iPrevTime != -1)
        calc(curTime - m_iPrevTime);
    m_iPrevTime = curTime;
}

void CColorBall::move(int msec)
{
    m_pos += m_speed * msec / 1000.;
}

QColor randColor(int mass)
{
    return QColor::fromHsv(
                250 - mass,
                255,
                255);
}

QPointF randSpeed()
{
    return QPointF(
                QRandomGenerator::global()->bounded(
                    CColorBall::maxSpeed() * 2) - CColorBall::maxSpeed(),
                QRandomGenerator::global()->bounded(
                    CColorBall::maxSpeed() * 2) - CColorBall::maxSpeed());
}

QPointF randPos()
{
    return QPointF(QRandomGenerator::global()->bounded(QWorld::size().width()),
                QRandomGenerator::global()->bounded(QWorld::size().height()));
//    return QPointF(QWorld::size().width() / 2,
//                QWorld::size().height() / 2);
}

qreal randSize()
{
    return QRandomGenerator::global()->bounded(10, maxBallDiameter);
}
