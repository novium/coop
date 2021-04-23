# coop
Write OOP in C, probably a bad idea

## Why?

This was written by me during the course [Imperative and Object-oriented Programming Methodology](https://wrigstad.com/ioopm). Since I had a bit too much time on my hands I decided to do some of the OOP course goals before we started with Java. Crazy idea, but worked out flawlessly!

## Usage

Here's an example from the assignment where we implemented a warehouse store manager. 

```c
#ifndef __ITEM_H__
#define __ITEM_H__

#include "new.h"
#include "Shelf.h"
#include "List.h"

extern Class_t Item;

typedef struct Item Item_t;

struct ItemV {
  // Methods (print the item)
  void (*print) (const Item_t *item);
  void (*print_name) (const Item_t *item);

  // Setters
  struct {
    void (*name)  (Item_t *item, const char *name);
    void (*desc)  (Item_t *item, const char *desc);
    void (*price) (Item_t *item, const int price);
  } set;

  // Getters
  struct {
    char *(*name)  (const Item_t *item);
    char *(*desc)  (const Item_t *item);
    int   (*price) (const Item_t *item);
  } get;

  // Methods (adding shelf locations to item)
  struct {
    void    (*add)    (const Item_t *item, Shelf_t *shelf);
    void    (*remove) (const Item_t *item, Shelf_t *shelf);
    List_t *(*list)   (const Item_t *item); // Returns a List of Shelf's
  } shelf;

};
#endif
```

```c
#define _GNU_SOURCE

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "Item.h"
#include "List.h"
#include "Shelf.h"
#include "new.h"

// NOTE: padded with 4 bytes. Perhaps make price a long?
struct Item {
  Class_t class;
  struct ItemV *vtable;

  char *name;
  char *desc;
  int   price;

  List_t *shelves;
};

static void Item_print_name(const Item_t * self);
static void Item_set_name(Item_t * self, const char * name);

static struct ItemV _Vtable = {
  Item_print, Item_print_name,
  { Item_set_name, Item_set_desc, Item_set_price },
  { Item_get_name, Item_get_desc, Item_get_price },
  { Item_shelf_add, Item_shelf_remove, Item_shelf_list }
};

static Item_t * Item_constructor(Item_t * self, va_list * app) {
  // Get arguments from va_list passed to new()
  const char * name  = va_arg(* app, const char *);
  const char * desc  = va_arg(* app, const char *);
  const int    price = va_arg(* app, const int);

  // Malloc space for strings and assert
  self->name = malloc(strlen(name) + 1);
  self->desc = malloc(strlen(desc) + 1);
  assert(self->name);
  assert(self->desc);
  strcpy(self->name, name);
  strcpy(self->desc, desc);

  // Last, but not at all least, assign price
  self->price = price;

  // Also create the shelflist
  self->shelves = new(List, false, false);

  // And add a vtable
  self->vtable = malloc(1 * sizeof(struct ItemV));
  memcpy(self->vtable, & _Vtable, sizeof(struct ItemV));

  return self;
}

static Item_t * Item_destructor(Item_t * self) {
  free(self->name); self->name = 0;
  free(self->desc); self->desc = 0;
  delete(self->shelves); // TODO: Remove shelves from main shelflist as well

  self->price = 0;

  return self;
}

static Item_t * Item_clone(const Item_t * self) {
  puts(" *** ITEM CLONING NOT IMPLEMENTED ***");
  (void) self;
  return NULL;
}

static int Item_differ(const Item_t * self, const Item_t * b) {
  return strcmp(self->name, b->name);
}

static const struct Class _Item = {
  sizeof(struct Item),
  (constructor_fun) Item_constructor,
  (destructor_fun)  Item_destructor,
  (clone_fun)       Item_clone,
  (differ_fun)      Item_differ
};

Class_t Item = & _Item;

static void Item_print_name(const Item_t * self) {
  printf("%s\n", self->name);
}

static void Item_set_name(Item_t * self, const char * name) {
  free(self->name);
  self->name = malloc(strlen(name) + 1);
  strcpy(self->name, name);
}
```