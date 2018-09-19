//
// MetaObject.h
//
// Library: Foundation
// Package: SharedLibrary
// Module:  ClassLoader
//
// Definition of the MetaObject class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_MetaObject_INCLUDED
#define Foundation_MetaObject_INCLUDED

#include <assert.h>
#include "POCO/Foundation.h"
#include "POCO/Exception.h"
#include <memory>


namespace Poco {


template <class B>
class AbstractMetaObject
	/// A MetaObject stores some information
	/// about a C++ class. The MetaObject class
	/// is used by the Manifest class.
	/// AbstractMetaObject is a common base class
	/// for all MetaObject in a rooted class hierarchy.
	/// A MetaObject can also be used as an object
	/// factory for its class.
{
public:
	AbstractMetaObject(const char* pName): _name(pName)
	{
        assert(_name);
	}

	virtual ~AbstractMetaObject()
	{
	}

	const char* name() const
	{
		return _name;
	}

    virtual std::unique_ptr<B> create() const = 0;
		/// Create a new instance of a class.


private:
	AbstractMetaObject();
	AbstractMetaObject(const AbstractMetaObject&);
	AbstractMetaObject& operator = (const AbstractMetaObject&);

	const char* _name;

};


template <class Class, class Base>
class MetaObject: public AbstractMetaObject<Base>
	/// A MetaObject stores some information
	/// about a C++ class. The MetaObject class
	/// is used by the Manifest class.
	/// A MetaObject can also be used as an object
	/// factory for its class.
{
public:
    MetaObject(const char* name): AbstractMetaObject<Base>(name)
	{
	}

    ~MetaObject() = default;

    std::unique_ptr<Base> create() const override
	{
        return std::unique_ptr<Base>(new Class);
	}
};


} // namespace Poco


#endif // Foundation_MetaObject_INCLUDED
