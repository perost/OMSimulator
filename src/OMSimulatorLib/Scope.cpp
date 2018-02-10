/*
 * This file is part of OpenModelica.
 *
 * Copyright (c) 1998-CurrentYear, Open Source Modelica Consortium (OSMC),
 * c/o Linköpings universitet, Department of Computer and Information Science,
 * SE-58183 Linköping, Sweden.
 *
 * All rights reserved.
 *
 * THIS PROGRAM IS PROVIDED UNDER THE TERMS OF GPL VERSION 3 LICENSE OR
 * THIS OSMC PUBLIC LICENSE (OSMC-PL) VERSION 1.2.
 * ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS PROGRAM CONSTITUTES
 * RECIPIENT'S ACCEPTANCE OF THE OSMC PUBLIC LICENSE OR THE GPL VERSION 3,
 * ACCORDING TO RECIPIENTS CHOICE.
 *
 * The OpenModelica software and the Open Source Modelica
 * Consortium (OSMC) Public License (OSMC-PL) are obtained
 * from OSMC, either from the above address,
 * from the URLs: http://www.ida.liu.se/projects/OpenModelica or
 * http://www.openmodelica.org, and in the OpenModelica distribution.
 * GNU version 3 is obtained from: http://www.gnu.org/copyleft/gpl.html.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without
 * even the implied warranty of  MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE, EXCEPT AS EXPRESSLY SET FORTH
 * IN THE BY RECIPIENT SELECTED SUBSIDIARY LICENSE CONDITIONS OF OSMC-PL.
 *
 * See the full OSMC Public License conditions for more details.
 *
 */

#include "FMICompositeModel.h"
#include "Logging.h"
#include "Scope.h"
#include "TLMCompositeModel.h"
#include "Types.h"

#include <boost/filesystem.hpp>

oms2::Scope::Scope()
{
  boost::filesystem::path tempPath = boost::filesystem::temp_directory_path();
  tempDir = tempPath.string();
}

oms2::Scope::~Scope()
{
  // free memory if no one else did it
  for (auto it=models.begin(); it != models.end(); it++)
    oms2::Model::deleteModel(it->second);
}

oms2::Scope& oms2::Scope::getInstance()
{
  // the only instance
  static Scope scope;
  return scope;
}

oms_status_t oms2::Scope::newFMIModel(const std::string& name)
{
  Scope& scope = oms2::Scope::getInstance();

  // check if name is in scope
  auto it = scope.models.find(name);
  if (it != scope.models.end())
  {
    logError("A model called \"" + name + "\" is already in the scope.");
    return oms_status_error;
  }

  Model* model = oms2::FMICompositeModel::newModel(name);
  if (!model)
    return oms_status_error;

  scope.models[name] = model;
  return oms_status_ok;
}

oms_status_t oms2::Scope::newTLMModel(const std::string& name)
{
  Scope& scope = oms2::Scope::getInstance();

  // check if name is in scope
  auto it = scope.models.find(name);
  if (it != scope.models.end())
  {
    logError("A model called \"" + name + "\" is already in the scope.");
    return oms_status_error;
  }

  Model* model = oms2::TLMCompositeModel::newModel(name);
  if (!model)
    return oms_status_error;

  scope.models[name] = model;
  return oms_status_ok;
}

oms_status_t oms2::Scope::unloadModel(const std::string& name)
{
  Scope& scope = oms2::Scope::getInstance();

  // check if name is in scope
  auto it = scope.models.find(name);
  if (it == scope.models.end())
  {
    logError("There is no model called \"" + name + "\" in the scope.");
    return oms_status_error;
  }

  oms2::Model::deleteModel(it->second);
  scope.models.erase(it);

  return oms_status_ok;
}

oms_status_t oms2::Scope::renameModel(const std::string& identOld, const std::string& identNew)
{
  Scope& scope = oms2::Scope::getInstance();

  // check if identNew is in scope
  auto it = scope.models.find(identNew);
  if (it != scope.models.end())
  {
    logError("A model called \"" + identNew + "\" is already in the scope.");
    return oms_status_error;
  }

  // check if identOld is in scope
  if (it == scope.models.end())
  {
    logError("There is no model called \"" + identOld + "\" in the scope.");
    return oms_status_error;
  }

  it->second->setName(identNew);
  scope.models[identNew] = it->second;
  scope.models.erase(it);

  return oms_status_ok;
}

oms2::Model* oms2::Scope::getModel(const std::string& name)
{
  Scope& scope = oms2::Scope::getInstance();

  auto it = scope.models.find(name);
  if (it == scope.models.end())
  {
    logWarning("oms2::Scope::getModel: There is no model called \"" + name + "\" in the scope.");
    return NULL;
  }

  return it->second;
}

oms_status_t oms2::Scope::SetTempDirectory(const std::string& newTempDir)
{
  Scope& scope = oms2::Scope::getInstance();

  if (!boost::filesystem::is_directory(newTempDir))
  {
    if (!boost::filesystem::create_directory(newTempDir))
    {
      logError("changing working directory to \"" + std::string(newTempDir) + "\" failed");
      return oms_status_error;
    }
    else
      logInfo("new temp directory has been created: \"" + std::string(newTempDir) + "\"");
  }

  boost::filesystem::path path(newTempDir.c_str());
  path = boost::filesystem::canonical(path);
  scope.tempDir = path.string();

  logInfo("new temp directory: \"" + std::string(scope.tempDir) + "\"");
  return oms_status_ok;
}