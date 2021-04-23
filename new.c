#include <assert.h>
#include <stdlib.h>

#include "new.h"

void * new(Class_t class, ...) {
  Object_t * p = calloc(1, class->size);  // Allocate space needed for object

  assert(p);
  p->class = class;

  if(p->class->constructor) {
    va_list ap;
    va_start(ap, class);
    p = p->class->constructor(p, &ap);
    va_end(ap);
  }

  return p;
}

void delete(void * self) {
  Object_t * obj = self;

  if(self){
    if (obj->class && obj->class->destructor){
      self = obj->class->destructor(self);
    }

    if (obj->vtable){
      free(obj->vtable);
    }
  }
  free(self);
}

size_t sizeOf(const void * _self) {
  const Object_t * self = _self;
  assert(self && self->class);
  return self->class->size;
}
