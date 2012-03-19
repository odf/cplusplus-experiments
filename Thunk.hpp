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


template<typename T>
class AbstractThunkImpl
{
public:
    virtual const T operator() () = 0;
};

template<typename T, typename Functor>
class ThunkImpl : public AbstractThunkImpl<T>
{
private:
    typename std::tr1::shared_ptr<Functor> typedef FunPtr;

    FunPtr code_;
    bool pending_;
    T value_;

public:
    ThunkImpl() :
        code_(),
        pending_(false)
    {
        log << "--Making empty ThunkImpl   " << this << std::endl;
    }

    explicit ThunkImpl(const Functor& code) :
        code_(FunPtr(new Functor(code))),
        pending_(true)
    {
        log << "--Making delayed ThunkImpl " << this << std::endl;
    }

    explicit ThunkImpl(const T& value) :
        code_(),
        pending_(false),
        value_(value)
    {
        log << "--Making direct ThunkImpl  " << this << std::endl;
    }

    ThunkImpl(const ThunkImpl& other) :
        code_(other.code_),
        pending_(other.pending_),
        value_(other.value_)
    {
        log << "----Copying ThunkImpl      " << other << "  => "
            << this << std::endl;
    }

    ~ThunkImpl()
    {
        log << "----Destroying ThunkImpl   " << this << std::endl;
    }

    const T operator() ()
    {
        if (pending_)
        {
            log << "----Forcing ThunkImpl      " << this << std::endl;
            value_ = (*code_)();
            code_ = FunPtr();
            pending_ = false;
        }
        return value_;
    }
};

template<typename T>
class Thunk
{
private:
    typename std::tr1::shared_ptr<AbstractThunkImpl<T> > typedef ThunkPtr;
    typedef T(*FunPtr)();

    ThunkPtr content_;

public:
    Thunk() :
        content_()
    {
    }

    Thunk(const ThunkPtr& code) :
        content_(code)
    {
    }

    explicit Thunk(const FunPtr& code) :
        content_(ThunkPtr(new ThunkImpl<T, FunPtr>(code)))
    {
    }

    explicit Thunk(const T& value) :
        content_(ThunkPtr(new ThunkImpl<T, FunPtr>(value)))
    {
    }

    const T operator() () const
    {
        return content_->operator()();
    }

    bool isEmpty() const
    {
        return content_.get() == 0;
    }

    bool operator==(const Thunk& other) const
    {
        return content_.get() == other.content_.get();
    }
};

template<typename T, typename Functor>
Thunk<T> makeThunk(const Functor& code)
{
    return Thunk<T>(std::tr1::shared_ptr<AbstractThunkImpl<T> >(
                        new ThunkImpl<T, Functor>(code)));
}

}

#endif // !ODF_THUNK_HPP
