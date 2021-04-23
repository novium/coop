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
  struct ItemV * vtable;

  char * name;
  char * desc;
  int    price;

  List_t * shelves;
};

static void Item_print(const Item_t * self);
static void Item_print_name(const Item_t * self);
static void Item_set_name(Item_t * self, const char * name);
static void Item_set_desc(Item_t * self, const char * desc);
static void Item_set_price(Item_t * self, int amount);
static char * Item_get_name(const Item_t * self);
static char * Item_get_desc(const Item_t * self);
static int Item_get_price(const Item_t * self);
static void Item_shelf_add(const Item_t * self, Shelf_t * shelf);
static void Item_shelf_remove(const Item_t * self, Shelf_t * shelf);
static List_t * Item_shelf_list(const Item_t * self);

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


// Functions

static void Item_print(const Item_t * self) {
  printf("Namn:          %s\n", self->name);
  printf("Beskrivning:   %s\n", self->desc);
  printf("Pris:          %d\n", self->price);

  int length = vtable(ListV, self->shelves)->length(self->shelves);
  if(length == 0) {
    printf("Varan finns inte på några hyllor!\n\n");
    return;
  }

  printf("Varan finns på följande hyllor [HYLLA, ANTAL]:\n");
  for(int i = 0; i < length; i++) {
    Shelf_t * shelf = vtable(ListV, self->shelves)->get(self->shelves, i);
    char * shelfname = vtable(ShelfV, shelf)->get.name(shelf);
    int count = vtable(ShelfV, shelf)->get.amount(shelf);
    printf("[%s, %d] ", shelfname, count);
    //TODO: Check delete
    free(shelfname);
  }
  printf("\n");
}

static void Item_print_name(const Item_t * self) {
  printf("%s\n", self->name);
}

static void Item_set_name(Item_t * self, const char * name) {
  free(self->name);
  self->name = malloc(strlen(name) + 1);
  strcpy(self->name, name);
}

static void Item_set_desc(Item_t * self, const char * desc) {
  free(self->desc);
  self->desc = malloc(strlen(desc) + 1);
  strcpy(self->desc, desc);
}

static void Item_set_price(Item_t * self, int price) {
  self->price = price;
}

static char * Item_get_name(const Item_t * self) {
  char * res = malloc(strlen(self->name) + 1);
  strcpy(res, self->name);
  return res;
}

static char * Item_get_desc(const Item_t * self) {
  char * res = malloc(strlen(self->desc) + 1);
  strcpy(res, self->desc);
  return res;
}

static int Item_get_price(const Item_t * self) {
  return self->price;
}

static void Item_shelf_add(const Item_t * self, Shelf_t * shelf) {
  vtable(ListV, self->shelves)->append(self->shelves, shelf);
}

static void Item_shelf_remove(const Item_t * self, Shelf_t * shelf) {
  int index;
  index = vtable(ListV, self->shelves)->contains(self->shelves, shelf);

  if(index != -1) {
    vtable(ListV, self->shelves)->remove(self->shelves, index);
  }
}

static List_t * Item_shelf_list(const Item_t * self) {
  return self->shelves;
}
