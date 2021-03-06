// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by code_generator_v8.py. DO NOT MODIFY!

#include "config.h"
#include "V8ANGLEInstancedArrays.h"

#include "bindings/v8/ExceptionState.h"
#include "bindings/v8/V8DOMConfiguration.h"
#include "bindings/v8/V8HiddenValue.h"
#include "bindings/v8/V8ObjectConstructor.h"
#include "core/dom/ContextFeatures.h"
#include "core/dom/Document.h"
#include "platform/RuntimeEnabledFeatures.h"
#include "platform/TraceEvent.h"
#include "wtf/GetPtr.h"
#include "wtf/RefPtr.h"

namespace WebCore {

static void initializeScriptWrappableForInterface(ANGLEInstancedArrays* object)
{
    if (ScriptWrappable::wrapperCanBeStoredInObject(object))
        ScriptWrappable::fromObject(object)->setTypeInfo(&V8ANGLEInstancedArrays::wrapperTypeInfo);
    else
        ASSERT_NOT_REACHED();
}

} // namespace WebCore

void webCoreInitializeScriptWrappableForInterface(WebCore::ANGLEInstancedArrays* object)
{
    WebCore::initializeScriptWrappableForInterface(object);
}

namespace WebCore {
const WrapperTypeInfo V8ANGLEInstancedArrays::wrapperTypeInfo = { gin::kEmbedderBlink, V8ANGLEInstancedArrays::domTemplate, V8ANGLEInstancedArrays::derefObject, 0, 0, 0, V8ANGLEInstancedArrays::installPerContextEnabledMethods, 0, WrapperTypeObjectPrototype, RefCountedObject };

namespace ANGLEInstancedArraysV8Internal {

template <typename T> void V8_USE(T) { }

static void drawArraysInstancedANGLEMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "drawArraysInstancedANGLE", "ANGLEInstancedArrays", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 4)) {
        throwMinimumArityTypeError(exceptionState, 4, info.Length());
        return;
    }
    ANGLEInstancedArrays* impl = V8ANGLEInstancedArrays::toNative(info.Holder());
    unsigned mode;
    int first;
    int count;
    int primcount;
    {
        v8::TryCatch block;
        V8RethrowTryCatchScope rethrow(block);
        TONATIVE_VOID_EXCEPTIONSTATE_INTERNAL(mode, toUInt32(info[0], exceptionState), exceptionState);
        TONATIVE_VOID_EXCEPTIONSTATE_INTERNAL(first, toInt32(info[1], exceptionState), exceptionState);
        TONATIVE_VOID_EXCEPTIONSTATE_INTERNAL(count, toInt32(info[2], exceptionState), exceptionState);
        TONATIVE_VOID_EXCEPTIONSTATE_INTERNAL(primcount, toInt32(info[3], exceptionState), exceptionState);
    }
    impl->drawArraysInstancedANGLE(mode, first, count, primcount);
}

static void drawArraysInstancedANGLEMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("Blink", "DOMMethod");
    ANGLEInstancedArraysV8Internal::drawArraysInstancedANGLEMethod(info);
    TRACE_EVENT_SET_SAMPLING_STATE("V8", "V8Execution");
}

static void drawElementsInstancedANGLEMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "drawElementsInstancedANGLE", "ANGLEInstancedArrays", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 5)) {
        throwMinimumArityTypeError(exceptionState, 5, info.Length());
        return;
    }
    ANGLEInstancedArrays* impl = V8ANGLEInstancedArrays::toNative(info.Holder());
    unsigned mode;
    int count;
    unsigned type;
    long long offset;
    int primcount;
    {
        v8::TryCatch block;
        V8RethrowTryCatchScope rethrow(block);
        TONATIVE_VOID_EXCEPTIONSTATE_INTERNAL(mode, toUInt32(info[0], exceptionState), exceptionState);
        TONATIVE_VOID_EXCEPTIONSTATE_INTERNAL(count, toInt32(info[1], exceptionState), exceptionState);
        TONATIVE_VOID_EXCEPTIONSTATE_INTERNAL(type, toUInt32(info[2], exceptionState), exceptionState);
        TONATIVE_VOID_EXCEPTIONSTATE_INTERNAL(offset, toInt64(info[3], exceptionState), exceptionState);
        TONATIVE_VOID_EXCEPTIONSTATE_INTERNAL(primcount, toInt32(info[4], exceptionState), exceptionState);
    }
    impl->drawElementsInstancedANGLE(mode, count, type, offset, primcount);
}

static void drawElementsInstancedANGLEMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("Blink", "DOMMethod");
    ANGLEInstancedArraysV8Internal::drawElementsInstancedANGLEMethod(info);
    TRACE_EVENT_SET_SAMPLING_STATE("V8", "V8Execution");
}

static void vertexAttribDivisorANGLEMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "vertexAttribDivisorANGLE", "ANGLEInstancedArrays", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 2)) {
        throwMinimumArityTypeError(exceptionState, 2, info.Length());
        return;
    }
    ANGLEInstancedArrays* impl = V8ANGLEInstancedArrays::toNative(info.Holder());
    unsigned index;
    int divisor;
    {
        v8::TryCatch block;
        V8RethrowTryCatchScope rethrow(block);
        TONATIVE_VOID_EXCEPTIONSTATE_INTERNAL(index, toUInt32(info[0], exceptionState), exceptionState);
        TONATIVE_VOID_EXCEPTIONSTATE_INTERNAL(divisor, toInt32(info[1], exceptionState), exceptionState);
    }
    impl->vertexAttribDivisorANGLE(index, divisor);
}

static void vertexAttribDivisorANGLEMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("Blink", "DOMMethod");
    ANGLEInstancedArraysV8Internal::vertexAttribDivisorANGLEMethod(info);
    TRACE_EVENT_SET_SAMPLING_STATE("V8", "V8Execution");
}

} // namespace ANGLEInstancedArraysV8Internal

static const V8DOMConfiguration::MethodConfiguration V8ANGLEInstancedArraysMethods[] = {
    {"drawArraysInstancedANGLE", ANGLEInstancedArraysV8Internal::drawArraysInstancedANGLEMethodCallback, 0, 4},
    {"drawElementsInstancedANGLE", ANGLEInstancedArraysV8Internal::drawElementsInstancedANGLEMethodCallback, 0, 5},
    {"vertexAttribDivisorANGLE", ANGLEInstancedArraysV8Internal::vertexAttribDivisorANGLEMethodCallback, 0, 2},
};

static void configureV8ANGLEInstancedArraysTemplate(v8::Handle<v8::FunctionTemplate> functionTemplate, v8::Isolate* isolate)
{
    functionTemplate->ReadOnlyPrototype();

    v8::Local<v8::Signature> defaultSignature;
    defaultSignature = V8DOMConfiguration::installDOMClassTemplate(functionTemplate, "ANGLEInstancedArrays", v8::Local<v8::FunctionTemplate>(), V8ANGLEInstancedArrays::internalFieldCount,
        0, 0,
        0, 0,
        V8ANGLEInstancedArraysMethods, WTF_ARRAY_LENGTH(V8ANGLEInstancedArraysMethods),
        isolate);
    v8::Local<v8::ObjectTemplate> instanceTemplate ALLOW_UNUSED = functionTemplate->InstanceTemplate();
    v8::Local<v8::ObjectTemplate> prototypeTemplate ALLOW_UNUSED = functionTemplate->PrototypeTemplate();
    static const V8DOMConfiguration::ConstantConfiguration V8ANGLEInstancedArraysConstants[] = {
        {"VERTEX_ATTRIB_ARRAY_DIVISOR_ANGLE", 0x88FE},
    };
    V8DOMConfiguration::installConstants(functionTemplate, prototypeTemplate, V8ANGLEInstancedArraysConstants, WTF_ARRAY_LENGTH(V8ANGLEInstancedArraysConstants), isolate);

    // Custom toString template
    functionTemplate->Set(v8AtomicString(isolate, "toString"), V8PerIsolateData::from(isolate)->toStringTemplate());
}

v8::Handle<v8::FunctionTemplate> V8ANGLEInstancedArrays::domTemplate(v8::Isolate* isolate)
{
    return V8DOMConfiguration::domClassTemplate(isolate, const_cast<WrapperTypeInfo*>(&wrapperTypeInfo), configureV8ANGLEInstancedArraysTemplate);
}

bool V8ANGLEInstancedArrays::hasInstance(v8::Handle<v8::Value> v8Value, v8::Isolate* isolate)
{
    return V8PerIsolateData::from(isolate)->hasInstance(&wrapperTypeInfo, v8Value);
}

v8::Handle<v8::Object> V8ANGLEInstancedArrays::findInstanceInPrototypeChain(v8::Handle<v8::Value> v8Value, v8::Isolate* isolate)
{
    return V8PerIsolateData::from(isolate)->findInstanceInPrototypeChain(&wrapperTypeInfo, v8Value);
}

ANGLEInstancedArrays* V8ANGLEInstancedArrays::toNativeWithTypeCheck(v8::Isolate* isolate, v8::Handle<v8::Value> value)
{
    return hasInstance(value, isolate) ? fromInternalPointer(v8::Handle<v8::Object>::Cast(value)->GetAlignedPointerFromInternalField(v8DOMWrapperObjectIndex)) : 0;
}

v8::Handle<v8::Object> wrap(ANGLEInstancedArrays* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    ASSERT(impl);
    ASSERT(!DOMDataStore::containsWrapper<V8ANGLEInstancedArrays>(impl, isolate));
    return V8ANGLEInstancedArrays::createWrapper(impl, creationContext, isolate);
}

v8::Handle<v8::Object> V8ANGLEInstancedArrays::createWrapper(PassRefPtr<ANGLEInstancedArrays> impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    ASSERT(impl);
    ASSERT(!DOMDataStore::containsWrapper<V8ANGLEInstancedArrays>(impl.get(), isolate));
    if (ScriptWrappable::wrapperCanBeStoredInObject(impl.get())) {
        const WrapperTypeInfo* actualInfo = ScriptWrappable::fromObject(impl.get())->typeInfo();
        // Might be a XXXConstructor::wrapperTypeInfo instead of an XXX::wrapperTypeInfo. These will both have
        // the same object de-ref functions, though, so use that as the basis of the check.
        RELEASE_ASSERT_WITH_SECURITY_IMPLICATION(actualInfo->derefObjectFunction == wrapperTypeInfo.derefObjectFunction);
    }

    v8::Handle<v8::Object> wrapper = V8DOMWrapper::createWrapper(creationContext, &wrapperTypeInfo, toInternalPointer(impl.get()), isolate);
    if (UNLIKELY(wrapper.IsEmpty()))
        return wrapper;

    installPerContextEnabledProperties(wrapper, impl.get(), isolate);
    V8DOMWrapper::associateObjectWithWrapper<V8ANGLEInstancedArrays>(impl, &wrapperTypeInfo, wrapper, isolate, WrapperConfiguration::Independent);
    return wrapper;
}

void V8ANGLEInstancedArrays::derefObject(void* object)
{
    fromInternalPointer(object)->deref();
}

template<>
v8::Handle<v8::Value> toV8NoInline(ANGLEInstancedArrays* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    return toV8(impl, creationContext, isolate);
}

} // namespace WebCore
