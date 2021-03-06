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

#ifndef __ELASTOS_DROID_TRANSITION_VISIBILITYPROPAGATION_H__
#define __ELASTOS_DROID_TRANSITION_VISIBILITYPROPAGATION_H__

#include "elastos/droid/ext/frameworkdef.h"
#include "Elastos.Droid.Transition.h"
#include <elastos/core/Object.h>

namespace Elastos {
namespace Droid {
namespace Transition {

/**
 * Base class for <code>TransitionPropagation</code>s that care about
 * View Visibility and the center position of the View.
 */
class VisibilityPropagation
    : public Object
    , public IVisibilityPropagation
    , public ITransitionPropagation
{
public:
    CAR_INTERFACE_DECL()

    VisibilityPropagation();

    CARAPI CaptureValues(
        /* [in] */ ITransitionValues* values);

    CARAPI GetPropagationProperties(
        /* [out] */ ArrayOf<String>** result);

    /**
     * Returns {@link android.view.View#getVisibility()} for the View at the time the values
     * were captured.
     * @param values The TransitionValues captured at the start or end of the Transition.
     * @return {@link android.view.View#getVisibility()} for the View at the time the values
     * were captured.
     */
    CARAPI GetViewVisibility(
        /* [in] */ ITransitionValues* values,
        /* [out] */ Int32* result);

    /**
     * Returns the View's center x coordinate, relative to the screen, at the time the values
     * were captured.
     * @param values The TransitionValues captured at the start or end of the Transition.
     * @return the View's center x coordinate, relative to the screen, at the time the values
     * were captured.
     */
    CARAPI GetViewX(
        /* [in] */ ITransitionValues* values,
        /* [out] */ Int32* result);

    /**
     * Returns the View's center y coordinate, relative to the screen, at the time the values
     * were captured.
     * @param values The TransitionValues captured at the start or end of the Transition.
     * @return the View's center y coordinate, relative to the screen, at the time the values
     * were captured.
     */
    CARAPI GetViewY(
        /* [in] */ ITransitionValues* values,
        /* [out] */ Int32* result);

private:
    static CARAPI_(Int32) GetViewCoordinate(
        /* [in] */ ITransitionValues* values,
        /* [in] */ Int32 coordinateIndex);

private:
    /**
     * The property key used for {@link android.view.View#getVisibility()}.
     */
    static const String PROPNAME_VISIBILITY;

    /**
     * The property key used for the center of the View in screen coordinates. This is an
     * int[2] with the index 0 taking the x coordinate and index 1 taking the y coordinate.
     */
    static const String PROPNAME_VIEW_CENTER;

    static AutoPtr<ArrayOf<String> > VISIBILITY_PROPAGATION_VALUES;
};

} // namespace Transition
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_TRANSITION_VISIBILITYPROPAGATION_H__
