// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by code_generator_v8.py. DO NOT MODIFY!

#ifndef V8VTTCue_h
#define V8VTTCue_h

#include "bindings/core/v8/V8TextTrackCue.h"
#include "bindings/v8/V8Binding.h"
#include "bindings/v8/V8DOMWrapper.h"
#include "bindings/v8/WrapperTypeInfo.h"
#include "core/html/track/vtt/VTTCue.h"
#include "platform/heap/Handle.h"

namespace WebCore {

class V8VTTCue {
public:
    static bool hasInstance(v8::Handle<v8::Value>, v8::Isolate*);
    static v8::Handle<v8::Object> findInstanceInPrototypeChain(v8::Handle<v8::Value>, v8::Isolate*);
    static v8::Handle<v8::FunctionTemplate> domTemplate(v8::Isolate*);
    static VTTCue* toNative(v8::Handle<v8::Object> object)
    {
        return fromInternalPointer(object->GetAlignedPointerFromInternalField(v8DOMWrapperObjectIndex));
    }
    static VTTCue* toNativeWithTypeCheck(v8::Isolate*, v8::Handle<v8::Value>);
    static const WrapperTypeInfo wrapperTypeInfo;
    static void derefObject(void*);
    static void visitDOMWrapper(void*, const v8::Persistent<v8::Object>&, v8::Isolate*);
    static EventTarget* toEventTarget(v8::Handle<v8::Object>);
    static void constructorCallback(const v8::FunctionCallbackInfo<v8::Value>&);
    static const int eventListenerCacheIndex = v8DefaultWrapperInternalFieldCount + 0;
#if ENABLE(OILPAN)
    static const int persistentHandleIndex = v8DefaultWrapperInternalFieldCount + 1;
    static const int internalFieldCount = v8DefaultWrapperInternalFieldCount + 1 + 1;
#else
    static const int internalFieldCount = v8DefaultWrapperInternalFieldCount + 1;
#endif
    static inline void* toInternalPointer(VTTCue* impl)
    {
        return V8TextTrackCue::toInternalPointer(impl);
    }

    static inline VTTCue* fromInternalPointer(void* object)
    {
        return static_cast<VTTCue*>(V8TextTrackCue::fromInternalPointer(object));
    }
    static void installPerContextEnabledProperties(v8::Handle<v8::Object>, VTTCue*, v8::Isolate*) { }
    static void installPerContextEnabledMethods(v8::Handle<v8::Object>, v8::Isolate*) { }

private:
    friend v8::Handle<v8::Object> wrap(VTTCue*, v8::Handle<v8::Object> creationContext, v8::Isolate*);
    static v8::Handle<v8::Object> createWrapper(PassRefPtrWillBeRawPtr<VTTCue>, v8::Handle<v8::Object> creationContext, v8::Isolate*);
};

v8::Handle<v8::Object> wrap(VTTCue* impl, v8::Handle<v8::Object> creationContext, v8::Isolate*);

inline v8::Handle<v8::Value> toV8(VTTCue* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    if (UNLIKELY(!impl))
        return v8::Null(isolate);
    v8::Handle<v8::Value> wrapper = DOMDataStore::getWrapper<V8VTTCue>(impl, isolate);
    if (!wrapper.IsEmpty())
        return wrapper;
    return wrap(impl, creationContext, isolate);
}

template<typename CallbackInfo>
inline void v8SetReturnValue(const CallbackInfo& callbackInfo, VTTCue* impl)
{
    if (UNLIKELY(!impl)) {
        v8SetReturnValueNull(callbackInfo);
        return;
    }
    if (DOMDataStore::setReturnValueFromWrapper<V8VTTCue>(callbackInfo.GetReturnValue(), impl))
        return;
    v8::Handle<v8::Object> wrapper = wrap(impl, callbackInfo.Holder(), callbackInfo.GetIsolate());
    v8SetReturnValue(callbackInfo, wrapper);
}

template<typename CallbackInfo>
inline void v8SetReturnValueForMainWorld(const CallbackInfo& callbackInfo, VTTCue* impl)
{
    ASSERT(DOMWrapperWorld::current(callbackInfo.GetIsolate()).isMainWorld());
    if (UNLIKELY(!impl)) {
        v8SetReturnValueNull(callbackInfo);
        return;
    }
    if (DOMDataStore::setReturnValueFromWrapperForMainWorld<V8VTTCue>(callbackInfo.GetReturnValue(), impl))
        return;
    v8::Handle<v8::Value> wrapper = wrap(impl, callbackInfo.Holder(), callbackInfo.GetIsolate());
    v8SetReturnValue(callbackInfo, wrapper);
}

template<class CallbackInfo, class Wrappable>
inline void v8SetReturnValueFast(const CallbackInfo& callbackInfo, VTTCue* impl, Wrappable* wrappable)
{
    if (UNLIKELY(!impl)) {
        v8SetReturnValueNull(callbackInfo);
        return;
    }
    if (DOMDataStore::setReturnValueFromWrapperFast<V8VTTCue>(callbackInfo.GetReturnValue(), impl, callbackInfo.Holder(), wrappable))
        return;
    v8::Handle<v8::Object> wrapper = wrap(impl, callbackInfo.Holder(), callbackInfo.GetIsolate());
    v8SetReturnValue(callbackInfo, wrapper);
}

inline v8::Handle<v8::Value> toV8(PassRefPtrWillBeRawPtr<VTTCue> impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    return toV8(impl.get(), creationContext, isolate);
}

template<class CallbackInfo>
inline void v8SetReturnValue(const CallbackInfo& callbackInfo, PassRefPtrWillBeRawPtr<VTTCue> impl)
{
    v8SetReturnValue(callbackInfo, impl.get());
}

template<class CallbackInfo>
inline void v8SetReturnValueForMainWorld(const CallbackInfo& callbackInfo, PassRefPtrWillBeRawPtr<VTTCue> impl)
{
    v8SetReturnValueForMainWorld(callbackInfo, impl.get());
}

template<class CallbackInfo, class Wrappable>
inline void v8SetReturnValueFast(const CallbackInfo& callbackInfo, PassRefPtrWillBeRawPtr<VTTCue> impl, Wrappable* wrappable)
{
    v8SetReturnValueFast(callbackInfo, impl.get(), wrappable);
}

}
#endif // V8VTTCue_h
