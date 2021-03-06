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

#ifndef __CMONKEYCOMMANDEVENT_H__
#define __CMONKEYCOMMANDEVENT_H__

#include "_CMonkeyCommandEvent.h"
#include "MonkeyCommandEvent.h"

namespace Elastos {
namespace Droid {
namespace Commands {
namespace Monkey {

CarClass(CMonkeyCommandEvent) , public MonkeyCommandEvent
{
public:
    CAR_OBJECT_DECL()

    CARAPI GetEventType(
        /* [out] */ Int32 *eventType);

    CARAPI IsThrottlable(
        /* [out] */ Boolean *isThrottlable);

    CARAPI InjectEvent(
        /* [in] */ IIWindowManager *iwm,
        /* [in] */ IIActivityManager *iam,
        /* [in] */ Int32 verbose,
        /* [out] */ Int32 *result);

    CARAPI constructor(
        /* [in] */ const String& cmd);
};

} // namespace Monkey
} // namespace Commands
} // namespace Droid
} // namespace Elastos

#endif // __CMONKEYCOMMANDEVENT_H__
