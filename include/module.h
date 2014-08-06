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

#include "provider.h"
#include "injeqt-global.h"
#include "type.h"

#include <memory>

namespace injeqt { namespace v1 {

class INJEQT_API module
{

public:
	virtual ~module() {}

	template<typename T>
	void add_ready_object(QObject *object)
	{
		add_ready_object(make_type<T>(), object);
	}


	template<typename T>
	void add_type()
	{
		add_type(make_type<T>());
	}

	std::vector<std::unique_ptr<provider>> & providers();

private:
	std::vector<std::unique_ptr<provider>> _providers;

	void add_ready_object(const type &t, QObject *object);
	void add_type(const type &t);
	void add_provider(std::unique_ptr<provider> c);

};

}};
