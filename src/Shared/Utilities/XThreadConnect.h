#pragma once

#include <Log.h>

#include <QObject>

// A templated wrapper for cross-thread objects to help ensure that objects on another thread are
// not directly called across threads, and to ensure proper Qt connections can be made.

template <typename T,
         typename std::enable_if<std::is_base_of<QObject, T>::value>::type* = nullptr>
struct XThread
{
   XThread(T* obj)
      : Obj(obj)
   {};

   // Prevents a clang warning about bitwise OR (|) op on these connection types
   // NOLINTNEXTLINE
   static const Qt::ConnectionType UniqueQueuedConn = static_cast<Qt::ConnectionType>(
      Qt::UniqueConnection | Qt::QueuedConnection);

   template <typename... Args>
   QMetaObject::Connection connect(Args... args)
   {
      if(nullptr == Obj)
      {
         LogError("XThread::Obj is null at time of connection!");
         return QMetaObject::Connection();
      }

      return QObject::connect(Obj, args..., UniqueQueuedConn);
   }

   bool operator==(const T& rhs) const
   {
      return (nullptr != Obj) && (*Obj == rhs);
   }

   bool operator==(const T* rhs) const
   {
      return Obj == rhs;
   }

   bool isNull() const
   {
      return (nullptr == Obj);
   }

   T* Obj;
};
