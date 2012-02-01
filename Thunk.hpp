#ifndef ODF_THUNK_HPP
#define ODF_THUNK_HPP 1

#include <iostream>

namespace odf
{

template<typename T, typename Functor>
class Thunk {
private:
    bool pending_;
    Functor code_;
    T value_;
    int id_;

    size_t makeId()
    {
        static size_t next_id = 0;
        return ++next_id;
    }

public:
    Thunk() :
        pending_(false),
        value_(),
        id_(makeId())
    {
    }

    Thunk(const Functor code) :
        pending_(true),
        code_(code),
        id_(makeId())
    {
    }

    Thunk(T value) :
        pending_(false),
        value_(value),
        id_(makeId())
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

    friend std::ostream& operator<<(std::ostream& out, Thunk& obj)
    {
        out << "Thunk{id=" << obj.id_ << "}(";
        if (obj.pending_)
            out << "...";
        else
            out << obj();
        out << ")";
        
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
