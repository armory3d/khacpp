#include <hxcpp.h>
#include <math.h>

using namespace hx;

namespace hx
{

Class __BoolClass;
Class __IntClass;
Class __FloatClass;
Class __VoidClass;
Class Class_obj__mClass;


Class &GetBoolClass() { return __BoolClass; }
Class &GetIntClass() { return __IntClass; }
Class &GetFloatClass() { return __FloatClass; }
Class &GetVoidClass() { return __VoidClass; }



// --- "Simple" Data Objects ---------------------------------------------------


class IntData : public hx::Object
{
public:
   IntData(int inValue=0) : mValue(inValue) {};

   Class __GetClass() const { return __IntClass; }
   bool __Is(hx::Object *inClass) const { return dynamic_cast< IntData *>(inClass); }

   virtual int __GetType() const { return vtInt; }

   String toString() { return String(mValue); }
   String __ToString() const { return String(mValue); }
   double __ToDouble() const { return mValue; }
   int __ToInt() const { return mValue; }

   int __Compare(const hx::Object *inRHS) const
   {
      double diff = inRHS->__ToDouble() - mValue;
      return diff < 0 ? -1 : diff==0 ? 0 : 1;
   }


   int mValue;
};


class BoolData : public hx::Object
{
public:
   BoolData(bool inValue=false) : mValue(inValue) {};

   Class __GetClass() const { return __BoolClass; }
   bool __Is(hx::Object *inClass) const { return dynamic_cast< BoolData *>(inClass); }

   virtual int __GetType() const { return vtBool; }

   String __ToString() const { return mValue ? L"true" : L"false"; }
   String toString() { return mValue ? L"true" : L"false"; }
   double __ToDouble() const { return mValue; }
   int __ToInt() const { return mValue; }

   int __Compare(const hx::Object *inRHS) const
   {
      double diff = inRHS->__ToDouble() - (double)mValue;
      return diff < 0 ? -1 : diff==0 ? 0 : 1;
   }


   bool mValue;
};



class DoubleData : public hx::Object
{
public:
   DoubleData(double inValue=0) : mValue(inValue) {};

   Class __GetClass() const { return __FloatClass; }
   bool __Is(hx::Object *inClass) const { return dynamic_cast< DoubleData *>(inClass); }

   virtual int __GetType() const { return vtFloat; }
   String toString() { return String(mValue); }
   String __ToString() const { return String(mValue); }
   double __ToDouble() const { return mValue; }
   int __ToInt() const { return (int)mValue; }

   int __Compare(const hx::Object *inRHS) const
   {
      double diff = inRHS->__ToDouble() - (double)mValue;
      return diff < 0 ? 1 : diff==0 ? 0 : -1;
   }


   double mValue;
};



}



// --- Dynamic -------------------------------------------------


Dynamic::Dynamic(bool inVal) : super( new BoolData(inVal) ) { }
Dynamic::Dynamic(int inVal) : super( new IntData(inVal) ) { }
Dynamic::Dynamic(double inVal) : super( new DoubleData(inVal) ) { }
Dynamic::Dynamic(const cpp::CppInt32__ &inVal) : super( new IntData((int)inVal) ) { }
Dynamic::Dynamic(const String &inVal) : super( inVal.__s ? inVal.__ToObject() : 0 ) { }
Dynamic::Dynamic(const wchar_t *inVal) : super( inVal ? String(String(inVal)).__ToObject() : 0 ) { }


Dynamic Dynamic::operator+(const Dynamic &inRHS) const
{
   int t1 = mPtr ? mPtr->__GetType() : vtNull;
   int t2 = inRHS.mPtr ? inRHS.mPtr->__GetType() : vtNull;

   if ( (t1==vtInt || t1==vtFloat)  &&  (t2==vtInt || t2==vtFloat) )
   {
      return mPtr->__ToDouble() + inRHS.mPtr->__ToDouble();
   }
   if (!mPtr)
      return inRHS;
   if (!inRHS.mPtr)
      return this;

   return const_cast<hx::Object*>(mPtr)->toString() + const_cast<Dynamic&>(inRHS)->toString();
}

Dynamic Dynamic::operator+(const int &i) const
{
   int t = mPtr ? mPtr->__GetType() : vtNull;
   if (t==vtString)
      return Cast<String>() + String(i);
   return Cast<Int>() + i;
}

Dynamic Dynamic::operator+(const double &d) const
{
   int t = mPtr ? mPtr->__GetType() : vtNull;
   if (t==vtString)
      return Cast<String>() + String(d);
   return Cast<double>() + d;
}


double Dynamic::operator%(const Dynamic &inRHS) const
{
   if (mPtr->__GetType()==vtInt && inRHS.mPtr->__GetType()==vtInt)
      return mPtr->__ToInt() % inRHS->__ToInt();
   double lhs = mPtr->__ToDouble();
   double rhs = inRHS->__ToDouble();
   int even = (int)(lhs/rhs);
   double remain = lhs - even * rhs;
   if (remain<0) remain += fabs(rhs);
   return remain;
}


void Dynamic::CheckFPtr()
{
	if (!mPtr)
		throw HX_NULL_FUNCTION_POINTER;
}

namespace cpp
{
CppInt32__::CppInt32__(const Dynamic &inD) : mValue(inD->__ToInt()) { }
}

static bool NoCast(hx::Object *) { return false; }
static bool IsFloat(hx::Object *inPtr)
{
   return inPtr && (TCanCast<IntData>(inPtr) || TCanCast<DoubleData>(inPtr));
}


void Dynamic::__boot()
{
   Static(__BoolClass) = RegisterClass(HX_STRING(L"Bool",4),TCanCast<BoolData>,sNone,sNone, 0,0, 0 );
   Static(__IntClass) = RegisterClass(HX_STRING(L"Int",3),TCanCast<IntData>,sNone,sNone,0,0, 0 );
   Static(__FloatClass) = RegisterClass(HX_STRING(L"Float",5),IsFloat,sNone,sNone, 0,0,&__IntClass );
   Static(__VoidClass) = RegisterClass(HX_STRING(L"Void",4),NoCast,sNone,sNone,0,0,0, 0 );
}


