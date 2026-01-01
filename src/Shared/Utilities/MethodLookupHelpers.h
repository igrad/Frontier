#pragma once

#include <QMetaObject>
#include <QObject>

// Returns true if the provided QObject contains a declaration for the (fuzzily) normalized
// function signature provided. The signature will be normalized before the check.
inline bool QObjectHasMethodDeclared(const QObject* obj,
                                     const QString& functionSignature)
{
   if((nullptr == obj) || functionSignature.isEmpty())
   {
      return false;
   }

   const char* const normalizedSig = QMetaObject::normalizedSignature(functionSignature.toUtf8());
   return (0 <= obj->metaObject()->indexOfMethod(normalizedSig));
}

inline bool QObjectHasMethodDeclared(const QObject* obj,
                                     const std::string& functionSignature)
{
   return QObjectHasMethodDeclared(obj, QString::fromStdString(functionSignature));
}

inline bool QObjectHasMethodDeclared(const QObject* obj,
                                     const char* const functionSignature)
{
   return QObjectHasMethodDeclared(obj, QString(functionSignature));
}
