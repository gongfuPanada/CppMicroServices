/*=============================================================================

  Library: CppMicroServices

  Copyright (c) German Cancer Research Center,
    Division of Medical and Biological Informatics

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=============================================================================*/

#include <usConfig.h>

#include "usModuleContext.h"

#include "usModuleRegistry.h"
#include "usModulePrivate.h"
#include "usCoreModuleContext_p.h"
#include "usServiceRegistry_p.h"
#include "usServiceReferenceBasePrivate.h"

US_BEGIN_NAMESPACE

class ModuleContextPrivate {

public:

  ModuleContextPrivate(ModulePrivate* module)
  : module(module)
  {}

  ModulePrivate* module;
};


ModuleContext::ModuleContext(ModulePrivate* module)
  : d(new ModuleContextPrivate(module))
{}

ModuleContext::~ModuleContext()
{
  delete d;
}

Module* ModuleContext::GetModule() const
{
  return d->module->q;
}

Module* ModuleContext::GetModule(long id) const
{
  return ModuleRegistry::GetModule(id);
}

void ModuleContext::GetModules(std::vector<Module*>& modules) const
{
  ModuleRegistry::GetModules(modules);
}

ServiceRegistrationU ModuleContext::RegisterService(const InterfaceMap& service,
                                                    const ServiceProperties& properties)
{
  return d->module->coreCtx->services.RegisterService(d->module, service, properties);
}

std::vector<ServiceReferenceU > ModuleContext::GetServiceReferences(const std::string& clazz,
                                                                    const std::string& filter)
{
  std::vector<ServiceReferenceU> result;
  std::vector<ServiceReferenceBase> refs;
  d->module->coreCtx->services.Get(clazz, filter, d->module, refs);
  for (std::vector<ServiceReferenceBase>::const_iterator iter = refs.begin();
       iter != refs.end(); ++iter)
  {
    result.push_back(ServiceReferenceU(*iter));
  }
  return result;
}

ServiceReferenceU ModuleContext::GetServiceReference(const std::string& clazz)
{
  return d->module->coreCtx->services.Get(d->module, clazz);
}

void* ModuleContext::GetService(const ServiceReferenceBase& reference)
{
  if (!reference)
  {
    throw std::invalid_argument("Default constructed ServiceReference is not a valid input to getService()");
  }
  //ServiceReference internalRef(reference);
  return reference.d->GetService(d->module->q);
}

bool ModuleContext::UngetService(const ServiceReferenceBase& reference)
{
  ServiceReferenceBase ref = reference;
  return ref.d->UngetService(d->module->q, true);
}

void ModuleContext::AddServiceListener(const ServiceListener& delegate,
                                       const std::string& filter)
{
  d->module->coreCtx->listeners.AddServiceListener(this, delegate, NULL, filter);
}

void ModuleContext::RemoveServiceListener(const ServiceListener& delegate)
{
  d->module->coreCtx->listeners.RemoveServiceListener(this, delegate, NULL);
}

void ModuleContext::AddModuleListener(const ModuleListener& delegate)
{
  d->module->coreCtx->listeners.AddModuleListener(this, delegate, NULL);
}

void ModuleContext::RemoveModuleListener(const ModuleListener& delegate)
{
  d->module->coreCtx->listeners.RemoveModuleListener(this, delegate, NULL);
}

void ModuleContext::AddServiceListener(const ServiceListener& delegate, void* data,
                                       const std::string &filter)
{
  d->module->coreCtx->listeners.AddServiceListener(this, delegate, data, filter);
}

void ModuleContext::RemoveServiceListener(const ServiceListener& delegate, void* data)
{
  d->module->coreCtx->listeners.RemoveServiceListener(this, delegate, data);
}

void ModuleContext::AddModuleListener(const ModuleListener& delegate, void* data)
{
  d->module->coreCtx->listeners.AddModuleListener(this, delegate, data);
}

void ModuleContext::RemoveModuleListener(const ModuleListener& delegate, void* data)
{
  d->module->coreCtx->listeners.RemoveModuleListener(this, delegate, data);
}

US_END_NAMESPACE
