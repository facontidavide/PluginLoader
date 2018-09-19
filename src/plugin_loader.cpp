/*
 * Copyright (c) 2012, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "plugin_loader/plugin_loader.hpp"

#include <string>

namespace plugin_loader
{

bool PluginLoader::has_unmananged_instance_been_created_ = false;

bool PluginLoader::hasUnmanagedInstanceBeenCreated()
{
  return PluginLoader::has_unmananged_instance_been_created_;
}


PluginLoader::PluginLoader(const std::string & library_path, bool ondemand_load_unload)
: ondemand_load_unload_(ondemand_load_unload),
  library_path_(library_path),
  load_ref_count_(0),
  plugin_ref_count_(0)
{
  logDebug(
    "plugin_loader.PluginLoader: "
    "Constructing new PluginLoader (%p) bound to library %s.",
    this, library_path.c_str());
  if (!isOnDemandLoadUnloadEnabled()) {
    loadLibrary();
  }
}

PluginLoader::~PluginLoader()
{
  logDebug("%s",
    "plugin_loader.PluginLoader: "
    "Destroying class loader, unloading associated library...\n");
  unloadLibrary();  // TODO(mikaelarguedas): while(unloadLibrary() > 0){} ??
}

bool PluginLoader::isLibraryLoaded()
{
  return plugin_loader::impl::isLibraryLoaded(getLibraryPath(), this);
}

bool PluginLoader::isLibraryLoadedByAnyClassloader()
{
  return plugin_loader::impl::isLibraryLoadedByAnybody(getLibraryPath());
}

void PluginLoader::loadLibrary()
{
  std::unique_lock<std::recursive_mutex> lock(load_ref_count_mutex_);
  load_ref_count_ = load_ref_count_ + 1;
  plugin_loader::impl::loadLibrary(getLibraryPath(), this);
}

int PluginLoader::unloadLibrary()
{
  return unloadLibraryInternal(true);
}

int PluginLoader::unloadLibraryInternal(bool lock_plugin_ref_count)
{
  std::unique_lock<std::recursive_mutex> load_ref_lock(load_ref_count_mutex_);
  std::unique_lock<std::recursive_mutex> plugin_ref_lock;
  if (lock_plugin_ref_count) {
    plugin_ref_lock = std::unique_lock<std::recursive_mutex>(plugin_ref_count_mutex_);
  }

  if (plugin_ref_count_ > 0) {
    logWarn("%s",
      "plugin_loader.PluginLoader: "
      "SEVERE WARNING!!! Attempting to unload library while objects created by this loader "
      "exist in the heap! "
      "You should delete your objects before attempting to unload the library or "
      "destroying the PluginLoader. The library will NOT be unloaded.");
  } else {
    load_ref_count_ = load_ref_count_ - 1;
    if (0 == load_ref_count_) {
      plugin_loader::impl::unloadLibrary(getLibraryPath(), this);
    } else if (load_ref_count_ < 0) {
      load_ref_count_ = 0;
    }
  }
  return load_ref_count_;
}

}  // namespace plugin_loader
