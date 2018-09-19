//
// Exception.h
//
// Library: Foundation
// Package: Core
// Module:  Exception
//
// Definition of various Poco exception classes.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Exception_INCLUDED
#define Foundation_Exception_INCLUDED


#include <stdexcept>

namespace Poco {

/**
 * @class PluginLoader sException
 * @brief A base class for all plugin_loader exceptions that inherits from std::runtime_exception
 */
class PluginLoaderException : public std::runtime_error
{
public:
  explicit inline PluginLoaderException(const std::string & error_desc)
  : std::runtime_error(error_desc)
  {}
};

/**
 * @class LibraryLoadException
 * @brief An exception class thrown when plugin_loader is unable to load a runtime library
 */
class LibraryLoadException : public PluginLoaderException
{
public:
  explicit inline LibraryLoadException(const std::string & error_desc)
  : PluginLoaderException(error_desc)
  {}
};

/**
 * @class LibraryUnloadException
 * @brief An exception class thrown when plugin_loader is unable to unload a runtime library
 */
class LibraryUnloadException : public PluginLoaderException
{
public:
  explicit inline LibraryUnloadException(const std::string & error_desc)
  : PluginLoaderException(error_desc)
  {}
};

/**
 * @class CreateClassException
 * @brief An exception class thrown when plugin_loader is unable to create a plugin
 */
class CreateClassException : public PluginLoaderException
{
public:
  explicit inline CreateClassException(const std::string & error_desc)
  : PluginLoaderException(error_desc)
  {}
};

/**
 * @class NoPluginLoaderExistsException
 * @brief An exception class thrown when a multilibrary class loader does not have a PluginLoader bound to it
 */
class NoPluginLoaderExistsException : public PluginLoaderException
{
public:
  explicit inline NoPluginLoaderExistsException(const std::string & error_desc)
  : PluginLoaderException(error_desc)
  {}
};

/**
 * @class NoPluginLoaderExistsException
 * @brief An exception class thrown when a multilibrary class loader does not have a PluginLoader bound to it
 */
class NotFoundException : public PluginLoaderException
{
public:
  explicit inline NotFoundException(const std::string & error_desc)
  : PluginLoaderException(error_desc)
  {}
};



} // namespace Poco


#endif // Foundation_Exception_INCLUDED
