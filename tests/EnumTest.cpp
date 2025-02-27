/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>

#include "../Enum.h"

MY_ENUM(Fruits, uint8_t, ((apple, 1), (banana, 3), pear));

TEST(EnumTest, defaultDeath) {
  Fruits fruit{};
  // We must initialize Fruits to a valid explicit value; the default (=0) would
  // not work here.
  EXPECT_DEATH(toString(fruit), "");
  EXPECT_DEATH(toPretty(fruit), "");
}

TEST(EnumTest, toString) {
  Fruits fruit = Fruits::apple;
  EXPECT_EQ(toString(fruit), "apple");
  EXPECT_EQ(toPretty(fruit), "apple (=1)");
  MY_ENUM_STRING_VIEW appleView = toStringView(fruit);
  EXPECT_EQ(appleView, "apple");

  Fruits fruit2 = Fruits::banana;
  EXPECT_EQ(toString(fruit2), "banana");
  EXPECT_EQ(toPretty(fruit2), "banana (=3)");

  fruit2 = Fruits::pear;
  EXPECT_EQ(toString(fruit2), "pear");
  EXPECT_EQ(
      toPretty(fruit2),
      "pear (=4)");  // since 3+1 is 4, and pear is the successor of banana.
}

TEST(EnumTest, StringsAndValues) {
  MY_ENUM_STRING_VIEW typeName = getTypeName(Fruits());
  EXPECT_EQ(typeName, "Fruits");
  std::array<MY_ENUM_STRING_VIEW, 3> strings = getStrings(Fruits());
  EXPECT_EQ(strings[0], "apple");
  EXPECT_EQ(strings[1], "banana");
  EXPECT_EQ(strings[2], "pear");
  MY_ENUM_STRING_VIEW stringOfNames = getStringOfNames(Fruits());
  EXPECT_EQ(stringOfNames, std::string("apple, banana, pear"));
  std::array<uint8_t, 3> values = getValues(Fruits());
  EXPECT_EQ(values[0], 1);
  EXPECT_EQ(values[1], 3);
  EXPECT_EQ(values[2], 4);
  EXPECT_EQ(getCount(Fruits()), 3);

  EXPECT_EQ(getPosition(Fruits(values[0])), 0);
  EXPECT_EQ(getPosition(Fruits(values[1])), 1);
  EXPECT_EQ(getPosition(Fruits(values[2])), 2);
}

// Enum definition in global scope.
MY_ENUM_DEF(Autos, int, (van, truck, (sedan, 4)));

struct Garage {
  // Enum alias can be in class scope
  MY_ENUM_ALIAS(Autos);
};

TEST(EnumTest, trySetFromString) {
  Garage::Autos car;
  bool is_set = trySetFromString(car, "van");
  EXPECT_EQ(car, Garage::Autos::van);
  is_set = trySetFromString(car, "truck");
  ASSERT_TRUE(is_set);
  EXPECT_EQ(car, Garage::Autos::truck);
  is_set = trySetFromString(car, "sedan");
  ASSERT_TRUE(is_set);
  EXPECT_EQ(car, Garage::Autos::sedan);
  is_set = trySetFromString(car, "bike");
  ASSERT_FALSE(is_set);
}

// MY_ENUM(Foos, ((foo, 2), (foo_alias, 2)));
//
// Won't compile, but will trigger an error such as:
// "error: duplicate case value: 'foo' and 'foo_alias' both equal '2'".
