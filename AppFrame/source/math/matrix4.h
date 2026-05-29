#pragma once
#include <concepts>
#include <string>
#include <format>
#include "vector4.h"

template <std::floating_point T = float>
class Matrix4
{
public:
	std::array<std::array<T, 4>, 4> m;// 4x4の行列

	// noexcept:例外を投げないことを宣言するキーワード
	constexpr Matrix4() noexcept = default;

	// 行列の要素にアクセスするための演算子オーバーロード
	NDCE T& operator[](size_t row, size_t col)noexcept
	{
		return m[row][col];
	}

};
