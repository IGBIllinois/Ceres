
#pragma once


//////////////////////////////////////////////////////////////////////////////
/**
//  \name  bound input to a givin range
*/
//////////////////////////////////////////////////////////////////////////////

template<typename T>
T bound(T input, T min, T max)
{
    if (input < min) return min;
    if (input > max) return max;
    return input;
}


//////////////////////////////////////////////////////////////////////////////
/**
//  \name  edge_detect template
*/
//////////////////////////////////////////////////////////////////////////////
//@{
/**
//  \brief Performs edge detection
//
//  \par  Template Function
//        Implemented as a template to work with any variable type.  The template
//        overloads the basic operators so that the template behaves like the
//        underlying variable type.  The template uses a traits template to
//        control the threshold at which a rising or falling edge is detected.
//
//  \verbatim
//        The following is usage of the edge_detect template:
//                                            
//        edge_detect<bool>   b;
//                                            
//        b = false;
//        if ( !b )
//           cout << b.IsFalling() << endl;
//
//
//  \endverbatim
//
//  \param typename  - The type of the value.
//  \param traits    - A traits class to determine threshold values for the rising and
//                     falling edges.
//
*/

/**
*  \template edge_traits template
*
*  \brief    Implements default behavior for edge detection
*/
template<typename T>
struct edge_traits
{
    static bool lt(const T& current, const T& previous)  { return (current < previous); };
    static bool gt(const T& current, const T& previous)  { return (current > previous); };
    static bool eq(const T& current, const T& previous)  { return (current == previous); };
};

/**
*  \template edge_traits<bool> template
*
*  \brief    Implements edge detection for boolean values
*/
template<>
struct edge_traits<bool>
{
    static bool lt(const bool& current, const bool& previous) { return (!current && previous); };
    static bool gt(const bool& current, const bool& previous) { return (current && !previous); };
    static bool eq(const bool& current, const bool& previous) { return (current == previous); };
};

/**
*  \template edge_detect<typename TYPE, class TRAITS> template
*
*  \brief    Implements edge detection for a variable of TYPE, using the traits
*            from TRAITS.
*/
template<typename T, class Tr = edge_traits<T> >
class edge_detect 
{
public:
    typedef edge_detect<T, Tr>   _Myt;
    typedef T                    value_type;
    typedef Tr                   traits_type;

    edge_detect() : current_value(), previous_value() {}

    edge_detect(const T& x) : current_value(x), previous_value(x) {}

    edge_detect(const _Myt& X) : current_value(X.current_value), previous_value(X.previous_value) {}

    _Myt& operator=(const _Myt& rhs)
    { 
        if ( &rhs == this) return (*this);
        current_value = rhs.current_value;
        previous_value = rhs.previous_value;
        return (*this);
    }
            
    _Myt& operator=(const T& value)
    {
        previous_value = current_value; 
        current_value = value; 
        return (*this);
    }
    
//    operator T()        { return current_value; }
    operator T() const  { return current_value; }
                
     bool IsRising() const   { return Tr::gt(current_value, previous_value); }
     bool IsFalling() const  { return Tr::lt(current_value, previous_value); }
     bool HasChanged() const { return !Tr::eq(current_value,previous_value); }

     // The data method will bypass the edge-detection capabilities of this class.
     // Use it with caution!
     T& data() { return current_value; }
     
     // The previous method will bypass the edge-detection capabilities of this class.
     // Use it with caution!
     T& previous()              { return previous_value; }
     void previous(const T& e)  { previous_value = e; }

/*
     std::istream& operator>>(std::istream& is)
     {
        previous_value = current_value; 
        is >> current_value;
        return is;
     }

     std::ostream& operator<<(std::ostream& os)
     {
        os << current_value;
        return os;
     }
*/

protected:
    T current_value;
    T previous_value;
};

/*
template< typename T, class Tr >
std::istream& operator>>(std::istream& is, edge_detect< T, Tr >& ed)
{
    return ed >> is;
}

template< typename T, class Tr >
std::ostream& operator<<(std::ostream& os, edge_detect< T, Tr >& ed)
{
    return ed << os;
}
*/



//////////////////////////////////////////////////////////////////////////////
/**
//  \name  comparator template
*/
//////////////////////////////////////////////////////////////////////////////
//@{
/**
//  \brief Implements a basic comparator
//
//  \par  Template Function
//        Implemented as a template to work with any variable type.  The template
//        overloads the basic operators so that the template behaves like the
//        underlying variable type.  The template uses a traits template to
//        control the threshold at which a rising or falling edge is detected.
//
//  \verbatim
//        The following is usage of the edge_detect template:
//
//        comparator<bool>   b;
//
//        b = false;
//        if ( !b )
//           cout << b.IsFalling() << endl;
//
//
//  \endverbatim
//
//  \param typename  - The type of the value.
//  \param traits    - A traits class to determine threshold values for the rising and
//                      falling edges.
//
*/


/**
*  \template comparator_traits template
*
*  \brief    Implements default behavior for comparator
*/
template<typename T>
struct comparator_traits
{
    static bool lower_threshold(const T& current, const T& previous) { return (current < previous); };
    static bool upper_threshold(const T& current, const T& previous) { return (current > previous); };
    static bool within_threshold(const T& current, const T& previous) { return (previous <= current) && (current <= previous); };
    static bool outside_threshold(const T& current, const T& previous) { return (current < previous) || (current > previous); };
};

/**
*  \template comparator_traits<bool> template
*
*  \brief    Implements comparator for boolean values
*/
template<>
struct comparator_traits<bool>
{
    static bool lower_threshold(const bool& current, const bool& previous) { return (!current && previous); };
    static bool upper_threshold(const bool& current, const bool& previous) { return (current && !previous); };
    static bool within_threshold(const bool& current, const bool& previous) { return (current == previous); };
    static bool outside_threshold(const bool& current, const bool& previous) { return (current != previous); };
};


/**
*  \template comparator<typename TYPE, class TRAITS> template
*
*  \brief    Implements a "window comparator" or "window detector" for a 
*            variable of TYPE, using the traits from TRAITS.
*/
template<typename T, class Tr = comparator_traits<T> >
class comparator
{
public:
    typedef comparator<T, Tr>   _Myt;
    typedef T                    value_type;
    typedef Tr                   traits_type;

    comparator() : current_value(), reference_value(), previous_reference()
    {}

    comparator(const T& x) : current_value(x), reference_value(x), previous_reference(x)
    {}

    comparator(const _Myt& X) : current_value(X.current_value), 
        reference_value(X.reference_value), previous_reference(X.previous_reference)
    {}

    _Myt& operator=(const _Myt& rhs)
    {
        if (&rhs == this) return (*this);
        current_value = rhs.current_value;
        previous_reference = rhs.previous_reference;
        return (*this);
    }

    _Myt& operator=(const T& value)
    {
        previous_reference = reference_value;
        if (Tr::outside_threshold(value, reference_value))
            reference_value = value;
        current_value = value;
        return (*this);
    }

    operator T() const { return current_value; }

    bool IsRising() const   { return Tr::upper_threshold(current_value, previous_reference); }
    bool IsFalling() const  { return Tr::lower_threshold(current_value, previous_reference); }
    bool HasChanged() const { return Tr::outside_threshold(current_value, previous_reference); }

    void reset()
    {
        previous_reference = reference_value = current_value;
    }

    void set(const T& value)
    {
        previous_reference = reference_value = current_value = value;
    }

    // The data method will bypass the comparator capabilities of this class.
    // Use it with caution!
    T& data() { return current_value; }

    // The previous method will bypass the edge-detection capabilities of this class.
    // Use it with caution!
    T& reference() { return reference_value; }
    void reference(const T& e) { reference_value = e; }

    /*
         std::istream& operator>>(std::istream& is)
         {
            previous_value = current_value;
            is >> current_value;
            return is;
         }

         std::ostream& operator<<(std::ostream& os)
         {
            os << current_value;
            return os;
         }
    */

protected:
    T current_value;
    T reference_value;
    T previous_reference;
};

