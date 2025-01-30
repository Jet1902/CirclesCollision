#ifndef CCOLORBALL_H
#define CCOLORBALL_H

#include <QObject>

#include <QColor>
#include <QPointF>
#include <QSizeF>
#include <QVector>
#include <QtGlobal>
#include <QRectF>
#include <QPainter>
#include <QtMath>

qreal randSize();
QPointF randSpeed();
QPointF randPos();

const qreal maxBallDiameter = 40;
class CColorBall
{
public:
    CColorBall(){}
    CColorBall(const QPointF& pos, qreal size) : m_pos(pos), m_size(size) {}
    ~CColorBall(){}

    QPointF position() const
        { return m_pos; }
    qreal mass() const
        { return m_size * m_size; }
    QPointF speed() const
        { return m_speed; }
    void setSpeed(const QPointF& v)
        { m_speed = v; }

    qreal size() const
        { return m_size; }

    void move(int msec);

    static qreal maxSize()
        { return maxBallDiameter; }
    static qreal maxSpeed()
        { return 40.; }

protected:

private:
    QPointF     m_pos;
    qreal       m_size;
    QPointF     m_speed;    // Скорость пикс/с
};

typedef QVector<CColorBall> vBalls;

class QWorld : public QObject
{
    Q_OBJECT
public:
    explicit QWorld(QObject *pParent = nullptr);

    const vBalls& balls() const
        { return m_vBalls; }

    int numBalls() const
        { return m_vBalls.size(); }
    const CColorBall& ballByIdx(int idx) const
        { m_vBalls.at(idx); }

    static QSizeF size()
        { return m_size; }
    static qreal sizeRatio()
        { return m_size.width() / m_size.height(); }

    void addBall(QPointF pos = QPointF());
    void removeBall(int idx);

    bool isUsingQuadTree() const
        { return m_bUseQuadTree; }

signals:
    void updated(int msecTaken);

public slots:
    void setPlay(bool play)
        { m_bPlay = play; }
    void clear()
        { m_vBalls.clear(); }
    void setUseQuadTree(bool use)
        { m_bUseQuadTree = use; }

protected:
    void calc(int dT);
    void checkWorldBoundaries();
    void checkCollisions();                                 // Проверяет, сталкиваются ли шарики (пересечение)
    void checkCollisionsQuadTree();
    void collide(CColorBall* first, CColorBall* second);    // Рассчитывает отскок сталкивающихся шариков

private:
    bool    m_bPlay = true;     // Идет ли время
    bool    m_bUseQuadTree = false;
    vBalls  m_vBalls;           // Массив шариков
    int     m_iPrevTime = -1;   // Время предыдущего расчета
    const static QSizeF m_size; //= {1000, 1000};

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
};

QWorld& world();

#endif // CCOLORBALL_H
