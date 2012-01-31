#ifndef ODF_THUNK_HPP
#define ODF_THUNK_HPP 1

namespace odf
{

template<typename T, typename Functor>
class Thunk {
private:
    const Functor code_;
    bool pending_;
    T value_;

public:
    Thunk() :
        code_(),
        pending_(false),
        value_()
    {
    }

    Thunk(const Functor code) :
        code_(code),
        pending_(true),
        value_()
    {
    }

    Thunk(T value) :
        code_(),
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
Thunk<T, Functor> makeThunk(const Functor code)
{
    return Thunk<T, Functor>(code);
}

}

#endif // !ODF_THUNK_HPP
