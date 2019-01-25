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

#ifndef GRAPH_H
#define GRAPH_H

#include "../defaults.h"
#include "../tpool/tpool.h"

#include "hmap.h"

/**
 * @brief Definition of a graph nodeent
 */
typedef struct graph_node_s_ {
    unsigned long id; /**< The id of the graph nodeent */
    _Bool traversed; /**< Determines if node has been traversed already */
    char *path; /** < The path of the graph nodeent */
    struct graph_node_s_ **prev; /**< The previous nodeents */
    struct graph_node_s_ **next; /**< The next nodeents */
    int num_prev; /**< Amount of prev nodes */
    int num_next; /**< Amount of next nodes */
    void *data; /**< Pointer to the stored data */
    pthread_mutex_t mutex; /**< To sync access to this graph nodeent */

} graph_node_s;

/**
 * @brief Definition of a graph traverser
 */
typedef struct graph_traverser_s_ {
    tpool_s *pool; /**< The thread pool to traverse the graph */
    void (*callback)(graph_node_s *node, unsigned long depth); /**< Callback to run on all nodes */
} graph_traverser_s;

/**
 * @brief Definition of a graph data structure
 */
typedef struct graph_s_ {
    graph_node_s *root; /**< The root nodeent */
    void (*data_destroy)(void *data); /**< Callback to destroy all data */
    hmap_s *paths; /**< The paths of the graph */
    pthread_mutex_t mutex; /**< To sync access to this graph nodeent */
} graph_s;

graph_node_s *graph_node_new(void *data, char *key);
void graph_node_destroy(graph_node_s *e, void(*data_destroy)(void *data));
void graph_node_append(graph_node_s *e, graph_node_s *e2);
void graph_node_print(graph_node_s *e);

graph_traverser_s *graph_traverser_new(void (*callback)(graph_node_s *node, unsigned long depth), int max_threads);
void graph_traverser_destroy(graph_traverser_s* t);
void graph_traverser_traverse(graph_traverser_s* t, graph_s *g);

graph_s *graph_new(unsigned int init_size, void(*data_destroy)(void *data));
void graph_destroy(graph_s* g);
void graph_insert(graph_s* g, char *path, void *data);
void* graph_get(graph_s* g, char *path);
void graph_print(graph_s* g);

/**
 * @brief Serialise the graph to a file.
 *
 * @param g The graph.
 * @param file The destination file.
 */
void graph_serialise(graph_s* g, const char *file);

void graph_test();

#endif // GRAPH_H
