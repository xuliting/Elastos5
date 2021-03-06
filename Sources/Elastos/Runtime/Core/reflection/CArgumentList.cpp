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

#include "CArgumentList.h"
#include "CMethodInfo.h"
#include "CConstructorInfo.h"

CArgumentList::CArgumentList()
    : mParamBuf(NULL)
    , mParamElem(NULL)
    , mParamCount(0)
    , mParamBufSize(0)
    , mIsMethodInfo(FALSE)
{}

CArgumentList::~CArgumentList()
{
    // TODO
    if (!mInterfaceParams.IsEmpty() && mParamCount > 0) {
        for (UInt32 i = 0; i < mParamCount; ++i) {
            if (mParamElem[i].mType == CarDataType_Interface) {
                IInterface** prev = mInterfaceParams.Get(&i);
                if (prev && *prev) {
                    (*prev)->Release();
                }
            }
        }
    }

    if (mParamBuf) free(mParamBuf);
}

UInt32 CArgumentList::AddRef()
{
    return ElLightRefBase::AddRef();
}

UInt32 CArgumentList::Release()
{
    return ElLightRefBase::Release();
}

PInterface CArgumentList::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IArgumentList) {
        return (IArgumentList *)this;
    }
    else {
        return NULL;
    }
}

ECode CArgumentList::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    return E_NOT_IMPLEMENTED;
}

ECode CArgumentList::Init(
    /* [in] */ IFunctionInfo* functionInfo,
    /* [in] */ ParmElement* paramElem,
    /* [in] */ UInt32 paramCount,
    /* [in] */ UInt32 paramBufSize,
    /* [in] */ Boolean isMethodInfo)
{
    mParamElem = paramElem;
    mParamCount = paramCount;

    mParamBuf = (PByte)malloc(paramBufSize);
    if (mParamBuf == NULL) {
        return E_OUT_OF_MEMORY;
    }

    mParamBufSize = paramBufSize;
    memset(mParamBuf, 0, paramBufSize);
    mFunctionInfo = functionInfo;

    mIsMethodInfo = isMethodInfo;

    return NOERROR;
}

ECode CArgumentList::GetFunctionInfo(
    /* [out] */ IFunctionInfo** functionInfo)
{
    if (!functionInfo) {
        return E_INVALID_ARGUMENT;
    }
    *functionInfo = mFunctionInfo;
    (*functionInfo)->AddRef();
    return NOERROR;
}

ECode CArgumentList::SetParamValue(
    /* [in] */ Int32 index,
    /* [in] */ PVoid param,
    /* [in] */ CarDataType type,
    /* [in] */ ParamIOAttribute attrib,
    /* [in] */ Int32 pointer)
{
    if (type == CarDataType_CarArray
        && mParamElem[index].mType == CarDataType_ArrayOf) {
        type = mParamElem[index].mType;
    }

    if (index < 0 || index >= (Int32)mParamCount
        || (mParamElem[index].mAttrib != attrib)
        || (type != mParamElem[index].mType)
        || (type != CarDataType_LocalPtr
        && (type != CarDataType_ArrayOf || mParamElem[index].mPointer > 1)
        && mParamElem[index].mPointer != pointer)) {
        return E_INVALID_ARGUMENT;
    }

    if (!mParamBuf || mParamElem[index].mPos
        + mParamElem[index].mSize > mParamBufSize) {
        return E_INVALID_OPERATION;
    }

    if (mParamElem[index].mSize == 1) {
        *(Byte *)(mParamBuf + mParamElem[index].mPos) = *(Byte *)param;
    }
    else if (mParamElem[index].mSize == 2) {
        *(UInt16 *)(mParamBuf + mParamElem[index].mPos) = *(UInt16 *)param;
    }
    else if (mParamElem[index].mSize == 4) {
        if (type == CarDataType_String) {
            *(String **)(mParamBuf + mParamElem[index].mPos) = (String *)param;
        }
        else {
            *(UInt32 *)(mParamBuf + mParamElem[index].mPos) = *(UInt32 *)param;
        }
    }
    else if (mParamElem[index].mSize == 8) {
        *(UInt64 *)(mParamBuf + mParamElem[index].mPos) = *(UInt64 *)param;
    }
    else {
        return E_INVALID_OPERATION;
    }

    return NOERROR;
}

ECode CArgumentList::GetParamValue(
    /* [in] */ Int32 index,
    /* [in] */ CarDataType type,
    /* [in] */ ParamIOAttribute attrib,
    /* [in] */ Int32 pointer,
    /* [in, out] */ PVoid param)
{
    if (type == CarDataType_CarArray
        && mParamElem[index].mType == CarDataType_ArrayOf) {
        type = mParamElem[index].mType;
    }

     if (index < 0 || index >= (Int32)mParamCount
        || (mParamElem[index].mAttrib != attrib)
        || (type != mParamElem[index].mType)
        || (type != CarDataType_LocalPtr
        && (type != CarDataType_ArrayOf || mParamElem[index].mPointer > 1)
        && mParamElem[index].mPointer != pointer)) {
        assert(0);
        return E_INVALID_ARGUMENT;
    }

    if (!mParamBuf || mParamElem[index].mPos
        + mParamElem[index].mSize > mParamBufSize) {
        return E_INVALID_OPERATION;
    }

    if (mParamElem[index].mSize == 1) {
        *(Byte*)param = *(Byte*)(mParamBuf + mParamElem[index].mPos);
    }
    else if (mParamElem[index].mSize == 2) {
        *(UInt16*)param = *(UInt16*)(mParamBuf + mParamElem[index].mPos);
    }
    else if (mParamElem[index].mSize == 4) {
        if (type == CarDataType_String) {
            if (pointer == 0) {
                *(String*)param = **(String **)(mParamBuf + mParamElem[index].mPos);
            }
            else if (pointer == 1) {
                *(String**)param = *(String **)(mParamBuf + mParamElem[index].mPos);
            }
            else {
                return E_INVALID_OPERATION;
            }
        }
        else {
            *(UInt32*)param = *(UInt32*)(mParamBuf + mParamElem[index].mPos);
        }

        if (param != 0 && pointer == 1) {
            if (type == CarDataType_Interface) {
                if (*(IInterface**)param) {
                    (*(IInterface**)param)->AddRef();
                }
            }
            else if (type == CarDataType_ArrayOf) {
                _CarQuintet_AddRef(*(PCarQuintet*)param);
            }
        }
    }
    else if (mParamElem[index].mSize == 8) {
        *(Int64*)param = *(Int64*)(mParamBuf + mParamElem[index].mPos);
    }
    else {
        assert(0);
        return E_INVALID_OPERATION;
    }

    return NOERROR;
}

ECode CArgumentList::AssignOutputParamValue(
    /* [in] */ Int32 index,
    /* [in] */ PVoid param,
    /* [in] */ CarDataType type,
    /* [in] */ ParamIOAttribute attrib,
    /* [in] */ Int32 pointer)
{
    if (type == CarDataType_CarArray
        && mParamElem[index].mType == CarDataType_ArrayOf) {
        type = mParamElem[index].mType;
    }

    if (index < 0 || index >= (Int32)mParamCount
        || (mParamElem[index].mAttrib != attrib)
        || (type != mParamElem[index].mType)
        || (type != CarDataType_LocalPtr
        && mParamElem[index].mPointer != pointer)) {
        return E_INVALID_ARGUMENT;
    }

    if (!mParamBuf || mParamElem[index].mPos
        + mParamElem[index].mSize > mParamBufSize) {
        return E_INVALID_OPERATION;
    }

    if (mParamElem[index].mSize == 4) {
        switch (type) {
            case CarDataType_Int16:
                **(Int16**)(mParamBuf + mParamElem[index].mPos) = *(Int16*)param;
                break;
            case CarDataType_Int32:
                **(Int32**)(mParamBuf + mParamElem[index].mPos) = *(Int32*)param;
                break;
            case CarDataType_Int64:
                **(Int64**)(mParamBuf + mParamElem[index].mPos) = *(Int64*)param;
                break;
            case CarDataType_Byte:
                **(Byte**)(mParamBuf + mParamElem[index].mPos) = *(Byte*)param;
                break;
            case CarDataType_Float:
                **(Float**)(mParamBuf + mParamElem[index].mPos) = *(Float*)param;
                break;
            case CarDataType_Double:
                **(Double**)(mParamBuf + mParamElem[index].mPos) = *(Double*)param;
                break;
            case CarDataType_Char32:
                **(Char32**)(mParamBuf + mParamElem[index].mPos) = *(Char32*)param;
                break;
            case CarDataType_String:
                **(String**)(mParamBuf + mParamElem[index].mPos) = *(String*)param;
                break;
            case CarDataType_Boolean:
                **(Boolean**)(mParamBuf + mParamElem[index].mPos) = *(Boolean*)param;
                break;
            case CarDataType_Enum:
                **(Int32**)(mParamBuf + mParamElem[index].mPos) = *(Int32*)param;
                break;
            case CarDataType_Interface: {
                PInterface ptr = *(PInterface*)param;
                if (ptr != NULL)
                    ptr->AddRef();
                **(PInterface**)(mParamBuf + mParamElem[index].mPos) = ptr;
                break;
            }
            case CarDataType_EMuid:
                **(EMuid**)(mParamBuf + mParamElem[index].mPos) = *(EMuid*)param;
                break;
            case CarDataType_EGuid:
                **(EGuid**)(mParamBuf + mParamElem[index].mPos) = *(EGuid*)param;
                break;
            case CarDataType_ECode:
                **(ECode**)(mParamBuf + mParamElem[index].mPos) = *(ECode*)param;
                break;
            case CarDataType_ArrayOf: {
                PCarQuintet ptr = *(PCarQuintet*)param;
                if (pointer == 2) {
                    _CarQuintet_AddRef(ptr);
                    **(PCarQuintet**)(mParamBuf + mParamElem[index].mPos) = ptr;
                }
                else {
                    assert(0 && "TODO");
                }
                break;
            }
            default:
                assert(0 && "TODO");
                break;
        }
    }
    else {
        return E_INVALID_OPERATION;
    }

    return NOERROR;
}

ECode CArgumentList::GetInputArgumentOfInt16(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    return GetParamValue(index, CarDataType_Int16, ParamIOAttribute_In, 0, value);
}

ECode CArgumentList::SetInputArgumentOfInt16(
    /* [in] */ Int32 index,
    /* [in] */ Int16 value)
{
    UInt32 nParam = value;
    return SetParamValue(index, &nParam, CarDataType_Int16, ParamIOAttribute_In);
}

ECode CArgumentList::GetInputArgumentOfInt32(
    /* [in] */ Int32 index,
    /* [in] */ Int32* value)
{
    return GetParamValue(index, CarDataType_Int32, ParamIOAttribute_In, 0, value);
}

ECode CArgumentList::SetInputArgumentOfInt32(
    /* [in] */ Int32 index,
    /* [in] */ Int32 value)
{
    return SetParamValue(index, &value, CarDataType_Int32, ParamIOAttribute_In);
}

ECode CArgumentList::GetInputArgumentOfInt64(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    return GetParamValue(index, CarDataType_Int64, ParamIOAttribute_In, 0, value);
}

ECode CArgumentList::SetInputArgumentOfInt64(
    /* [in] */ Int32 index,
    /* [in] */ Int64 value)
{
    return SetParamValue(index, &value, CarDataType_Int64, ParamIOAttribute_In);
}

ECode CArgumentList::GetInputArgumentOfByte(
    /* [in] */ Int32 index,
    /* [out] */ Byte* value)
{
    return GetParamValue(index, CarDataType_Byte, ParamIOAttribute_In, 0, value);
}

ECode CArgumentList::SetInputArgumentOfByte(
    /* [in] */ Int32 index,
    /* [in] */ Byte value)
{
    UInt32 nParam = value;
    return SetParamValue(index, &nParam, CarDataType_Byte, ParamIOAttribute_In);
}

ECode CArgumentList::GetInputArgumentOfFloat(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    return GetParamValue(index, CarDataType_Float, ParamIOAttribute_In, 0, value);
}

ECode CArgumentList::SetInputArgumentOfFloat(
    /* [in] */ Int32 index,
    /* [in] */ Float value)
{
    return SetParamValue(index, &value, CarDataType_Float, ParamIOAttribute_In);
}

ECode CArgumentList::GetInputArgumentOfDouble(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    return GetParamValue(index, CarDataType_Double, ParamIOAttribute_In, 0, value);
}

ECode CArgumentList::SetInputArgumentOfDouble(
    /* [in] */ Int32 index,
    /* [in] */ Double value)
{
    return SetParamValue(index, &value, CarDataType_Double, ParamIOAttribute_In);
}

ECode CArgumentList::GetInputArgumentOfChar(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    return GetParamValue(index, CarDataType_Char32, ParamIOAttribute_In, 0, value);
}

ECode CArgumentList::SetInputArgumentOfChar(
    /* [in] */ Int32 index,
    /* [in] */ Char32 value)
{
    UInt32 nParam = value;
    return SetParamValue(index, &nParam, CarDataType_Char32, ParamIOAttribute_In);
}

ECode CArgumentList::GetInputArgumentOfString(
    /* [in] */ Int32 index,
    /* [out] */ String* value)
{
    return GetParamValue(index, CarDataType_String, ParamIOAttribute_In, 0, value);
}

ECode CArgumentList::SetInputArgumentOfString(
    /* [in] */ Int32 index,
    /* [in] */ const String& value)
{
    return SetParamValue(index, const_cast<String*>(&value),
        CarDataType_String, ParamIOAttribute_In);
}

ECode CArgumentList::GetInputArgumentOfBoolean(
    /* [in] */ Int32 index,
    /* [out] */ Boolean* value)
{
    return GetParamValue(index, CarDataType_Boolean, ParamIOAttribute_In, 0, value);
}

ECode CArgumentList::SetInputArgumentOfBoolean(
    /* [in] */ Int32 index,
    /* [in] */ Boolean value)
{
    UInt32 nParam = value;
    return SetParamValue(index, &nParam, CarDataType_Boolean, ParamIOAttribute_In);
}

ECode CArgumentList::GetInputArgumentOfEMuid(
    /* [in] */ Int32 index,
    /* [out] */ EMuid* value)
{
    return GetParamValue(index, CarDataType_EMuid, ParamIOAttribute_In, 0, value);
}

ECode CArgumentList::SetInputArgumentOfEMuid(
    /* [in] */ Int32 index,
    /* [in] */ EMuid* value)
{
    return SetParamValue(index, &value, CarDataType_EMuid, ParamIOAttribute_In);
}

ECode CArgumentList::GetInputArgumentOfEGuid(
    /* [in] */ Int32 index,
    /* [out] */ EGuid* value)
{
    return GetParamValue(index, CarDataType_EGuid, ParamIOAttribute_In, 0, value);
}

ECode CArgumentList::SetInputArgumentOfEGuid(
    /* [in] */ Int32 index,
    /* [in] */ EGuid* value)
{
    return SetParamValue(index, &value, CarDataType_EGuid, ParamIOAttribute_In);
}

ECode CArgumentList::GetInputArgumentOfECode(
    /* [in] */ Int32 index,
    /* [out] */ ECode* value)
{
    return GetParamValue(index, CarDataType_ECode, ParamIOAttribute_In, 0, value);
}

ECode CArgumentList::SetInputArgumentOfECode(
    /* [in] */ Int32 index,
    /* [in] */ ECode value)
{
    return SetParamValue(index, &value, CarDataType_ECode, ParamIOAttribute_In);
}

ECode CArgumentList::SetInputArgumentOfLocalPtr(
    /* [in] */ Int32 index,
    /* [in] */ LocalPtr value)
{
    return SetParamValue(index, &value, CarDataType_LocalPtr,
            ParamIOAttribute_In);
}

ECode CArgumentList::SetInputArgumentOfLocalType(
    /* [in] */ Int32 index,
    /* [in] */ PVoid value)
{
    return SetParamValue(index, value, CarDataType_LocalType,
            ParamIOAttribute_In);
}

ECode CArgumentList::GetInputArgumentOfEnum(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    return GetParamValue(index, CarDataType_Enum, ParamIOAttribute_In, 0, value);
}

ECode CArgumentList::SetInputArgumentOfEnum(
    /* [in] */ Int32 index,
    /* [in] */ Int32 value)
{
    return SetParamValue(index, &value, CarDataType_Enum, ParamIOAttribute_In);
}

ECode CArgumentList::GetInputArgumentOfCarArray(
    /* [in] */ Int32 index,
    /* [out] */ PCarQuintet* value)
{
    return GetParamValue(index, CarDataType_CarArray, ParamIOAttribute_In, 1, value);
}

ECode CArgumentList::SetInputArgumentOfCarArray(
    /* [in] */ Int32 index,
    /* [in] */ PCarQuintet value)
{
    return SetParamValue(index, &value, CarDataType_CarArray,
            ParamIOAttribute_In);
}

ECode CArgumentList::SetInputArgumentOfStructPtr(
    /* [in] */ Int32 index,
    /* [in] */ PVoid value)
{
    return SetParamValue(index, &value, CarDataType_Struct, ParamIOAttribute_In);
}

ECode CArgumentList::GetInputArgumentOfObjectPtr(
    /* [in] */ Int32 index,
    /* [out] */ PInterface* value)
{
    return GetParamValue(index, CarDataType_Interface, ParamIOAttribute_In, 1, value);
}

ECode CArgumentList::SetInputArgumentOfObjectPtr(
    /* [in] */ Int32 index,
    /* [in] */ PInterface value)
{
    if (CarDataType_Interface != mParamElem[index].mType) {
        return E_INVALID_ARGUMENT;
    }

    if (value) {
        CMethodInfo* methodInfo = NULL;
        ECode ec = NOERROR;
        if (mIsMethodInfo) {
            methodInfo = (CMethodInfo *)mFunctionInfo.Get();
        }
        else {
            methodInfo = ((CConstructorInfo *)mFunctionInfo.Get())->mMethodInfo;
        }

        Int32 base = methodInfo->mClsModule->mBase;
        TypeDescriptor* typeDesc = &(getParamDescAddr(base,
                methodInfo->mMethodDescriptor->mParams, index)->mType);
        if (typeDesc->mType == Type_alias) {
            ec = methodInfo->mClsModule->AliasToOriginal(typeDesc, &typeDesc);
            if (FAILED(ec)) return ec;
        }

        InterfaceDirEntry* ifDir = getInterfaceDirAddr(base,
                methodInfo->mClsMod->mInterfaceDirs, typeDesc->mIndex);
        EIID iid = adjustInterfaceDescAddr(base, ifDir->mDesc)->mIID;
        value = value->Probe(iid);
        if (!value) return E_NO_INTERFACE;

        value->AddRef();
        IInterface** prev = mInterfaceParams.Get(&index);
        if (prev && *prev) {
            (*prev)->Release();
        }
        mInterfaceParams.Put(&index, (IInterface**)&value);
    }

    return SetParamValue(index, &value, CarDataType_Interface,
            ParamIOAttribute_In, 1);
}

ECode CArgumentList::IsOutputArgumentNullPtr(
    /* [in] */ Int32 index,
    /* [out] */ Boolean* isNull)
{
    if (isNull == NULL || index < 0 || index >= (Int32)mParamCount
        || (mParamElem[index].mAttrib == ParamIOAttribute_In)) {
        return E_INVALID_ARGUMENT;
    }

    if (mParamElem[index].mSize == 4) {
        if (mParamElem[index].mType == CarDataType_String) {
            *isNull = *(String **)(mParamBuf + mParamElem[index].mPos) == NULL;
        }
        else {
            *isNull = *(UInt32 *)(mParamBuf + mParamElem[index].mPos) == 0;
        }
    }
    else {
        return E_INVALID_OPERATION;
    }

    return NOERROR;
}

ECode CArgumentList::AssignOutputArgumentOfInt16Ptr(
    /* [in] */ Int32 index,
    /* [in] */ Int16 value)
{
    return AssignOutputParamValue(index, &value, CarDataType_Int16,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::SetOutputArgumentOfInt16Ptr(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    return SetParamValue(index, &value, CarDataType_Int16,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::AssignOutputArgumentOfInt32Ptr(
    /* [in] */ Int32 index,
    /* [in] */ Int32 value)
{
    return AssignOutputParamValue(index, &value, CarDataType_Int32,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::SetOutputArgumentOfInt32Ptr(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    return SetParamValue(index, &value, CarDataType_Int32,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::AssignOutputArgumentOfInt64Ptr(
    /* [in] */ Int32 index,
    /* [out] */ Int64 value)
{
    return AssignOutputParamValue(index, &value, CarDataType_Int64,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::SetOutputArgumentOfInt64Ptr(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    return SetParamValue(index, &value, CarDataType_Int64,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::AssignOutputArgumentOfBytePtr(
    /* [in] */ Int32 index,
    /* [out] */ Byte value)
{
    return AssignOutputParamValue(index, &value, CarDataType_Byte,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::SetOutputArgumentOfBytePtr(
    /* [in] */ Int32 index,
    /* [out] */ Byte* value)
{
    return SetParamValue(index, &value, CarDataType_Byte,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::AssignOutputArgumentOfFloatPtr(
    /* [in] */ Int32 index,
    /* [in] */ Float value)
{
    return AssignOutputParamValue(index, &value, CarDataType_Float,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::SetOutputArgumentOfFloatPtr(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    return SetParamValue(index, &value, CarDataType_Float,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::AssignOutputArgumentOfDoublePtr(
    /* [in] */ Int32 index,
    /* [in] */ Double value)
{
    return AssignOutputParamValue(index, &value, CarDataType_Double,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::SetOutputArgumentOfDoublePtr(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    return SetParamValue(index, &value, CarDataType_Double,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::AssignOutputArgumentOfCharPtr(
    /* [in] */ Int32 index,
    /* [in] */ Char32 value)
{
    return AssignOutputParamValue(index, &value, CarDataType_Char32,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::SetOutputArgumentOfCharPtr(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    return SetParamValue(index, &value, CarDataType_Char32,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::AssignOutputArgumentOfStringPtr(
    /* [in] */ Int32 index,
    /* [in] */ const String& value)
{
    return AssignOutputParamValue(index, const_cast<String*>(&value), CarDataType_String,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::SetOutputArgumentOfStringPtr(
    /* [in] */ Int32 index,
    /* [out] */ String* value)
{
    return SetParamValue(index, value, CarDataType_String,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::AssignOutputArgumentOfBooleanPtr(
    /* [in] */ Int32 index,
    /* [in] */ Boolean value)
{
    return AssignOutputParamValue(index, &value, CarDataType_Boolean,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::SetOutputArgumentOfBooleanPtr(
    /* [in] */ Int32 index,
    /* [out] */ Boolean* value)
{
    return SetParamValue(index, &value, CarDataType_Boolean,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::AssignOutputArgumentOfEMuidPtr(
    /* [in] */ Int32 index,
    /* [in] */ EMuid* value)
{
    return AssignOutputParamValue(index, value, CarDataType_EMuid,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::SetOutputArgumentOfEMuidPtr(
    /* [in] */ Int32 index,
    /* [out] */ EMuid* value)
{
    return SetParamValue(index, &value, CarDataType_EMuid,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::AssignOutputArgumentOfEGuidPtr(
    /* [in] */ Int32 index,
    /* [in] */ EGuid* value)
{
    return AssignOutputParamValue(index, value, CarDataType_EGuid,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::SetOutputArgumentOfEGuidPtr(
    /* [in] */ Int32 index,
    /* [out] */ EGuid* value)
{
    return SetParamValue(index, &value, CarDataType_EGuid,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::AssignOutputArgumentOfECodePtr(
    /* [in] */ Int32 index,
    /* [in] */ ECode value)
{
    return AssignOutputParamValue(index, &value, CarDataType_ECode,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::SetOutputArgumentOfECodePtr(
    /* [in] */ Int32 index,
    /* [out] */ ECode* value)
{
    return SetParamValue(index, &value, CarDataType_ECode,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::SetOutputArgumentOfLocalPtrPtr(
    /* [in] */ Int32 index,
    /* [out] */ LocalPtr* value)
{
    return SetParamValue(index, &value, CarDataType_LocalPtr,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::SetOutputArgumentOfLocalTypePtr(
    /* [in] */ Int32 index,
    /* [out] */ PVoid value)
{
    return SetParamValue(index, &value, CarDataType_LocalType,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::AssignOutputArgumentOfEnumPtr(
    /* [in] */ Int32 index,
    /* [in] */ Int32 value)
{
    return AssignOutputParamValue(index, &value, CarDataType_Enum,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::SetOutputArgumentOfEnumPtr(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    return SetParamValue(index, &value, CarDataType_Enum,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::AssignOutputArgumentOfCarArrayPtr(
    /* [in] */ Int32 index,
    /* [in] */ PCarQuintet value)
{
    return AssignOutputParamValue(index, &value, CarDataType_CarArray,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::SetOutputArgumentOfCarArrayPtr(
    /* [in] */ Int32 index,
    /* [out] */ PCarQuintet value)
{
    return SetParamValue(index, &value, CarDataType_CarArray,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::AssignOutputArgumentOfCarArrayPtrPtr(
    /* [in] */ Int32 index,
    /* [out] */ PCarQuintet value)
{
    return AssignOutputParamValue(index, &value, CarDataType_CarArray,
            ParamIOAttribute_CalleeAllocOut, 2);
}

ECode CArgumentList::SetOutputArgumentOfCarArrayPtrPtr(
    /* [in] */ Int32 index,
    /* [out] */ PCarQuintet* value)
{
    return SetParamValue(index, &value, CarDataType_CarArray,
            ParamIOAttribute_CalleeAllocOut, 2);
}

ECode CArgumentList::SetOutputArgumentOfStructPtr(
    /* [in] */ Int32 index,
    /* [out] */ PVoid value)
{
    return SetParamValue(index, &value, CarDataType_Struct,
            ParamIOAttribute_CallerAllocOut, 1);
}

ECode CArgumentList::SetOutputArgumentOfStructPtrPtr(
    /* [in] */ Int32 index,
    /* [out] */ PVoid* value)
{
    return SetParamValue(index, &value, CarDataType_Struct,
            ParamIOAttribute_CalleeAllocOut, 2);
}

ECode CArgumentList::AssignOutputArgumentOfObjectPtrPtr(
    /* [in] */ Int32 index,
    /* [in] */ PInterface value)
{
    return AssignOutputParamValue(index, &value, CarDataType_Interface,
            ParamIOAttribute_CallerAllocOut, 2);
}

ECode CArgumentList::SetOutputArgumentOfObjectPtrPtr(
    /* [in] */ Int32 index,
    /* [out] */ PInterface* value)
{
    return SetParamValue(index, &value, CarDataType_Interface,
            ParamIOAttribute_CallerAllocOut, 2);
}
