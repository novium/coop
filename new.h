#ifndef __NEW_H__
#define __NEW_H__

#include <stddef.h>
#include <stdarg.h>

/* Not entirely certain of what the best syntax is
 * Either vtable(struct Vtable, object)->func()
 * Or something more involved giving access to the entire class.
 */
//#define vtable(type, object) ((struct type *) (object->class->vtable))
#define vtable(type, object) ((struct type *) (((struct Object *) object)->vtable))
// #define vtable1(type, object) ((struct type *) object) no.


typedef const struct Class* Class_t;

struct Object {
  Class_t class;

  // This is loosely defined as an "array" of function pointers
  // with variable length parameters. Theese are defined
  // in the object headers.
  // To be precise it's a struct, that is then treated here
  // as an array due to it being of an unknow quantity etc.
  void (**vtable) (void * self, ...);
};

typedef struct Object Object_t;

// Object constructor, takes class and arguments
typedef void    *(*constructor_fun) (void * self, va_list * app);
// Object destructor
typedef void    *(*destructor_fun)  (void * self);
// Clone function
typedef void    *(*clone_fun)       (const void * self);
// Compare function, think Java equals, returns 0 if equal, any non-zero
// integer should be looked in the object documentation
typedef int     (*differ_fun)       (const void * a, const void * b);

// All object are required to export this class,
// especially size, constructor and destructor.
struct Class {
  size_t            size; // Size of object

  constructor_fun   constructor;
  destructor_fun    destructor;

  clone_fun         clone;
  differ_fun        differ;
};

// Creates a new object based on it's class
void * new(Class_t class, ...);
// Deletes an object
void delete(void * self);
// Gets the size of an object (based on it's class)
size_t sizeOf(const void * _self);

#endif
