#pragma once

#include <array>

#include <cassert>

#include <iostream>

namespace aech::math
{
	template<typename ScalarType, size_t Dimension>
	struct vec_t
	{
		std::array<ScalarType, Dimension> data{};

		// initialize all components to zero
		vec_t() = default;


		// initialize all components to scalar
		vec_t(ScalarType scalar)
		{
			std::fill(data.begin(), data.end(), scalar);
		}

		template<typename... T> requires(sizeof... (T) == Dimension && std::is_convertible_v<T, ScalarType>)
		vec_t(T... args)
			: data{ args... }
		{
			
		}

		// initialize from init list containing <= number of elements of vector
		vec_t(const std::initializer_list<ScalarType>& init_list)
		{
			assert(init_list.size() <= Dimension);
			
			std::copy(init_list.begin(), init_list.end(), data.begin());
		}

		template<size_t OtherDimension>
		vec_t(const vec_t<ScalarType, OtherDimension>& vec, ScalarType scalar)
		{
			assert(OtherDimension == Dimension - 1);
			
			std::copy(vec.data.begin(), vec.data.end(), data.begin());
			data[OtherDimension] = scalar;
		}

		template<size_t OtherDimension> requires (OtherDimension > Dimension)
		vec_t(const vec_t<ScalarType, OtherDimension>& vec)
		{
			assert(OtherDimension > Dimension);

			std::copy_n(vec.data.begin(), Dimension, data.begin());
		}

		template<size_t OtherDimension>
		vec_t(const vec_t<ScalarType, OtherDimension> &vec, const std::initializer_list<ScalarType> &init_list)
		{
			assert(OtherDimension + init_list.size() == Dimension);
			
			auto it = std::copy(vec.data.begin(), vec.data.end(), data.begin());
			std::copy(init_list.begin(), init_list.end(), it);
		}
		
		vec_t& operator+=(const vec_t& rhs)
		{
			for (size_t i = 0; i < Dimension; i++)
			{
				data[i] += rhs.data[i];
			}

			return *this;
		}
		
		vec_t& operator-=(const vec_t& rhs)
		{
			for (size_t i = 0; i < Dimension; i++)
			{
				data[i] -= rhs.data[i];
			}

			return *this;
		}
		
		vec_t& operator*=(float rhs)
		{
			for (auto &el: data)
			{
				el *= rhs;
			}

			return *this;
		}

		vec_t& operator/=(float rhs)
		{
			for (auto& el : data)
			{
				el /= rhs;
			}

			return *this;
		}

		float& operator[](size_t index)
		{
			assert(index < Dimension);
			return data[index];
		}

		const float& operator[](size_t index) const
		{
			assert(index < Dimension);
			return data[index];
		}
	};

	template<size_t Dimension, typename ScalarType>
	vec_t<ScalarType, Dimension> operator+(vec_t<ScalarType, Dimension> lhs, const vec_t<ScalarType, Dimension>& rhs)
	{
		return lhs += rhs;
	}
	
	template<size_t Dimension, typename ScalarType>
	vec_t<ScalarType, Dimension> operator-(vec_t<ScalarType, Dimension> lhs, const vec_t<ScalarType, Dimension>& rhs)
	{
		return lhs -= rhs;
	}
	
	template<size_t Dimension, typename ScalarType>
	vec_t<ScalarType, Dimension> operator*(vec_t<ScalarType, Dimension> lhs, ScalarType rhs)
	{
		return lhs *= rhs;
	}
	
	template<size_t Dimension, typename ScalarType>
	vec_t<ScalarType, Dimension> operator/(vec_t<ScalarType, Dimension> lhs, ScalarType rhs)
	{
		return lhs /= rhs;
	}

	template<size_t Dimension, typename ScalarType>
	vec_t<ScalarType, Dimension> operator*(ScalarType lhs, vec_t<ScalarType, Dimension> rhs)
	{
		return rhs *= lhs;
	}
} // namespace aech::math
