#ifndef _FUNCTOR_UTILITIES_H_
#define _FUNCTOR_UTILITIES_H_

namespace Utility
{
    /*!
        \details An abstract base class for a simple pointer to member function wrapper.
    */
    struct Delegate
    {
        virtual ~Delegate() {}
        virtual void operator()() = 0;
    };

    /*!
        \details The concrete implementation of Delegate specific to the given type.
    */
    template<typename Type>
    struct TypedDelegate : public Delegate
    {
        typedef void (Type::*FunctionPtr)();

        TypedDelegate(Type &object, FunctionPtr pFunction)
            : m_object(object)
            , m_pFunction(pFunction)
        {
        }

        virtual void operator()()
        {
            (m_object.*m_pFunction)();
        }

    private:
        Type &m_object;
        FunctionPtr m_pFunction;
    };

    /*!
        \details A default deleter for pointers and containers of pointers.
    */
    struct DefaultDelete
    {
        template<typename Type>
        void operator()(Type *&pPointer)
        {
            delete pPointer;
            pPointer = YI_NULL;
        }

        template<typename KeyType, typename ValueType>
        void operator()(std::pair<const KeyType, ValueType> &map)
        {
            delete map.second;
            map.second = YI_NULL;
        }
    };
}

#endif // _FUNCTOR_UTILITIES_H_

