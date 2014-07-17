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

#include "sorted-unique-vector.h"

#include <QtTest/QtTest>

using namespace injeqt::v1;

class sorted_unique_vector_test : public QObject
{
	Q_OBJECT

	static int extract_key(const int &v)
	{
		return v;
	}

	using suv_int = sorted_unique_vector<int, int, extract_key>;

private slots:
	void should_be_empty_after_default_construction();
	void should_be_empty_after_clear();
	void should_be_valid_after_conversion_from_unique_vector();
	void should_be_valid_after_conversion_from_non_unique_vector();
	void should_be_valid_after_conversion_from_unique_sorted_vector();
	void should_be_valid_after_conversion_from_non_unique_sorted_vector();
	void should_be_valid_after_adding_less_than_smallest_element();
	void should_be_valid_after_adding_smallest_element();
	void should_be_valid_after_adding_medium_element();
	void should_be_valid_after_adding_largest_element();
	void should_be_valid_after_adding_greater_than_largest_element();
	void should_be_valid_after_merging_itself();
	void should_be_valid_after_merging_lesser_elements();
	void should_be_valid_after_merging_lesser_or_equal_elements();
	void should_be_valid_after_merging_misc_elements();
	void should_be_valid_after_merging_misc_unique_elements();
	void should_be_valid_after_merging_greater_or_equal_elements();
	void should_be_valid_after_merging_greater_elements();

};

void sorted_unique_vector_test::should_be_empty_after_default_construction()
{
	auto data = suv_int{};

	QVERIFY(data.empty());
	QCOMPARE(data.size(), 0ul);
	QCOMPARE(data.content(), (std::vector<int>{}));
}

void sorted_unique_vector_test::should_be_empty_after_clear()
{
	auto data = suv_int{std::vector<int>{1, 4, 5, 2}};

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 4ul);

	data.clear();

	QVERIFY(data.empty());
	QCOMPARE(data.size(), 0ul);
}

void sorted_unique_vector_test::should_be_valid_after_conversion_from_unique_vector()
{
	auto data = suv_int{std::vector<int>{1, 4, 5, 2}};

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 4ul);
	QCOMPARE(data.content(), (std::vector<int>{1, 2, 4, 5}));
}

void sorted_unique_vector_test::should_be_valid_after_conversion_from_non_unique_vector()
{
	auto data = suv_int{std::vector<int>{1, 4, 5, 2, 1, 4, 5, 2}};

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 4ul);
	QCOMPARE(data.content(), (std::vector<int>{1, 2, 4, 5}));
}

void sorted_unique_vector_test::should_be_valid_after_conversion_from_unique_sorted_vector()
{
	auto data = suv_int{std::vector<int>{1, 2, 4, 5}};

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 4ul);
	QCOMPARE(data.content(), (std::vector<int>{1, 2, 4, 5}));
}

void sorted_unique_vector_test::should_be_valid_after_conversion_from_non_unique_sorted_vector()
{
	auto data = suv_int{std::vector<int>{1, 1, 2, 2, 4, 4, 5, 5}};

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 4ul);
	QCOMPARE(data.content(), (std::vector<int>{1, 2, 4, 5}));
}

void sorted_unique_vector_test::should_be_valid_after_adding_less_than_smallest_element()
{
	auto data = suv_int{std::vector<int>{1, 2, 4, 5}};
	data.add(0);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 5ul);
	QCOMPARE(data.content(), (std::vector<int>{0, 1, 2, 4, 5}));
}

void sorted_unique_vector_test::should_be_valid_after_adding_smallest_element()
{
	auto data = suv_int{std::vector<int>{1, 2, 4, 5}};
	data.add(1);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 4ul);
	QCOMPARE(data.content(), (std::vector<int>{1, 2, 4, 5}));
}

void sorted_unique_vector_test::should_be_valid_after_adding_medium_element()
{
	auto data = suv_int{std::vector<int>{1, 2, 4, 5}};
	data.add(3);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 5ul);
	QCOMPARE(data.content(), (std::vector<int>{1, 2, 3, 4, 5}));
}

void sorted_unique_vector_test::should_be_valid_after_adding_largest_element()
{
	auto data = suv_int{std::vector<int>{1, 2, 4, 5}};
	data.add(5);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 4ul);
	QCOMPARE(data.content(), (std::vector<int>{1, 2, 4, 5}));
}

void sorted_unique_vector_test::should_be_valid_after_adding_greater_than_largest_element()
{
	auto data = suv_int{std::vector<int>{1, 2, 4, 5}};
	data.add(6);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 5ul);
	QCOMPARE(data.content(), (std::vector<int>{1, 2, 4, 5, 6}));
}

void sorted_unique_vector_test::should_be_valid_after_merging_itself()
{
	auto data = suv_int{std::vector<int>{1, 2, 4, 5}};
	data.merge(data);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 4ul);
	QCOMPARE(data.content(), (std::vector<int>{1, 2, 4, 5}));
}

void sorted_unique_vector_test::should_be_valid_after_merging_lesser_elements()
{
	auto data = suv_int{std::vector<int>{1, 2, 4, 5}};
	auto data_to_add = suv_int{std::vector<int>{0, -1, -2}};
	data.merge(data_to_add);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 7ul);
	QCOMPARE(data.content(), (std::vector<int>{-2, -1, 0, 1, 2, 4, 5}));
}

void sorted_unique_vector_test::should_be_valid_after_merging_lesser_or_equal_elements()
{
	auto data = suv_int{std::vector<int>{1, 2, 4, 5}};
	auto data_to_add = suv_int{std::vector<int>{1, 0, -1, -2}};
	data.merge(data_to_add);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 7ul);
	QCOMPARE(data.content(), (std::vector<int>{-2, -1, 0, 1, 2, 4, 5}));
}

void sorted_unique_vector_test::should_be_valid_after_merging_misc_elements()
{
	auto data = suv_int{std::vector<int>{0, 5, 10, 1, 3, 2}};
	auto data_to_add = suv_int{std::vector<int>{-1, 17, 2, 3, 4, 5}};
	data.merge(data_to_add);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 9ul);
	QCOMPARE(data.content(), (std::vector<int>{-1, 0, 1, 2, 3, 4, 5, 10, 17}));
}

void sorted_unique_vector_test::should_be_valid_after_merging_misc_unique_elements()
{
	auto data = suv_int{std::vector<int>{0, 5, 10, 1, 3, 2}};
	auto data_to_add = suv_int{std::vector<int>{-1, 17, 4}};
	data.merge(data_to_add);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 9ul);
	QCOMPARE(data.content(), (std::vector<int>{-1, 0, 1, 2, 3, 4, 5, 10, 17}));
}

void sorted_unique_vector_test::should_be_valid_after_merging_greater_or_equal_elements()
{
	auto data = suv_int{std::vector<int>{1, 2, 4, 5}};
	auto data_to_add = suv_int{std::vector<int>{7, 8, 6}};
	data.merge(data_to_add);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 7ul);
	QCOMPARE(data.content(), (std::vector<int>{1, 2, 4, 5, 6, 7, 8}));
}

void sorted_unique_vector_test::should_be_valid_after_merging_greater_elements()
{
	auto data = suv_int{std::vector<int>{1, 2, 4, 5}};
	auto data_to_add = suv_int{std::vector<int>{7, 5, 6}};
	data.merge(data_to_add);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 6ul);
	QCOMPARE(data.content(), (std::vector<int>{1, 2, 4, 5, 6, 7}));
}

QTEST_APPLESS_MAIN(sorted_unique_vector_test)
#include "sorted-unique-vector-test.moc"