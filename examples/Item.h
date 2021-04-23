#ifndef __ITEM_H__
#define __ITEM_H__

#include "new.h"
#include "Shelf.h"
#include "List.h"

extern Class_t Item;

typedef struct Item Item_t;
/**
 * Item Vtable
 */
struct ItemV {
  /**
   * \param item void *
   *
   * Prints item
   */
  void (* print) (const Item_t * item);
  void (* print_name) (const Item_t * item);

  struct {
    /**
     * \param item void *
     * \param name char *
     *
     * Sets name of item to name
     */
    void (* name) (Item_t * item, const char * name);
    /**
     * \param item void *
     * \param desc char *
     *
     * Sets description of item to desc
     */
    void (* desc) (Item_t * item, const char * desc);
    /**
     * \param item void *
     * \param price int
     *
     * Sets price of item to price
     */
    void (* price) (Item_t * item, const int price);
  } set;

  struct {
    char *  (* name)    (const Item_t * item);
    char *  (* desc)    (const Item_t * item);
    int     (* price)   (const Item_t * item);
  } get;

  struct {
    /**
     * \param item void *
     * \param shelf void *
     *
     * Adds a shelf to item
     */
    void (* add) (const Item_t * item, Shelf_t * shelf);
    /**
     * \param item void *
     * \param shelf void *
     *
     * Finds and removes shelf from item
     */
    void (* remove) (const Item_t * item, Shelf_t * shelf);
    /**
     * \param item void *
     *
     * Returns a list of Shelf's in item
     */
    List_t * (* list) (const Item_t * item); // Returns a List of Shelf's
  } shelf;

};


#endif
