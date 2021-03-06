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

TEST(Matrix, MultMatrix) {
  Matrix m1{{1, 2}, {3, 4}};
  Matrix m2{{5, 6}, {7, 8}};
  Matrix m3{{19, 22}, {43, 50}};
  EXPECT_EQ(m1 * m2, m3);
}

TEST(Matrix, MultMatrixAlt) {
  Matrix m1{{1, 2, 3}, {4, 5, 6}};
  Matrix m2{{7, 8, 9, 10}, {11, 12, 13, 14}, {15, 16, 17, 18}};
  Matrix m3{{74, 80, 86, 92}, {173, 188, 203, 218}};
  EXPECT_EQ(m1 * m2, m3);
}

TEST(Matrix, ReverseMatrix) {
  Matrix m{{1, 2}, {3, 4}};
  auto mm = m * mpow(m, -1);
  EXPECT_EQ(Matrix::BuildIdentity(m.size()), m * mpow(m, -1));
}

TEST(Matrix, ReverseMatrixAlt) {
  Matrix m{{7, 4}, {5, 3}};
  auto r = mpow(m, -1);
  EXPECT_EQ(mpow(m, -1), Matrix({{3, -4}, {-5, 7}}));
}

TEST(Matrix, SpecialLab2Test) {
  Matrix x = {{1, 2}, {2, 1}, {1, 1}, {5, 3}, {1, 1}};
  Matrix y = Matrix{{1, 2, 1, 3, 2}}.BuildTransposeMatrix();

  Matrix xt = x.BuildTransposeMatrix();

  EXPECT_EQ(xt * x, Matrix({{32, 21}, {21, 16}}));
  EXPECT_EQ(mpow(xt * x, -1),
            Matrix({{2.0 - 6.0 * (21.0 / 71.0), 6.0 * (32.0 / 71.0) - 3},
                    {-21.0 / 71.0, 32.0 / 71.0}}));

  EXPECT_TRUE((mpow(xt * x, -1) * xt)
                  .Equals(Matrix{{-4.0 + 258.0 / 71.0, 1.0 - 60.0 / 71.0,
                                  -1.0 + 66.0 / 71.0, 1.0 - 54.0 / 71.0,
                                  -1.0 + 66.0 / 71.0},
                                 {43.0 / 71.0, -10.0 / 71.0, 11.0 / 71.0,
                                  -9.0 / 71.0, 11.0 / 71.0}},
                          1e-8));
  EXPECT_TRUE((mpow(xt * x, -1) * xt * y)
                  .Equals(Matrix{{-2.0 + 174.0 / 71.0}, {29.0 / 71.0}}, 1e-8));
}
