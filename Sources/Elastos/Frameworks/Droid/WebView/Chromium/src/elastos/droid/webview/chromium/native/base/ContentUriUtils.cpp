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

#include "Elastos.Droid.Content.h"
#include "Elastos.Droid.Database.h"
#include "Elastos.Droid.Net.h"
#include "Elastos.Droid.Os.h"
#include "Elastos.CoreLibrary.IO.h"
#include "elastos/droid/webkit/webview/chromium/native/base/ContentUriUtils.h"
#include "elastos/droid/webkit/webview/chromium/native/base/api/ContentUriUtils_dec.h"

using Elastos::Droid::Net::CUriHelper;
using Elastos::Droid::Net::IUriHelper;
using Elastos::IO::ICloseable;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {
namespace Base {

const String ContentUriUtils::TAG("ContentUriUtils");

// Prevent instantiation.
ContentUriUtils::ContentUriUtils()
{
}

/**
 * Opens the content URI for reading, and returns the file descriptor to
 * the caller. The caller is responsible for closing the file desciptor.
 *
 * @param context {@link Context} in interest
 * @param uriString the content URI to open
 * @returns file desciptor upon sucess, or -1 otherwise.
 */
//@CalledByNative
Int32 ContentUriUtils::OpenContentUriForRead(
    /* [in] */ IContext* context,
    /* [in] */ const String& uriString)
{
    AutoPtr<IParcelFileDescriptor> pfd = GetParcelFileDescriptor(context, uriString);
    if (pfd != NULL) {
        Int32 fd;
        pfd->DetachFd(&fd);
        return fd;
    }
    return -1;
}

/**
 * Check whether a content URI exists.
 *
 * @param context {@link Context} in interest.
 * @param uriString the content URI to query.
 * @returns true if the uri exists, or false otherwise.
 */
//@CalledByNative
Boolean ContentUriUtils::ContentUriExists(
    /* [in] */ IContext* context,
    /* [in] */ const String& uriString)
{
    AutoPtr<IParcelFileDescriptor> pfd = GetParcelFileDescriptor(context, uriString);
    if (pfd == NULL) {
        return FALSE;
    }
    return TRUE;
}

/**
 * Helper method to open a content URI and return the ParcelFileDescriptor.
 *
 * @param context {@link Context} in interest.
 * @param uriString the content URI to open.
 * @returns ParcelFileDescriptor of the content URI, or NULL if the file does not exist.
 */
AutoPtr<IParcelFileDescriptor> ContentUriUtils::GetParcelFileDescriptor(
    /* [in] */ IContext* context,
    /* [in] */ const String& uriString)
{
    AutoPtr<IContentResolver> resolver;
    context->GetContentResolver((IContentResolver**)&resolver);
    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    AutoPtr<IUri> uri;
    helper->Parse(uriString, (IUri**)&uri);

    AutoPtr<IParcelFileDescriptor> pfd;
    //try {
        resolver->OpenFileDescriptor(uri, String("r"), (IParcelFileDescriptor**)&pfd);
    //} catch (java.io.FileNotFoundException e) {
    //    Log.w(TAG, "Cannot find content uri: " + uriString, e);
    //}
    return pfd;
}

/**
 * Method to resolve the display name of a content URI.
 *
 * @param uri the content URI to be resolved.
 * @param contentResolver the content resolver to query.
 * @param columnField the column field to query.
 * @returns the display name of the @code uri if present in the database
 *  or an empty string otherwise.
 */
String ContentUriUtils::GetDisplayName(
    /* [in] */ IUri* uri,
    /* [in] */ IContentResolver* contentResolver,
    /* [in] */ const String& columnField)
{
    if (contentResolver == NULL || uri == NULL) {
        return String("");
    }

    AutoPtr<ICursor> cursor;
    //try {
        contentResolver->Query(uri, NULL, String(NULL), NULL, String(NULL), (ICursor**)&cursor);

        Int32 count;
        if (cursor != NULL && (cursor->GetCount(&count), count >= 1)) {
            Boolean bFirst;
            cursor->MoveToFirst(&bFirst);
            Int32 index;
            cursor->GetColumnIndex(columnField, &index);
            if (index > -1) {
                String strRet;
                cursor->GetString(index, &strRet);
                if (cursor != NULL) ICloseable::Probe(cursor)->Close();
                return strRet;
            }
        }
    //} catch (NullPointerException e) {
        // Some android models don't handle the provider call correctly.
        // see crbug.com/345393
    //    return "";
    //} finally {
    if (cursor != NULL) ICloseable::Probe(cursor)->Close();
    //}
    return String("");
}

Int32 ContentUriUtils::OpenContentUriForRead(
    /* [in] */ IInterface* context,
    /* [in] */ const String& uriString)
{
    AutoPtr<IContext> c = IContext::Probe(context);
    return OpenContentUriForRead(c, uriString);
}

Boolean ContentUriUtils::ContentUriExists(
    /* [in] */ IInterface* context,
    /* [in] */ const String& uriString)
{
    AutoPtr<IContext> c = IContext::Probe(context);
    return ContentUriExists(c, uriString);
}

} // namespace Base
} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
