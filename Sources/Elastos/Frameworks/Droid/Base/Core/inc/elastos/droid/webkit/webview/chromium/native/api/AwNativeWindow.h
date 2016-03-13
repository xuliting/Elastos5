#ifndef __ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_NATIVE_API_AWBYTEBUFFER_H__
#define __ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_NATIVE_API_AWBYTEBUFFER_H__

#include "elastos/droid/ext/frameworkext.h"
#include <android/native_window.h>

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {
namespace Api {

class AwNativeWindow
{
public:
    static CARAPI_(void*) ElaNativeWindowCallback_Init();

private:
    static CARAPI_(ANativeWindow*) GetFromSurface(
        /* [in] */ IInterface* obj);
};

} // namespace Api
} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_NATIVE_API_AWBYTEBUFFER_H__
