// Copyright 2018 Mimecast Ltd.
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

#include "btree.h"

btree_s* btree_new()
{
    btree_s *b = Malloc(btree_s);
    *b = (btree_s) {
        .root = NULL, .size = 0
    };
    return b;
}

void btree_destroy(btree_s* b)
{
    if (b->root)
        btreelem_destroy_r(b->root);
    free(b);
}

void btree_destroy2(btree_s* b)
{
    if (b->root)
        btreelem_destroy_r2(b->root);
    free(b);
}

int btree_insert(btree_s* b, long key, void *data)
{
    int ret = 0;

    if (b->root == NULL) {
        b->root = btreelem_new(key, data);
        ret = 1;
    } else {
        ret = btreelem_insert_r(b->root, key, data);
    }

    if (ret == 1)
        b->size++;

    return ret;
}

void* btree_get(btree_s* b, long key)
{
    if (b->root == NULL)
        return NULL;

    return btreelem_get_r(b->root, key);
}

bool btree_has_range_l(btree_s* b, const long start, const long end)
{
    if (b->root == NULL)
        return false;

    return btreelem_has_range_lr(b->root, start, end);
}

long btree_get_l(btree_s* b, long key)
{
    void *data = btree_get(b, key);
    if (data)
        return (long)data;
    else
        return -1;
}

void btree_ensure_range_l(btree_s* b, const long start, const long end, const long threshold)
{
    if (b->root == NULL) {
        btree_insert(b, start, (void*)end);
    } else {
        if (1 == btreelem_ensure_range_lr(b->root, start, end, threshold))
            b->size++;
    }
}

void btree_print(btree_s* b)
{
    Put("btree:%p size:%d", (void*)b, b->size);
    btreelem_print_r(b->root, 1);
}

void btree_run_cb2(btree_s* b, void (*cb)(long key, void *data, void *data2), void *data2)
{
    btreelem_run_cb2_r(b->root, cb, data2);
}

btreelem_s* btreelem_new(long key, void *data)
{
    btreelem_s *e = Malloc(btreelem_s);

    *e = (btreelem_s) {
        .key = key, .data = data, .left = NULL, .right = NULL
    };

    return e;
}

void btreelem_destroy_r(btreelem_s* e)
{
    if (e->left)
        btreelem_destroy_r(e->left);
    if (e->right)
        btreelem_destroy_r(e->right);

    free(e);
}

void btreelem_destroy_r2(btreelem_s* e)
{
    if (e->left)
        btreelem_destroy_r(e->left);
    if (e->right)
        btreelem_destroy_r(e->right);
    if (e->data)
        btree_destroy(e->data);

    free(e);
}

int btreelem_insert_r(btreelem_s* e, long key, void *data)
{
    int ret = 1;

    if (e->key == key) {
        ret = 0;
    }

    else if (e->key > key) {
        if (e->left == NULL)
            e->left = btreelem_new(key, data);
        else
            ret = btreelem_insert_r(e->left, key, data);
    }

    else {
        if (e->right == NULL)
            e->right = btreelem_new(key, data);
        else
            ret = btreelem_insert_r(e->right, key, data);
    }

    return ret;
}

int btreelem_ensure_range_lr(btreelem_s *e, const long start, const long end, const long threshold)
{
    int ret = 0;
    long value = (long) e->data;

    //Debug("%ld %ld %ld", start, end, threshold);

    if (e->key == start) {
        if (value < end) {
            e->data = (void*) end;
        } else {
            // Nothing to do, range already present
        }

    } else if (e->key > start) {
        if (e->left == NULL) {
            if (value <= end) {
                e->key = start;
                e->data = (void*)end;
            } else {
                e->left = btreelem_new(start, (void*)end);
                ret = 1;
            }
        } else {
            ret = btreelem_ensure_range_lr(e->left, start, end, threshold);
        }

    } else { // if (e->key < start)
        if (value >= start) {
            if (value < end) {
                e->data = (void*) end;
            } else {
                // Nothing to do, range already present
            }
        } else {
            if (e->right == NULL) {
                e->right = btreelem_new(start, (void*)end);
                ret = 1;
            } else {
                ret = btreelem_ensure_range_lr(e->right, start, end, threshold);
            }
        }
    }

    return ret;
}

void* btreelem_get_r(btreelem_s* e, long key)
{
    void *data = NULL;

    if (e->key == key) {
        data = e->data;

    } else if (e->key > key) {
        if (e->left)
            data = btreelem_get_r(e->left, key);

    } else {
        if (e->right)
            data = btreelem_get_r(e->right, key);
    }

    return data;
}

bool btreelem_has_range_lr(btreelem_s* e, const long start, const long end)
{
    long value = (long)e->data;

    if (e->key <= start && value >= end) {
        return true;

    } else if (e->key > start) {
        if (e->left)
            return btreelem_has_range_lr(e->left, start, end);
        else
            return false;

    } else {
        if (e->right)
            return btreelem_has_range_lr(e->right, start, end);
        else
            return false;
    }
}

void btreelem_print_r(btreelem_s* e, int depth)
{
    if (!e)
        return;

    for (int i = 0; i < depth; ++i) {
        Out(" ");
    }
    Put("key:%ld data:%ld", e->key, (long) e->data);

    if (e->left)
        btreelem_print_r(e->left, depth);

    if (e->right)
        btreelem_print_r(e->right, depth+1);
}

void btreelem_run_cb2_r(btreelem_s* e, void (*cb)(long key, void *data, void *data2), void *data2)
{
    if (!e)
        return;

    cb(e->key, e->data, data2);

    if (e->left)
        btreelem_run_cb2_r(e->left, cb, data2);

    if (e->right)
        btreelem_run_cb2_r(e->right, cb, data2);
}

void btree_test(void)
{
    btree_s *b = btree_new();
    void* somedata = (void*)b;

    assert(1 == btree_insert(b, 1, (void*)1));
    assert(1 == b->size);
    assert(1 == btree_insert(b, 2, (void*)2));
    assert(2 == b->size);
    assert(1 == btree_insert(b, 3, (void*)3));
    assert(3 == b->size);
    assert(1 == btree_get_l(b, 1));

    assert(1 == btree_insert(b, 1234, somedata));
    assert(4 == b->size);
    assert(1 == btree_insert(b, 13, somedata));
    assert(5 == b->size);
    assert(1 == btree_insert(b, 666, somedata));
    assert(6 == b->size);
    assert(0 == btree_insert(b, 13, somedata));
    assert(6 == b->size);

    assert(NULL != btree_get(b, 666));
    assert(NULL == btree_get(b, 777));

    assert(0 == btree_insert(b, 666, somedata));
    assert(1 == btree_insert(b, 42, (void*)42));
    assert(42 == btree_get_l(b, 42));

    btree_print(b);
    btree_destroy(b);

    b = btree_new();
    assert(0 == b->size);

    btree_ensure_range_l(b, 0, 23, 0);
    assert(btree_get_l(b, 0) == (long) btree_get(b, 0));
    assert(23 == btree_get_l(b, 0));
    assert(btree_has_range_l(b, 2, 10));

    assert(!btree_has_range_l(b, 300, 325));
    btree_ensure_range_l(b, 300, 325, 0);
    assert(2 == b->size);
    assert(325 == btree_get_l(b, 300));
    assert(btree_has_range_l(b, 300, 325));
    assert(!btree_has_range_l(b, 300, 326));

    btree_ensure_range_l(b, 200, 3321, 0);
    assert(2 == b->size);

    btree_ensure_range_l(b, 200, 1000, 0);
    assert(2 == b->size);
    assert(3321 == btree_get_l(b, 200));

    btree_ensure_range_l(b, 0, 23, 0);
    btree_ensure_range_l(b, 10, 10, 0);
    btree_ensure_range_l(b, 22, 25, 0);
    assert(25 == btree_get_l(b, 0));
    assert(2 == b->size);

    assert(!btree_has_range_l(b, 4000, 4000));
    btree_ensure_range_l(b, 4000, 4000, 0);
    assert(3 == b->size);
    assert(btree_has_range_l(b, 4000, 4000));
    assert(!btree_has_range_l(b, 4000, 4001));
    assert(!btree_has_range_l(b, 3999, 4000));
    btree_print(b);

    btree_print(b);
    btree_destroy(b);
}
