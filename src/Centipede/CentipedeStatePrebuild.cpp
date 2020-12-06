/******************************************************************************************************
 * (C) 2020 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#include "CentipedeStatePrebuild.h"
#include "CmdMediator.h"
#include "Logger.h"

CentipedeStatePrebuild::CentipedeStatePrebuild (CentipedeStateContext &context) :
  CentipedeStateAbstractBase (context)
{
}

CentipedeStatePrebuild::~CentipedeStatePrebuild ()
{
}

void CentipedeStatePrebuild::begin ()
{
  LOG4CPP_DEBUG_S ((*mainCat)) << "CentipedeStatePrebuild::begin";
}

void CentipedeStatePrebuild::end ()
{
  LOG4CPP_DEBUG_S ((*mainCat)) << "CentipedeStatePrebuild::end";
}

void CentipedeStatePrebuild::handleKeyPress (CmdMediator &cmdMediator,
                                             Qt::Key key,
                                             bool atLeastOneSelectedItem)
{
}

void CentipedeStatePrebuild::handleMouseMove (CmdMediator *cmdMediator,
                                              QPointF posScreen)
{
}
  
void CentipedeStatePrebuild::handleMousePress (CmdMediator *cmdMediator,
                                               QPointF posScreen)
{
}

void CentipedeStatePrebuild::handleMouseRelease (CmdMediator *cmdMediator,
                                                 QPointF posScreen)
{
}