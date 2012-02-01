#ifndef ODF_THUNK_HPP
#define ODF_THUNK_HPP 1

#include <tr1/memory>
#include <iostream>

namespace odf
{

template<typename T, typename Functor>
class ThunkImpl
{
private:
    bool pending_;
    Functor code_;
    T value_;

public:
    ThunkImpl(const Functor code) :
        pending_(true),
        code_(code)
    {
    }

    ThunkImpl(T value) :
        pending_(false),
        value_(value)
    {
    }

    T operator() ()
    {
        if (pending_)
        {
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
    Thunk() :
        content_()
    {
        std::cout << "Constructing empty Thunk     => " << *this
                  << std::endl;
    }

    Thunk(const Functor code) :
        content_(Ptr(new ThunkImpl<T, Functor>(code)))
    {
        std::cout << "Constructing delayed Thunk   => " << *this
                  << std::endl;
    }

    Thunk(T value) :
        content_(Ptr(new ThunkImpl<T, Functor>(value)))
    {
        std::cout << "Constructing immediate Thunk => " << *this
                  << std::endl;
    }

    Thunk(const Thunk& other) :
        content_(other.content_)
    {
        std::cout << "  Copying Thunk " << other << "  => " << *this
                  << std::endl;
    }

    T operator() ()
    {
        return content_->operator()();
    }

    friend std::ostream& operator<<(std::ostream& out, const Thunk& obj)
    {
        out << obj.content_.get();
        return out;
    }
};

template<typename T, typename Functor>
Thunk<T, Functor> makeThunk(const Functor code)
{
    return Thunk<T, Functor>(code);
}

}

#endif // !ODF_THUNK_HPP
