#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <signal/YiSignal.h>

template<typename DataType>
class Message
{

public:

    /*!
        \details Constructs a message with the given data.
    */
    Message(const DataType &rData);

    /*!
        \details Constructs a message with data constructed with the given argument.
    */
    template<typename ArgType>
    Message(const ArgType &rArg);

    /*!
        \details Constructs a message with data constructed with the given arguments.
    */
    template<typename Arg1Type, typename Arg2Type>
    Message(const Arg1Type &rArg1, const Arg2Type &rArg2);

    /*!
        \details Constructs a message with data constructed with the given arguments.
    */
    template<typename Arg1Type, typename Arg2Type, typename Arg3Type>
    Message(const Arg1Type &rArg1, const Arg2Type &rArg2, const Arg3Type &rArg3);

    /*!
        \details Destructor.
    */
    virtual ~Message();

    /*!
        \details Subscribes the given object's function to this message.
    */
    template<typename ObjectType>
    static void Subscribe(ObjectType &rObject, void (ObjectType::*pFunction)(CYISharedPtr<DataType>));

    /*!
        \details Unsubscribes the given object's function from this message.
    */
    template<typename ObjectType>
    static void Unsubscribe(ObjectType &rObject, void (ObjectType::*pFunction)(CYISharedPtr<DataType>));

    /*!
        \details Sends this message to any subscribed objects.
    */
    void Send();

    /*!
        \details The message's data.
    */
    DataType data;

private:

    /*!
        \details Used to deliver message to subscribers.
    */
    static CYISignal<CYISharedPtr<DataType> > SendMessage;

    YI_DISALLOW_COPY_AND_ASSIGN(Message<DataType>);
};

#include "messages/Message_inl.h"

#endif // _MESSAGE_H_

