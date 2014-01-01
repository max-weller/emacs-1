
// v8_helper.h
// header file for emacs test

#ifdef __cplusplus
extern "C" {
#endif


  extern void init_v8();
  extern void syms_of_v8_b();

#ifdef __cplusplus

  Lisp_Object js_to_lisp(Handle<Value> jsValue);

}
#endif


