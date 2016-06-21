#ifndef __ELASTOS_DROID_INTERNAL_TELEPHONY_CDMA_CCDMALTESERVICESTATETRACKER_H__
#define __ELASTOS_DROID_INTERNAL_TELEPHONY_CDMA_CCDMALTESERVICESTATETRACKER_H__

#include "_Elastos_Droid_Internal_Telephony_Cdma_CCdmaLteServiceStateTracker.h"
#include "elastos/droid/internal/telephony/cdma/CdmaLteServiceStateTracker.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Telephony {
namespace Cdma {

CarClass(CCdmaLteServiceStateTracker)
    , public CdmaLteServiceStateTracker
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Cdma
} // namespace Telephony
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_TELEPHONY_CDMA_CCDMALTESERVICESTATETRACKER_H__
