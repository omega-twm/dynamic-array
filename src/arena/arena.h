#pragma once

#include <stdint.h>
#include <stdlib.h>
#include "common.h"

/*!
 * Type of arena. 'arena_t' is an alias for 'struct arena'
 */
typedef struct arena arena_t;

/*!
 * Type of arena save. 'arena_save_t' is an alias for 'struct arena_save'
 *
 * This lets us do temporary work, like building a temprary array mid function
 * without needing a seperate scratch arena
 */
typedef struct arena_save arena_save_t;

arena_t arena_make(size_t capacity);

void *arena_allocate_aligned(arena_t *a, size_t size, size_t align);

void *arena_allocate_zero(arena_t *a, size_t size);

void *arena_allocate(arena_t *a, size_t size);

void arena_reset(arena_t *a);

void arena_destroy(arena_t *a);

/*!
 * Returns area used in given arena
 */
size_t arena_used(arena_t *a);

/*!
 * Returns remaining area used in given arena
 */
size_t arena_remaining(arena_t *a);

/*!
 * @bried Saves the arena state
 * @param a: An arena that has already been created
 * @returns The saved state of given arena
 */
arena_save_t arena_save(arena_t *a);

/*!
 * Restores arena to previous state
 *
 * usage:
 *    'arena_save_t save = arena_save(a);
 *     // do temorary work...
 *     arena_restore(save); // rolls back as if the allocations never happened
 *    '
 * @param save: arena save state
 */
void arena_restore(arena_save_t save);
