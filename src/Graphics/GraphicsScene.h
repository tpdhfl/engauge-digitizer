/******************************************************************************************************
 * (C) 2014 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#ifndef GRAPHICS_SCENE_H
#define GRAPHICS_SCENE_H

#include "CmdMediator.h"
#include "GraphicsLinesForCurves.h"
#include <QGraphicsScene>
#include <QObject>
#include <QStringList>

class CmdMediator;
class Curve;
class CurvesGraphs;
class CurveStyles;
class GeometryWindow;
class GraphicsPoint;
class LineStyle;
class MainWindow;
class PointStyle;
class QGraphicsPathItem;
class QTextStream;
class ScaleBar;
class Transformation;

/// Add point and line handling to generic QGraphicsScene. The primary tasks are:
/// -# update the graphics items to stay in sync with the explicit Points in the Document
/// -# update the graphics items to stay in sync with the implicit lines between the Points, according to Document settings
///
/// This class stores points and lines as QGraphicsItems, but also maintains identifier-to-QGraphicsItems mappings to
/// the points and lines are accessible for updates (like when dragging points around and we need to update the attached lines).
class GraphicsScene : public QGraphicsScene
{
  // We use Q_OBJECT so translations work
  Q_OBJECT;
  
public:
  /// Single constructor.
  GraphicsScene(MainWindow *mainWindow);

  /// Virtual destructor needed since using Q_OBJECT
  virtual ~GraphicsScene();

  /// Add one temporary point to m_graphicsLinesForCurves. Non-temporary points are handled by the updateLineMembership functions
  void addTemporaryPoint (const QString &identifier,
                          GraphicsPoint *point);

  /// Add temporary scale bar to scene. The scale bar is different from points and lines (always a complete set of 2 points
  /// and one line, and drawn using different point and line styles) that it is handled outside m_graphisLinesForCurves
  void addTemporaryScaleBar (GraphicsPoint *point0,
                             GraphicsPoint *point1,
                             const QString &pointIdentifier0,
                             const QString &pointIdentifier1);

  /// Create one QGraphicsItem-based object that represents one Point. It is NOT added to m_graphicsLinesForCurves (see addPoint)
  GraphicsPoint *createPoint (const MainWindow &mainWindow,
                              const QString &identifier,
                              const PointStyle &pointStyle,
                              const QPointF &posScreen,
                              GeometryWindow *geometryWindow);

  /// Hide all graphics items, except background image, in preparation for preview during IMPORT_TYPE_ADVANCED
  void hideAllItemsExceptImage();

  /// Return a list of identifiers for the points that have moved since the last call to resetPositionHasChanged.
  QStringList positionHasChangedPointIdentifiers () const;

  /// Debugging method that supports print method of this class and printStream method of some other class(es)
  void printStream (QString indentation,
                    QTextStream &str);

  /// Remove specified point. This aborts if the point does not exist
  void removePoint (const QString &identifier);

  /// Remove temporary point if it exists. Temporary point handling is so complicated that this method quietly
  /// allows redundant calls to this method, without complaining that the point has already been removed when called again
  void removeTemporaryPointIfExists ();

  /// Remove temporary scale bar, composed of two points and the line between them
  void removeTemporaryScaleBarIfExists ();

  /// Reset, when loading a document after the first, to same state that first document was at when loaded
  void resetOnLoad();

  /// Reset positionHasChanged flag for all items. Typically this is done as part of mousePressEvent.
  void resetPositionHasChangedFlags();

  /// Show or hide all Curves (if showAll is true) or just the selected Curve (if showAll is false);
  void showCurves (bool show,
                   bool showAll = false,
                   const QString &curveName = "");

  /// Update the Points and their Curves after executing a command. After a mouse drag, the lines are already updated and
  /// updating would be done on out of date information (since that would be brought up to date by the NEXT command)
  void updateAfterCommand (CmdMediator &cmdMediator,
                           const MainWindow &mainWindow,
                           double highlightOpacity,
                           GeometryWindow *geometryWindow,
                           const Transformation &transformation);

  /// Update curve styles after settings changed.
  void updateCurveStyles(const MainWindow &mainWindow,
                         const CurveStyles &modelCurveStyles);

  /// A mouse move has just occurred so move the selected points, since they were dragged. The transformation is needed
  /// so the screen coordinates can be converted to graph coordinates when updating point ordinals
  void updateGraphicsLinesToMatchGraphicsPoints (const CurveStyles &modelCurveStyles,
                                                 const Transformation &transformation);

private:

  /// Dump all important cursors
  QString dumpCursors () const;

  const QGraphicsPixmapItem *image () const;

  /// Remove expired curves and add new curves
  void updateCurves (CmdMediator &cmdMediator);

  /// Update path item showing where multi-valued issues are occuring. Nothing appears if there are no problems
  void updatePathItemMultiValued (const QPainterPath &pathMultiValued,
                                  const LineStyle &lineMultiValued);

  /// Update Points using a multi-pass algorithm.
  void updatePointMembership (CmdMediator &cmdMediator,
                              const MainWindow &mainWindow,
                              GeometryWindow *geometryWindow,
                              const Transformation &transformation);

  /// Curve name to GraphicsLinesForCurve
  GraphicsLinesForCurves m_graphicsLinesForCurves;

  /// Special path item that in happy times is never seen. It appears in place of bad line segments on the
  /// other curves. Bad=segment is multi-valued for a function since functions should always be single-valued.
  /// This special curve lives here rather than in m_graphicsLinesForCurves so it is decoupled from
  /// that member (especially since that gets serialized)
  QGraphicsPathItem *m_pathItemMultiValued;
};

#endif // GRAPHICS_SCENE_H
