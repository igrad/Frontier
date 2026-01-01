#pragma once

#include <QObject>

// A templated wrapper for cross-thread pointers to help ensure that objects on another thread are
// not directly called across threads, and to ensure proper Qt connections can be made.
// XPtr is not currently usable in cross-thread signal/data args or with QVariants since
// it is not registered with the QMetaObject system (because doing so with templates is ugly).
// Once the cross-thread args are passed into the current thread, wrap them with XPtrs
// before passing them around within the same thread. To pass to another thread, simply pass the
// raw Object pointer within.
template <typename T>
struct XPtr
{
   explicit XPtr(T* object = nullptr)
      : Object(object)
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

   T* Object = nullptr;
};
