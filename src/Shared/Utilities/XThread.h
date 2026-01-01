#pragma once

#include <QObject>

// A templated wrapper for cross-thread objects to help ensure that objects on another thread are
// not directly called across threads, and to ensure proper Qt connections can be made.
template <typename T>
struct XThread
{
   XThread(T* object)
      : Object(object)
   {}

   XThread(const T& object)
      : Object(&object)
   {}

   inline bool isNull() const
   {
      return (nullptr == Object);
   }

   inline const T* get() const
   {
      return Object;
   }

   [[deprecated("This object lives on another thread!")]]
   inline T* operator->() const
   {
      return Object;
   }

   T* Object;
};
