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

#ifndef __ELASTOS_DROID_GOOGLE_MMS_UTILITY_DRMCONVERTSESSION_H__
#define __ELASTOS_DROID_GOOGLE_MMS_UTILITY_DRMCONVERTSESSION_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/core/Object.h"

using Elastos::Droid::Content::IContext;
//TODO using Elastos::Droid::Drm::IDrmManagerClient;

namespace Elastos {
namespace Droid {
namespace Google {
namespace Mms {
namespace Utility {

class DrmConvertSession
    : public Object
    , public IDrmConvertSession
{
public:
    CAR_INTERFACE_DECL();

    /**
      * Start of converting a file.
      *
      * @param context The context of the application running the convert session.
      * @param mimeType Mimetype of content that shall be converted.
      * @return A convert session or null in case an error occurs.
      */
    static CARAPI_(AutoPtr<IDrmConvertSession>) Open(
        /* [in] */ IContext* context,
        /* [in] */ const String& mimeType);

    /**
      * Convert a buffer of data to protected format.
      *
      * @param buffer Buffer filled with data to convert.
      * @param size The number of bytes that shall be converted.
      * @return A Buffer filled with converted data, if execution is ok, in all
      *         other case null.
      */
    virtual CARAPI Convert(
        /* [in] */ ArrayOf<Byte>* inBuffer,
        /* [in] */ Int32 size,
        /* [out] */ ArrayOf<Byte>** result);

    /**
      * Ends a conversion session of a file.
      *
      * @param fileName The filename of the converted file.
      * @return Downloads.Impl.STATUS_SUCCESS if execution is ok.
      *         Downloads.Impl.STATUS_FILE_ERROR in case converted file can not
      *         be accessed. Downloads.Impl.STATUS_NOT_ACCEPTABLE if a problem
      *         occurs when accessing drm framework.
      *         Downloads.Impl.STATUS_UNKNOWN_ERROR if a general error occurred.
      */
    virtual CARAPI Close(
        /* [in] */ const String& filename,
        /* [out] */ Int32* result);

private:
    DrmConvertSession(
        /* [in] */ IInterface/*TODO IDrmManagerClient*/* drmClient,
        /* [in] */ Int32 convertSessionId);

private:
    AutoPtr<IInterface/*TODO IDrmManagerClient*/> mDrmClient;
    Int32 mConvertSessionId;
    static const String TAG;
};

} // namespace Utility
} // namespace Mms
} // namespace Google
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GOOGLE_MMS_UTILITY_DRMCONVERTSESSION_H__

