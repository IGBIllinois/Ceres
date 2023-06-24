
/*
 * Hyperspectral data is stored as two-dimension matrix data
 * where one dimension represents the spatial dimension and
 * the other dimension represents the spectral dimension.
 * 
 * The spatial dimension is the set of locations (pixels) within
 * the cameras field of view.
 *
 * The spectral dimension is the set of wavelengths at a given
 * location.
 *
 * The value at a given element within the matix (Sn, λn) is
 * the intensity of light at that given location and wavelength.
 */

#pragma once

#include <array>
#include <vector>
#include <cassert>
#include <iterator>
#include <algorithm>

namespace hyperspectral
{
#ifdef MSVC
	template <class T> constexpr T identity_value = {1};
	template <> constexpr float identity_value<float> = 1.0f;
	template <> constexpr double identity_value<double> = 1.0;

	template <class T> constexpr T zero_value = {};
	template <> constexpr float zero_value<float> = 0.0f;
	template <> constexpr double zero_value<double> = 0.0;
#else
	template <class T> inline constexpr T identity_value = {1};
	template <> inline constexpr float identity_value<float> = 1.0f;
	template <> inline constexpr double identity_value<double> = 1.0;

	template <class T> inline constexpr T zero_value = {};
	template <> inline constexpr float zero_value<float> = 0.0f;
	template <> inline constexpr double zero_value<double> = 0.0;
#endif

	template<typename T>
	struct Iterator
	{
		// Iterator tags here...
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;

		Iterator(T* pos, std::size_t stride) : mpPos(pos), mStride(stride) {}

		reference operator*() const { return *mpPos; }
		pointer operator->() { return mpPos; }

		// Prefix increment
		Iterator& operator++() { mpPos += mStride; return *this; }
		Iterator& operator--() { mpPos -= mStride; return *this; }

		// Postfix increment
		Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
		Iterator operator--(int) { Iterator tmp = *this; --(*this); return tmp; }

		friend bool operator== (const Iterator& a, const Iterator& b) { return a.mpPos == b.mpPos; };
		friend bool operator!= (const Iterator& a, const Iterator& b) { return a.mpPos != b.mpPos; };

	private:
		T* mpPos;
		std::size_t mStride;
	};

	template<typename T>
	class view
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using difference_type = std::ptrdiff_t;
		using iterator = const Iterator<T>;
		using const_iterator = iterator;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using iterator_category = typename iterator::iterator_category;


		view(const T* start, const T* last, std::size_t stride = 1);

		bool empty() const;
		std::size_t size() const;

		iterator begin() const { return cbegin(); };
		iterator end() const   { return cend(); };
		const_iterator cbegin() const { return Iterator<T>(mpStart, mStride); };
		const_iterator cend() const   { return Iterator<T>(mpLast + mStride, mStride); };

		reverse_iterator rbegin() const { return crbegin(); };
		reverse_iterator rend() const   { return crend(); };
		const_reverse_iterator crbegin() const { return reverse_iterator(Iterator<T>(mpLast + mStride, mStride)); };
		const_reverse_iterator crend() const   { return reverse_iterator(Iterator<T>(mpStart, mStride)); };

		const T& operator[](std::size_t i) const;

	private:
		const T* mpStart;
		const T* mpLast;
		std::size_t mStride;
	};

	template<typename T>
	class span
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using difference_type = std::ptrdiff_t;
		using iterator = Iterator<T>;
		using const_iterator = const iterator;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using iterator_category = typename iterator::iterator_category;

		span(T* start, T* last, std::size_t stride = 1);

		bool empty() const;
		std::size_t size() const;

		iterator begin() { return Iterator<T>(mpStart, mStride); };
		iterator end()   { return Iterator<T>(mpLast + mStride, mStride); };
		const_iterator begin() const   { return cbegin(); };
		const_iterator end() const     { return cend(); };
		const_iterator cbegin() const  { return Iterator<T>(mpStart, mStride); };
		const_iterator cend() const    { return Iterator<T>(mpLast + mStride, mStride); };

		reverse_iterator rbegin() { return reverse_iterator(Iterator<T>(mpLast + mStride, mStride)); };
		reverse_iterator rend()   { return reverse_iterator(Iterator<T>(mpStart, mStride)); };
		const_reverse_iterator rbegin() const { return crbegin(); };
		const_reverse_iterator rend() const   { return crend(); };
		const_reverse_iterator crbegin() const { return reverse_iterator(Iterator<T>(mpLast + mStride, mStride)); };
		const_reverse_iterator crend() const { return reverse_iterator(Iterator<T>(mpStart, mStride)); };

		const T& operator[](std::size_t i) const;
		T& operator[](std::size_t i);

		operator view<T>() const;

	private:
		T* mpStart;
		T* mpLast;
		std::size_t mStride;
	};

	/** Spectral Major Dynamically Sized Hyperspectral Data */
	/*
	 * For spectral major data means that the wavelength data
	 * is stored sequentially for a given spatial point.
	 * 
	 * [S0: λ0...λn], [S1: λ0...λn] ..., [SM: λ0...λn]
	 * 
	 * Since the wavelength data is stored close to together,
	 * use this type for fast access to the spectral data for
	 * a given point.
	 */
	template<typename T>
	class data_spectal_major
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

		data_spectal_major();
		data_spectal_major(std::size_t num_of_pixels, std::size_t num_of_wavelengths);
		data_spectal_major(const data_spectal_major& m);

		float min_wavelength_nm() const;
		float max_wavelength_nm() const;
		std::size_t num_wavelengths() const;
		std::size_t num_pixels() const;

		bool empty() const;
		void resize(std::size_t num_of_pixels, std::size_t num_of_wavelengths);
		void clear();

		view<T> row(std::size_t r) const;
		span<T> row(std::size_t r);

		view<T> column(std::size_t c) const;
		span<T> column(std::size_t c);

		const value_type& get(std::size_t pixel_index, std::size_t wavelength_index) const;
		void set(std::size_t pixel_index, std::size_t wavelength_index, const value_type& v);

		std::size_t size() const;
		const std::vector<T>& data() const;
		std::vector<T>& data();

	protected:
		std::vector<T> mData;
		std::size_t mNumPixels;
		std::size_t mNumWavelengths;
		float mMinWavelength_nm;
		float mMaxWavelength_nm;
	};


	/** Spatial Major Dynamically Sized Hyperspectral Data */
	/*
	 * For spatial major data means that the spatial data
	 * is stored sequentially for a given spectral point.
	 *
	 * [λ0: S0...Sn], [λ1: S0...Sn] ..., [λm: S0...Sn]
	 *
	 * Since the spatial data is stored close to together,
	 * use this type for fast access to the spatial data for
	 * a given wavelength.
	 * 
	 * The HySpex cameras use this format.
	 */
	template<typename T>
	class matrix_col_major
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

		matrix_col_major();
		matrix_col_major(std::size_t num_of_rows, std::size_t num_of_cols);
		matrix_col_major(const matrix_col_major& m);

		bool empty() const;
		std::size_t num_columns() const;
		std::size_t num_rows() const;

		void resize(std::size_t num_of_rows, std::size_t num_of_cols);
		void clear();

		view<T> row(std::size_t r) const;
		span<T> row(std::size_t r);

		view<T> column(std::size_t c) const;
		span<T> column(std::size_t c);

		const value_type& get(std::size_t row, std::size_t col) const;
		void set(std::size_t row, std::size_t col, const value_type& v);

		std::size_t size() const;
		const std::vector<T>& data() const;
		std::vector<T>& data();

	protected:
		std::vector<T> mData;
		std::size_t mNumOfRows;
		std::size_t mNumOfCols;
	};


	/** Row Major Fixed Sized Matrix */
	template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
	class matrix_row_major_fixed
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

		matrix_row_major_fixed();
		matrix_row_major_fixed(const matrix_row_major_fixed& m);
		matrix_row_major_fixed& operator=(const matrix_row_major_fixed&) = default;

		std::size_t num_columns() const;
		std::size_t num_rows() const;

		void zero();
		void identity();

		view<T> row(std::size_t r) const;
		span<T> row(std::size_t r);

		view<T> column(std::size_t c) const;
		span<T> column(std::size_t c);

		const value_type& get(std::size_t row, std::size_t col) const;
		void set(std::size_t row, std::size_t col, const value_type& v);

		std::size_t size() const;
		const std::array<T, num_of_rows* num_of_cols>& data() const;
		std::array<T, num_of_rows* num_of_cols>& data();

	protected:
		std::array<T, num_of_rows * num_of_cols> mData;
	};


	/** Column Major Fixed Sized Matrix */
	template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
	class matrix_col_major_fixed
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

		matrix_col_major_fixed();
		matrix_col_major_fixed(const matrix_col_major_fixed& m);
		matrix_col_major_fixed& operator=(const matrix_col_major_fixed&) = default;

		std::size_t num_columns() const;
		std::size_t num_rows() const;

		void zero();
		void identity();

		view<T> row(std::size_t r) const;
		span<T> row(std::size_t r);

		view<T> column(std::size_t c) const;
		span<T> column(std::size_t c);

		void setColumn(std::size_t c, view<T> data);
		void setColumn(std::size_t c, const std::vector<T>& data);
		void setColumn(std::size_t c, const std::array<T, num_of_rows>& data);

		const value_type& get(std::size_t row, std::size_t col) const;
		void set(std::size_t row, std::size_t col, const value_type& v);

		std::size_t size() const;
		const std::array<T, num_of_rows* num_of_cols>& data() const;
		std::array<T, num_of_rows* num_of_cols>& data();

	protected:
		std::array<T, num_of_rows* num_of_cols> mData;
	};


	/**
	 * Helper functions 
	 */
	template<typename T>
	matrix_row_major<T> to_matrix_row_major(const matrix_col_major<T>& m);

	template<typename T>
	matrix_col_major<T> to_matrix_col_major(const matrix_row_major<T>& m);

	template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
	matrix_row_major_fixed<T, num_of_rows, num_of_cols> to_matrix_row_major(const matrix_col_major_fixed<T, num_of_rows, num_of_cols>& m);

	template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
	matrix_col_major_fixed<T, num_of_rows, num_of_cols> to_matrix_col_major(const matrix_row_major_fixed<T, num_of_rows, num_of_cols>& m);

	template<typename T>
	std::vector<T> to_vector(const span<T>& s);

} // End namespace ouster


/******************************************************************************
 *  I M P L E M E N T A T I O N   D E T A I L S
 *****************************************************************************/

 /////////////////////////////////////////////////////////////////////////////////////
 // View
 /////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline ouster::view<T>::view(const T* start, const T* last, std::size_t stride)
	: mpStart(start), mpLast(last), mStride(stride)
{}

template<typename T>
inline bool ouster::view<T>::empty() const
{
	return mpLast == mpStart;
}

template<typename T>
inline std::size_t ouster::view<T>::size() const
{
	if (mpLast == mpStart) return 0;
	return (std::distance(mpStart, mpLast) / mStride) + 1;
}

/*
template<typename T>
typename ouster::view<T>::iterator ouster::view<T>::begin() const
{
	return Iterator<T>(mpStart, mStride);
}

template<typename T>
typename ouster::view<T>::iterator ouster::view<T>::end() const
{
	return Iterator<T>(mpLast + 1, 0);
}

template<typename T>
typename ouster::view<T>::const_iterator ouster::view<T>::cbegin() const
{
	return const_iterator(mpStart, mStride);
}

template<typename T>
typename ouster::view<T>::const_iterator ouster::view<T>::cend() const
{
	return const_iterator(mpLast + 1, 0);
}


template<typename T>
typename ouster::view<T>::reverse_iterator ouster::view<T>::rbegin() const
{
	return reverse_iterator(mpLast, mStride);
}

template<typename T>
typename ouster::view<T>::reverse_iterator ouster::view<T>::rend() const
{
	return reverse_iterator(mpStart - 1, 0);
}

template<typename T>
typename ouster::view<T>::const_reverse_iterator ouster::view<T>::crbegin() const
{
	return const_reverse_iterator(mpLast, mStride);
}

template<typename T>
typename ouster::view<T>::const_reverse_iterator ouster::view<T>::crend() const
{
	return const_reverse_iterator(mpStart - 1, 0);
}
*/

template<typename T>
inline const T& ouster::view<T>::operator[](std::size_t i) const
{
	return *(mpStart + i * mStride);
}


/////////////////////////////////////////////////////////////////////////////////////
// Span
/////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline ouster::span<T>::span(T* start, T* last, std::size_t stride)
	: mpStart(start), mpLast(last), mStride(stride)
{}

template<typename T>
inline bool ouster::span<T>::empty() const
{
	return mpLast == mpStart;
}

template<typename T>
inline std::size_t ouster::span<T>::size() const
{
	if (mpLast == mpStart) return 0;
	return (std::distance(mpStart, mpLast) / mStride) + 1;
}

/*
template<typename T>
typename ouster::span<T>::iterator ouster::span<T>::begin()
{
	return iterator(mpStart, mStride);
}

template<typename T>
typename ouster::span<T>::iterator ouster::span<T>::end()
{
	return iterator(mpLast + 1, 0);
}

template<typename T>
typename ouster::span<T>::const_iterator ouster::span<T>::begin() const
{
	return const_iterator(mpStart, mStride);
}

template<typename T>
typename ouster::span<T>::const_iterator ouster::span<T>::end() const
{
	return const_iterator(mpLast + 1, 0);
}

template<typename T>
typename ouster::span<T>::const_iterator ouster::span<T>::cbegin() const
{
	return const_iterator(mpStart, mStride);
}

template<typename T>
typename ouster::span<T>::const_iterator ouster::span<T>::cend() const
{
	return const_iterator(mpLast + 1, 0);
}


template<typename T>
typename ouster::span<T>::reverse_iterator ouster::span<T>::rbegin()
{
	return reverse_iterator(mpLast, mStride);
}

template<typename T>
typename ouster::span<T>::reverse_iterator ouster::span<T>::rend()
{
	return reverse_iterator(mpStart - 1, 0);
}

template<typename T>
typename ouster::span<T>::const_reverse_iterator ouster::span<T>::rbegin() const
{
	return reverse_iterator(mpLast, mStride);
}

template<typename T>
typename ouster::span<T>::const_reverse_iterator ouster::span<T>::rend() const
{
	return reverse_iterator(mpStart - 1, 0);
}

template<typename T>
typename ouster::span<T>::const_reverse_iterator ouster::span<T>::crbegin() const
{
	return const_reverse_iterator(mpLast, mStride);
}

template<typename T>
typename ouster::span<T>::const_reverse_iterator ouster::span<T>::crend() const
{
	return const_reverse_iterator(mpStart - 1, 0);
}
*/


template<typename T>
inline const T& ouster::span<T>::operator[](std::size_t i) const
{
	return *(mpStart + i * mStride);
}

template<typename T>
inline T& ouster::span<T>::operator[](std::size_t i)
{
	return *(mpStart + i * mStride);
}

template<typename T>
inline ouster::span<T>::operator ouster::view<T>() const
{
	return view<T>(mpStart, mpLast, mStride);
}


/////////////////////////////////////////////////////////////////////////////////////
// Row Major Dynamically Sized Matrix
/////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline ouster::matrix_row_major<T>::matrix_row_major()
	: mNumOfRows(0), mNumOfCols(0)
{
	mData.clear();
}

template<typename T>
inline ouster::matrix_row_major<T>::matrix_row_major(std::size_t num_of_rows, std::size_t num_of_cols)
	: mNumOfRows(num_of_rows), mNumOfCols(num_of_cols)
{
	mData.resize(num_of_rows * num_of_cols);
}

template<typename T>
inline ouster::matrix_row_major<T>::matrix_row_major(const matrix_row_major<T>& m)
	: mData(m.mData), mNumOfRows(m.mNumOfRows), mNumOfCols(m.mNumOfCols)
{
}

template<typename T>
inline bool ouster::matrix_row_major<T>::empty() const
{
	return mData.empty();
}

template<typename T>
inline std::size_t ouster::matrix_row_major<T>::num_columns() const
{
	return mNumOfCols;
}

template<typename T>
inline std::size_t ouster::matrix_row_major<T>::num_rows() const
{
	return mNumOfRows;
}

template<typename T>
inline void ouster::matrix_row_major<T>::resize(std::size_t num_of_rows, std::size_t num_of_cols)
{
	mNumOfRows = num_of_rows;
	mNumOfCols = num_of_cols;
	mData.resize(mNumOfRows * mNumOfCols);
}

template<typename T>
inline void ouster::matrix_row_major<T>::clear()
{
	mData.clear();
}

template<typename T>
inline ouster::view<T> ouster::matrix_row_major<T>::row(std::size_t r) const
{
	return view<T>(&mData[r * mNumOfCols], &mData[(r + 1) * mNumOfCols - 1], 1);
}

template<typename T>
inline ouster::span<T> ouster::matrix_row_major<T>::row(std::size_t r)
{
	return span<T>(&mData[r * mNumOfCols], &mData[(r + 1) * mNumOfCols - 1], 1);
}

template<typename T>
inline ouster::view<T> ouster::matrix_row_major<T>::column(std::size_t c) const
{
	return view<T>(&mData[c], &mData[(mNumOfRows - 1) * mNumOfCols + c], mNumOfCols);
}

template<typename T>
inline ouster::span<T> ouster::matrix_row_major<T>::column(std::size_t c)
{
	return span<T>(&mData[c], &mData[(mNumOfRows - 1) * mNumOfCols + c], mNumOfCols);
}

template<typename T>
inline const typename ouster::matrix_row_major<T>::value_type& ouster::matrix_row_major<T>::get(std::size_t row, std::size_t col) const
{
	return mData[row * mNumOfCols + col];
}

template<typename T>
void ouster::matrix_row_major<T>::set(std::size_t row, std::size_t col, const value_type& v)
{
	mData[row * mNumOfCols + col] = v;
}

template<typename T>
std::size_t ouster::matrix_row_major<T>::size() const
{
	return mNumOfRows * mNumOfCols;
}

template<typename T>
const std::vector<T>& ouster::matrix_row_major<T>::data() const
{
	return mData;
}

template<typename T>
std::vector<T>& ouster::matrix_row_major<T>::data()
{
	return mData;
}


/////////////////////////////////////////////////////////////////////////////////////
// Column Major Dynamically Sized Matrix
/////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline ouster::matrix_col_major<T>::matrix_col_major()
	: mNumOfRows(0), mNumOfCols(0)
{
	mData.clear();
}

template<typename T>
inline ouster::matrix_col_major<T>::matrix_col_major(std::size_t num_of_rows, std::size_t num_of_cols)
	: mNumOfRows(num_of_rows), mNumOfCols(num_of_cols)
{
	mData.resize(num_of_rows * num_of_cols);
}

template<typename T>
inline ouster::matrix_col_major<T>::matrix_col_major(const matrix_col_major<T>& m)
	: mData(m.mData), mNumOfRows(m.mNumOfRows), mNumOfCols(m.mNumOfCols)
{
}

template<typename T>
inline bool ouster::matrix_col_major<T>::empty() const
{
	return mData.empty();
}

template<typename T>
inline std::size_t ouster::matrix_col_major<T>::num_columns() const
{
	return mNumOfCols;
}

template<typename T>
inline std::size_t ouster::matrix_col_major<T>::num_rows() const
{
	return mNumOfRows;
}

template<typename T>
inline void ouster::matrix_col_major<T>::resize(std::size_t num_of_rows, std::size_t num_of_cols)
{
	mNumOfRows = num_of_rows;
	mNumOfCols = num_of_cols;
	mData.resize(mNumOfRows * mNumOfCols);
}

template<typename T>
inline void ouster::matrix_col_major<T>::clear()
{
	mData.clear();
}

template<typename T>
inline ouster::view<T> ouster::matrix_col_major<T>::row(std::size_t r) const
{
	return view<T>(&mData[r], &mData[(mNumOfCols - 1) * mNumOfRows + r], mNumOfRows);
}

template<typename T>
inline ouster::span<T> ouster::matrix_col_major<T>::row(std::size_t r)
{
	return span<T>(&mData[r], &mData[(mNumOfCols - 1) * mNumOfRows + r], mNumOfRows);
}

template<typename T>
inline ouster::view<T> ouster::matrix_col_major<T>::column(std::size_t c) const
{
	return view<T>(&mData[c * mNumOfRows], &mData[(c + 1) * mNumOfRows - 1], 1);
}

template<typename T>
inline ouster::span<T> ouster::matrix_col_major<T>::column(std::size_t c)
{
	return span<T>(&mData[c * mNumOfRows], &mData[(c + 1) * mNumOfRows - 1], 1);
}

template<typename T>
inline const typename ouster::matrix_col_major<T>::value_type& ouster::matrix_col_major<T>::get(std::size_t row, std::size_t col) const
{
	return mData[row + col * mNumOfRows];
}

template<typename T>
void ouster::matrix_col_major<T>::set(std::size_t row, std::size_t col, const value_type& v)
{
	mData[row + col * mNumOfRows] = v;
}

template<typename T>
std::size_t ouster::matrix_col_major<T>::size() const
{
	return mNumOfRows * mNumOfCols;
}

template<typename T>
const std::vector<T>& ouster::matrix_col_major<T>::data() const
{
	return mData;
}

template<typename T>
std::vector<T>& ouster::matrix_col_major<T>::data()
{
	return mData;
}


/////////////////////////////////////////////////////////////////////////////////////
// Row Major Fixed Sized Matrix
/////////////////////////////////////////////////////////////////////////////////////
template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline ouster::matrix_row_major_fixed<T, num_of_rows, num_of_cols>::matrix_row_major_fixed()
{
	zero();
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline ouster::matrix_row_major_fixed<T, num_of_rows, num_of_cols>::matrix_row_major_fixed(const matrix_row_major_fixed<T, num_of_rows, num_of_cols>& m)
	: mData(m.mData)
{
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline std::size_t ouster::matrix_row_major_fixed<T, num_of_rows, num_of_cols>::num_columns() const
{
	return num_of_cols;
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline std::size_t ouster::matrix_row_major_fixed<T, num_of_rows, num_of_cols>::num_rows() const
{
	return num_of_rows;
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline void ouster::matrix_row_major_fixed<T, num_of_rows, num_of_cols>::zero()
{
	std::fill(mData.begin(), mData.end(), zero_value<T>);
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline void ouster::matrix_row_major_fixed<T, num_of_rows, num_of_cols>::identity()
{
	assert(num_of_rows == num_of_cols);

	zero();

	for (std::size_t r = 0; r < num_of_rows; ++r)
	{
		set(r, r, identity_value<T>);
	}
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline ouster::view<T>
		ouster::matrix_row_major_fixed<T, num_of_rows, num_of_cols>::row(std::size_t r) const
{
	assert(r < num_of_rows);
	return view(&mData[r * num_of_cols], &mData[(r + 1) * num_of_cols - 1], 1);
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline ouster::span<T>
		ouster::matrix_row_major_fixed<T, num_of_rows, num_of_cols>::row(std::size_t r)
{
	assert(r < num_of_rows);
	return span<T>(&mData[r * num_of_cols], &mData[(r + 1) * num_of_cols - 1], 1);;
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline ouster::view<T>
	ouster::matrix_row_major_fixed<T, num_of_rows, num_of_cols>::column(std::size_t c) const
{
	return view<T>(&mData[c], &mData[(num_of_rows - 1) * num_of_cols + c], num_of_cols);
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline ouster::span<T> 
	ouster::matrix_row_major_fixed<T, num_of_rows, num_of_cols>::column(std::size_t c)
{
	return span<T>(&mData[c], &mData[(num_of_rows - 1) * num_of_cols + c], num_of_cols);
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline const typename ouster::matrix_row_major_fixed<T, num_of_rows, num_of_cols>::value_type&
		ouster::matrix_row_major_fixed<T, num_of_rows, num_of_cols>::get(std::size_t row, std::size_t col) const
{
	assert(col < num_of_cols);
	assert(row < num_of_rows);
	return mData[row * num_of_cols + col];
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline void ouster::matrix_row_major_fixed<T, num_of_rows, num_of_cols>::set(std::size_t row, std::size_t col, const value_type& v)
{
	assert(col < num_of_cols);
	assert(row < num_of_rows);
	mData[row * num_of_cols + col] = v;
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
std::size_t ouster::matrix_row_major_fixed<T, num_of_rows, num_of_cols>::size() const
{
	return num_of_rows * num_of_cols;
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
const std::array<T, num_of_rows* num_of_cols>& ouster::matrix_row_major_fixed<T, num_of_rows, num_of_cols>::data() const
{
	return mData;
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
std::array<T, num_of_rows* num_of_cols>& ouster::matrix_row_major_fixed<T, num_of_rows, num_of_cols>::data()
{
	return mData;
}


/////////////////////////////////////////////////////////////////////////////////////
// Column Major Fixed Sized Matrix
/////////////////////////////////////////////////////////////////////////////////////
template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline ouster::matrix_col_major_fixed<T, num_of_rows, num_of_cols>::matrix_col_major_fixed()
{
	zero();
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline ouster::matrix_col_major_fixed<T, num_of_rows, num_of_cols>::matrix_col_major_fixed(const matrix_col_major_fixed<T, num_of_rows, num_of_cols>& m)
	: mData(m.mData)
{
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline std::size_t ouster::matrix_col_major_fixed<T, num_of_rows, num_of_cols>::num_columns() const
{
	return num_of_cols;
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline std::size_t ouster::matrix_col_major_fixed<T, num_of_rows, num_of_cols>::num_rows() const
{
	return num_of_rows;
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline void ouster::matrix_col_major_fixed<T, num_of_rows, num_of_cols>::zero()
{
	std::fill(mData.begin(), mData.end(), zero_value<T>);
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline void ouster::matrix_col_major_fixed<T, num_of_rows, num_of_cols>::identity()
{
	assert(num_of_rows == num_of_cols);

	zero();

	for (std::size_t c = 0; c < num_of_cols; ++c)
	{
		set(c, c, identity_value<T>);
	}
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline ouster::view<T> 
	ouster::matrix_col_major_fixed<T, num_of_rows, num_of_cols>::row(std::size_t r) const
{
	return view<T>(&mData[r], &mData[(num_of_cols - 1) * num_of_rows + r], num_of_rows);
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline ouster::span<T>
	ouster::matrix_col_major_fixed<T, num_of_rows, num_of_cols>::row(std::size_t r)
{
	return span<T>(&mData[r], &mData[(num_of_cols - 1) * num_of_rows + r], num_of_rows);
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline ouster::view<T>
		ouster::matrix_col_major_fixed<T, num_of_rows, num_of_cols>::column(std::size_t c) const
{
	assert(c < num_of_cols);
	return view<T>(&mData[c * num_of_rows], &mData[(c + 1) * num_of_rows - 1], 1);
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline ouster::span<T> ouster::matrix_col_major_fixed<T, num_of_rows, num_of_cols>::column(std::size_t c)
{
	assert(c < num_of_cols);
	return span<T>(&mData[c * num_of_rows], &mData[(c + 1) * num_of_rows - 1], 1);
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
void ouster::matrix_col_major_fixed<T, num_of_rows, num_of_cols>::setColumn(std::size_t c, view<T> data)
{
	assert(c < num_of_cols);
	assert(data.size() == num_of_rows);

	std::size_t n = c * num_of_rows;
	for (std::size_t r = 0; r < num_of_rows; ++r)
	{
		mData[n++] = data[r];
	}
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
void ouster::matrix_col_major_fixed<T, num_of_rows, num_of_cols>::setColumn(std::size_t c, const std::vector<T>& data)
{
	assert(c < num_of_cols);
	assert(data.size() == num_of_rows);

	std::size_t n = c * num_of_rows;
	for (std::size_t r = 0; r < num_of_rows; ++r)
	{
		mData[n++] = data[r];
	}
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
void ouster::matrix_col_major_fixed<T, num_of_rows, num_of_cols>::setColumn(std::size_t c, const std::array<T, num_of_rows>& data)
{
	assert(c < num_of_cols);
	assert(data.size() == num_of_rows);

	std::size_t n = c * num_of_rows;
	for (std::size_t r = 0; r < num_of_rows; ++r)
	{
		mData[n++] = data[r];
	}
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline const typename ouster::matrix_col_major_fixed<T, num_of_rows, num_of_cols>::value_type&
	ouster::matrix_col_major_fixed<T, num_of_rows, num_of_cols>::get(std::size_t row, std::size_t col) const
{
	assert(col < num_of_cols);
	assert(row < num_of_rows);
	return mData[row + col * num_of_rows];
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
inline void ouster::matrix_col_major_fixed<T, num_of_rows, num_of_cols>::set(std::size_t row, std::size_t col, const value_type& v)
{
	assert(col < num_of_cols);
	assert(row < num_of_rows);
	mData[row + col * num_of_rows] = v;
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
std::size_t ouster::matrix_col_major_fixed<T, num_of_rows, num_of_cols>::size() const
{
	return num_of_rows * num_of_cols;
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
const std::array<T, num_of_rows* num_of_cols>& ouster::matrix_col_major_fixed<T, num_of_rows, num_of_cols>::data() const
{
	return mData;
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
std::array<T, num_of_rows* num_of_cols>& ouster::matrix_col_major_fixed<T, num_of_rows, num_of_cols>::data()
{
	return mData;
}


/******************************************************************************
 * H E L P E R   F U N C T I O N S
******************************************************************************/

template<typename T>
ouster::matrix_row_major<T> ouster::to_matrix_row_major(const ouster::matrix_col_major<T>& m)
{
	if (m.empty()) return matrix_row_major<T>();

	matrix_row_major<T> result(m.num_rows(), m.num_columns());

	for (std::size_t col = 0; col < m.num_columns(); ++col)
	{
		for (std::size_t row = 0; row < m.num_rows(); ++row)
		{
			result.set(row, col, m.get(row, col));
		}
	}

	return result;
}

template<typename T>
ouster::matrix_col_major<T> ouster::to_matrix_col_major(const ouster::matrix_row_major<T>& m)
{
	if (m.empty()) return matrix_col_major<T>();

	matrix_col_major<T> result(m.num_rows(), m.num_columns());

	for (std::size_t row = 0; row < m.num_rows(); ++row)
	{
		for (std::size_t col = 0; col < m.num_columns(); ++col)
		{
			result.set(row, col, m.get(row, col));
		}
	}

	return result;
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
ouster::matrix_row_major_fixed<T, num_of_rows, num_of_cols> ouster::to_matrix_row_major(const matrix_col_major_fixed<T, num_of_rows, num_of_cols>& m)
{
	if (m.empty()) return matrix_row_major_fixed<T, num_of_rows, num_of_cols>();

	matrix_row_major_fixed<T, num_of_rows, num_of_cols> result;

	for (std::size_t col = 0; col < m.num_columns(); ++col)
	{
		for (std::size_t row = 0; row < m.num_rows(); ++row)
		{
			result.set(row, col, m.get(row, col));
		}
	}

	return result;
}

template<typename T, std::size_t num_of_rows, std::size_t num_of_cols>
ouster::matrix_col_major_fixed<T, num_of_rows, num_of_cols> ouster::to_matrix_col_major(const matrix_row_major_fixed<T, num_of_rows, num_of_cols>& m)
{
	if (m.empty()) return matrix_col_major_fixed<T, num_of_rows, num_of_cols>();

	matrix_col_major_fixed<T, num_of_rows, num_of_cols> result;

	for (std::size_t row = 0; row < m.num_rows(); ++row)
	{
		for (std::size_t col = 0; col < m.num_columns(); ++col)
		{
			result.set(row, col, m.get(row, col));
		}
	}

	return result;
}

template<typename T>
inline std::vector<T> to_vector(const ouster::span<T>& s)
{
	std::size_t n = s.size();
	std::vector<T> result(n);
	for (std::size_t i = 0; i < n; ++i)
		result[i] = s[i];
	return result;
}

