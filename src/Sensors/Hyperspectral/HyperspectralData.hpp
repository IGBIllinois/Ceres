
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
		using iterator = Iterator<const T>;
		using const_iterator = iterator;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using iterator_category = typename iterator::iterator_category;


		view(const T* start, const T* last, std::size_t stride = 1);

		bool empty() const;
		std::size_t size() const;

		iterator begin() const { return cbegin(); };
		iterator end() const   { return cend(); };
		const_iterator cbegin() const { return Iterator<const T>(mpStart, mStride); };
		const_iterator cend() const   { return Iterator<const T>(mpLast + mStride, mStride); };

		reverse_iterator rbegin() const { return crbegin(); };
		reverse_iterator rend() const   { return crend(); };
		const_reverse_iterator crbegin() const { return reverse_iterator(Iterator<const T>(mpLast + mStride, mStride)); };
		const_reverse_iterator crend() const   { return reverse_iterator(Iterator<const T>(mpStart, mStride)); };

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
		using const_iterator = Iterator<const T>;
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
		const_iterator cbegin() const  { return Iterator<const T>(mpStart, mStride); };
		const_iterator cend() const    { return Iterator<const T>(mpLast + mStride, mStride); };

		reverse_iterator rbegin() { return reverse_iterator(Iterator<T>(mpLast + mStride, mStride)); };
		reverse_iterator rend()   { return reverse_iterator(Iterator<T>(mpStart, mStride)); };
		const_reverse_iterator rbegin() const { return crbegin(); };
		const_reverse_iterator rend() const   { return crend(); };
		const_reverse_iterator crbegin() const { return const_reverse_iterator(Iterator<const T>(mpLast + mStride, mStride)); };
		const_reverse_iterator crend() const { return const_reverse_iterator(Iterator<const T>(mpStart, mStride)); };

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
	class spectral_major_data
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

		spectral_major_data();
		spectral_major_data(std::size_t num_of_pixels, std::size_t num_of_wavelengths);
		spectral_major_data(const spectral_major_data& m);

		std::size_t num_wavelengths() const;
		std::size_t num_pixels() const;

		bool empty() const;
		void resize(std::size_t num_of_pixels, std::size_t num_of_wavelengths);
		void clear();

		view<T> pixels(std::size_t wavelength_index) const;
		span<T> pixels(std::size_t wavelength_index);

		view<T> wavelengths(std::size_t pixel_index) const;
		span<T> wavelengths(std::size_t pixel_index);

		const value_type& get(std::size_t pixel_index, std::size_t wavelength_index) const;
		void set(std::size_t pixel_index, std::size_t wavelength_index, const value_type& v);

		std::size_t size() const;
		const std::vector<T>& data() const;
		std::vector<T>& data();

	protected:
		std::vector<T> mData;
		std::size_t mNumPixels;
		std::size_t mNumWavelengths;
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
	class spatial_major_data
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

		spatial_major_data();
		spatial_major_data(std::size_t num_of_pixels, std::size_t num_of_wavelengths);
		spatial_major_data(const spatial_major_data& m);

		bool empty() const;
		std::size_t num_wavelengths() const;
		std::size_t num_pixels() const;

		void resize(std::size_t num_of_pixels, std::size_t num_of_wavelengths);
		void clear();

		view<T> pixels(std::size_t wavelength_index) const;
		span<T> pixels(std::size_t wavelength_index);

		view<T> wavelengths(std::size_t pixel_index) const;
		span<T> wavelengths(std::size_t pixel_index);

		const value_type& get(std::size_t pixel_index, std::size_t wavelength_index) const;
		void set(std::size_t pixel_index, std::size_t wavelength_index, const value_type& v);

		std::size_t size() const;
		const std::vector<T>& data() const;
		std::vector<T>& data();

	protected:
		std::vector<T> mData;
		std::size_t mNumPixels;
		std::size_t mNumWavelengths;
	};


	/** Spectral Major Fixed Sized Hyperspectral Data */
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
	template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
	class spectral_major_data_fixed
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

		spectral_major_data_fixed();
		spectral_major_data_fixed(const spectral_major_data_fixed& m);
		spectral_major_data_fixed& operator=(const spectral_major_data_fixed&) = default;

		std::size_t num_wavelengths() const;
		std::size_t num_pixels() const;

		void zero();
		void identity();

		view<T> pixels(std::size_t wavelength_index) const;
		span<T> pixels(std::size_t wavelength_index);

		view<T> wavelengths(std::size_t pixel_index) const;
		span<T> wavelengths(std::size_t pixel_index);

		const value_type& get(std::size_t pixel_index, std::size_t wavelength_index) const;
		void set(std::size_t pixel_index, std::size_t wavelength_index, const value_type& v);

		std::size_t size() const;
		const std::array<T, NUM_PIXELS * NUM_WAVELENGTHS>& data() const;
		std::array<T, NUM_PIXELS * NUM_WAVELENGTHS>& data();

	protected:
		std::array<T, NUM_PIXELS * NUM_WAVELENGTHS> mData;
	};


	/** Spatial Major Fixed Sized Hyperspectral Data */
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
	template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
	class spatial_major_data_fixed
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

		spatial_major_data_fixed();
		spatial_major_data_fixed(const spatial_major_data_fixed& m);
		spatial_major_data_fixed& operator=(const spatial_major_data_fixed&) = default;

		std::size_t num_wavelengths() const;
		std::size_t num_pixels() const;

		void zero();
		void identity();

		view<T> pixels(std::size_t wavelength_index) const;
		span<T> pixels(std::size_t wavelength_index);

		view<T> wavelengths(std::size_t pixel_index) const;
		span<T> wavelengths(std::size_t pixel_index);

		const value_type& get(std::size_t pixel_index, std::size_t wavelength_index) const;
		void set(std::size_t pixel_index, std::size_t wavelength_index, const value_type& v);

		std::size_t size() const;
		const std::array<T, NUM_PIXELS * NUM_WAVELENGTHS>& data() const;
		std::array<T, NUM_PIXELS * NUM_WAVELENGTHS>& data();

	protected:
		std::array<T, NUM_PIXELS * NUM_WAVELENGTHS> mData;
	};


	/**
	 * Helper functions 
	 */
	template<typename T>
	spectral_major_data<T> to_spectral_major_data(const spatial_major_data<T>& m);

	template<typename T>
	spatial_major_data<T> to_spatial_major_data(const spectral_major_data<T>& m);

	template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
	spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>
		to_spectral_major_data(const spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>& m);

	template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
	spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>
		to_spatial_major_data(const spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>& m);

	template<typename T>
	std::vector<T> to_vector(const span<T>& s);

} // End namespace hyperspectral


/******************************************************************************
 *  I M P L E M E N T A T I O N   D E T A I L S
 *****************************************************************************/

 /////////////////////////////////////////////////////////////////////////////////////
 // View
 /////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline hyperspectral::view<T>::view(const T* start, const T* last, std::size_t stride)
	: mpStart(start), mpLast(last), mStride(stride)
{}

template<typename T>
inline bool hyperspectral::view<T>::empty() const
{
	return mpLast == mpStart;
}

template<typename T>
inline std::size_t hyperspectral::view<T>::size() const
{
	if (mpLast == mpStart) return 0;
	return (std::distance(mpStart, mpLast) / mStride) + 1;
}

template<typename T>
inline const T& hyperspectral::view<T>::operator[](std::size_t i) const
{
	return *(mpStart + i * mStride);
}


/////////////////////////////////////////////////////////////////////////////////////
// Span
/////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline hyperspectral::span<T>::span(T* start, T* last, std::size_t stride)
	: mpStart(start), mpLast(last), mStride(stride)
{}

template<typename T>
inline bool hyperspectral::span<T>::empty() const
{
	return mpLast == mpStart;
}

template<typename T>
inline std::size_t hyperspectral::span<T>::size() const
{
	if (mpLast == mpStart) return 0;
	return (std::distance(mpStart, mpLast) / mStride) + 1;
}

template<typename T>
inline const T& hyperspectral::span<T>::operator[](std::size_t i) const
{
	return *(mpStart + i * mStride);
}

template<typename T>
inline T& hyperspectral::span<T>::operator[](std::size_t i)
{
	return *(mpStart + i * mStride);
}

template<typename T>
inline hyperspectral::span<T>::operator hyperspectral::view<T>() const
{
	return view<T>(mpStart, mpLast, mStride);
}


/////////////////////////////////////////////////////////////////////////////////////
// Spectral Major Dynamically Sized Hyperspectral Data
/////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline hyperspectral::spectral_major_data<T>::spectral_major_data()
	: mNumPixels(0), mNumWavelengths(0)
{
	mData.clear();
}

template<typename T>
inline hyperspectral::spectral_major_data<T>::spectral_major_data(std::size_t num_pixels, std::size_t num_wavelengths)
	: mNumPixels(num_pixels), mNumWavelengths(num_wavelengths)
{
	mData.resize(num_pixels * num_wavelengths);
}

template<typename T>
inline hyperspectral::spectral_major_data<T>::spectral_major_data(const spectral_major_data<T>& m)
	: mData(m.mData), mNumPixels(m.mNumPixels), mNumWavelengths(m.mNumWavelengths)
{
}

template<typename T>
inline bool hyperspectral::spectral_major_data<T>::empty() const
{
	return mData.empty();
}

template<typename T>
inline std::size_t hyperspectral::spectral_major_data<T>::num_wavelengths() const
{
	return mNumWavelengths;
}

template<typename T>
inline std::size_t hyperspectral::spectral_major_data<T>::num_pixels() const
{
	return mNumPixels;
}

template<typename T>
inline void hyperspectral::spectral_major_data<T>::resize(std::size_t num_pixels, std::size_t num_wavelengths)
{
	mNumPixels = num_pixels;
	mNumWavelengths = num_wavelengths;
	mData.resize(mNumPixels * mNumWavelengths);
}

template<typename T>
inline void hyperspectral::spectral_major_data<T>::clear()
{
	mNumPixels = 0;
	mNumWavelengths = 0;
	mData.clear();
}

template<typename T>
inline hyperspectral::view<T> hyperspectral::spectral_major_data<T>::pixels(std::size_t wavelength_index) const
{
	assert(wavelength_index < mNumWavelengths);
	return view<T>(&mData[wavelength_index], &mData[(mNumPixels - 1) * mNumWavelengths + wavelength_index], mNumWavelengths);
}

template<typename T>
inline hyperspectral::span<T> hyperspectral::spectral_major_data<T>::pixels(std::size_t wavelength_index)
{
	assert(wavelength_index < mNumWavelengths);
	return span<T>(&mData[wavelength_index], &mData[(mNumPixels - 1) * mNumWavelengths + wavelength_index], mNumWavelengths);
}

template<typename T>
inline hyperspectral::view<T> hyperspectral::spectral_major_data<T>::wavelengths(std::size_t pixel_index) const
{
	assert(pixel_index < mNumPixels);
	return view<T>(&mData[pixel_index * mNumWavelengths], &mData[(pixel_index + 1) * mNumWavelengths - 1], 1);
}

template<typename T>
inline hyperspectral::span<T> hyperspectral::spectral_major_data<T>::wavelengths(std::size_t pixel_index)
{
	assert(pixel_index < mNumPixels);
	return span<T>(&mData[pixel_index * mNumWavelengths], &mData[(pixel_index + 1) * mNumWavelengths - 1], 1);
}

template<typename T>
inline const typename hyperspectral::spectral_major_data<T>::value_type& 
	hyperspectral::spectral_major_data<T>::get(std::size_t pixel_index, std::size_t wavelength_index) const
{
	assert((pixel_index < mNumPixels) && (wavelength_index < mNumWavelengths));
	return mData[pixel_index * mNumWavelengths + wavelength_index];
}

template<typename T>
void hyperspectral::spectral_major_data<T>::set(std::size_t pixel_index, std::size_t wavelength_index, const value_type& v)
{
	assert((pixel_index < mNumPixels) && (wavelength_index < mNumWavelengths));
	mData[pixel_index * mNumWavelengths + wavelength_index] = v;
}

template<typename T>
std::size_t hyperspectral::spectral_major_data<T>::size() const
{
	return mNumPixels * mNumWavelengths;
}

template<typename T>
const std::vector<T>& hyperspectral::spectral_major_data<T>::data() const
{
	return mData;
}

template<typename T>
std::vector<T>& hyperspectral::spectral_major_data<T>::data()
{
	return mData;
}


/////////////////////////////////////////////////////////////////////////////////////
// Spatial Major Dynamically Sized Hyperspectral Data
/////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline hyperspectral::spatial_major_data<T>::spatial_major_data()
	: mNumPixels(0), mNumWavelengths(0)
{
	mData.clear();
}

template<typename T>
inline hyperspectral::spatial_major_data<T>::spatial_major_data(std::size_t num_of_pixels, std::size_t num_of_wavelengths)
	: mNumPixels(num_of_pixels), mNumWavelengths(num_of_wavelengths)
{
	mData.resize(num_of_pixels * num_of_wavelengths);
}

template<typename T>
inline hyperspectral::spatial_major_data<T>::spatial_major_data(const spatial_major_data<T>& m)
	: mData(m.mData), mNumPixels(m.mNumPixels), mNumWavelengths(m.mNumWavelengths)
{
}

template<typename T>
inline bool hyperspectral::spatial_major_data<T>::empty() const
{
	return mData.empty();
}

template<typename T>
inline std::size_t hyperspectral::spatial_major_data<T>::num_wavelengths() const
{
	return mNumWavelengths;
}

template<typename T>
inline std::size_t hyperspectral::spatial_major_data<T>::num_pixels() const
{
	return mNumPixels;
}

template<typename T>
inline void hyperspectral::spatial_major_data<T>::resize(std::size_t num_of_pixels, std::size_t num_of_wavelengths)
{
	mNumPixels = num_of_pixels;
	mNumWavelengths = num_of_wavelengths;
	mData.resize(mNumPixels * mNumWavelengths);
}

template<typename T>
inline void hyperspectral::spatial_major_data<T>::clear()
{
	mNumPixels = 0;
	mNumWavelengths = 0;
	mData.clear();
}

template<typename T>
inline hyperspectral::view<T> hyperspectral::spatial_major_data<T>::pixels(std::size_t wavelength_index) const
{
	assert(wavelength_index < mNumWavelengths);
	return view<T>(&mData[wavelength_index * mNumPixels], &mData[(wavelength_index + 1) * mNumPixels - 1], 1);
}

template<typename T>
inline hyperspectral::span<T> hyperspectral::spatial_major_data<T>::pixels(std::size_t wavelength_index)
{
	assert(wavelength_index < mNumWavelengths);
	return span<T>(&mData[wavelength_index * mNumPixels], &mData[(wavelength_index + 1) * mNumPixels - 1], 1);
}

template<typename T>
inline hyperspectral::view<T> hyperspectral::spatial_major_data<T>::wavelengths(std::size_t pixel_index) const
{
	assert(pixel_index < mNumPixels);
	return view<T>(&mData[pixel_index], &mData[(mNumWavelengths - 1) * mNumPixels + pixel_index], mNumPixels);
}

template<typename T>
inline hyperspectral::span<T> hyperspectral::spatial_major_data<T>::wavelengths(std::size_t pixel_index)
{
	assert(pixel_index < mNumPixels);
	return span<T>(&mData[pixel_index], &mData[(mNumWavelengths - 1) * mNumPixels + pixel_index], mNumPixels);
}

template<typename T>
inline const typename hyperspectral::spatial_major_data<T>::value_type&
	hyperspectral::spatial_major_data<T>::get(std::size_t pixel_index, std::size_t wavelength_index) const
{
	return mData[pixel_index + wavelength_index * mNumPixels];
}

template<typename T>
void hyperspectral::spatial_major_data<T>::set(std::size_t pixel_index, std::size_t wavelength_index, const value_type& v)
{
	mData[pixel_index + wavelength_index * mNumPixels] = v;
}

template<typename T>
std::size_t hyperspectral::spatial_major_data<T>::size() const
{
	return mNumPixels * mNumWavelengths;
}

template<typename T>
const std::vector<T>& hyperspectral::spatial_major_data<T>::data() const
{
	return mData;
}

template<typename T>
std::vector<T>& hyperspectral::spatial_major_data<T>::data()
{
	return mData;
}


/////////////////////////////////////////////////////////////////////////////////////
// Spectral Major Fixed Sized Hyperspectral Data
/////////////////////////////////////////////////////////////////////////////////////
template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline hyperspectral::spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::spectral_major_data_fixed()
{
	zero();
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline hyperspectral::spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::spectral_major_data_fixed
		(const spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>& m)
	: mData(m.mData)
{
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline std::size_t hyperspectral::spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::num_wavelengths() const
{
	return NUM_WAVELENGTHS;
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline std::size_t hyperspectral::spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::num_pixels() const
{
	return NUM_PIXELS;
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline void hyperspectral::spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::zero()
{
	std::fill(mData.begin(), mData.end(), zero_value<T>);
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline void hyperspectral::spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::identity()
{
	assert(NUM_PIXELS == NUM_WAVELENGTHS);

	zero();

	for (std::size_t x = 0; x < NUM_PIXELS; ++x)
	{
		set(x, x, identity_value<T>);
	}
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline hyperspectral::view<T>
	hyperspectral::spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::pixels(std::size_t wavelength_index) const
{
	assert(wavelength_index < NUM_WAVELENGTHS);
	return view<T>(&mData[wavelength_index], &mData[(NUM_PIXELS - 1) * NUM_WAVELENGTHS + wavelength_index], NUM_WAVELENGTHS);
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline hyperspectral::span<T>
	hyperspectral::spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::pixels(std::size_t wavelength_index)
{
	assert(wavelength_index < NUM_WAVELENGTHS);
	return span<T>(&mData[wavelength_index], &mData[(NUM_PIXELS - 1) * NUM_WAVELENGTHS + wavelength_index], NUM_WAVELENGTHS);
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline hyperspectral::view<T>
hyperspectral::spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::wavelengths(std::size_t pixel_index) const
{
	assert(pixel_index < NUM_PIXELS);
	return view<T>(&mData[pixel_index * NUM_WAVELENGTHS], &mData[(pixel_index + 1) * NUM_WAVELENGTHS - 1], 1);
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline hyperspectral::span<T>
	hyperspectral::spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::wavelengths(std::size_t pixel_index)
{
	assert(pixel_index < NUM_PIXELS);
	return span<T>(&mData[pixel_index * NUM_WAVELENGTHS], &mData[(pixel_index + 1) * NUM_WAVELENGTHS - 1], 1);
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline const typename hyperspectral::spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::value_type&
	hyperspectral::spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::get
		(std::size_t pixel_index, std::size_t wavelength_index) const
{
	assert(wavelength_index < NUM_WAVELENGTHS);
	assert(pixel_index < NUM_PIXELS);
	return mData[pixel_index * NUM_WAVELENGTHS + wavelength_index];
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline void hyperspectral::spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::set
	(std::size_t pixel_index, std::size_t wavelength_index, const value_type& v)
{
	assert(wavelength_index < NUM_WAVELENGTHS);
	assert(pixel_index < NUM_PIXELS);
	mData[pixel_index * NUM_WAVELENGTHS + wavelength_index] = v;
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
std::size_t hyperspectral::spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::size() const
{
	return NUM_PIXELS * NUM_WAVELENGTHS;
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
const std::array<T, NUM_PIXELS* NUM_WAVELENGTHS>&
	hyperspectral::spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::data() const
{
	return mData;
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
std::array<T, NUM_PIXELS* NUM_WAVELENGTHS>&
	hyperspectral::spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::data()
{
	return mData;
}


/////////////////////////////////////////////////////////////////////////////////////
// Spatial Major Fixed Sized Hyperspectral Data
/////////////////////////////////////////////////////////////////////////////////////
template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline hyperspectral::spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::spatial_major_data_fixed()
{
	zero();
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline hyperspectral::spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::spatial_major_data_fixed
	(const spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>& m)
	: mData(m.mData)
{
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline std::size_t hyperspectral::spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::num_wavelengths() const
{
	return NUM_WAVELENGTHS;
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline std::size_t hyperspectral::spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::num_pixels() const
{
	return NUM_PIXELS;
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline void hyperspectral::spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::zero()
{
	std::fill(mData.begin(), mData.end(), zero_value<T>);
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline void hyperspectral::spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::identity()
{
	assert(NUM_PIXELS == NUM_WAVELENGTHS);

	zero();

	for (std::size_t c = 0; c < NUM_WAVELENGTHS; ++c)
	{
		set(c, c, identity_value<T>);
	}
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline hyperspectral::view<T>
	hyperspectral::spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::pixels(std::size_t wavelength_index) const
{
	assert(wavelength_index < NUM_WAVELENGTHS);
	return view<T>(&mData[wavelength_index * NUM_PIXELS], &mData[(wavelength_index + 1) * NUM_PIXELS - 1], 1);
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline hyperspectral::span<T>
	hyperspectral::spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::pixels(std::size_t wavelength_index)
{
	assert(wavelength_index < NUM_WAVELENGTHS);
	return span<T>(&mData[wavelength_index * NUM_PIXELS], &mData[(wavelength_index + 1) * NUM_PIXELS - 1], 1);
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline hyperspectral::view<T>
	hyperspectral::spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::wavelengths(std::size_t pixel_index) const
{
	assert(pixel_index < NUM_PIXELS);
	return view<T>(&mData[pixel_index], &mData[(NUM_WAVELENGTHS - 1) * NUM_PIXELS + pixel_index], NUM_PIXELS);
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline hyperspectral::span<T> 
	hyperspectral::spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::wavelengths(std::size_t pixel_index)
{
	assert(pixel_index < NUM_PIXELS);
	return span<T>(&mData[pixel_index], &mData[(NUM_WAVELENGTHS - 1) * NUM_PIXELS + pixel_index], NUM_PIXELS);
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline const typename hyperspectral::spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::value_type&
	hyperspectral::spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::get
		(std::size_t pixel_index, std::size_t wavelength_index) const
{
	assert(wavelength_index < NUM_WAVELENGTHS);
	assert(pixel_index < NUM_PIXELS);
	return mData[pixel_index + wavelength_index * NUM_PIXELS];
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
inline void hyperspectral::spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::set
	(std::size_t pixel_index, std::size_t wavelength_index, const value_type& v)
{
	assert(wavelength_index < NUM_WAVELENGTHS);
	assert(pixel_index < NUM_PIXELS);
	mData[pixel_index + wavelength_index * NUM_PIXELS] = v;
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
std::size_t hyperspectral::spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::size() const
{
	return NUM_PIXELS * NUM_WAVELENGTHS;
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
const std::array<T, NUM_PIXELS* NUM_WAVELENGTHS>& hyperspectral::spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::data() const
{
	return mData;
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
std::array<T, NUM_PIXELS* NUM_WAVELENGTHS>& hyperspectral::spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>::data()
{
	return mData;
}


/******************************************************************************
 * H E L P E R   F U N C T I O N S
******************************************************************************/

template<typename T>
hyperspectral::spectral_major_data<T> hyperspectral::to_spectral_major_data(const hyperspectral::spatial_major_data<T>& m)
{
	if (m.empty()) return spectral_major_data<T>();

	spectral_major_data<T> result(m.num_rows(), m.num_columns());

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
hyperspectral::spatial_major_data<T> hyperspectral::to_spatial_major_data(const hyperspectral::spectral_major_data<T>& m)
{
	if (m.empty()) return spatial_major_data<T>();

	spatial_major_data<T> result(m.num_rows(), m.num_columns());

	for (std::size_t row = 0; row < m.num_rows(); ++row)
	{
		for (std::size_t col = 0; col < m.num_columns(); ++col)
		{
			result.set(row, col, m.get(row, col));
		}
	}

	return result;
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
hyperspectral::spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>
	hyperspectral::to_spectral_major_data(const spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>& m)
{
	if (m.empty()) return spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>();

	spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS> result;

	for (std::size_t col = 0; col < m.num_columns(); ++col)
	{
		for (std::size_t row = 0; row < m.num_pixels(); ++row)
		{
			result.set(row, col, m.get(row, col));
		}
	}

	return result;
}

template<typename T, std::size_t NUM_PIXELS, std::size_t NUM_WAVELENGTHS>
hyperspectral::spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>
	hyperspectral::to_spatial_major_data(const spectral_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>& m)
{
	if (m.empty()) return spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS>();

	spatial_major_data_fixed<T, NUM_PIXELS, NUM_WAVELENGTHS> result;

	for (std::size_t row = 0; row < m.num_pixels(); ++row)
	{
		for (std::size_t col = 0; col < m.num_columns(); ++col)
		{
			result.set(row, col, m.get(row, col));
		}
	}

	return result;
}

template<typename T>
inline std::vector<T> to_vector(const hyperspectral::span<T>& s)
{
	std::size_t n = s.size();
	std::vector<T> result(n);
	for (std::size_t i = 0; i < n; ++i)
		result[i] = s[i];
	return result;
}

