#ifndef __ELASTOS_APPS_DIALER_LIST_SHORTCUTCARDSADAPTER_H__
#define __ELASTOS_APPS_DIALER_LIST_SHORTCUTCARDSADAPTER_H__

#include "BaseAdapter.h"

namespace Elastos{
namespace Apps{
namespace Dialer {
namespace List {

/**
 * An adapter that displays call shortcuts from {@link com.android.dialer.calllog.CallLogAdapter}
 * in the form of cards.
 */
class ShortcutCardsAdapter
    : public BaseAdapter
    , public IShortcutCardsAdapter
{
public:
    /**
     * The swipeable call log row.
     */
    class SwipeableShortcutCard
        : public FrameLayout
        , public ISwipeHelperCallback
    {
    public:
        CAR_INTERFACE_DECL()

        SwipeableShortcutCard(
            /* [in] */ ShortcutCardsAdapter* host);

        CARAPI constructor(
            /* [in] */ IContext* context);

        // @Override
        CARAPI GetChildAtPosition(
            /* [in] */ IMotionEvent* ev,
            /* [out] */ IView** child);

        // @Override
        CARAPI GetChildContentView(
            /* [in] */ IView* v,
            /* [out] */ IView** child);

        // @Override
        CARAPI OnScroll();

        // @Override
        CARAPI CanChildBeDismissed(
            /* [in] */ IView* v,
            /* [out] */ Boolean* result);

        // @Override
        CARAPI OnBeginDrag(
            /* [in] */ IView* v);

        // @Override
        CARAPI OnChildDismissed(
            /* [in] */ IView* v);

        // @Override
        CARAPI OnDragCancelled(
            /* [in] */ IView* v);

        // @Override
        CARAPI OnInterceptTouchEvent(
            /* [in] */ IMotionEvent ev,
            /* [out] */ Boolean* result);

        // @Override
        CARAPI OnTouchEvent(
            /* [in] */ IMotionEvent ev,
            /* [out] */ Boolean* result);

        CARPAI_(void) SetOnItemSwipeListener(
            /* [in] */ ISwipeHelperOnItemGestureListener* listener);

        /**
         * Clips the card by a specified amount.
         *
         * @param ratioHidden A float indicating how much of each edge of the card should be
         *         clipped. If 0, the entire card is displayed. If 0.5f, each edge is hidden
         *         entirely, thus obscuring the entire card.
         */
        CARPAI_(void) ClipCard(
            /* [in] */ Float ratioHidden);

    private:
        CARPAI_(void) PrepareChildView(
            /* [in] */ IView* view);

        CARPAI_(void) SetChildrenOpacity(
            /* [in] */ IViewGroup* viewGroup,
            /* [in] */ Float alpha);

    private:
        ShortcutCardsAdapter* mHost;
        AutoPtr<ISwipeHelper> mSwipeHelper;
        AutoPtr<ISwipeHelperOnItemGestureListener> mOnItemSwipeListener;

        Float mPreviousTranslationZ; // = 0;
        AutoPtr<IRect> mClipRect; // = new Rect();
    };

private:
    class CustomDataSetObserver
        : public DataSetObserver
    {
    public:
        CustomDataSetObserver(
            /* [in] */ ShortcutCardsAdapter* host);

        // @Override
        CARAPI OnChanged();

    private:
        ShortcutCardsAdapter* mHost;
    };

    class CallLogOnItemSwipeListener
        : public Object
        , public ISwipeHelperOnItemGestureListener
    {
    public:
        CAR_INTERFACE_DECL()

        CallLogOnItemSwipeListener(
            /* [in] */ ShortcutCardsAdapter* host);

        // @Override
        CARAPI OnSwipe(
            /* [in] */ IView* view);

        // @Override
        CARAPI OnTouch();

        // @Override
        CARAPI IsSwipeEnabled(
            /* [out] */ Boolean* result);

    private:
        ShortcutCardsAdapter* mHost;
    };

    class CallLogQueryHandlerListener
        : public Object
        : public ICallLogQueryHandlerListener
    {
    public:
        CAR_INTERFACE_DECL()

        CallLogQueryHandlerListener(
            /* [in] */ ShortcutCardsAdapter* host);

        // @Override
        CARAPI OnVoicemailStatusFetched(
            /* [in] */ ICursor* statusCursor);

        // @Override
        CARAPI OnCallsFetched(
            /* [in] */ ICursor* combinedCursor,
            /* [out] */ Boolean* result);

    private:
        ShortcutCardsAdapter* mHost;
    };

public:
    CAR_INTERFACE_DECL()

    ShortcutCardsAdapter();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IListsFragment* fragment,
        /* [in] */ ICallLogAdapter* callLogAdapter);

    /**
     * Determines the number of items in the adapter.
     * mCallLogAdapter contains the item for the most recent caller.
     * mContactTileAdapter contains the starred contacts.
     * The +1 is to account for the presence of the favorites menu.
     *
     * @return Number of items in the adapter.
     */
    // @Override
    CARAPI GetCount(
        /* [out] */ Int32* count);

    // @Override
    CARAPI GetItem(
        /* [in] */ Int32 position,
        /* [out] */ IInterface** item);

    // @Override
    CARAPI GetItemId(
        /* [in] */ Int32 position,
        /* [out] */ Int64** id);

    // @Override
    CARAPI HasStableIds(
        /* [out] */ Boolean* result);

    /**
     * Determine the number of view types present.
     */
    // @Override
    CARAPI GetViewTypeCount(
        /* [out] */ Int32* count);

    // @Override
    CARAPI GetItemViewType(
        /* [in] */ Int32 position,
        /* [out] */ Int32* type);

    // @Override
    CARAPI GetView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent,
        /* [out] */ IView** view);

    // @Override
    CARAPI AreAllItemsEnabled(
        /* [out] */ Boolean* enabled);

    // @Override
    CARAPI IsEnabled(
        /* [in] */ Int32 position,
        /* [out] */ Boolean* enabled);

private:
    static const String TAG; // = ShortcutCardsAdapter.class.getSimpleName();
    static const Float CLIP_CARD_BARELY_HIDDEN_RATIO; // = 0.001f;
    static const Float CLIP_CARD_MOSTLY_HIDDEN_RATIO; // = 0.9f;
    // Fade out 5x faster than the hidden ratio.
    static const Float CLIP_CARD_OPACITY_RATIO; // = 5f;

    AutoPtr<ICallLogAdapter> mCallLogAdapter;

    AutoPtr<IListsFragment> mFragment;

    Int32 mCallLogMarginHorizontal;
    Int32 mCallLogMarginTop;
    Int32 mCallLogMarginBottom;
    Int32 mCallLogPaddingStart;
    Int32 mCallLogPaddingTop;
    Int32 mCallLogPaddingBottom;
    Int32 mCardMaxHorizontalClip;
    Int32 mShortCardBackgroundColor;

    AutoPtr<IContext> mContext;

    AutoPtr<IDataSetObserver> mObserver;

    AutoPtr<ICallLogQueryHandler> mCallLogQueryHandler;

    AutoPtr<ISwipeHelperOnItemGestureListener> mCallLogOnItemSwipeListene;
    AutoPtr<ICallLogQueryHandlerListener> mCallLogQueryHandlerListener;
};

} // List
} // Dialer
} // Apps
} // Elastos

#endif //__ELASTOS_APPS_DIALER_LIST_SHORTCUTCARDSADAPTER_H__
