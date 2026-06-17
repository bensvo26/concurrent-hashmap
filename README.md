# Concurrent Hash Map

A thread-safe hash map in C using separate chaining and per-bucket locking for concurrent access.

## Overview

This project implements a hash map that supports safe concurrent reads and writes from multiple threads. Rather than guarding the whole table with a single lock, each bucket has its own mutex, so threads operating on different buckets never block each other. This fine-grained locking lets the map scale across threads instead of serializing every operation.

## Design

- **Separate chaining**: each bucket holds a singly linked list of key/value nodes, so collisions are handled by appending to the bucket's list.
- **Per-bucket locking**: every bucket carries its own `pthread_mutex_t`. Operations lock only the bucket they touch, allowing true parallelism across buckets.
- **Hashing**: keys map to buckets via modulo over the table size.

## API

- `init(size)` — create the map with a fixed number of buckets
- `put(item)` — insert a key/value pair, or update the value if the key exists
- `get(key)` — look up a key, returns a pointer to the item or NULL

## Tech Stack

C, pthreads, Make

## Building

\`\`\`bash
make
\`\`\`

This produces the shared library `libhashmap.dylib`.

## Testing

Two test programs are included:

**Correctness** — verifies basic insert/lookup, value updates, missing-key handling, and concurrent inserts across 4 threads with no lost or corrupted entries.

\`\`\`bash
./test_correctness
\`\`\`

**Performance** — benchmarks throughput under a mix of `get` and `put` operations while scaling the thread count (1, 2, 3, 5), reporting time per million operations.

\`\`\`bash
./test_performance
\`\`\`

## What I Learned

This project taught me how lock granularity affects concurrency. Starting from the idea of a thread-safe map, I learned why a single global lock serializes everything and how giving each bucket its own mutex lets independent operations run in parallel. Writing the performance test also made the tradeoffs concrete, since I could measure how throughput changed as I added threads.
