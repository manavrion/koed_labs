#include "gtest\gtest.h"
#include "matrix.h"

using namespace koed_shared;

TEST(Matrix, Creation) {
  EXPECT_EQ(Matrix({{1}}), Matrix::BuildIdentity(1));
  EXPECT_EQ((Matrix{{1, 0}, {0, 1}}), Matrix::BuildIdentity(2));
}

TEST(Matrix, Column) {
  Matrix m{{1, 2}, {3, 4}};
  EXPECT_EQ(m.GetColumn(0)[0], 1);
  EXPECT_EQ(m.GetColumn(0)[1], 3);
  EXPECT_EQ(m.GetColumn(1)[0], 2);
  EXPECT_EQ(m.GetColumn(1)[1], 4);
}

TEST(Matrix, MeanAndDispersion) {
  Matrix m{{1, 1}, {2, 4}};
  EXPECT_EQ(mean_of_values(m.GetRow(0)), 1);
  EXPECT_EQ(dispersion_of_values(m.GetRow(0)), 0);
  EXPECT_EQ(mean_of_values(m.GetRow(1)), 3);
  EXPECT_EQ(dispersion_of_values(m.GetRow(1)), 1);
}

TEST(Matrix, StandardizedMatrix) {
  Matrix m{{1, 2}, {3, 4}};
  EXPECT_EQ(m.BuildStandardizedMatrix(), Matrix({{-1, -1}, {1, 1}}));
}
