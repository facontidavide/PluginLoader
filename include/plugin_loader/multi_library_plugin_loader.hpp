/*
 * Software License Agreement (BSD License)
 *
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
 *     * Neither the name of the copyright holders nor the names of its
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

#ifndef PLUGIN_LOADER_MULTI_LIBRARY_plugin_loader_HPP_
#define PLUGIN_LOADER_MULTI_LIBRARY_plugin_loader_HPP_

#include <mutex>
#include <cstddef>
#include <map>
#include <string>
#include <vector>

#include "plugin_loader/plugin_loader.hpp"
#include "plugin_loader/visibility_control.hpp"

namespace plugin_loader
{

typedef std::string LibraryPath;
typedef std::map<LibraryPath, plugin_loader::PluginLoader *> LibraryToPluginLoaderMap;
typedef std::vector<PluginLoader *> PluginLoaderVector;

/**
* @class MultiLibraryPluginLoader
* @brief A PluginLoader that can bind more than one runtime library
*/
class PLUGIN_LOADER_PUBLIC MultiLibraryPluginLoader
{
public:
  /**
   * @brief Constructor for the class
   * @param enable_ondemand_loadunload - Flag indicates if classes are to be loaded/unloaded automatically as plugin_loader are created and destroyed
   */
  explicit MultiLibraryPluginLoader(bool enable_ondemand_loadunload);

  /**
  * @brief Virtual destructor for class
  */
  virtual ~MultiLibraryPluginLoader();

  /**
   * @brief Creates an instance of an object of given class name with ancestor class Base
   * This version does not look in a specific library for the factory, but rather the first open library that defines the classs
   * @param Base - polymorphic type indicating base class
   * @param class_name - the name of the concrete plugin class we want to instantiate
   * @return A std::shared_ptr<Base> to newly created plugin
   */
  template<class Base>
  std::shared_ptr<Base> createSharedInstance(const std::string & class_name)
  {
    logDebug(
      "plugin_loader::MultiLibraryPluginLoader: "
      "Attempting to create instance of class type %s.",
      class_name.c_str());
    PluginLoader * loader = getPluginLoaderForClass<Base>(class_name);
    if (nullptr == loader) {
      throw plugin_loader::CreateClassException(
              "MultiLibraryPluginLoader: Could not create object of class type " +
              class_name +
              " as no factory exists for it. Make sure that the library exists and "
              "was explicitly loaded through MultiLibraryPluginLoader::loadLibrary()");
    }

    return loader->createSharedInstance<Base>(class_name);
  }

  /**
   * @brief Creates an instance of an object of given class name with ancestor class Base
   * This version takes a specific library to make explicit the factory being used
   * @param Base - polymorphic type indicating base class
   * @param class_name - the name of the concrete plugin class we want to instantiate
   * @param library_path - the library from which we want to create the plugin
   * @return A std::shared_ptr<Base> to newly created plugin
   */
  template<class Base>
  std::shared_ptr<Base>
  createSharedInstance(const std::string & class_name, const std::string & library_path)
  {
    PluginLoader * loader = getPluginLoaderForLibrary(library_path);
    if (nullptr == loader) {
      throw plugin_loader::NoPluginLoaderExistsException(
              "Could not create instance as there is no PluginLoader in "
              "MultiLibraryPluginLoader bound to library " + library_path +
              " Ensure you called MultiLibraryPluginLoader::loadLibrary()");
    }
    return loader->createSharedInstance<Base>(class_name);
  }

  /**
   * @brief Creates an instance of an object of given class name with ancestor class Base
   * Same as createSharedInstance() except it returns a std::shared_ptr.
   */
  template<class Base>
  std::shared_ptr<Base> createInstance(const std::string & class_name)
  {
    logDebug(
      "plugin_loader::MultiLibraryPluginLoader: "
      "Attempting to create instance of class type %s.",
      class_name.c_str());
    PluginLoader * loader = getPluginLoaderForClass<Base>(class_name);
    if (nullptr == loader) {
      throw plugin_loader::CreateClassException(
              "MultiLibraryPluginLoader: Could not create object of class type " +
              class_name +
              " as no factory exists for it. Make sure that the library exists and "
              "was explicitly loaded through MultiLibraryPluginLoader::loadLibrary()");
    }

    return loader->createInstance<Base>(class_name);
  }

  /**
   * @brief Creates an instance of an object of given class name with ancestor class Base
   * Same as createSharedInstance() except it returns a std::shared_ptr.
   */
  template<class Base>
  std::shared_ptr<Base>
  createInstance(const std::string & class_name, const std::string & library_path)
  {
    PluginLoader * loader = getPluginLoaderForLibrary(library_path);
    if (nullptr == loader) {
      throw plugin_loader::NoPluginLoaderExistsException(
              "Could not create instance as there is no PluginLoader in "
              "MultiLibraryPluginLoader bound to library " + library_path +
              " Ensure you called MultiLibraryPluginLoader::loadLibrary()");
    }
    return loader->createInstance<Base>(class_name);
  }

  /**
   * @brief Creates an instance of an object of given class name with ancestor class Base
   * Same as createSharedInstance() except it returns a std::unique_ptr.
   */
  template<class Base>
  PluginLoader::UniquePtr<Base> createUniqueInstance(const std::string & class_name)
  {
    logDebug(
      "plugin_loader::MultiLibraryPluginLoader: Attempting to create instance of class type %s.",
      class_name.c_str());
    PluginLoader * loader = getPluginLoaderForClass<Base>(class_name);
    if (nullptr == loader) {
      throw plugin_loader::CreateClassException(
              "MultiLibraryPluginLoader: Could not create object of class type " + class_name +
              " as no factory exists for it. "
              "Make sure that the library exists and was explicitly loaded through "
              "MultiLibraryPluginLoader::loadLibrary()");
    }
    return loader->createUniqueInstance<Base>(class_name);
  }

  /**
   * @brief Creates an instance of an object of given class name with ancestor class Base
   * Same as createSharedInstance() except it returns a std::unique_ptr.
   */
  template<class Base>
  PluginLoader::UniquePtr<Base>
  createUniqueInstance(const std::string & class_name, const std::string & library_path)
  {
    PluginLoader * loader = getPluginLoaderForLibrary(library_path);
    if (nullptr == loader) {
      throw plugin_loader::NoPluginLoaderExistsException(
              "Could not create instance as there is no PluginLoader in "
              "MultiLibraryPluginLoader bound to library " + library_path +
              " Ensure you called MultiLibraryPluginLoader::loadLibrary()");
    }
    return loader->createUniqueInstance<Base>(class_name);
  }

  /**
   * @brief Creates an instance of an object of given class name with ancestor class Base
   * This version does not look in a specific library for the factory, but rather the first open library that defines the classs
   * This version should not be used as the plugin system cannot do automated safe loading/unloadings
   * @param Base - polymorphic type indicating base class
   * @param class_name - the name of the concrete plugin class we want to instantiate
   * @return An unmanaged Base* to newly created plugin
   */
  template<class Base>
  Base * createUnmanagedInstance(const std::string & class_name)
  {
    PluginLoader * loader = getPluginLoaderForClass<Base>(class_name);
    if (nullptr == loader) {
      throw plugin_loader::CreateClassException(
              "MultiLibraryPluginLoader: Could not create class of type " + class_name);
    }
    return loader->createUnmanagedInstance<Base>(class_name);
  }

  /**
   * @brief Creates an instance of an object of given class name with ancestor class Base
   * This version takes a specific library to make explicit the factory being used
   * This version should not be used as the plugin system cannot do automated safe loading/unloadings
   * @param Base - polymorphic type indicating Base class
   * @param class_name - name of class for which we want to create instance
   * @param library_path - the fully qualified path to the runtime library
   */
  template<class Base>
  Base * createUnmanagedInstance(const std::string & class_name, const std::string & library_path)
  {
    PluginLoader * loader = getPluginLoaderForLibrary(library_path);
    if (nullptr == loader) {
      throw plugin_loader::NoPluginLoaderExistsException(
              "Could not create instance as there is no PluginLoader in MultiLibraryPluginLoader "
              "bound to library " + library_path +
              " Ensure you called MultiLibraryPluginLoader::loadLibrary()");
    }
    return loader->createUnmanagedInstance<Base>(class_name);
  }

  /**
   * @brief Indicates if a class has been loaded and can be instantiated
   * @param Base - polymorphic type indicating Base class
   * @param class_name - name of class that is be inquired about
   * @return true if loaded, false otherwise
   */
  template<class Base>
  bool isClassAvailable(const std::string & class_name)
  {
    std::vector<std::string> available_classes = getAvailableClasses<Base>();
    return available_classes.end() != std::find(
      available_classes.begin(), available_classes.end(), class_name);
  }

  /**
   * @brief Indicates if a library has been loaded into memory
   * @param library_path - The full qualified path to the runtime library
   * @return true if library is loaded, false otherwise
   */
  bool isLibraryAvailable(const std::string & library_path);

  /**
   * @brief Gets a list of all classes that are loaded by the class loader
   * @param Base - polymorphic type indicating Base class
   * @return A vector<string> of the available classes
   */
  template<class Base>
  std::vector<std::string> getAvailableClasses()
  {
    std::vector<std::string> available_classes;
    for (auto & loader : getAllAvailablePluginLoaders()) {
      std::vector<std::string> loader_classes = loader->getAvailableClasses<Base>();
      available_classes.insert(
        available_classes.end(), loader_classes.begin(), loader_classes.end());
    }
    return available_classes;
  }

  /**
   * @brief Gets a list of all classes loaded for a particular library
   * @param Base - polymorphic type indicating Base class
   * @return A vector<string> of the available classes in the passed library
   */
  template<class Base>
  std::vector<std::string> getAvailableClassesForLibrary(const std::string & library_path)
  {
    PluginLoader * loader = getPluginLoaderForLibrary(library_path);
    if (nullptr == loader) {
      throw plugin_loader::NoPluginLoaderExistsException(
              "There is no PluginLoader in MultiLibraryPluginLoader bound to library " +
              library_path +
              " Ensure you called MultiLibraryPluginLoader::loadLibrary()");
    }
    return loader->getAvailableClasses<Base>();
  }

  /**
   * @brief Gets a list of all libraries opened by this class loader
   @ @return A list of libraries opened by this class loader
   */
  std::vector<std::string> getRegisteredLibraries();

  /**
   * @brief Loads a library into memory for this class loader
   * @param library_path - the fully qualified path to the runtime library
   */
  void loadLibrary(const std::string & library_path);

  /**
   * @brief Unloads a library for this class loader
   * @param library_path - the fully qualified path to the runtime library
   */
  int unloadLibrary(const std::string & library_path);

private:
  /**
   * @brief Indicates if on-demand (lazy) load/unload is enabled so libraries are loaded/unloaded automatically as needed
   */
  bool isOnDemandLoadUnloadEnabled() {return enable_ondemand_loadunload_;}

  /**
   * @brief Gets a handle to the class loader corresponding to a specific runtime library
   * @param library_path - the library from which we want to create the plugin
   * @return A pointer to the PluginLoader*, == nullptr if not found
   */
  PluginLoader * getPluginLoaderForLibrary(const std::string & library_path);

  /**
   * @brief Gets a handle to the class loader corresponding to a specific class
   * @param class_name - name of class for which we want to create instance
   * @return A pointer to the PluginLoader*, == nullptr if not found
   */
  template<typename Base>
  PluginLoader * getPluginLoaderForClass(const std::string & class_name)
  {
    PluginLoaderVector loaders = getAllAvailablePluginLoaders();
    for (PluginLoaderVector::iterator i = loaders.begin(); i != loaders.end(); ++i) {
      if (!(*i)->isLibraryLoaded()) {
        (*i)->loadLibrary();
      }
      if ((*i)->isClassAvailable<Base>(class_name)) {
        return *i;
      }
    }
    return nullptr;
  }

  /**
   * @brief Gets all class loaders loaded within scope
   */
  PluginLoaderVector getAllAvailablePluginLoaders();

  /**
   * @brief Destroys all PluginLoaders
   */
  void shutdownAllPluginLoaders();

private:
  bool enable_ondemand_loadunload_;
  LibraryToPluginLoaderMap active_plugin_loaders_;
  std::mutex loader_mutex_;
};


}  // namespace plugin_loader
#endif  // PLUGIN_LOADER_MULTI_LIBRARY_plugin_loader_HPP_
