#ifndef ODF_THUNK_HPP
#define ODF_THUNK_HPP 1

#include <tr1/memory>
#include <iostream>

namespace odf
{

#ifdef DEBUG
std::ostream& log = std::cout;
#else
#include "nullstream.hpp"

odf::nullstream log;
#endif


template<typename T, typename Functor>
class ThunkImpl
{
private:
    bool pending_;
    Functor code_;
    T value_;

public:
    ThunkImpl() :
        pending_(false)
    {
        log << "Making empty ThunkImpl   => " << this << std::endl;
    }

    ThunkImpl(const Functor code) :
        pending_(true),
        code_(code)
    {
        log << "Making delayed ThunkImpl => " << this << std::endl;
    }

    ThunkImpl(T value) :
        pending_(false),
        value_(value)
    {
        log << "Making direct ThunkImpl  => " << this << std::endl;
    }

    ThunkImpl(const ThunkImpl& other) :
        pending_(other.pending_),
        code_(other.code_),
        value_(other.value_)
    {
        log << "  Copying ThunkImpl " << other << "  => "
            << this << std::endl;
    }

    ~ThunkImpl()
    {
        log << "  Destroying ThunkImpl      " << this << std::endl;
    }

    T operator() ()
    {
        if (pending_)
        {
            log << "  Forcing ThunkImpl         " << this << std::endl;
            value_ = code_();
            pending_ = false;
        }
        return value_;
    }
};

template<typename T, typename Functor>
class Thunk
{
private:
    typename std::tr1::shared_ptr<ThunkImpl<T, Functor> > typedef Ptr;

    Ptr content_;

public:
    Thunk(const Functor code) :
        content_(Ptr(new ThunkImpl<T, Functor>(code)))
    {
    }

    Thunk(T value) :
        content_(Ptr(new ThunkImpl<T, Functor>(value)))
    {
    }

    T operator() ()
    {
        return content_->operator()();
    }
};

template<typename T, typename Functor>
Thunk<T, Functor> makeThunk(const Functor code)
{
    return Thunk<T, Functor>(code);
}

}

#endif // !ODF_THUNK_HPP
