#ifndef ODF_THUNK_HPP
#define ODF_THUNK_HPP 1

namespace odf
{

template<typename T, typename Functor>
class Thunk {
private:
    bool pending_;
    union
    {
        Functor code_;
        T value_;
    };

public:
    Thunk() :
        pending_(false),
        value_()
    {
    }

    Thunk(const Functor code) :
        pending_(true),
        code_(code)
    {
    }

    Thunk(T value) :
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
