#ifndef CBALLSQUADTREE_H
#define CBALLSQUADTREE_H

#include "qballswidget.h"

#include <QPainter>

class CColorBall;
// QuadTree для работы с шариками
class CBallsQuadTree
{
public:
    CBallsQuadTree();
    CBallsQuadTree(const QRectF& rect) : m_boundary(rect){}

    bool insert(CColorBall *pBall);

    QVector<CColorBall*> queryRange(const QRectF& range);

    void paint(QPainter& painter, QBallsWidget& w) const;

protected:
    void subdivide();

private:
    QRectF m_boundary;

    QVector<CColorBall*> m_vBallPtrs;   // указатели на шарики, которые попадают в наш m_boundary, пока мы не поделимся

    const int m_maxNum = 3;
    QVector<CBallsQuadTree> m_vNodes;   // прямоугольники, разделяющие m_boundary: 0 - верхний правый, 1 - нижний правый, 2 - нижний левый, 3 - верхний левый
};

#endif // CBALLSQUADTREE_H
