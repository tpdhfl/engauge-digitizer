/******************************************************************************************************
 * (C) 2014 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#include "Curve.h"
#include "CurveStyles.h"
#include "DataKey.h"
#include "EngaugeAssert.h"
#include "GraphicsLinesForCurve.h"
#include "GraphicsLinesForCurves.h"
#include "GraphicsPoint.h"
#include "GraphicsPointAbstractBase.h"
#include "GraphicsScene.h"
#include <iostream>
#include "LineStyle.h"
#include "Logger.h"
#include "MainWindow.h"
#include "Point.h"
#include <QGraphicsItem>
#include <QPainterPath>
#include <QTextStream>
#include "QtToString.h"
#include "Transformation.h"

GraphicsLinesForCurves::GraphicsLinesForCurves()
{
}

void GraphicsLinesForCurves::addPoint (const QString &curveName,
                                       const QString &pointIdentifier,
                                       double ordinal,
                                       GraphicsPoint &point)
{
  LOG4CPP_INFO_S ((*mainCat)) << "GraphicsLinesForCurves::addPoint"
                              << " curve=" << curveName.toLatin1().data()
                              << " identifier=" << pointIdentifier.toLatin1().data()
                              << " ordinal=" << ordinal
                              << " pos=" << QPointFToString (point.pos()).toLatin1().data();

  m_graphicsLinesForCurve [curveName]->addPoint (pointIdentifier,
                                                 ordinal,
                                                 point);
}

void GraphicsLinesForCurves::addRemoveCurves (GraphicsScene &scene,
                                              const QStringList &curveNames)
{
  LOG4CPP_INFO_S ((*mainCat)) << "GraphicsLinesForCurves::addRemoveCurves"
                              << " curveCount=" << m_graphicsLinesForCurve.count();

  // Add new curves
  QStringList::const_iterator itrC;
  for (itrC = curveNames.begin (); itrC != curveNames.end (); itrC++) {

    QString curveName = *itrC;

    if (!m_graphicsLinesForCurve.contains (curveName)) {

      GraphicsLinesForCurve *item = new GraphicsLinesForCurve(curveName);
      scene.addItem (item);

      m_graphicsLinesForCurve [curveName] = item;
    }
  }

  // Remove expired curves
  GraphicsLinesContainer::const_iterator itrG, itrGNext;
  for (itrG = m_graphicsLinesForCurve.begin (); itrG != m_graphicsLinesForCurve.end (); itrG = itrGNext) {

    const QString curveName = itrG.key ();
    GraphicsLinesForCurve *graphicsLines = itrG.value();

    itrGNext = itrG;
    itrGNext++;

    if (!curveNames.contains (curveName)) {

      delete graphicsLines;
      m_graphicsLinesForCurve.remove (curveName);
    }
  }
}

void GraphicsLinesForCurves::lineMembershipPurge(const CurveStyles &curveStyles,
                                                 SplineDrawer &splineDrawer,
                                                 QPainterPath &pathMultiValued,
                                                 LineStyle &lineMultiValued)
{
  LOG4CPP_INFO_S ((*mainCat)) << "GraphicsLinesForCurves::lineMembershipPurge";

  GraphicsLinesContainer::const_iterator itr;
  for (itr = m_graphicsLinesForCurve.begin (); itr != m_graphicsLinesForCurve.end (); itr++) {

    const QString curveName = itr.key ();
    GraphicsLinesForCurve *graphicsLines = itr.value();

    graphicsLines->lineMembershipPurge (curveStyles.lineStyle (curveName),
                                        splineDrawer,
                                        pathMultiValued,
                                        lineMultiValued);
  }
}

void GraphicsLinesForCurves::lineMembershipReset()
{
  LOG4CPP_INFO_S ((*mainCat)) << "GraphicsLinesForCurves::lineMembershipReset";

  GraphicsLinesContainer::const_iterator itr;
  for (itr = m_graphicsLinesForCurve.begin (); itr != m_graphicsLinesForCurve.end (); itr++) {

    GraphicsLinesForCurve *graphicsLines = itr.value();

    graphicsLines->lineMembershipReset ();
  }
}

void GraphicsLinesForCurves::print () const
{
  QString text;
  QTextStream str (&text);

  printStream ("", str);
  std::cerr << text.toLatin1().data();
}

void GraphicsLinesForCurves::printStream (QString indentation,
                                          QTextStream &str) const
{
  str << indentation << "GraphicsLinesForCurves\n";

  indentation += INDENTATION_DELTA;

  GraphicsLinesContainer::const_iterator itr;
  for (itr = m_graphicsLinesForCurve.begin (); itr != m_graphicsLinesForCurve.end (); itr++) {

    const GraphicsLinesForCurve *graphicsLines = itr.value();

    graphicsLines->printStream (indentation,
                                str);
  }
}

void GraphicsLinesForCurves::removePoint(const QString &identifier)
{
  LOG4CPP_INFO_S ((*mainCat)) << "GraphicsLinesForCurves::removePoint"
                              << " point=" << identifier.toLatin1().data ()
                              << " curveCount=" << m_graphicsLinesForCurve.count();

  QString curveName = Point::curveNameFromPointIdentifier(identifier);

  ENGAUGE_ASSERT (m_graphicsLinesForCurve.contains (curveName));
  double ordinal = m_graphicsLinesForCurve [curveName]->identifierToOrdinal (identifier);
  m_graphicsLinesForCurve [curveName]->removePoint(ordinal);
}

void GraphicsLinesForCurves::removeTemporaryPointIfExists()
{
  LOG4CPP_INFO_S ((*mainCat)) << "GraphicsLinesForCurves::removeTemporaryPointIfExists";

  QString curveName = Point::curveNameFromPointIdentifier(Point::temporaryPointIdentifier());

  ENGAUGE_ASSERT (m_graphicsLinesForCurve.contains (curveName));
  m_graphicsLinesForCurve [curveName]->removeTemporaryPointIfExists ();
}

void GraphicsLinesForCurves::resetOnLoad()
{
  LOG4CPP_INFO_S ((*mainCat)) << "GraphicsLinesForCurves::resetOnLoad";

  GraphicsLinesContainer::iterator itr;
  for (itr = m_graphicsLinesForCurve.begin(); itr != m_graphicsLinesForCurve.end(); itr++) {
    GraphicsLinesForCurve *curve = itr.value();
    delete curve;
  }

  m_graphicsLinesForCurve.clear();
}

void GraphicsLinesForCurves::updateAfterCommand (GraphicsScene &scene,
                                                 const MainWindow &mainWindow,
                                                 const CurveStyles &curveStyles,
                                                 const QString &curveName,
                                                 const Point &point,
                                                 GeometryWindow *geometryWindow)
{
  LOG4CPP_DEBUG_S ((*mainCat)) << "GraphicsLinesForCurves::updateAfterCommand"
                               << " point=" << point.identifier().toLatin1().data()
                               << " curveCount=" << m_graphicsLinesForCurve.count();

  ENGAUGE_ASSERT (m_graphicsLinesForCurve.contains (curveName));
  m_graphicsLinesForCurve [curveName]->updateAfterCommand (scene,
                                                           mainWindow,
                                                           curveStyles.pointStyle(curveName),
                                                           point,
                                                           geometryWindow);
}

void GraphicsLinesForCurves::updateCurveStyles (const MainWindow &mainWindow,
                                                const CurveStyles &modelCurveStyles)
{
  LOG4CPP_INFO_S ((*mainCat)) << "GraphicsLinesForCurves::updateCurveStyles";

  GraphicsLinesContainer::const_iterator itr;
  for (itr = m_graphicsLinesForCurve.begin (); itr != m_graphicsLinesForCurve.end (); itr++) {

    QString curveName = itr.key();

    m_graphicsLinesForCurve [curveName]->updateCurveStyle (mainWindow,
                                                           modelCurveStyles.curveStyle (curveName));
  }
}

void GraphicsLinesForCurves::updateGraphicsLinesToMatchGraphicsPoints (const CurveStyles &curveStyles,
                                                                       SplineDrawer &splineDrawer,
                                                                       QPainterPath &pathMultiValued,
                                                                       LineStyle &lineMultiValued)
{
  LOG4CPP_INFO_S ((*mainCat)) << "GraphicsLinesForCurves::updateGraphicsLinesToMatchGraphicsPoints";

  GraphicsLinesContainer::const_iterator itr;
  for (itr = m_graphicsLinesForCurve.begin (); itr != m_graphicsLinesForCurve.end (); itr++) {

    QString curveName = itr.key();

    // This is where we add lines for non-axes curves
    if (curveName != AXIS_CURVE_NAME) {

      m_graphicsLinesForCurve [curveName]->updateGraphicsLinesToMatchGraphicsPoints(curveStyles.lineStyle (curveName),
                                                                                    splineDrawer,
                                                                                    pathMultiValued,
                                                                                    lineMultiValued);
    }
  }
}

void GraphicsLinesForCurves::updateHighlightOpacity (double highlightOpacity)
{
  LOG4CPP_INFO_S ((*mainCat)) << "GraphicsLinesForCurves::updateHighlightOpacity"
                              << " highlightOpacity=" << highlightOpacity;

  GraphicsLinesContainer::const_iterator itr;
  for (itr = m_graphicsLinesForCurve.begin (); itr != m_graphicsLinesForCurve.end (); itr++) {

    QString curveName = itr.key();

    m_graphicsLinesForCurve [curveName]->updateHighlightOpacity (highlightOpacity);
  }
}

void GraphicsLinesForCurves::updatePointOrdinalsAfterDrag (const CurveStyles &curveStyles,
                                                           const Transformation &transformation)
{
  LOG4CPP_INFO_S ((*mainCat)) << "GraphicsLinesForCurves::updatePointOrdinalsAfterDrag";

  GraphicsLinesContainer::const_iterator itr;
  for (itr = m_graphicsLinesForCurve.begin (); itr != m_graphicsLinesForCurve.end (); itr++) {

    QString curveName = itr.key();
    GraphicsLinesForCurve *graphicsLines = itr.value();

    graphicsLines->updatePointOrdinalsAfterDrag (curveStyles.lineStyle (curveName),
                                                 transformation);
  }
}
