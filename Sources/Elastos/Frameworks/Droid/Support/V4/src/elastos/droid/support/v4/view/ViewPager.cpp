
#include "elastos/droid/support/v4/view/ViewPager.h"
#include "elastos/droid/support/v4/view/ViewConfigurationCompat.h"
#include "elastos/droid/support/v4/view/ViewCompat.h"
#include "elastos/droid/support/v4/view/CViewPagerSavedState.h"
#include "elastos/droid/support/v4/widget/CEdgeEffectCompat.h"
#include "elastos/droid/os/Build.h"
#include "elastos/droid/os/SystemClock.h"
#include "elastos/droid/R.h"
#include <elastos/core/Math.h>
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Support::V4::View::EIID_IViewPager;
using Elastos::Droid::Support::V4::View::EIID_IViewPagerOnPageChangeListener;
using Elastos::Droid::Support::V4::View::EIID_IViewPagerSavedState;
using Elastos::Droid::Support::V4::Widget::CEdgeEffectCompat;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::View::IViewConfiguration;
using Elastos::Droid::View::IViewConfigurationHelper;
using Elastos::Droid::View::CViewConfigurationHelper;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::IVelocityTrackerHelper;
using Elastos::Droid::View::CVelocityTrackerHelper;
using Elastos::Droid::View::IMotionEventHelper;
using Elastos::Droid::View::CMotionEventHelper;
using Elastos::Droid::Widget::CScroller;
using Elastos::Core::StringUtils;
using Elastos::Utility::ICollections;
using Elastos::Utility::CCollections;
using Elastos::Utility::CArrayList;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Support {
namespace V4 {
namespace View {

//=================================================================
// ViewPager::ItemInfoComparator
//=================================================================

CAR_INTERFACE_IMPL(ViewPager::ItemInfoComparator, Object, IComparator)

ECode ViewPager::ItemInfoComparator::Compare(
    /* [in] */  IInterface* a,
    /* [in] */  IInterface* b,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<ItemInfo> lhs = (ItemInfo*)(IObject*)a;
    AutoPtr<ItemInfo> rhs = (ItemInfo*)(IObject*)b;
    *result = lhs->mPosition - rhs->mPosition;
    return NOERROR;
}


//=================================================================
// ViewPager::MyInterpolator
//=================================================================

CAR_INTERFACE_IMPL(ViewPager::MyInterpolator, Object, IInterpolator)

ECode ViewPager::MyInterpolator::GetInterpolation(
    /* [in] */ Float t,
    /* [out] */ Float* interpolation)
{
    VALIDATE_NOT_NULL(interpolation)
    t -= 1.0f;
    *interpolation = t * t * t * t * t + 1.0f;
    return NOERROR;
}


//=================================================================
// ViewPager::EndScrollRunnable
//=================================================================

ECode ViewPager::EndScrollRunnable::Run()
{
    mHost->SetScrollState(IViewPager::SCROLL_STATE_IDLE);
    mHost->Populate();
    return NOERROR;
}


//=================================================================
// ViewPager::SimpleOnPageChangeListener
//=================================================================

CAR_INTERFACE_IMPL(ViewPager::SimpleOnPageChangeListener, Object, IViewPagerOnPageChangeListener)

ECode ViewPager::SimpleOnPageChangeListener::OnPageScrolled(
    /* [in] */ Int32 position,
    /* [in] */ Float positionOffset,
    /* [in] */ Int32 positionOffsetPixels)
{
    // This space for rent
    return NOERROR;
}

ECode ViewPager::SimpleOnPageChangeListener::OnPageSelected(
    /* [in] */ Int32 position)
{
    // This space for rent
    return NOERROR;
}

ECode ViewPager::SimpleOnPageChangeListener::OnPageScrollStateChanged(
    /* [in] */ Int32 state)
{
    // This space for rent
    return NOERROR;
}


//=================================================================
// ViewPager::SavedState
//=================================================================

CAR_INTERFACE_IMPL(ViewPager::SavedState, BaseSavedState, IViewPagerSavedState)

ECode ViewPager::SavedState::WriteToParcel(
    /* [in] */ IParcel* out)
{
    BaseSavedState::WriteToParcel(out);
    out->WriteInt32(mPosition);
    out->WriteInterfacePtr(mAdapterState);
    return NOERROR;
}

ECode ViewPager::SavedState::ReadFromParcel(
    /* [in] */ IParcel* in)
{
    BaseSavedState::ReadFromParcel(source);
    source->ReadInt32(&mPosition);
    source->ReadInterfacePtr((Handle32*)(IInterface**)&temp);
    mAdapterState = IParcelable::Probe(temp)
    return NOERROR;
}

ECode ViewPager::SavedState::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    AutoPtr<ISystem> system;
    CSystem::AcquireSingleton((ISystem**)&system);
    Int32 hashCode;
    system->IdentityHashCode((IViewPager*)this, &hashCode);
    *str = String("FragmentPager.SavedState{") + StringUtils::ToHexString(hashCode)
            + " position=" + StringUtils::ToString(mPosition) + "}";
    return NOERROR;
}


//=================================================================
// ViewPager
//=================================================================

const String ViewPager::TAG("ViewPager");
const Boolean ViewPager::DEBUG;
const Boolean ViewPager::USE_CACHE;
const Int32 ViewPager::DEFAULT_OFFSCREEN_PAGES;
const Int32 ViewPager::MAX_SETTLE_DURATION;
const Int32 ViewPager::MIN_DISTANCE_FOR_FLING;
const Int32 ViewPager::DEFAULT_GUTTER_SIZE;
const Int32 ViewPager::MIN_FLING_VELOCITY;

static AutoPtr<ArrayOf<Int32> > InitLayoutAttrs()
{
    AutoPtr<ArrayOf<Int32> > attrs = ArrayOf<Int32>::Alloc(1);
    (*attrs)[0] = R::attr::layout_gravity;
    return attrs;
}
const AutoPtr<ArrayOf<Int32> > ViewPager::LAYOUT_ATTRS = InitLayoutAttrs();

AutoPtr<IComparator> ViewPager::InitComparator()
{
    return (IComparator*)new ItemInfoComparator();
}
const AutoPtr<IComparator> ViewPager::COMPARATOR = ViewPager::InitComparator();

AutoPtr<IInterpolator> ViewPager::InitInterpolator()
{
    return (IInterpolator*)new MyInterpolator();
}
const AutoPtr<IInterpolator> ViewPager::sInterpolator = ViewPager::InitInterpolator();
const Int32 ViewPager::INVALID_POINTER;
const Int32 ViewPager::CLOSE_ENOUGH;
const Int32 ViewPager::DRAW_ORDER_DEFAULT;
const Int32 ViewPager::DRAW_ORDER_FORWARD;
const Int32 ViewPager::DRAW_ORDER_REVERSE;

AutoPtr<ViewPositionComparator> ViewPager::InitVPComparator()
{
    return new ViewPositionComparator();
}
const AutoPtr<ViewPositionComparator> ViewPager::sPositionComparator = ViewPager::InitVPComparator();

ViewPager::ViewPager()
    : mExpectedAdapterCount(0)
    , mCurItem(0)
    , mRestoredCurItem(-1)
    , mPageMargin(0)
    , mTopPageBounds(0)
    , mBottomPageBounds(0)
    , mFirstOffset(-Elastos::Core::Math::FLOAT_MAX_VALUE)
    , mLastOffset(Elastos::Core::Math::FLOAT_MAX_VALUE)
    , mChildWidthMeasureSpec(0)
    , mChildHeightMeasureSpec(0)
    , mInLayout(FALSE)
    , mScrollingCacheEnabled(FALSE)
    , mPopulatePending(FALSE)
    , mOffscreenPageLimit(DEFAULT_OFFSCREEN_PAGES)
    , mIsBeingDragged(FALSE)
    , mIsUnableToDrag(FALSE)
    , mIgnoreGutter(FALSE)
    , mDefaultGutterSize(0)
    , mGutterSize(0)
    , mTouchSlop(0)
    , mLastMotionX(0)
    , mLastMotionY(0)
    , mInitialMotionX(0)
    , mInitialMotionY(0)
    , mActivePointerId(INVALID_POINTER)
    , mMinimumVelocity(0)
    , mMaximumVelocity(0)
    , mFlingDistance(0)
    , mCloseEnough(0)
    , mFakeDragging(FALSE)
    , mFakeDragBeginTime(0)
    , mFirstLayout(TRUE)
    , mNeedCalculatePageOffsets(FALSE)
    , mCalledSuper(FALSE)
    , mDecorChildCount(0)
    , mDrawingOrder(0)
    , mScrollState(SCROLL_STATE_IDLE)
{
    CArrayList::New((IArrayList**)&mItems);
    mTempItem = new ItemInfo();
    CRect::New((IRect**)&mTempRect);
    mEndScrollRunnable = (IRunnable*)new EndScrollRunnable(this);
}

CAR_INTERFACE_IMPL(ViewPager, ViewGroup, IViewPager);

ECode ViewPager::constructor(
    /* [in] */ IContext* context)
{
    ViewGroup::constructor(context);
    InitViewPager();
    return NOERROR;
}

ECode ViewPager::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    ViewGroup::constructor(context, attrs);
    InitViewPager();
    return NOERROR;
}

void ViewPager::InitViewPager()
{
    SetWillNotDraw(FALSE);
    SetDescendantFocusability(FOCUS_AFTER_DESCENDANTS);
    SetFocusable(TRUE);
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    mScroller = NULL;
    CScroller::New(context, sInterpolator, (IScroller**)&mScroller);
    AutoPtr<IViewConfigurationHelper> helper;
    CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&helper);
    AutoPtr<IViewConfiguration> configuration;
    helper->Get(context, (IViewConfiguration**)&configuration);
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IDisplayMetrics> dm;
    res->GetDisplayMetrics((IDisplayMetrics**)&dm);
    Float density;
    dm->GetDensity(&density);

    ViewConfigurationCompat::GetScaledPagingTouchSlop(configuration, &mTouchSlop);
    mMinimumVelocity = (Int32)(MIN_FLING_VELOCITY * density);
    configuration->GetScaledMaximumFlingVelocity(&mMaximumVelocity);
    CEdgeEffectCompat::New(context, (IEdgeEffectCompat**)&mLeftEdge);
    CEdgeEffectCompat::New(context, (IEdgeEffectCompat**)&mRightEdge);

    mFlingDistance = (Int32)(MIN_DISTANCE_FOR_FLING * density);
    mCloseEnough = (Int32)(CLOSE_ENOUGH * density);
    mDefaultGutterSize = (Int32)(DEFAULT_GUTTER_SIZE * density);

    AutoPtr<IAccessibilityDelegateCompat> delegate = (IAccessibilityDelegateCompat*)new MyAccessibilityDelegate(this);
    AutoPtr<IView> v = IView::Probe(this);
    ViewCompat::SetAccessibilityDelegate(v, delegate);

    Int32 result;
    if (ViewCompat::GetImportantForAccessibility(v, &result), result
            == IViewCompat::IMPORTANT_FOR_ACCESSIBILITY_AUTO) {
        ViewCompat::SetImportantForAccessibility(v, IViewCompat::IMPORTANT_FOR_ACCESSIBILITY_YES);
    }
}

ECode ViewPager::OnDetachedFromWindow()
{
    RemoveCallbacks(mEndScrollRunnable);
    return ViewGroup::OnDetachedFromWindow();
}

void ViewPager::SetScrollState(
    /* [in] */ Int32 newState)
{
    if (mScrollState == newState) {
        return;
    }

    mScrollState = newState;
    if (mPageTransformer != NULL) {
        // PageTransformers can do complex things that benefit from hardware layers.
        EnableLayers(newState != SCROLL_STATE_IDLE);
    }
    if (mOnPageChangeListener != NULL) {
        mOnPageChangeListener->OnPageScrollStateChanged(newState);
    }
}

ECode ViewPager::SetAdapter(
    /* [in] */ IPagerAdapter* adapter)
{
    if (mAdapter != NULL) {
        mAdapter->UnregisterDataSetObserver(mObserver);
        mAdapter->StartUpdate(IViewGroup::Probe(this));
        Int32 size;
        mItems->GetSize(&size);
        for (Int32 i = 0; i < size; i++) {
            AutoPtr<IInterface> value;
            mItems->Get(i, (IInterface**)&value);
            AutoPtr<ItemInfo> ii = (ItemInfo*)(IObject*)value.Get();
            mAdapter->DestroyItem(IViewGroup::(this), ii->mPosition, ii->mObject);
        }
        mAdapter->FinishUpdate(IViewGroup::(this));
        mItems->Clear();
        RemoveNonDecorViews();
        mCurItem = 0;
        ScrollTo(0, 0);
    }

    AutoPtr<IPagerAdapter> oldAdapter = mAdapter;
    mAdapter = adapter;
    mExpectedAdapterCount = 0;

    if (mAdapter != NULL) {
        if (mObserver == NULL) {
            mObserver = new PagerObserver(this);
        }
        mAdapter->RegisterDataSetObserver((IDataSetObserver*)mObserver);
        mPopulatePending = FALSE;
        Boolean wasFirstLayout = mFirstLayout;
        mFirstLayout = TRUE;
        mAdapter->GetCount(&mExpectedAdapterCount);
        if (mRestoredCurItem >= 0) {
            mAdapter->RestoreState(mRestoredAdapterState, mRestoredClassLoader);
            SetCurrentItemInternal(mRestoredCurItem, FALSE, TRUE);
            mRestoredCurItem = -1;
            mRestoredAdapterState = NULL;
            mRestoredClassLoader = NULL;
        }
        else if (!wasFirstLayout) {
            Populate();
        }
        else {
            RequestLayout();
        }
    }

    if (mAdapterChangeListener != NULL && oldAdapter != adapter) {
        mAdapterChangeListener->OnAdapterChanged(oldAdapter, adapter);
    }
    return NOERROR;
}

void ViewPager::RemoveNonDecorViews()
{
    Int32 count;
    GetChildCount(&count);
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child;
        GetChildAt(i, (IView**)&child);
        AutoPtr<IViewGroupLayoutParams> lp;
        child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
        AutoPtr<ViewPagerLayoutParams> params = (ViewPagerLayoutParams*)lp.Get();
        if (!params->mIsDecor) {
            RemoveViewAt(i);
            i--;
        }
    }
}

ECode ViewPager::GetAdapter(
    /* [out] */ IPagerAdapter** adapter)
{
    VALIDATE_NOT_NULL(adapter)
    *adapter = mAdapter;
    REFCOUNT_ADD(*adapter)
    return NOERROR;
}

void ViewPager::SetOnAdapterChangeListener(
    /* [in] */ IViewPagerOnAdapterChangeListener* listener)
{
    mAdapterChangeListener = listener;
}

Int32 ViewPager::GetClientWidth()
{
    Int32 width, left, right;
    GetMeasuredWidth(&width);
    GetPaddingLeft(&left);
    GetPaddingRight(&right);
    return width - left - right;
}

ECode ViewPager::SetCurrentItem(
    /* [in] */ Int32 item)
{
    mPopulatePending = FALSE;
    SetCurrentItemInternal(item, !mFirstLayout, FALSE);
    return NOERROR;
}

ECode ViewPager::SetCurrentItem(
    /* [in] */ Int32 item,
    /* [in] */ Boolean smoothScroll)
{
    mPopulatePending = FALSE;
    SetCurrentItemInternal(item, smoothScroll, FALSE);
    return NOERROR;
}

ECode ViewPager::GetCurrentItem(
    /* [out] */ Int32* item)
{
    VALIDATE_NOT_NULL(item)
    *item = mCurItem;
    return NOERROR;
}

void ViewPager::SetCurrentItemInternal(
    /* [in] */ Int32 item,
    /* [in] */ Boolean smoothScroll,
    /* [in] */ Boolean always)
{
    SetCurrentItemInternal(item, smoothScroll, always, 0);
}

void ViewPager::SetCurrentItemInternal(
    /* [in] */ Int32 item,
    /* [in] */ Boolean smoothScroll,
    /* [in] */ Boolean always,
    /* [in] */ Int32 velocity)
{
    Int32 count;
    if (mAdapter == NULL || (mAdapter->GetCount(&count), count <= 0)) {
        SetScrollingCacheEnabled(FALSE);
        return;
    }
    Int32 size;
    if (!always && mCurItem == item && (mItems->GetSize(&size), size != 0)) {
        SetScrollingCacheEnabled(FALSE);
        return;
    }

    if (item < 0) {
        item = 0;
    }
    else if (mAdapter->GetCount(&count), item <= count) {
        item = count - 1;
    }
    Int32 pageLimit = mOffscreenPageLimit;
    if (item > (mCurItem + pageLimit) || item < (mCurItem - pageLimit)) {
        // We are doing a jump by more than one page.  To avoid
        // glitches, we want to keep all current pages in the view
        // until the scroll ends.
        mItems->GetSize(&size);
        for (Int32 i = 0; i < size; i++) {
            AutoPtr<IInterface> value;
            mItems->Get(i, (IInterface**)&value);
            AutoPtr<ItemInfo> ii = (ItemInfo*)(IObject*)value.Get();
            ii->mScrolling = TRUE;
        }
    }
    Boolean dispatchSelected = mCurItem != item;

    if (mFirstLayout) {
        // We don't have any idea how big we are yet and shouldn't have any pages either.
        // Just set things up and let the pending layout handle things.
        mCurItem = item;
        if (dispatchSelected && mOnPageChangeListener != NULL) {
            mOnPageChangeListener->OnPageSelected(item);
        }
        if (dispatchSelected && mInternalPageChangeListener != NULL) {
            mInternalPageChangeListener->OnPageSelected(item);
        }
        RequestLayout();
    }
    else {
        Populate(item);
        ScrollToItem(item, smoothScroll, velocity, dispatchSelected);
    }
}

void ViewPager::ScrollToItem(
    /* [in] */ Int32 item,
    /* [in] */ Boolean smoothScroll,
    /* [in] */ Int32 velocity,
    /* [in] */ Boolean dispatchSelected)
{
    AutoPtr<ItemInfo> curInfo = InfoForPosition(item);
    Int32 destX = 0;
    if (curInfo != NULL) {
        Int32 width = GetClientWidth();
        destX = (Int32)(width * Elastos::Core::Math::Max(mFirstOffset,
                Elastos::Core::Math::Min(curInfo->mOffset, mLastOffset)));
    }
    if (smoothScroll) {
        SmoothScrollTo(destX, 0, velocity);
        if (dispatchSelected && mOnPageChangeListener != NULL) {
            mOnPageChangeListener->OnPageSelected(item);
        }
        if (dispatchSelected && mInternalPageChangeListener != NULL) {
            mInternalPageChangeListener->OnPageSelected(item);
        }
    }
    else {
        if (dispatchSelected && mOnPageChangeListener != NULL) {
            mOnPageChangeListener->OnPageSelected(item);
        }
        if (dispatchSelected && mInternalPageChangeListener != NULL) {
            mInternalPageChangeListener->OnPageSelected(item);
        }
        CompleteScroll(FALSE);
        ScrollTo(destX, 0);
        PageScrolled(destX);
    }
}

ECode ViewPager::SetOnPageChangeListener(
    /* [in] */ IViewPagerOnPageChangeListener* listener)
{
    mOnPageChangeListener = listener;
    return NOERROR;
}

ECode ViewPager::SetPageTransformer(
    /* [in] */ Boolean reverseDrawingOrder,
    /* [in] */ IViewPagerPageTransformer* transformer)
{
    if (Build::VERSION::SDK_INT >= 11) {
        Boolean hasTransformer = transformer != NULL;
        Boolean needsPopulate = hasTransformer != (mPageTransformer != NULL);
        mPageTransformer = transformer;
        SetChildrenDrawingOrderEnabledCompat(hasTransformer);
        if (hasTransformer) {
            mDrawingOrder = reverseDrawingOrder ? DRAW_ORDER_REVERSE : DRAW_ORDER_FORWARD;
        }
        else {
            mDrawingOrder = DRAW_ORDER_DEFAULT;
        }
        if (needsPopulate) Populate();
    }
}

void ViewPager::SetChildrenDrawingOrderEnabledCompat(
    /* [in] */ Boolean enable)
{
    if (Build::VERSION::SDK_INT >= 7) {
        // if (mSetChildrenDrawingOrderEnabled == NULL) {
        //     try {
        //     mSetChildrenDrawingOrderEnabled = ViewGroup.class.getDeclaredMethod(
        //             "setChildrenDrawingOrderEnabled", new Class[] { Boolean.TYPE });
        //     } catch (NoSuchMethodException e) {
        //         Log.e(TAG, "Can't find setChildrenDrawingOrderEnabled", e);
        //     }
        // }
        // try {
        //     mSetChildrenDrawingOrderEnabled.invoke(this, enable);
        // } catch (Exception e) {
        //     Log.e(TAG, "Error changing children drawing order", e);
        // }
        SetChildrenDrawingOrderEnabled(enable);
    }
}

Int32 ViewPager::GetChildDrawingOrder(
    /* [in] */ Int32 childCount,
    /* [in] */ Int32 i)
{
    Int32 index = mDrawingOrder == DRAW_ORDER_REVERSE ? childCount - 1 - i : i;
    AutoPtr<IInterface> value;
    mDrawingOrderedChildren->Get(index, (IInterface**)&value);
    AutoPtr<IView> v = IView::Probe(value);
    AutoPtr<IViewGroupLayoutParams> lp;
    v->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    Int32 result = ((ViewPagerLayoutParams*)lp.Get())->mChildIndex;
    return result;
}

AutoPtr<IViewPagerOnPageChangeListener> ViewPager::SetInternalPageChangeListener(
    /* [in] */ IViewPagerOnPageChangeListener* listener)
{
    AutoPtr<IViewPagerOnPageChangeListener> oldListener = mInternalPageChangeListener;
    mInternalPageChangeListener = listener;
    return oldListener;
}

ECode ViewPager::GetOffscreenPageLimit(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    *value = mOffscreenPageLimit;
    return NOERROR;
}

ECode ViewPager::SetOffscreenPageLimit(
    /* [in] */ Int32 limit)
{
    if (limit < DEFAULT_OFFSCREEN_PAGES) {
        Slogger::W(TAG, "Requested offscreen page limit %d too small; defaulting to %d",
                limit, DEFAULT_OFFSCREEN_PAGES);
        limit = DEFAULT_OFFSCREEN_PAGES;
    }
    if (limit != mOffscreenPageLimit) {
        mOffscreenPageLimit = limit;
        Populate();
    }
    return NOERROR;
}

ECode ViewPager::SetPageMargin(
    /* [in] */ Int32 marginPixels)
{
    Int32 oldMargin = mPageMargin;
    mPageMargin = marginPixels;

    Int32 width;
    GetWidth(&width);
    RecomputeScrollPosition(width, width, marginPixels, oldMargin);

    RequestLayout();
    return NOERROR;
}

ECode ViewPager::GetPageMargin(
    /* [out] */ Int32* margin)
{
    VALIDATE_NOT_NULL(margin)
    *margin = mPageMargin;
    return NOERROR;
}

ECode ViewPager::SetPageMarginDrawable(
    /* [in] */ IDrawable* d)
{
    mMarginDrawable = d;
    if (d != NULL) RefreshDrawableState();
    SetWillNotDraw(d == NULL);
    return Invalidate();
}

ECode ViewPager::SetPageMarginDrawable(
    /* [in] */ Int32 resId)
{
    AutoPtr<IContext> ctx;
    GetContext((IContext**)&ctx);
    AutoPtr<IResources> res;
    ctx->GetResources((IResources**)&res);
    AutoPtr<IDrawable> d;
    GetDrawable(resId, (IDrawable**)&d);
    return SetPageMarginDrawable(d);
}

Boolean ViewPager::VerifyDrawable(
    /* [in] */ IDrawable* who)
{
    return ViewGroup::VerifyDrawable(who) || who == mMarginDrawable.Get();
}

ECode ViewPager::DrawableStateChanged()
{
    ViewGroup::DrawableStateChanged();
    AutoPtr<IDrawable> d = mMarginDrawable;
    Boolean isStateful;
    if (d != NULL && (d->IsStateful(&isStateful), isStateful)) {
        AutoPtr<ArrayOf<Int32> > state;
        GetDrawableState((ArrayOf<Int32>**)&state);
        Boolean result;
        d->SetState(state, &result);
    }
    return NOERROR;
}

Float ViewPager::DistanceInfluenceForSnapDuration(
    /* [in] */ Float f)
{
    f -= 0.5f; // center the values about 0.
    f *= 0.3f * Elastos::Core::Math::PI / 2.0f;
    return (Float) Elastos::Core::Math::Sin(f);
}

void ViewPager::SmoothScrollTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    SmoothScrollTo(x, y, 0);
}

void ViewPager::SmoothScrollTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 velocity)
{
    Int32 count;
    if (GetChildCount() == 0) {
        // Nothing to do.
        SetScrollingCacheEnabled(FALSE);
        return;
    }
    Int32 sx, sy;
    GetScrollX(&sx);
    GetScrollY(&sy);
    Int32 dx = x - sx;
    Int32 dy = y - sy;
    if (dx == 0 && dy == 0) {
        CompleteScroll(FALSE);
        Populate();
        SetScrollState(SCROLL_STATE_IDLE);
        return;
    }

    SetScrollingCacheEnabled(TRUE);
    SetScrollState(SCROLL_STATE_SETTLING);

    Int32 width;
    GetClientWidth(&width);
    Int32 halfWidth = width / 2;
    Float distanceRatio = Elastos::Core::Math::Min(1f, 1.0f * Elastos::Core::Math::Abs(dx) / width);
    Float distance = halfWidth + halfWidth *
            DistanceInfluenceForSnapDuration(distanceRatio);

    Int32 duration = 0;
    velocity = Elastos::Core::Math::Abs(velocity);
    if (velocity > 0) {
        duration = 4 * Elastos::Core::Math::Round(1000 * Elastos::Core::Math::Abs(distance / velocity));
    }
    else {
        Int32 pageW;
        mAdapter->GetPageWidth(mCurItem, &pageW)
        Float pageWidth = width * pageW;
        Float pageDelta = (Float) Elastos::Core::Math::Abs(dx) / (pageWidth + mPageMargin);
        duration = (Int32)((pageDelta + 1) * 100);
    }
    duration = Elastos::Core::Math::Min(duration, MAX_SETTLE_DURATION);

    mScroller->StartScroll(sx, sy, dx, dy, duration);
    ViewCompat:PostInvalidateOnAnimation(this);
}

AutoPtr<ItemInfo> ViewPager::AddNewItem(
    /* [in] */ Int32 position,
    /* [in] */ Int32 index)
{
    AutoPtr<ItemInfo> ii = new ItemInfo();
    ii->mPosition = position;
    ii->mObject = NULL;
    mAdapter->InstantiateItem(IView::Probe(this), position, &ii->mObject);
    mAdapter->GetPageWidth(position, &ii->mWidthFactor);
    Int32 size;
    if (index < 0 || (mItems->GetSize(&size), index >= size)) {
        mItems->Add(ii);
    }
    else {
        mItems->Add(index, ii);
    }
    return ii;
}

void ViewPager::DataSetChanged()
{
    // This method only gets called if our observer is attached, so mAdapter is non-null.

    Int32 adapterCount;
    mAdapter->GetCount(&adapterCount);
    mExpectedAdapterCount = adapterCount;
    Int32 size;
    mItems->GetSize(&size);
    Boolean needPopulate = size < mOffscreenPageLimit * 2 + 1 && size < adapterCount;
    Int32 newCurrItem = mCurItem;

    Boolean isUpdating = FALSE;
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> value;
        mItems->Get(i, (IInterface**)&value);
        AutoPtr<ItemInfo> ii = (ItemInfo*)(IObject*)value.Get();
        Int32 newPos;
        mAdapter->GetItemPosition(ii->mObject, &newPos);

        if (newPos == IPagerAdapter::POSITION_UNCHANGED) {
            continue;
        }

        if (newPos == IPagerAdapter::POSITION_NONE) {
            mItems->Remove(i);
            i--;

            if (!isUpdating) {
                mAdapter->StartUpdate(IView::Probe(this));
                isUpdating = TRUE;
            }

            mAdapter->DestroyItem(IView::Probe(this), ii->mPosition, ii->mObject);
            needPopulate = TRUE;

            if (mCurItem == ii->mPosition) {
                // Keep the current item in the valid range
                newCurrItem = Elastos::Core::Math::Max(0, Elastos::Core::Math::Min(mCurItem, adapterCount - 1));
                needPopulate = TRUE;
            }
            continue;
        }

        if (ii->mPosition != newPos) {
            if (ii->mPosition == mCurItem) {
                // Our current item changed position. Follow it.
                newCurrItem = newPos;
            }

            ii->mPosition = newPos;
            needPopulate = TRUE;
        }
    }

    if (isUpdating) {
        mAdapter->FinishUpdate(IView::Probe(this));
    }

    AutoPtr<ICollections> coll;
    CCollections::AcquireSingleton((ICollections**)&coll);
    coll->Sort(IList::Probe(mItems), COMPARATOR);

    if (needPopulate) {
        // Reset our known page widths; populate will recompute them.
        Int32 childCount;
        GetChildCount(&childCount);
        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IView> child;
            GetChildAt(i, (IView**)&child);
            AutoPtr<IViewGroupLayoutParams> lp;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
            AutoPtr<ViewPagerLayoutParams> params = (ViewPagerLayoutParams*)lp.Get();
            if (!params->mIsDecor) {
                params->mWidthFactor = 0.f;
            }
        }

        SetCurrentItemInternal(newCurrItem, FALSE, TRUE);
        RequestLayout();
    }
}

ECode ViewPager::Populate()
{
    return Populate(mCurItem);
}

ECode ViewPager::Populate(
    /* [in] */ Int32 newCurrentItem)
{
    AutoPtr<ItemInfo> oldCurInfo;
    Int32 focusDirection = IView::FOCUS_FORWARD;
    if (mCurItem != newCurrentItem) {
        focusDirection = mCurItem < newCurrentItem ? IView::FOCUS_RIGHT : IView::FOCUS_LEFT;
        oldCurInfo = InfoForPosition(mCurItem);
        mCurItem = newCurrentItem;
    }

    if (mAdapter == NULL) {
        SortChildDrawingOrder();
        return;
    }

    // Bail now if we are waiting to populate.  This is to hold off
    // on creating views from the time the user releases their finger to
    // fling to a new position until we have finished the scroll to
    // that position, avoiding glitches from happening at that point.
    if (mPopulatePending) {
        if (DEBUG) Slogger::I(TAG, "populate is pending, skipping for now...");
        SortChildDrawingOrder();
        return;
    }

    // Also, don't populate until we are attached to a window.  This is to
    // avoid trying to populate before we have restored our view hierarchy
    // state and conflicting with what is restored.
    AutoPtr<IBinder> token;
    if (GetWindowToken((IBinder**)&token), token == NULL) {
        return;
    }

    mAdapter->StartUpdate(IView::Probe(this));

    Int32 pageLimit = mOffscreenPageLimit;
    Int32 startPos = Elastos::Core::Math::Max(0, mCurItem - pageLimit);
    Int32 N;
    mAdapter->GetCount(&N);
    Int32 endPos = Elastos::Core::Math::Min(N - 1, mCurItem + pageLimit);

    if (N != mExpectedAdapterCount) {
        String resName;
        // try {
        AutoPtr<IResources> res;
        GetResources((IResources**)&res);
        Int32 id;
        GetId(&id);
        if (FAILED(res->GetResourcesName(id, &resName))) {
            resName = StringUtils::ToHexString(id);
        }
        // } catch (Resources.NotFoundException e) {
        //     resName = Integer.toHexString(getId());
        // }
        Slogger::E(TAG, "The application's PagerAdapter changed the adapter's contents without calling PagerAdapter#notifyDataSetChanged! Expected adapter item count: %d, found: %d Pager id: %s Pager class:  Problematic adapter: "
                , mExpectedAdapterCount, N, resName.string()/*, getClass(), mAdapter.getClass()*/);
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    // Locate the currently focused item or add it if needed.
    Int32 curIndex = -1;
    AutoPtr<ItemInfo> curItem;
    Int32 size;
    mItems->GetSize(&size);
    for (curIndex = 0; curIndex < size; curIndex++) {
        AutoPtr<IInterface> value;
        mItems->Get(curIndex, (IInterface**)&value);
        AutoPtr<ItemInfo> ii = (ItemInfo*)(IObject*)value.Get();
        if (ii->mPosition >= mCurItem) {
            if (ii->mPosition == mCurItem) curItem = ii;
            break;
        }
    }

    if (curItem == NULL && N > 0) {
        curItem = AddNewItem(mCurItem, curIndex);
    }

    // Fill 3x the available width or up to the number of offscreen
    // pages requested to either side, whichever is larger.
    // If we have no current item we have no work to do.
    if (curItem != NULL) {
        Float extraWidthLeft = 0.f;
        Int32 itemIndex = curIndex - 1;
        AutoPtr<ItemInfo> ii;
        if (itemIndex >= 0) {
            AutoPtr<IInterface> value;
            mItems->Get(itemIndex, (IInterface**)&value);
            ii = (ItemInfo*)(IObject*)value.Get();
        }
        Int32 clientWidth;
        GetClientWidth(&clientWidth);
        Int32 paddingL;
        GetPaddingLeft(&paddingL);
        Float leftWidthNeeded = clientWidth <= 0 ? 0 :
                2.f - curItem->mWidthFactor + (Float)paddingL / (Float) clientWidth;
        for (Int32 pos = mCurItem - 1; pos >= 0; pos--) {
            if (extraWidthLeft >= leftWidthNeeded && pos < startPos) {
                if (ii == NULL) {
                    break;
                }
                if (pos == ii->mPosition && !ii->mScrolling) {
                    mItems->Remove(itemIndex);
                    mAdapter->DestroyItem(IView::Probe(this), pos, ii->mObject);
                    if (DEBUG) {
                        Slogger::I(TAG, "populate() - destroyItem() with pos: %d view: %s", pos, TO_CSTR(ii->mObject));
                    }
                    itemIndex--;
                    curIndex--;
                    ii = NULL;
                    if (itemIndex >= 0) {
                        AutoPtr<IInterface> value;
                        mItems->Get(itemIndex, (IInterface**)&value);
                        ii = (ItemInfo*)(IObject*)value.Get();
                    }
                }
            }
            else if (ii != NULL && pos == ii->mPosition) {
                extraWidthLeft += ii->mWidthFactor;
                itemIndex--;
                ii = NULL;
                if (itemIndex >= 0) {
                    AutoPtr<IInterface> value;
                    mItems->Get(itemIndex, (IInterface**)&value);
                    ii = (ItemInfo*)(IObject*)value.Get();
                }
            }
            else {
                ii = AddNewItem(pos, itemIndex + 1);
                extraWidthLeft += ii->mWidthFactor;
                curIndex++;
                ii = NULL;
                if (itemIndex >= 0) {
                    AutoPtr<IInterface> value;
                    mItems->Get(itemIndex, (IInterface**)&value);
                    ii = (ItemInfo*)(IObject*)value.Get();
                }
            }
        }

        Float extraWidthRight = curItem->mWidthFactor;
        itemIndex = curIndex + 1;
        if (extraWidthRight < 2.f) {
            mItems->GetSize(&size);
            ii = NULL;
            if (itemIndex < size) {
                AutoPtr<IInterface> value;
                mItems->Get(itemIndex, (IInterface**)&value);
                ii = (ItemInfo*)(IObject*)value.Get();
            }
            Int32 paddingR;
            GetPaddingRight(&paddingR);
            Float rightWidthNeeded = clientWidth <= 0 ? 0 : (Float)paddingR / (Float)clientWidth + 2.f;
            for (Int32 pos = mCurItem + 1; pos < N; pos++) {
                if (extraWidthRight >= rightWidthNeeded && pos > endPos) {
                    if (ii == NULL) {
                        break;
                    }
                    if (pos == ii->mPosition && !ii->mScrolling) {
                        mItems->Remove(itemIndex);
                        mAdapter->DestroyItem(this, pos, ii->mObject);
                        if (DEBUG) {
                            Slogger::I(TAG, "populate() - destroyItem() with pos: %d view: %s", pos, TO_CSTR(ii->mObject));
                        }
                        mItems->GetSize(&size);
                        ii = NULL;
                        if (itemIndex < size) {
                            AutoPtr<IInterface> value;
                            mItems->Get(itemIndex, (IInterface**)&value);
                            ii = (ItemInfo*)(IObject*)value.Get();
                        }
                    }
                }
                else if (ii != NULL && pos == ii->mPosition) {
                    extraWidthRight += ii->mWidthFactor;
                    itemIndex++;
                    mItems->GetSize(&size);
                    ii = NULL;
                    if (itemIndex < size) {
                        AutoPtr<IInterface> value;
                        mItems->Get(itemIndex, (IInterface**)&value);
                        ii = (ItemInfo*)(IObject*)value.Get();
                    }
                }
                else {
                    ii = AddNewItem(pos, itemIndex);
                    itemIndex++;
                    extraWidthRight += ii->mWidthFactor;
                    mItems->GetSize(&size);
                    ii = NULL;
                    if (itemIndex < size) {
                        AutoPtr<IInterface> value;
                        mItems->Get(itemIndex, (IInterface**)&value);
                        ii = (ItemInfo*)(IObject*)value.Get();
                    }
                }
            }
        }

        CalculatePageOffsets(curItem, curIndex, oldCurInfo);
    }

    if (DEBUG) {
        Slogger::I(TAG, "Current page list:");
        mItems->GetSize(&size);
        for (Int32 i = 0; i < size; i++) {
            AutoPtr<IInterface> value;
            mItems->Get(i, (IInterface**)&value);
            AutoPtr<ItemInfo> itemInfo = (ItemInfo*)(IObject*)value.Get();
            Slogger::I(TAG, "#%d: page %d", itemInfo->mPosition);
        }
    }

    AutoPtr<IView> v = IView::Probe(this);
    mAdapter->SetPrimaryItem(v, mCurItem, curItem != NULL ? curItem->mObject : NULL);

    mAdapter->FinishUpdate(v);

    // Check width measurement of current pages and drawing sort order.
    // Update LayoutParams as needed.
    Int32 childCount;
    GetChildCount(&childCount);
    for (Int32 i = 0; i < childCount; i++) {
        AutoPtr<IView> child;
        GetChildAt(i, (IView**)&child);
        AutoPtr<IViewGroupLayoutParams> lp;
        child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
        AutoPtr<ViewPagerLayoutParams> params = (ViewPagerLayoutParams*)lp.Get();
        params->mChildIndex = i;
        if (!params->mIsDecor && params->mWidthFactor == 0.f) {
            // 0 means requery the adapter for this, it doesn't have a valid width.
            AutoPtr<ItemInfo> ii = InfoForChild(child);
            if (ii != NULL) {
                params->mWidthFactor = ii->mWidthFactor;
                params->mPosition = ii->mPosition;
            }
        }
    }
    SortChildDrawingOrder();

    Boolean hasFocus;
    if (HasFocus(&hasFocus), hasFocus) {
        AutoPtr<IView> currentFocused;
        FindFocus((IView**)&currentFocused);
        AutoPtr<ItemInfo> ii = currentFocused != NULL ? InfoForAnyChild(currentFocused) : NULL;
        if (ii == NULL || ii->mPosition != mCurItem) {
            Int32 c;
            GetChildCount(&c);
            for (Int32 i = 0; i < c; i++) {
                AutoPtr<IView> child;
                GetChildAt(i, (IView**)&child);
                ii = InfoForChild(child);
                if (ii != NULL && ii->mPosition == mCurItem) {
                    Boolean isFocus;
                    if (child->RequestFocus(focusDirection, &isFocus), isFocus) {
                        break;
                    }
                }
            }
        }
    }
    return NOERROR;
}

void ViewPager::SortChildDrawingOrder()
{
    if (mDrawingOrder != DRAW_ORDER_DEFAULT) {
        if (mDrawingOrderedChildren == NULL) {
            CArrayList::New((IArrayList**)&mDrawingOrderedChildren);
        }
        else {
            mDrawingOrderedChildren->Clear();
        }
        Int32 childCount;
        GetChildCount(&childCount);
        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IView> child;
            GetChildAt(i, (IView**)&child);
            mDrawingOrderedChildren->Add(child);
        }
        AutoPtr<ICollections> coll;
        CCollections::AcquireSingleton((ICollections**)&coll);
        coll->Sort(IList::Probe(mDrawingOrderedChildren), sPositionComparator);
    }
}

void ViewPager::CalculatePageOffsets(
    /* [in] */ ItemInfo* curItem,
    /* [in] */ Int32 curIndex,
    /* [in] */ ItemInfo* oldCurInfo)
{
    Int32 N;
    mAdapter->GetCount(&N);
    Int32 width;
    GetClientWidth(&width);
    Float marginOffset = width > 0 ? (Float) mPageMargin / width : 0;
    // Fix up offsets for later layout.
    if (oldCurInfo != NULL) {
        Int32 oldCurPosition = oldCurInfo->mPosition;
        // Base offsets off of oldCurInfo.
        if (oldCurPosition < curItem->mPosition) {
            Int32 itemIndex = 0;
            AutoPtr<ItemInfo> ii;
            Float offset = oldCurInfo->mOffset + oldCurInfo->mWidthFactor + marginOffset;
            Int32 size;
            mItems->GetSize(&size);
            for (Int32 pos = oldCurPosition + 1;
                    pos <= curItem->mPosition && itemIndex < size; pos++) {
                AutoPtr<IInterface> value;
                mItems->Get(itemIndex, (IInterface**)&value);
                ii = (ItemInfo*)(IObject*)value.Get();
                while (pos > ii->mPosition && itemIndex < size - 1) {
                    itemIndex++;
                    AutoPtr<IInterface> v;
                    mItems->Get(itemIndex, (IInterface**)&v);
                    ii = (ItemInfo*)(IObject*)v.Get();
                }
                while (pos < ii->mPosition) {
                    // We don't have an item populated for this,
                    // ask the adapter for an offset.
                    Int32 width;
                    mAdapter->GetPageWidth(pos, &width);
                    offset += width + marginOffset;
                    pos++;
                }
                ii->mOffset = offset;
                offset += ii->mWidthFactor + marginOffset;
            }
        }
        else if (oldCurPosition > curItem->mPosition) {
            Int32 size;
            mItems->GetSize(&size);
            Int32 itemIndex = size - 1;
            AutoPtr<ItemInfo> ii;
            Float offset = oldCurInfo->mOffset;
            for (Int32 pos = oldCurPosition - 1;
                    pos >= curItem->mPosition && itemIndex >= 0; pos--) {
                AutoPtr<IInterface> value;
                mItems->Get(itemIndex, (IInterface**)&value);
                ii = (ItemInfo*)(IObject*)value.Get();
                while (pos < ii->mPosition && itemIndex > 0) {
                    itemIndex--;
                    AutoPtr<IInterface> v;
                    mItems->Get(itemIndex, (IInterface**)&v);
                    ii = (ItemInfo*)(IObject*)v.Get();
                }
                while (pos > ii->mPosition) {
                    // We don't have an item populated for this,
                    // ask the adapter for an offset.
                    Int32 width;
                    mAdapter->GetPageWidth(pos, &width);
                    offset -= width + marginOffset;
                    pos--;
                }
                offset -= ii->mWidthFactor + marginOffset;
                ii->mOffset = offset;
            }
        }
    }

    // Base all offsets off of curItem.
    Int32 itemCount;
    mItems->GetSize(&itemCount);
    Float offset = curItem->mOffset;
    Int32 pos = curItem->mPosition - 1;
    mFirstOffset = curItem->mPosition == 0 ? curItem->mOffset : -Elastos::Core::Math::Float_MAX_VALUE;
    mLastOffset = curItem->mPosition == N - 1 ?
            curItem->mOffset + curItem->mWidthFactor - 1 : Elastos::Core::Math::Float_MAX_VALUE;
    // Previous pages
    for (Int32 i = curIndex - 1; i >= 0; i--, pos--) {
        AutoPtr<IInterface> value;
        mItems->Get(i, (IInterface**)&value);
        AutoPtr<ItemInfo> ii = (ItemInfo*)(IObject*)value.Get();
        while (pos > ii->Mposition) {
            Int32 width;
            mAdapter->GetPageWidth(pos--, &width);
            offset -= width + marginOffset;
        }
        offset -= ii->mWidthFactor + marginOffset;
        ii->mOffset = offset;
        if (ii->mPosition == 0) mFirstOffset = offset;
    }
    offset = curItem->mOffset + curItem->mWidthFactor + marginOffset;
    pos = curItem->mPosition + 1;
    // Next pages
    for (Int32 i = curIndex + 1; i < itemCount; i++, pos++) {
        AutoPtr<IInterface> value;
        mItems->Get(i, (IInterface**)&value);
        AutoPtr<ItemInfo> ii = (ItemInfo*)(IObject*)value.Get();
        while (pos < ii->mPosition) {
            Int32 width;
            mAdapter->GetPageWidth(pos++, &width);
            offset += width + marginOffset;
        }
        if (ii->mPosition == N - 1) {
            mLastOffset = offset + ii->mWidthFactor - 1;
        }
        ii->mOffset = offset;
        offset += ii->mWidthFactor + marginOffset;
    }

    mNeedCalculatePageOffsets = FALSE;
}

AutoPtr<IParcelable> ViewPager::OnSaveInstanceState()
{
    AutoPtr<IParcelable> superState = ViewGroup::OnSaveInstanceState();
    AutoPtr<CViewPagerSavedState> ss;
    CViewPagerSavedState::NewByFriend(superState, (*CViewPagerSavedState**)&ss);
    ss->mPosition = mCurItem;
    if (mAdapter != NULL) {
        mAdapter->SaveState((IParcelable**)&ss->mAdapterState);
    }
    return IParcelable::Probe(ss);
}

void ViewPager::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    AutoPtr<IViewPagerSavedState> ss = IViewPagerSavedState::Probe(state);
    if (savedState == NULL) {
        ViewGroup::OnRestoreInstanceState(state);
        return;
    }

    AutoPtr<IParcelable> superState;
    ss->GetSuperState((IParcelable**)&superState);
    ViewGroup::OnRestoreInstanceState(superState);

    AutoPtr<CViewPagerSavedState> ssObj = (CViewPagerSavedState*)ss.Get();
    if (mAdapter != NULL) {
        mAdapter->RestoreState(ssObj->mAdapterState, ssObj->mLoader);
        SetCurrentItemInternal(ss->mPosition, FALSE, TRUE);
    }
    else {
        mRestoredCurItem = ss->mPosition;
        mRestoredAdapterState = ss->mAdapterState;
        mRestoredClassLoader = ss->mLoader;
    }
}

ECode ViewPager::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params)
{
    if (!CheckLayoutParams(params)) {
        params = GenerateLayoutParams(params);
    }
    AutoPtr<ViewPagerLayoutParams> lp = (ViewPagerLayoutParams*)params;
    AutoPtr<IViewPagerDecor> decor = IViewPagerDecor::Probe(child);
    lp->mIsDecor |= (decor != NULL);
    if (mInLayout) {
        if (lp != NULL && lp->mIsDecor) {
            Slogger::E(TAG, "Cannot add pager decor view during layout");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        lp->mNeedsMeasure = TRUE;
        AddViewInLayout(child, index, params);
    }
    else {
        ViewGroup::AddView(child, index, params);
    }

    if (USE_CACHE) {
        Int32 visibility;
        if (child->GetVisibility(&visibility), visibility != GONE) {
            child->SetDrawingCacheEnabled(mScrollingCacheEnabled);
        }
        else {
            child->SetDrawingCacheEnabled(FALSE);
        }
    }
    return NOERROR;
}

ECode ViewPager::RemoveView(
    /* [in] */ IView* view)
{
    if (mInLayout) {
        RemoveViewInLayout(view);
    }
    else {
        ViewGroup::RemoveView(view);
    }
    return NOERROR;
}

AutoPtr<ItemInfo> ViewPager::InfoForChild(
    /* [in] */ IView* child)
{
    Int32 size;
    mItems->GetSize(&size);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> value;
        mItems->Get(i, (IInterface**)&value);
        AutoPtr<ItemInfo> ii = (ItemInfo*)(IObject*)value.Get();
        Boolean result;
        if (mAdapter->IsViewFromObject(child, ii->mObject, &result), result) {
            return ii;
        }
    }
    return NULL;
}

AutoPtr<ItemInfo> ViewPager::InfoForAnyChild(
    /* [in] */ IView* child)
{
    AutoPtr<IViewParent> parent;
    child->GetParent((IViewParent**)&parent);
    while (parent != IViewParent::Probe(this)) {
        if (parent == NULL || IView::Probe(parent) == NULL) {
            return NULL;
        }
        child = IView::Probe(parent);
    }
    return InfoForChild(child);
}

AutoPtr<ItemInfo> ViewPager::InfoForPosition(
    /* [in] */ Int32 position)
{
    Int32 size;
    mItems->GetSize(&size);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> value;
        mItems->Get(i, (IInterface**)&value);
        AutoPtr<ItemInfo> ii = (ItemInfo*)(IObject*)value.Get();
        if (ii->mPosition == position) {
            return ii;
        }
    }
    return NULL;
}

ECode ViewPager::OnAttachedToWindow()
{
    ViewGroup::OnAttachedToWindow();
    mFirstLayout = TRUE;
    return NOERROR;
}

void ViewPager::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    // For simple implementation, our internal size is always 0.
    // We depend on the container to specify the layout size of
    // our view.  We can't really know what it is since we will be
    // adding and removing different arbitrary views and do not
    // want the layout to change as this happens.
    SetMeasuredDimension(GetDefaultSize(0, widthMeasureSpec),
            GetDefaultSize(0, heightMeasureSpec));

    Int32 measuredWidth;
    GetMeasuredWidth(&measuredWidth);
    Int32 maxGutterSize = measuredWidth / 10;
    mGutterSize = Elastos::Core::Math::Min(maxGutterSize, mDefaultGutterSize);

    // Children are just made to fill our space.
    Int32 paddingL, paddingR, paddingT, paddingB, height;
    GetPaddingLeft(&paddingL);
    GetPaddingRight(&paddingR);
    GetPaddingTop(&paddingT);
    GetPaddingBottom(&paddingB);
    GetMeasuredHeight(&height);
    Int32 childWidthSize = measuredWidth - paddingL - paddingR;
    Int32 childHeightSize = height - paddingT - paddingB;

    /*
     * Make sure all children have been properly measured. Decor views first.
     * Right now we cheat and make this less complicated by assuming decor
     * views won't intersect. We will pin to edges based on gravity.
     */
    Int32 size;
    GetChildCount(&size);
    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IView> child;
        GetChildAt(i, (IView**)&child);
        Int32 visibility;
        if (child->GetVisibility(&visibility), visibility != GONE) {
            AutoPtr<IViewGroupLayoutParams> temp;
            child->GetLayoutParams((IViewGroupLayoutParams**)&temp);
            AutoPtr<ViewPagerLayoutParams> lp = (ViewPagerLayoutParams*)temp.Get();
            if (lp != NULL && lp->mIsDecor) {
                Int32 hgrav = lp->mGravity & IGravity::HORIZONTAL_GRAVITY_MASK;
                Int32 vgrav = lp->mGravity & IGravity::VERTICAL_GRAVITY_MASK;
                Int32 widthMode = MeasureSpec::AT_MOST;
                Int32 heightMode = MeasureSpec::AT_MOST;
                Boolean consumeVertical = vgrav == IGravity::TOP || vgrav == IGravity::BOTTOM;
                Boolean consumeHorizontal = hgrav == IGravity::LEFT || hgrav == IGravity::RIGHT;

                if (consumeVertical) {
                    widthMode = MeasureSpec::EXACTLY;
                }
                else if (consumeHorizontal) {
                    heightMode = MeasureSpec::EXACTLY;
                }

                Int32 widthSize = childWidthSize;
                Int32 heightSize = childHeightSize;
                if (lp->mWidth != IViewGroupLayoutParams::WRAP_CONTENT) {
                    widthMode = MeasureSpec.EXACTLY;
                    if (lp->mWidth != IViewGroupLayoutParams::FILL_PARENT) {
                        widthSize = lp.width;
                    }
                }
                if (lp->mHeight != IViewGroupLayoutParams::WRAP_CONTENT) {
                    heightMode = MeasureSpec::EXACTLY;
                    if (lp->mHeight != IViewGroupLayoutParams::FILL_PARENT) {
                        heightSize = lp->mHeight;
                    }
                }
                Int32 widthSpec = MeasureSpec::MakeMeasureSpec(widthSize, widthMode);
                Int32 heightSpec = MeasureSpec::MakeMeasureSpec(heightSize, heightMode);
                child->Measure(widthSpec, heightSpec);

                if (consumeVertical) {
                    Int32 h;
                    child->GetMeasuredHeight(&h);
                    childHeightSize -= h;
                }
                else if (consumeHorizontal) {
                    Int32 w;
                    child->GetMeasuredWidth(&w);
                    childWidthSize -= w;
                }
            }
        }
    }

    mChildWidthMeasureSpec = MeasureSpec::MakeMeasureSpec(childWidthSize, MeasureSpec::EXACTLY);
    mChildHeightMeasureSpec = MeasureSpec::MakeMeasureSpec(childHeightSize, MeasureSpec::EXACTLY);

    // Make sure we have created all fragments that we need to have shown.
    mInLayout = TRUE;
    Populate();
    mInLayout = FALSE;

    // Page views next.
    GetChildCount(&size);
    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IView> child;
        GetChildAt(i, (IView**)&child);
        Int32 visibility;
        if (child->GetVisibility(&visibility), visibility != GONE) {
            if (DEBUG) {
                Slogger::V(TAG, "Measuring #%d %s: %d", i, TO_CSTR(child), mChildWidthMeasureSpec);
            }

            AutoPtr<IViewGroupLayoutParams> temp;
            child->GetLayoutParams((IViewGroupLayoutParams**)&temp);
            AutoPtr<ViewPagerLayoutParams> lp = (ViewPagerLayoutParams*)temp.Get();
            if (lp == NULL || !lp->mIsDecor) {
                Int32 widthSpec = MeasureSpec::MakeMeasureSpec(
                        (Int32)(childWidthSize * lp->mWidthFactor), MeasureSpec::EXACTLY);
                child->Measure(widthSpec, mChildHeightMeasureSpec);
            }
        }
    }
}

void ViewPager::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    ViewGroup::OnSizeChanged(w, h, oldw, oldh);

    // Make sure scroll position is set correctly.
    if (w != oldw) {
        RecomputeScrollPosition(w, oldw, mPageMargin, mPageMargin);
    }
}

void ViewPager::RecomputeScrollPosition(
    /* [in] */ Int32 width,
    /* [in] */ Int32 oldWidth,
    /* [in] */ Int32 margin,
    /* [in] */ Int32 oldMargin)
{
    Boolean isEmpty;
    if (oldWidth > 0 && (mItems->IsEmpty(&isEmpty), !isEmpty)) {
        Int32 paddingL, paddingR;
        GetPaddingLeft(&paddingL);
        GetPaddingRight(&paddingR);
        Int32 widthWithMargin = width - paddingL - paddingR + margin;
        Int32 oldWidthWithMargin = oldWidth - paddingL - paddingR + oldMargin;
        Int32 xpos;
        GetScrollX(&xpos);
        Float pageOffset = (Float)xpos / oldWidthWithMargin;
        Int32 newOffsetPixels = (Int32)(pageOffset * widthWithMargin);

        Int32 ypos;
        GetScrollY(&ypos);
        ScrollTo(newOffsetPixels, ypos);
        Boolean isFinished;
        if (mScroller->IsFinished(&isFinished), !isFinished) {
            // We now return to your regularly scheduled scroll, already in progress.
            Int32 duration;
            mScroller->GetDuration(&duration);
            Int32 passed;
            mScroller->TimePassed(&passed);
            Int32 newDuration = duration - passed;
            AutoPtr<ItemInfo> targetInfo = InfoForPosition(mCurItem);
            mScroller->StartScroll(newOffsetPixels, 0,
                    (Int32)(targetInfo->mOffset * width), 0, newDuration);
        }
    }
    else {
        AutoPtr<ItemInfo> ii = InfoForPosition(mCurItem);
        Float scrollOffset = ii != NULL ? Elastos::Core::Math::Min(ii->mOffset, mLastOffset) : 0;
        Int32 paddingL, paddingR;
        GetPaddingLeft(&paddingL);
        GetPaddingRight(&paddingR);
        Int32 scrollPos = (Int32)(scrollOffset * (width - paddingL - paddingR));
        Int32 xpos;
        if (GetScrollX(&xpos), scrollPos != xpos) {
            CompleteScroll(FALSE);
            Int32 ypos;
            GetScrollY(&ypos);
            ScrollTo(scrollPos, ypos);
        }
    }
}

ECode ViewPager::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    Int32 count;
    GetChildCount(&count);
    Int32 width = r - l;
    Int32 height = b - t;
    Int32 paddingLeft;
    GetPaddingLeft(&paddingLeft);
    Int32 paddingTop;
    GetPaddingTop(&paddingTop);
    Int32 paddingRight;
    GetPaddingRight(&paddingRight);
    Int32 paddingBottom;
    GetPaddingBottom(&paddingBottom);
    Int32 scrollX;
    GetScrollX(&scrollX);

    Int32 decorCount = 0;

    // First pass - decor views. We need to do this in two passes so that
    // we have the proper offsets for non-decor views later.
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child;
        GetChildAt(i, (IView**)&child);
        Int32 visibility;
        if (child->GetVisibility(&visibility), visibility != GONE) {
            AutoPtr<IViewGroupLayoutParams> temp;
            child->GetLayoutParams((IViewGroupLayoutParams**)&temp);
            AutoPtr<ViewPagerLayoutParams> lp = (ViewPagerLayoutParams*)temp.Get();
            Int32 childLeft = 0;
            Int32 childTop = 0;
            if (lp->mIsDecor) {
                Int32 hgrav = lp->mGravity & IGravity::HORIZONTAL_GRAVITY_MASK;
                Int32 vgrav = lp->mGravity & IGravity::VERTICAL_GRAVITY_MASK;
                switch (hgrav) {
                    default:
                        childLeft = paddingLeft;
                        break;
                    case IGravity::LEFT: {
                        childLeft = paddingLeft;
                        Int32 w;
                        child->GetMeasuredWidth(&w);
                        paddingLeft += w;
                        break;
                    }
                    case IGravity::CENTER_HORIZONTAL: {
                        Int32 w;
                        child->GetMeasuredWidth(&w);
                        childLeft = Elastos::Core::Math::Max((width - w) / 2, paddingLeft);
                        break;
                    }
                    case IGravity::RIGHT: {
                        Int32 w;
                        child->GetMeasuredWidth(&w);
                        childLeft = width - paddingRight - w;
                        paddingRight += w;
                        break;
                    }
                }
                switch (vgrav) {
                    default:
                        childTop = paddingTop;
                        break;
                    case IGravity::TOP: {
                        Int32 h;
                        child->GetMeasuredHeight(&h);
                        childTop = paddingTop;
                        paddingTop += h;
                        break;
                    }
                    case IGravity::CENTER_VERTICAL: {
                        Int32 h;
                        child->GetMeasuredHeight(&h);
                        childTop = Elastos::Core::Math::Max((height - h) / 2, paddingTop);
                        break;
                    }
                    case IGravity::BOTTOM: {
                        Int32 h;
                        child->GetMeasuredHeight(&h);
                        childTop = height - paddingBottom - h;
                        paddingBottom += h;
                        break;
                    }
                }
                childLeft += scrollX;
                Int32 w, h;
                child->GetMeasuredWidth(&w);
                child->GetMeasuredHeight(&h);
                child->Layout(childLeft, childTop, childLeft + w, childTop + h);
                decorCount++;
            }
        }
    }

    Int32 childWidth = width - paddingLeft - paddingRight;
    // Page views. Do this once we have the right padding offsets from above.
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child;
        GetChildAt(i, (IView**)&child);
        Int32 visibility;
        if (child->GetVisibility(&visibility), visibility != GONE) {
            AutoPtr<IViewGroupLayoutParams> temp;
            child->GetLayoutParams((IViewGroupLayoutParams**)&temp);
            AutoPtr<ViewPagerLayoutParams> lp = (ViewPagerLayoutParams*)temp.Get();
            AutoPtr<ItemInfo> ii;
            if (!lp->mIsDecor && (ii = InfoForChild(child)) != NULL) {
                Int32 loff = (Int32)(childWidth * ii->mOffset);
                Int32 childLeft = paddingLeft + loff;
                Int32 childTop = paddingTop;
                if (lp->mNeedsMeasure) {
                    // This was added during layout and needs measurement.
                    // Do it now that we know what we're working with.
                    lp->mNeedsMeasure = FALSE;
                    Int32 widthSpec = MeasureSpec::MakeMeasureSpec((Int32)(childWidth * lp->mWidthFactor),
                            MeasureSpec::EXACTLY);
                    Int32 heightSpec = MeasureSpec::MakeMeasureSpec((Int32)(height - paddingTop - paddingBottom),
                            MeasureSpec::EXACTLY);
                    child->Measure(widthSpec, heightSpec);
                }
                if (DEBUG) {
                    Int32 w, h;
                    child->GetMeasuredWidth(&w);
                    child->GetMeasuredHeight(&h);
                    Slogger::V(TAG, "Positioning #%d %s f=%s:%d,%d %dx%d",
                            i, TO_CSTR(child), TO_CSTR(ii->mObject), childLeft, childTop, w, h);
                }
                Int32 w, h;
                child->GetMeasuredWidth(&w);
                child->GetMeasuredHeight(&h);
                child->Layout(childLeft, childTop, childLeft + w, childTop + h);
            }
        }
    }
    mTopPageBounds = paddingTop;
    mBottomPageBounds = height - paddingBottom;
    mDecorChildCount = decorCount;

    if (mFirstLayout) {
        ScrollToItem(mCurItem, FALSE, 0, FALSE);
    }
    mFirstLayout = FALSE;
    return NOERROR;
}

ECode ViewPager::ComputeScroll()
{
    Boolean isFinished, result;
    if ((mScroller->IsFinished(&isFinished), !isFinished) &&
            (mScroller->ComputeScrollOffset(&result), result)) {
        Int32 oldX, oldY, x, y;
        GetScrollX(&oldX);
        GetScrollY(&oldY);
        mScroller->GetCurrX(&x);
        mScroller->GetCurrY(&y);

        if (oldX != x || oldY != y) {
            ScrollTo(x, y);
            Boolean pageScrolled;
            if (pageScrolled(x, &pageScrolled), !pageScrolled) {
                mScroller->AbortAnimation();
                ScrollTo(0, y);
            }
        }

        // Keep on drawing until the animation has finished.
        ViewCompat::PostInvalidateOnAnimation(IView::Probe(this));
        return NOERROR;
    }

    // Done with scroll, clean up state.
    CompleteScroll(TRUE);
    return NOERROR;
}

ECode ViewPager::PageScrolled(
    /* [in] */ Int32 xpos,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    Int32 size
    if (mItems->GetSize(&size), size == 0) {
        mCalledSuper = FALSE;
        OnPageScrolled(0, 0, 0);
        if (!mCalledSuper) {
            Slogger::E(TAG, "onPageScrolled did not call superclass implementation");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<ItemInfo> ii = InfoForCurrentScrollPosition();
    Int32 width;
    GetClientWidth(&width);
    Int32 widthWithMargin = width + mPageMargin;
    Float marginOffset = (Float)mPageMargin / width;
    Int32 currentPage = ii->mPosition;
    Float pageOffset = (((Float)xpos / width) - ii->mOffset) / (ii->mWidthFactor + marginOffset);
    Int32 offsetPixels = (Int32)(pageOffset * widthWithMargin);

    mCalledSuper = FALSE;
    OnPageScrolled(currentPage, pageOffset, offsetPixels);
    if (!mCalledSuper) {
        Slogger::E(TAG, "onPageScrolled did not call superclass implementation");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    *result = TRUE;
    return NOERROR;
}

ECode ViewPager::OnPageScrolled(
    /* [in] */ Int32 position,
    /* [in] */ Float offset,
    /* [in] */ Int32 offsetPixels)
{
    if (mDecorChildCount > 0) {
        Int32 scrollX, paddingLeft, paddingRight;
        GetScrollX(&scrollX);
        GetPaddingLeft(&paddingLeft);
        GetPaddingRight(&paddingRight);
        Int32 width;
        GetWidth(&width);
        Int32 childCount;
        GetChildCount(&childCount);
        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IView> child;
            GetChildAt(i, (IView**)&child);
            AutoPtr<IViewGroupLayoutParams> temp;
            child->GetLayoutParams((IViewGroupLayoutParams**)&temp);
            AutoPtr<ViewPagerLayoutParams> lp = (ViewPagerLayoutParams*)temp.Get();
            if (!lp->mIsDecor) continue;

            Int32 hgrav = lp->mGravity & IGravity::HORIZONTAL_GRAVITY_MASK;
            Int32 childLeft = 0;
            switch (hgrav) {
                default:
                    childLeft = paddingLeft;
                    break;
                case IGravity::LEFT: {
                    childLeft = paddingLeft;
                    Int32 w;
                    child->GetWidth(&w);
                    paddingLeft += w;
                    break;
                }
                case IGravity::CENTER_HORIZONTAL: {
                    Int32 w;
                    child->GetMeasuredWidth(&w);
                    childLeft = Elastos::Core::Math::Max((width - w) / 2, paddingLeft);
                    break;
                }
                case IGravity::RIGHT: {
                    Int32 w;
                    child->GetMeasuredWidth(&w);
                    childLeft = width - paddingRight - w;
                    paddingRight += w;
                    break;
                }
            }
            childLeft += scrollX;

            Int32 left;
            child->GetLeft(&left);
            Int32 childOffset = childLeft - left;
            if (childOffset != 0) {
                child->OffsetLeftAndRight(childOffset);
            }
        }
    }

    if (mOnPageChangeListener != NULL) {
        mOnPageChangeListener->OnPageScrolled(position, offset, offsetPixels);
    }
    if (mInternalPageChangeListener != NULL) {
        mInternalPageChangeListener->OnPageScrolled(position, offset, offsetPixels);
    }

    if (mPageTransformer != NULL) {
        Int32 scrollX;
        GetScrollX(&scrollX);
        Int32 childCount;
        GetChildCount(&childCount);
        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IView> child;
            GetChildAt(i, (IView**)&child);
            AutoPtr<IViewGroupLayoutParams> temp;
            child->GetLayoutParams((IViewGroupLayoutParams**)&temp);
            AutoPtr<ViewPagerLayoutParams> lp = (ViewPagerLayoutParams*)temp.Get();

            if (lp->mIsDecor) continue;

            Int32 left;
            child->GetLeft(&left);
            Int32 width;
            GetClientWidth(&width);
            Float transformPos = (Float)(left - scrollX) / width;
            mPageTransformer->TransformPage(child, transformPos);
        }
    }

    mCalledSuper = TRUE;
    return NOERROR;
}

void ViewPager::CompleteScroll(
    /* [in] */ Boolean postEvents)
{
    Boolean needPopulate = mScrollState == SCROLL_STATE_SETTLING;
    if (needPopulate) {
        // Done with scroll, no longer want to cache view drawing.
        SetScrollingCacheEnabled(FALSE);
        mScroller->AbortAnimation();
        Int32 oldX, oldy;
        GetScrollX(&oldX);
        GetScrollY(&oldY);
        Int32 x, y;
        mScroller->GetCurrX(&x);
        mScroller->GetCurrY(&y);
        if (oldX != x || oldY != y) {
            ScrollTo(x, y);
        }
    }
    mPopulatePending = FALSE;
    Int32 size;
    mItems->GetSize(&size);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> value;
        mItems->Get(i, (IInterface**)&value);
        AutoPtr<ItemInfo> ii = (ItemInfo*)(IObject*)value.Get();
        if (ii->mScrolling) {
            needPopulate = TRUE;
            ii.scrolling = FALSE;
        }
    }
    if (needPopulate) {
        if (postEvents) {
            ViewCompat::PostOnAnimation(IView::Probe(this), mEndScrollRunnable);
        }
        else {
            mEndScrollRunnable->Run();
        }
    }
}

Boolean ViewPager::IsGutterDrag(
    /* [in] */ Float x,
    /* [in] */ Float dx)
{
    Int32 width;
    GetWidth(&width);
    return (x < mGutterSize && dx > 0) || (x > width - mGutterSize && dx < 0);
}

void ViewPager::EnableLayers(
    /* [in] */ Boolean enable)
{
    Int32 childCount;
    GetChildCount(&childCount);
    for (Int32 i = 0; i < childCount; i++) {
        Int32 layerType = enable ?
                ViewCompat::LAYER_TYPE_HARDWARE : ViewCompat::LAYER_TYPE_NONE;
        AutoPtr<IView> child;
        GetChildAt(i, (IView**)&child);
        ViewCompat::SetLayerType(child, layerType, NULL);
    }
}

ECode ViewPager::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)

    /*
     * This method JUST determines whether we want to intercept the motion.
     * If we return true, onMotionEvent will be called and we do the actual
     * scrolling there.
     */

    Int32 a;
    ev->GetAction(&a);
    Int32 action = a & IMotionEvent::ACTION_MASK;

    // Always take care of the touch gesture being complete.
    if (action == IMotionEvent::ACTION_CANCEL || action == IMotionEvent::ACTION_UP) {
        // Release the drag.
        if (DEBUG) Slogger::V(TAG, "Intercept done!");
        mIsBeingDragged = FALSE;
        mIsUnableToDrag = FALSE;
        mActivePointerId = INVALID_POINTER;
        if (mVelocityTracker != NULL) {
            mVelocityTracker->Recycle();
            mVelocityTracker = NULL;
        }
        *res = FALSE;
        return NOERROR;
    }

    // Nothing more to do here if we have decided whether or not we
    // are dragging.
    if (action != IMotionEvent::ACTION_DOWN) {
        if (mIsBeingDragged) {
            if (DEBUG) Slogger::V(TAG, "Intercept returning true!");
            *res = TRUE;
            return NOERROR;
        }
        if (mIsUnableToDrag) {
            if (DEBUG) Slogger::V(TAG, "Intercept returning false!");
            *res = FALSE;
            return NOERROR;
        }
    }

    switch (action) {
        case IMotionEvent::ACTION_MOVE: {
            /*
             * mIsBeingDragged == false, otherwise the shortcut would have caught it. Check
             * whether the user has moved far enough from his original down touch.
             */

            /*
            * Locally do absolute value. mLastMotionY is set to the y value
            * of the down event.
            */
            Int32 activePointerId = mActivePointerId;
            if (activePointerId == INVALID_POINTER) {
                // If we don't have a valid id, the touch down wasn't on content.
                break;
            }

            Int32 pointerIndex;
            ev->FindPointerIndex(activePointerId, &pointerIndex);
            Float x;
            ev->GetX(pointerIndex, &x);
            Float dx = x - mLastMotionX;
            Float xDiff = Elastos::Core::Math::Abs(dx);
            Float y;
            ev->GetY(pointerIndex, &y);
            Float yDiff = Elastos::Core::Math::Abs(y - mInitialMotionY);
            if (DEBUG) Slogger::V(TAG, "Moved x to %f,%f diff=%f,%f", x, y, xDiff, yDiff);

            if (dx != 0 && !IsGutterDrag(mLastMotionX, dx) &&
                    CanScroll(IView::Probe(this), FALSE, (Int32)dx, (Int32)x, (Int32)y)) {
                // Nested view has scrollable area under this point. Let it be handled there.
                mLastMotionX = x;
                mLastMotionY = y;
                mIsUnableToDrag = TRUE;
                *res = FALSE;
                return NOERROR;
            }
            if (xDiff > mTouchSlop && xDiff * 0.5f > yDiff) {
                if (DEBUG) Slogger::V(TAG, "Starting drag!");
                mIsBeingDragged = TRUE;
                RequestParentDisallowInterceptTouchEvent(TRUE);
                SetScrollState(SCROLL_STATE_DRAGGING);
                mLastMotionX = dx > 0 ? mInitialMotionX + mTouchSlop : mInitialMotionX - mTouchSlop;
                mLastMotionY = y;
                SetScrollingCacheEnabled(TRUE);
            }
            else if (yDiff > mTouchSlop) {
                // The finger has moved enough in the vertical
                // direction to be counted as a drag...  abort
                // any attempt to drag horizontally, to work correctly
                // with children that have scrolling containers.
                if (DEBUG) Slogger::V(TAG, "Starting unable to drag!");
                mIsUnableToDrag = TRUE;
            }
            if (mIsBeingDragged) {
                // Scroll to follow the motion event
                if (PerformDrag(x)) {
                    ViewCompat::PostInvalidateOnAnimation(IView::Probe(this);
                }
            }
            break;
        }

        case IMotionEvent::ACTION_DOWN: {
            /*
             * Remember location of down touch.
             * ACTION_DOWN always refers to pointer index 0.
             */
            Float x, y;
            ev->GetX(&x);
            ev->GetY(&y);
            mLastMotionX = mInitialMotionX = x;
            mLastMotionY = mInitialMotionY = y;
            ev->GetPointerId(0, &mActivePointerId);
            mIsUnableToDrag = FALSE;

            mScroller->ComputeScrollOffset();
            if (mScrollState == SCROLL_STATE_SETTLING) {
                Int32 finalX, currX;
                mScroller->GetFinalX(&finalX);
                mScroller->GetCurrX(currX);
                if (Elastos::Core::Math::Abs(finalX - currX) > mCloseEnough) {
                    // Let the user 'catch' the pager as it animates.
                    mScroller->AbortAnimation();
                    mPopulatePending = FALSE;
                    Populate();
                    mIsBeingDragged = TRUE;
                    RequestParentDisallowInterceptTouchEvent(TRUE);
                    SetScrollState(SCROLL_STATE_DRAGGING);
                }
                else {
                    CompleteScroll(FALSE);
                    mIsBeingDragged = FALSE;
                }
            }
            else {
                CompleteScroll(FALSE);
                mIsBeingDragged = FALSE;
            }

            if (DEBUG) Slogger::V(TAG, "Down at %d,%d mIsBeingDragged=%dmIsUnableToDrag=%d",
                    mLastMotionX, mLastMotionY, mIsBeingDragged, mIsUnableToDrag);
            break;
        }

        case IMotionEvent::ACTION_POINTER_UP:
            OnSecondaryPointerUp(ev);
            break;
    }

    if (mVelocityTracker == NULL) {
        AutoPtr<IVelocityTrackerHelper> helper;
        CVelocityTrackerHelper::AcquireSingleton((IVelocityTrackerHelper**)&helper);
        helper->Obtain((IVelocityTracker**)&mVelocityTracker);
    }
    mVelocityTracker->AddMovement(ev);

    /*
     * The only time we want to intercept motion events is if we are in the
     * drag mode.
     */
    *res = mIsBeingDragged;
    return NOERROR;
}

ECode ViewPager::OnTouchEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    if (mFakeDragging) {
        // A fake drag is in progress already, ignore this real one
        // but still eat the touch events.
        // (It is likely that the user is multi-touching the screen.)
        *res = TRUE;
        return NOERROR;
    }

    Int32 action, flags;
    if ((ev->GetAction(&action), action == IMotionEvent::ACTION_DOWN) &&
            (ev->GetEdgeFlags(&flags), flags != 0)) {
        // Don't handle edge touches immediately -- they may actually belong to one of our
        // descendants.
        *res = FALSE;
        return NOERROR;
    }

    Int32 count;
    if (mAdapter == NULL || (mAdapter->GetCount(&count), count == 0)) {
        // Nothing to present or scroll; nothing to touch.
        *res = FALSE;
        return NOERROR;
    }

    if (mVelocityTracker == NULL) {
        AutoPtr<IVelocityTrackerHelper> helper;
        CVelocityTrackerHelper::AcquireSingleton((IVelocityTrackerHelper**)&helper);
        helper->Obtain((IVelocityTracker**)&mVelocityTracker);
    }
    mVelocityTracker->AddMovement(ev);

    ev->GetAction(&action);
    Boolean needsInvalidate = FALSE;

    switch (action & IMotionEvent::ACTION_MASK) {
        case IMotionEvent::ACTION_DOWN: {
            mScroller->AbortAnimation();
            mPopulatePending = FALSE;
            Populate();

            // Remember where the motion event started
            Float x, y;
            ev->GetX(&x);
            ev->GetY(&y);
            mLastMotionX = mInitialMotionX = x;
            mLastMotionY = mInitialMotionY = y;
            ev->GetPointerId(0, &mActivePointerId);
            break;
        }
        case IMotionEvent::ACTION_MOVE:
            if (!mIsBeingDragged) {
                Int32 pointerIndex;
                ev->FindPointerIndex(mActivePointerId, &pointerIndex);
                Float x;
                ev->GetX(pointerIndex, &x);
                Float xDiff = Elastos::Core::Math::Abs(x - mLastMotionX);
                Float y;
                ev->GetY(pointerIndex, &y);
                Float yDiff = Elastos::Core::Math::Abs(y - mLastMotionY);
                if (DEBUG) Slogger::V(TAG, "Moved x to %f,%f diff=%f,%f", x, y, xDiff, yDiff);
                if (xDiff > mTouchSlop && xDiff > yDiff) {
                    if (DEBUG) Slogger::V(TAG, "Starting drag!");
                    mIsBeingDragged = TRUE;
                    RequestParentDisallowInterceptTouchEvent(TRUE);
                    mLastMotionX = x - mInitialMotionX > 0 ? mInitialMotionX + mTouchSlop :
                            mInitialMotionX - mTouchSlop;
                    mLastMotionY = y;
                    SetScrollState(SCROLL_STATE_DRAGGING);
                    SetScrollingCacheEnabled(TRUE);

                    // Disallow Parent Intercept, just in case
                    AutoPtr<IViewParent> parent;
                    GetParent((IViewParent**)&parent);
                    if (parent != NULL) {
                        parent->RequestDisallowInterceptTouchEvent(TRUE);
                    }
                }
            }
            // Not else! Note that mIsBeingDragged can be set above.
            if (mIsBeingDragged) {
                // Scroll to follow the motion event
                Int32 activePointerIndex;
                ev->FindPointerIndex(mActivePointerId, &activePointerIndex);
                Float x;
                ev->GetX(activePointerIndex, &x);
                needsInvalidate |= PerformDrag(x);
            }
            break;
        case IMotionEvent::ACTION_UP:
            if (mIsBeingDragged) {
                AutoPtr<IVelocityTracker> velocityTracker = mVelocityTracker;
                velocityTracker->ComputeCurrentVelocity(1000, mMaximumVelocity);
                Float xVelocity;
                velocityTracker->GetXVelocity(velocityTracker, mActivePointerId, &xVelocity);
                Int32 initialVelocity = (Int32)xVelocity;
                mPopulatePending = TRUE;
                Int32 width;
                GetClientWidth(&width);
                Int32 scrollX;
                GetScrollX(&scrollX);
                AutoPtr<ItemInfo> ii = InfoForCurrentScrollPosition();
                Int32 currentPage = ii->mPosition;
                Float pageOffset = (((Float)scrollX / width) - ii->mOffset) / ii->mWidthFactor;
                Int32 activePointerIndex;
                ev->FindPointerIndex(mActivePointerId, &activePointerIndex);
                Float x;
                ev->GetX(activePointerIndex, &x);
                Int32 totalDelta = (Int32)(x - mInitialMotionX);
                Int32 nextPage = DetermineTargetPage(currentPage, pageOffset, initialVelocity,
                        totalDelta);
                SetCurrentItemInternal(nextPage, TRUE, TRUE, initialVelocity);

                mActivePointerId = INVALID_POINTER;
                EndDrag();
                Boolean onReleaseL, onReleaseR;
                mLeftEdge->OnRelease(&onReleaseL);
                mRightEdge->OnRelease(&onReleaseR);
                needsInvalidate = mLeftEdge.onRelease() | onReleaseR;
            }
            break;
        case IMotionEvent::ACTION_CANCEL:
            if (mIsBeingDragged) {
                ScrollToItem(mCurItem, TRUE, 0, FALSE);
                mActivePointerId = INVALID_POINTER;
                EndDrag();
                Boolean onReleaseL, onReleaseR;
                mLeftEdge->OnRelease(&onReleaseL);
                mRightEdge->OnRelease(&onReleaseR);
                needsInvalidate = onReleaseL | onReleaseR;
            }
            break;
        case IMotionEvent::ACTION_POINTER_DOWN: {
            Int32 index;
            ev->GetActionIndex(ev, &index);
            Float x;
            ev->GetX(index, &x);
            mLastMotionX = x;
            ev->GetPointerId(index, &mActivePointerId);
            break;
        }
        case IMotionEvent::ACTION_POINTER_UP:
            OnSecondaryPointerUp(ev);
            Int32 index;
            ev->FindPointerIndex(mActivePointerId, &index);
            ev->GetX(index, &mLastMotionX);
            break;
    }
    if (needsInvalidate) {
        ViewCompat::PostInvalidateOnAnimation(IView::Probe(this));
    }
    *res = TRUE;
}

void ViewPager::RequestParentDisallowInterceptTouchEvent(
    /* [in] */ Boolean disallowIntercept)
{
    AutoPtr<IViewParent> parent;
    GetParent((IViewParent**)&parent);
    if (parent != NULL) {
        parent->RequestDisallowInterceptTouchEvent(disallowIntercept);
    }
}

Boolean ViewPager::PerformDrag(
    /* [in] */ Float x)
{
    Boolean needsInvalidate = FALSE;

    Float deltaX = mLastMotionX - x;
    mLastMotionX = x;

    Float oldScrollX;
    GetScrollX(&oldScrollX);
    Float scrollX = oldScrollX + deltaX;
    Int32 width;
    GetClientWidth(&width);

    Float leftBound = width * mFirstOffset;
    Float rightBound = width * mLastOffset;
    Boolean leftAbsolute = TRUE;
    Boolean rightAbsolute = TRUE;

    AutoPtr<IInterface> value;
    mItems->Get(0, (IInterface**)&value);
    AutoPtr<ItemInfo> firstItem = (ItemInfo*)(IObject*)value.Get();
    Int32 size;
    mItems->GetSize(&size);
    AutoPtr<IInterface> last;
    mItems->Get(size - 1, (IInterface**)&last);
    AutoPtr<ItemInfo> lastItem = (ItemInfo*)last.Get();
    if (firstItem->mPosition != 0) {
        leftAbsolute = FALSE;
        leftBound = firstItem->mOffset * width;
    }
    Int32 count;
    mAdapter->GetCount(&count);
    if (lastItem->mPosition != count - 1) {
        rightAbsolute = FALSE;
        rightBound = lastItem->mOffset * width;
    }

    if (scrollX < leftBound) {
        if (leftAbsolute) {
            Float over = leftBound - scrollX;
            mLeftEdge->OnPull(Elastos::Core::Math::Abs(over) / width, &needsInvalidate);
        }
        scrollX = leftBound;
    }
    else if (scrollX > rightBound) {
        if (rightAbsolute) {
            Float over = scrollX - rightBound;
            mRightEdge->OnPull(Elastos::Core::Math::Abs(over) / width, &needsInvalidate);
        }
        scrollX = rightBound;
    }
    // Don't lose the rounded component
    mLastMotionX += scrollX - (Int32)scrollX;
    Int32 scrollY;
    GetScrollY(&scrollY);
    ScrollTo((Int32)scrollX, scrollY);
    PageScrolled((Int32)scrollX);

    return needsInvalidate;
}

AutoPtr<ItemInfo> ViewPager::InfoForCurrentScrollPosition()
{
    Int32 width;
    GetClientWidth(&width);
    Int32 scrollX;
    GetScrollX(&scrollX);
    Float scrollOffset = width > 0 ? (Float)scrollX / width : 0;
    Float marginOffset = width > 0 ? (Float)mPageMargin / width : 0;
    Int32 lastPos = -1;
    Float lastOffset = 0.f;
    Float lastWidth = 0.f;
    Boolean first = TRUE;

    AutoPtr<ItemInfo> lastItem;
    mItems->GetSize(&size);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> value;
        mItems->Get(i, (IInterface**)&value);
        AutoPtr<ItemInfo> ii = (ItemInfo*)value.Get();
        Float offset;
        if (!first && ii->mPosition != lastPos + 1) {
            // Create a synthetic item for a missing page.
            ii = mTempItem;
            ii->mOffset = lastOffset + lastWidth + marginOffset;
            ii->mPosition = lastPos + 1;
            mAdapter->GetPageWidth(ii->mPosition, &ii->mWidthFactor);
            i--;
        }
        offset = ii->mOffset;

        Float leftBound = offset;
        Float rightBound = offset + ii->mWidthFactor + marginOffset;
        if (first || scrollOffset >= leftBound) {
            if (scrollOffset < rightBound || (mItems->GetSize(&size), i == size - 1) {
                return ii;
            }
        }
        else {
            return lastItem;
        }
        first = FALSE;
        lastPos = ii->Position;
        lastOffset = offset;
        lastWidth = ii->mWidthFactor;
        lastItem = ii;
    }

    return lastItem;
}

Int32 ViewPager::DetermineTargetPage(
    /* [in] */ Int32 currentPage,
    /* [in] */ Float pageOffset,
    /* [in] */ Int32 velocity,
    /* [in] */ Int32 deltaX)
{
    Int32 targetPage;
    if (Elastos::Core::Math::Abs(deltaX) > mFlingDistance && Elastos::Core::Math::Abs(velocity) > mMinimumVelocity) {
        targetPage = velocity > 0 ? currentPage : currentPage + 1;
    }
    else {
        Float truncator = currentPage >= mCurItem ? 0.4f : 0.6f;
        targetPage = (Int32)(currentPage + pageOffset + truncator);
    }

    Int32 size;
    if (mItems->GetSize(&size), size > 0) {
        AutoPtr<IInterface> first;
        mItems->Get(0, (IInterface**)&first);
        AutoPtr<ItemInfo> firstItem = (ItemInfo*)(IObject*)first.Get();
        AutoPtr<IInterface> last;
        mItems->Get(size - 1, (IInterface**)&last);
        AutoPtr<ItemInfo> lastItem = (ItemInfo*)(IObject*)last.Get();

        // Only let the user target pages we have items for
        targetPage = Elastos::Core::Math::Max(
                firstItem->mPosition, Elastos::Core::Math::Min(targetPage, lastItem->mPosition));
    }

    return targetPage;
}

ECode ViewPager::Draw(
    /* [in] */ ICanvas* canvas)
{
    ViewGroup::Draw(canvas);
    Boolean needsInvalidate = FALSE;

    Int32 overScrollMode = ViewCompat::GetOverScrollMode(IView::Probe(this));
    Int32 count;
    if (overScrollMode == ViewCompat::OVER_SCROLL_ALWAYS ||
            (overScrollMode == ViewCompat::OVER_SCROLL_IF_CONTENT_SCROLLS &&
                    mAdapter != NULL && (mAdapter->GetCount(&count), count > 1))) {
        Boolean isFinished;
        if (mLeftEdge->IsFinished(&isFinished), !isFinished) {
            Int32 restoreCount;
            canvas->Save(&restoreCount);
            Int32 h, paddingTop, paddingBottom;
            GetHeight(&h);
            GetPaddingTop(&paddingTop);
            GetPaddingBottom(&paddingBottom);
            Int32 height = h - paddingTop - paddingBottom;
            Int32 width;
            GetWidth(&width);

            canvas->Rotate(270);
            canvas->Translate(-height + paddingTop, mFirstOffset * width);
            mLeftEdge->SetSize(height, width);
            Boolean result;
            mLeftEdge->Draw(canvas, &result);
            needsInvalidate |= result;
            canvas->RestoreToCount(restoreCount);
        }
        if (mRightEdge->IsFinished(&isFinished), !isFinished) {
            Int32 restoreCount;
            canvas->Save(&restoreCount);
            Int32 width;
            GetWidth(&width);
            Int32 h, paddingTop, paddingBottom;
            GetHeight(&h);
            GetPaddingTop(&paddingTop);
            GetPaddingBottom(&paddingBottom);
            Int32 height = h - paddingTop - paddingBottom;

            canvas->Rotate(90);
            canvas->Translate(-paddingTop, -(mLastOffset + 1) * width);
            mRightEdge->SetSize(height, width);
            Boolean result;
            mRightEdge->Draw(canvas, &result);
            needsInvalidate |= result;
            canvas->RestoreToCount(restoreCount);
        }
    }
    else {
        mLeftEdge->Finish();
        mRightEdge->Finish();
    }

    if (needsInvalidate) {
        // Keep animating
        ViewCompat::PostInvalidateOnAnimation(this);
    }
    return NOERROR;
}

void ViewPager::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    ViewGroup::OnDraw(canvas);

    // Draw the margin drawable between pages if needed.
    Int32 size
    if (mPageMargin > 0 && mMarginDrawable != null && (mItems->GetSize(&size), size > 0) && mAdapter != NULL) {
        Int32 scrollX;
        GetScrollX(&scrollX);
        Int32 width;
        GetWidth(&width);

        Float marginOffset = (Float)mPageMargin / width;
        Int32 itemIndex = 0;
        AutoPtr<IInterface> first;
        mItems->Get(0, (IInterface**)&first);
        AutoPtr<ItemInfo> ii = (ItemInfo*)(IObject*)first.Get();
        Float offset = ii->mOffset;
        Int32 itemCount;
        mItems->GetSize(&itemCount);
        Int32 firstPos = ii->mPosition;
        AutoPtr<IInterface> last;
        mItems->Get(itemCount - 1, (IInterface**)&last);
        AutoPtr<ItemInfo> lastItem = (ItemInfo*)(IObject*)last.Get();
        Int32 lastPos = lastItem->mPosition;
        for (Int32 pos = firstPos; pos < lastPos; pos++) {
            while (pos > ii->mPosition && itemIndex < itemCount) {
                AutoPtr<IInterface> value;
                mItems->Get(++itemIndex, (IInterface**)&value);
                ii = (ItemInfo*)(IObject*)value.Get();
            }

            Float drawAt;
            if (pos == ii->mPosition) {
                drawAt = (ii->mOffset + ii->mWidthFactor) * width;
                offset = ii->mOffset + ii->mWidthFactor + marginOffset;
            }
            else {
                Float widthFactor;
                mAdapter->GetPageWidth(pos, &widthFactor);
                drawAt = (offset + widthFactor) * width;
                offset += widthFactor + marginOffset;
            }

            if (drawAt + mPageMargin > scrollX) {
                mMarginDrawable->SetBounds((Int32)drawAt, mTopPageBounds,
                        (Int32)(drawAt + mPageMargin + 0.5f), mBottomPageBounds);
                mMarginDrawable->Draw(canvas);
            }

            if (drawAt > scrollX + width) {
                break; // No more visible, no sense in continuing
            }
        }
    }
}

ECode ViewPager::BeginFakeDrag(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (mIsBeingDragged) {
        *result = FALSE;
        return NOERROR;
    }
    mFakeDragging = TRUE;
    setScrollState(SCROLL_STATE_DRAGGING);
    mInitialMotionX = mLastMotionX = 0;
    if (mVelocityTracker == NULL) {
        AutoPtr<IVelocityTrackerHelper> helper;
        CVelocityTrackerHelper::AcquireSingleton((IVelocityTrackerHelper**)&helper);
        helper->Obtain((IVelocityTracker**)&mVelocityTracker);
    }
    else {
        mVelocityTracker->Clear();
    }
    Int64 time = SystemClock::GetUptimeMillis();
    AutoPtr<IMotionEventHelper> helper;
    CMotionEventHelper::AcquireSingleton((IMotionEventHelper**)&helper);
    AutoPtr<IMotionEvent> ev;
    helper->Obtain(time, time, IMotionEvent::ACTION_DOWN, 0, 0, 0, (IMotionEvent**)&ev);
    mVelocityTracker->AddMovement(ev);
    ev->Recycle();
    mFakeDragBeginTime = time;
    *result = TRUE;
    return NOERROR;
}

ECode ViewPager::EndFakeDrag()
{
    if (!mFakeDragging) {
        Slogger::E(TAG, "No fake drag in progress. Call beginFakeDrag first.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoPtr<IVelocityTracker> velocityTracker = mVelocityTracker;
    velocityTracker->ComputeCurrentVelocity(1000, mMaximumVelocity);
    Float xVelocity;
    velocityTracker->GetXVelocity(velocityTracker, mActivePointerId, &xVelocity);
    Int32 initialVelocity = (Int32)xVelocity;
    mPopulatePending = TRUE;
    Int32 width;
    GetClientWidth(&width);
    Int32 scrollX;
    GetScrollX(&scrollX);
    AutoPtr<ItemInfo> ii = InfoForCurrentScrollPosition();
    Int32 currentPage = ii->mPosition;
    Float pageOffset = (((Float) scrollX / width) - ii->mOffset) / ii->mWidthFactor;
    Int32 totalDelta = (Int32)(mLastMotionX - mInitialMotionX);
    Int32 nextPage = DetermineTargetPage(currentPage, pageOffset, initialVelocity, totalDelta);
    SetCurrentItemInternal(nextPage, TRUE, TRUE, initialVelocity);
    EndDrag();

    mFakeDragging = FALSE;
    return NOERROR;
}

ECode ViewPager::FakeDragBy(
    /* [in] */ Float xOffset)
{
    // begin from this
    if (!mFakeDragging) {
        throw new IllegalStateException("No fake drag in progress. Call beginFakeDrag first.");
    }

    mLastMotionX += xOffset;

    float oldScrollX = getScrollX();
    float scrollX = oldScrollX - xOffset;
    final int width = getClientWidth();

    float leftBound = width * mFirstOffset;
    float rightBound = width * mLastOffset;

    final ItemInfo firstItem = mItems.get(0);
    final ItemInfo lastItem = mItems.get(mItems.size() - 1);
    if (firstItem.position != 0) {
        leftBound = firstItem.offset * width;
    }
    if (lastItem.position != mAdapter.getCount() - 1) {
        rightBound = lastItem.offset * width;
    }

    if (scrollX < leftBound) {
        scrollX = leftBound;
    } else if (scrollX > rightBound) {
        scrollX = rightBound;
    }
    // Don't lose the rounded component
    mLastMotionX += scrollX - (int) scrollX;
    scrollTo((int) scrollX, getScrollY());
    pageScrolled((int) scrollX);

    // Synthesize an event for the VelocityTracker.
    final long time = SystemClock.uptimeMillis();
    final MotionEvent ev = MotionEvent.obtain(mFakeDragBeginTime, time, MotionEvent.ACTION_MOVE,
            mLastMotionX, 0, 0);
    mVelocityTracker.addMovement(ev);
    ev.recycle();
}

ECode ViewPager::IsFakeDragging(
    /* [out] */ Boolean* result);

ECode ViewPager::CanScrollHorizontally(
    /* [in] */ Int32 direction,
    /* [out] */ Boolean* canScroll);

ECode ViewPager::CanScroll(
    /* [in] */ IView* v,
    /* [in] */ Boolean checkV,
    /* [in] */ Int32 dx,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [out] */ Boolean* canScroll);

ECode ViewPager::ExecuteKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result);

ECode ViewPager::ArrowScroll(
    /* [in] */ Int32 direction,
    /* [out] */ Boolean* result);

} // namespace View
} // namespace V4
} // namespace Support
} // namespace Droid
} // namespace Elastos
