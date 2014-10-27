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

#include "exception/ambiguous-types.cpp"
#include "exception/default-constructor-not-found.cpp"
#include "exception/dependency-duplicated.cpp"
#include "exception/dependency-on-self.cpp"
#include "exception/dependency-on-subtype.cpp"
#include "exception/dependency-on-supertype.cpp"
#include "exception/empty-type.cpp"
#include "exception/exception.cpp"
#include "exception/instantiation-failed.cpp"
#include "exception/interface-not-implemented.cpp"
#include "exception/invalid-dependency.cpp"
#include "exception/invalid-qobject.cpp"
#include "exception/invalid-setter.cpp"
#include "exception/qobject-type.cpp"
#include "exception/unavailable-required-types.cpp"
#include "exception/unique-factory-method-not-found.cpp"
#include "exception/unknown-type.cpp"
#include "exception/unresolvable-dependencies.cpp"
#include "default-constructor-method.cpp"
#include "dependencies.cpp"
#include "dependency.cpp"
#include "factory-method.cpp"
#include "implementation.cpp"
#include "implemented-by.cpp"
#include "injector-core.cpp"
#include "interfaces-utils.cpp"
#include "module.cpp"
#include "module-impl.cpp"
#include "resolve-dependencies.cpp"
#include "required-to-instantiate.cpp"
#include "resolved-dependency.cpp"
#include "provider-by-default-constructor.cpp"
#include "provider-by-factory.cpp"
#include "provider-ready.cpp"
#include "setter-method.cpp"
#include "type.cpp"
#include "type-dependencies.cpp"
#include "type-relations.cpp"
#include "types-model.cpp"

#include "../mocks/mocked-provider.h"
#include "expect.h"
#include "utils.h"

#include <QtTest/QtTest>
#include <string>

using namespace injeqt::internal;
using namespace injeqt::v1;

class not_default_constructible_type : public QObject
{
	Q_OBJECT
};

class not_default_constructible_type_subtype : public not_default_constructible_type
{
	Q_OBJECT
};

class default_constructible_type : public QObject
{
	Q_OBJECT
public:
	Q_INVOKABLE default_constructible_type() {}
};

class factory_type : public QObject
{
	Q_OBJECT
public:
	Q_INVOKABLE not_default_constructible_type * create() const { return nullptr; }
};

class factory_subtype_type : public QObject
{
	Q_OBJECT
public:
	Q_INVOKABLE not_default_constructible_type_subtype * create() const { return nullptr; }
};

class factory_double_type : public QObject
{
	Q_OBJECT
public:
	Q_INVOKABLE not_default_constructible_type * create1() const { return nullptr; }
	Q_INVOKABLE not_default_constructible_type * create2() const { return nullptr; }
};

class module_test : public QObject
{
	Q_OBJECT

private slots:
	void should_accept_empty_module();
	void should_not_accept_null_ready_object();
	void should_not_accept_qobject_ready_object();
	void should_accept_valid_ready_object();
	void should_accept_subtype_ready_object();
	void should_not_accept_supertype_ready_object();
	void should_not_accept_qobject_type();
	void should_not_accept_not_default_constructible_type();
	void should_accept_default_constructible_type();
	void should_not_accept_qobject_factory_type();
	void should_not_accept_qobject_created_type();
	void should_accept_valid_factory_type();
	void should_accept_supertype_factory_type();
	void should_not_accept_subtype_factory_type();
	void should_not_accept_double_factory_type();

};

void module_test::should_accept_empty_module()
{
	class test_module : public module { };
	test_module{};
}

void module_test::should_not_accept_null_ready_object()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			add_ready_object<not_default_constructible_type>(nullptr);
		}
	};

	expect<exception::invalid_qobject>({"not_default_constructible_type"}, [&](){
		test_module{};
	});
}

void module_test::should_not_accept_qobject_ready_object()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			not_default_constructible_type object;
			add_ready_object<QObject>(&object);
		}
	};

	expect<exception::qobject_type>([&](){
		test_module{};
	});
}

void module_test::should_accept_valid_ready_object()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			not_default_constructible_type object;
			add_ready_object<not_default_constructible_type>(&object);
		}
	};
	test_module{};
}

void module_test::should_accept_subtype_ready_object()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			not_default_constructible_type_subtype object;
			add_ready_object<not_default_constructible_type>(&object);
		}
	};
	test_module{};
}

void module_test::should_not_accept_supertype_ready_object()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			not_default_constructible_type object;
			add_ready_object<not_default_constructible_type_subtype>(&object);
		}
	};

	expect<exception::interface_not_implemented>({"not_default_constructible_type", "not_default_constructible_type_subtype"}, [&](){
		test_module{};
	});
}

void module_test::should_not_accept_qobject_type()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			add_type<QObject>();
		}
	};

	expect<exception::qobject_type>([&](){
		test_module{};
	});
}

void module_test::should_not_accept_not_default_constructible_type()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			add_type<not_default_constructible_type>();
		}
	};

	expect<exception::default_constructor_not_found>({"not_default_constructible_type"}, [&](){
		test_module{};
	});
}

void module_test::should_accept_default_constructible_type()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			add_type<default_constructible_type>();
		}
	};
	test_module{};
}

void module_test::should_not_accept_qobject_factory_type()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			add_factory<not_default_constructible_type, QObject>();
		}
	};

	expect<exception::qobject_type>([&](){
		test_module{};
	});
}

void module_test::should_not_accept_qobject_created_type()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			add_factory<QObject, factory_type>();
		}
	};

	expect<exception::qobject_type>([&](){
		test_module{};
	});
}

void module_test::should_accept_valid_factory_type()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			add_factory<not_default_constructible_type, factory_type>();
			add_factory<not_default_constructible_type_subtype, factory_subtype_type>();
		}
	};
	test_module{};
}

void module_test::should_not_accept_subtype_factory_type()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			add_factory<not_default_constructible_type_subtype, factory_type>();
		}
	};

	expect<exception::unique_factory_method_not_found>({"not_default_constructible_type_subtype"}, [&](){
		test_module{};
	});
}

void module_test::should_accept_supertype_factory_type()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			add_factory<not_default_constructible_type, factory_subtype_type>();
		}
	};

	test_module{};
}

void module_test::should_not_accept_double_factory_type()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			add_factory<not_default_constructible_type, factory_double_type>();
		}
	};

	expect<exception::unique_factory_method_not_found>({"not_default_constructible_type"}, [&](){
		test_module{};
	});
}

QTEST_APPLESS_MAIN(module_test)
#include "module-test.moc"