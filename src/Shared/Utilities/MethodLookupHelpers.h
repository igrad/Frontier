#pragma once

#include <QMetaObject>
#include <QMetaMethod>
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

   const QByteArray normalizedSig =
      QMetaObject::normalizedSignature(functionSignature.toStdString().c_str());
   const QString s(normalizedSig);
   return (0 <= obj->metaObject()->indexOfSlot(normalizedSig));
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
