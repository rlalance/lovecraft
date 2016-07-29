#ifndef _MESSAGE_INL_H_
#define _MESSAGE_INL_H_

template<typename DataType>
CYISignal<CYISharedPtr<DataType> > Message<DataType>::SendMessage;

template<typename DataType>
Message<DataType>::Message(const DataType &rData)
    : data(rData)
{
}

template<typename DataType>
template<typename ArgType>
Message<DataType>::Message(const ArgType &rArg) :
    data(rArg)
{
}

template<typename DataType>
template<typename Arg1Type, typename Arg2Type>
Message<DataType>::Message(const Arg1Type &rArg1, const Arg2Type &rArg2) :
    data(rArg1, rArg2)
{
}

template<typename DataType>
template<typename Arg1Type, typename Arg2Type, typename Arg3Type>
Message<DataType>::Message(const Arg1Type &rArg1, const Arg2Type &rArg2, const Arg3Type &rArg3) :
    data(rArg1, rArg2, rArg3)
{
}

template<typename DataType>
Message<DataType>::~Message()
{
}

template<typename DataType>
template<typename ObjectType>
void Message<DataType>::Subscribe(ObjectType &rObject, void (ObjectType::*pFunction)(CYISharedPtr<DataType>))
{
    SendMessage.Connect(rObject, pFunction);
}

template<typename DataType>
template<typename ObjectType>
void Message<DataType>::Unsubscribe(ObjectType &rObject, void (ObjectType::*pFunction)(CYISharedPtr<DataType>))
{
    SendMessage.Disconnect(rObject, pFunction);
}

template<typename DataType>
void Message<DataType>::Send()
{
    SendMessage.Emit(CYISharedPtr<DataType>(new DataType(data)));
}

#endif // _MESSAGE_INL_H_

