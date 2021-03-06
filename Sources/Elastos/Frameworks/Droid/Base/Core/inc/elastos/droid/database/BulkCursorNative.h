//=========================================================================
// Copyright (C) 2012 The Elastos Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=========================================================================

#ifndef __ELASTOS_DROID_DATABASE_BULKCURSORNATIVE_H__
#define __ELASTOS_DROID_DATABASE_BULKCURSORNATIVE_H__

#include "Elastos.Droid.Os.h"
#include "Elastos.Droid.App.h"
#include "Elastos.Droid.Content.h"
#include "Elastos.Droid.Database.h"
#include <elastos/core/Object.h>
#include <elastos/droid/ext/frameworkext.h>

using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Database {

class BulkCursorProxy
    : public Object
    , public IBulkCursorProxy
    , public IBulkCursor
{
public:
    CAR_INTERFACE_DECL()

    TO_STRING_IMPL("ContentProviderProxy")

    BulkCursorProxy(
        /* [in] */ IBinder* binder);

    CARAPI AsBinder(
        /* [out] */ IBinder** binder);

    CARAPI GetWindow(
        /* [in] */ Int32 position,
        /* [out] */ ICursorWindow** result);

    CARAPI OnMove(
        /* [in] */ Int32 position);

    CARAPI Deactivate();

    CARAPI Close();

    CARAPI Requery(
        /* [in] */ IIContentObserver* observer,
        /* [out] */ Int32* result);

    CARAPI GetExtras(
        /* [out] */ IBundle** result);

    CARAPI Respond(
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** result);

private:
    AutoPtr<IBulkCursor> mRemote;
};

class BulkCursorNative
    : public Object
    , public IBulkCursor
    , public IBinder
{
public:
    CAR_INTERFACE_DECL()

    TO_STRING_IMPL("BulkCursorNative")

    /**
     * Cast a Binder object into a content resolver interface, generating
     * a proxy if needed.
     */
    static AutoPtr<IBulkCursor> AsInterface(
        /* [in] */ IBinder* obj);

    /**
     * Gets a cursor window that contains the specified position.
     * The window will contain a range of rows around the specified position.
     */
    // CARAPI GetWindow(
    //     /* [in] */ Int32 position,
    //     /* [out] */ ICursorWindow** cursorWindow);

};

} //Database
} //Droid
} //Elastos

#endif // __ELASTOS_DROID_DATABASE_BULKCURSORNATIVE_H__
