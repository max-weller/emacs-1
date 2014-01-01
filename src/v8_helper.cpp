

// EMACS


// v8_helper.cpp
// cpp file for emacs test

#include <v8.h>
using namespace v8;

#include <config.h>

extern "C" {
#undef __cplusplus
#define _INCLUDED_FROM_V8_HELPER
#include "lisp.h"
#define __cplusplus
#undef _INCLUDED_FROM_V8_HELPER
}
#include "v8_helper.h"


// probably going to need this for some vectorlike objects or for symbols
// http://create.tpsitulsa.com/wiki/V8/Persistent_Handles#Weak_Handles



bool v8_is_initialized = false;
Handle<FunctionTemplate> symbol_templ;

void GetSymbolName(Local<String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
  Local<Object> self = info.Holder();
  //Local<External> ext = Local<External>::Cast(self->GetInternalField(0));
  Lisp_Object ptr = (Lisp_Object) self->GetAlignedPointerFromInternalField(0); //(Lisp_Object)ext->Value();
  
  info.GetReturnValue().Set (String::New(SSDATA(SYMBOL_NAME(ptr))));
}
void init_v8() {
  printf("init_v8 called\n");
 
  V8::Initialize();

  // Get the default Isolate created at startup.
  Isolate* isolate = Isolate::GetCurrent();

  // Create a stack-allocated handle scope.
  HandleScope handle_scope(isolate);

  symbol_templ = FunctionTemplate::New();
 
  //get the point's instance template
  Handle<ObjectTemplate> symbol_templ_template = symbol_templ->InstanceTemplate();
 
  //set its internal field count to one (we'll put references to the C++ point here later)
  symbol_templ_template->SetInternalFieldCount(1);
 
  symbol_templ_template->SetAccessor(String::New("name"), GetSymbolName, 0);


  printf("init_v8 done\n");
  v8_is_initialized = true;
}

Handle<Value> wrap_lisp_symbol(Lisp_Object sym) {
  Local<Object> obj = symbol_templ->GetFunction()->NewInstance();
  //obj->SetInternalField(0, External::New((void*)sym));
  obj->SetAlignedPointerInInternalField(0, (void*) sym);
  return obj;
}

DEFUN ("run-javascript", Frun_javascript, Srun_javascript, 1, 1, "p",
       doc: /* Test function for v8 integration */)
  (Lisp_Object n)
{
  if (!v8_is_initialized) init_v8();

  // Get the default Isolate created at startup.
  Isolate* isolate = Isolate::GetCurrent();

  // Create a stack-allocated handle scope.
  HandleScope handle_scope(isolate);

  // Create a new context. 
  Handle<Context> context = Context::New(isolate);

  // Enter the created context for compiling and 
  // running the hello world script.
  Context::Scope context_scope(context);


  // Create a string containing the JavaScript source code.
  Handle<String> source = String::New(SSDATA(n));


  // Compile the source code. 
  Handle<Script> script = Script::Compile(source);

  // Run the script to get the result. 
  Handle<Value> result = script->Run();



  return js_to_lisp(result);
}

const char* ToCString(const Handle<Value> value) {
  v8::String::Utf8Value str(value);
  return *str ? *str : "<string conversion failed>";
}

Lisp_Object lisp_call_to_js_call(ptrdiff_t argc, Lisp_Object * argv) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  
  
  
}

Handle<Value> lisp_to_js(Lisp_Object obj) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  if (INTEGERP(obj)) {
    return scope.Close(Number::New(XINT(obj)));
  }
  if (STRINGP(obj)) {
    return scope.Close(String::New(SSDATA(obj)));
  }
  if (FLOATP(obj)) {
    return scope.Close(Number::New(XFLOAT_DATA(obj)));
  }
  if (SYMBOLP(obj)) {
    return scope.Close(wrap_lisp_symbol(obj));
  }
  return Undefined();
}

Lisp_Object js_to_lisp(Handle<Value> jsValue) {
  Isolate* isolate = Isolate::GetCurrent();
  if (jsValue->IsNull() || jsValue->IsUndefined()) return Qnil;
  if (jsValue->IsString()) {
    // Convert the result to an ASCII string and print it. 
    const char* str = ToCString(jsValue);
    return make_string(str, strlen(str));
  }
  if (jsValue->IsNumber()) {
    return make_number(jsValue->NumberValue());
  }
  if (jsValue->IsBoolean()) {
    return jsValue->BooleanValue() ? Qt : Qnil;
  }
  if (jsValue->IsArray()) {
    Handle<Array> array = Handle<Array>::Cast(jsValue);
    Lisp_Object list = Qnil;
    
    for(int i = array->Length() - 1; i >= 0; i--) {
      list = Fcons(js_to_lisp(array->Get(Integer::New(i))), list);
    }
    return list;
    //return make_number(array->Length());
  }
  if (jsValue->IsFunction()) {
    Handle<Function> func = Handle<Function>::Cast(jsValue);
    v8::Persistent<v8::Function> fpt(isolate, func);
    struct Lisp_Subr* lf = ALLOCATE_PSEUDOVECTOR(struct Lisp_Subr, min_args, PVEC_SUBR);
    lf->header.size = PVEC_SUBR << PSEUDOVECTOR_AREA_BITS;
    lf->function.aMANY = &lisp_call_to_js_call;
    lf->symbol_name = "JS Function";
    lf->intspec = "P";
    lf->doc = (char*) (void*) *func;
    return make_lisp_ptr(lf, Lisp_Vectorlike);
  }
  error("Unable to marshal %s to Lisp_Object.", ToCString(jsValue->ToString()));
  
}


void syms_of_v8_b() {
    defsubr(&Srun_javascript);
}


