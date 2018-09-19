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

#ifndef plugin_loader__EXCEPTIONS_HPP_
#define plugin_loader__EXCEPTIONS_HPP_

#include <stdexcept>
#include <string>

namespace plugin_loader
{

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

}  // namespace plugin_loader
#endif  // plugin_loader__EXCEPTIONS_HPP_
