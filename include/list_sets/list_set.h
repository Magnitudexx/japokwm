#ifndef LIST_SET_H
#define LIST_SET_H

#include <stdlib.h>
#include <glib.h>
#include <wayland-client-core.h>

#include "utils/coreUtils.h"

struct tag;
struct container;

typedef bool is_condition_t(
        void *arg,
        GPtrArray *src_list,
        struct container *con
        );

GPtrArray *list_create_sub_list(GPtrArray *list, int start_idx, int end_idx);
GPtrArray *list2D_create_filtered_sub_list(
        GPtrArray2D *list,
        bool is_condition(struct container *con));
GPtrArray *list_create_filtered_sub_list_with_order(
        GPtrArray *list,
        GPtrArray *conditions);
GPtrArray *list_create_filtered_sub_list_with_order(
        GPtrArray2D *list,
        GPtrArray *conditions);
GPtrArray *list_create_filtered_sub_list(
        GPtrArray2D *list,
        bool is_condition(struct container *con));
void lists_cat_to_list(GPtrArray2D *dest, GPtrArray2D *src);
void lists_append_list_under_condition(
        GPtrArray2D *dest,
        GPtrArray2D *src,
        is_condition_t condition,
        struct tag *tag
        );
void list_append_list_under_condition(
        GPtrArray *dest,
        GPtrArray *src,
        is_condition_t is_condition,
        void *arg
        );
GPtrArray *list2D_flatten(GPtrArray2D *dest);
void sub_list_write_to_parent_list2D(GPtrArray2D *parent, GPtrArray2D *child);
void sub_list_write_to_parent_list1D(GPtrArray *parent, GPtrArray *child);
void sub_list_write_to_parent_list(GPtrArray2D *parent, GPtrArray *child);
void lists_clear(GPtrArray2D *lists);

#endif /* LIST_SET_H */
