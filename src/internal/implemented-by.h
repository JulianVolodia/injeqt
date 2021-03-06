/*
 * %injeqt copyright begin%
 * Copyright 2014 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * %injeqt copyright end%
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#pragma once

#include <injeqt/injeqt.h>
#include <injeqt/type.h>

#include "internal.h"

/**
 * @file
 * @brief Contains classes and functions for representing "interface is implemented by type" relations.
 */

namespace injeqt { namespace internal {

/**
 * @brief Represents "interface is implemented by type" relations.
 *
 * In Injeqt dependencies between objects are described by types. When an object of some type
 * T is added to injector using module class then it can be passed as a dependency of type
 * T or any of T supertypes - excluding only supertypes common with other configured objects.
 * This class represens such relation. It is mostly used in implemnted_by_mapping set to
 * represents all types that injector knows and can handle.
 */
class INJEQT_INTERNAL_API implemented_by final
{

public:
	/**
	 * @brief Create new instance of implemented_by.
	 * @param interface_type type of available interface
	 * @param implementation_type type of object implementing interface
	 * @pre !interface_type.is_empty()
	 * @pre !implementation_type.is_empty()
	 * @pre implements(implementation_type, interface_type)
	 */
	explicit implemented_by(type interface_type, type implementation_type);

	/**
	 * @return type of available interface.
	 */
	type interface_type() const;

	/**
	 * @return type of object implementing interface.
	 */
	type implementation_type() const;

private:
	type _interface_type;
	type _implementation_type;

};

INJEQT_INTERNAL_API bool operator == (const implemented_by &x, const implemented_by &y);
INJEQT_INTERNAL_API bool operator != (const implemented_by &x, const implemented_by &y);

}}
