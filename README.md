# Disk Scheduling

This project implements various disk scheduling algorithms used in operating systems. It simulates how an operating system might manage I/O requests, handle request arrivals and completions, and schedule disk operations efficiently.

## Algorithms Implemented

1. **First-Come-First-Served (FCFS)**: Processes requests in the order they arrive.
2. **Shortest-Seek-Time-First (SSTF)**: Chooses the request with the minimum seek time from the current position.
3. **LOOK**: An elevator-like algorithm that services requests in one direction until no more requests in that direction, then reverses.

## Key Features

- Simulates an I/O Request Queue as an array of Request Control Blocks (RCBs).
- Each RCB tracks its request ID, arrival timestamp, cylinder, address, and process ID.
- Handles scenarios such as request arrivals when the disk is busy or free, and request completions.
- Implements different scheduling strategies for selecting the next request to service.

## How It Works

Each disk scheduling algorithm manages the request queue based on its specific criteria:

- **FCFS**: Processes requests in the order they arrive, maintaining a simple queue structure.
- **SSTF**: Selects the request closest to the current cylinder position, minimizing seek time.
- **LOOK**: Services requests in one direction (increasing or decreasing cylinder numbers) until no more requests in that direction, then reverses.

The request handling functions simulate both the arrival of new requests and the completion of current requests, returning the appropriate RCB based on the scheduling policy.

## Usage

Include the necessary headers in your project and call the functions as needed:

```c
#include "rcb.h"

// FCFS functions
struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp);
struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt);

// SSTF functions
struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp);
struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder);

// LOOK functions
struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp);
struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder, int scan_direction);
```

## Implementation Details

### Request Control Block (RCB) Structure

```c
struct RCB {
    int request_id;
    int arrival_timestamp;
    int cylinder;
    int address;
    int process_id;
};
```

### Key Functions

1. `handle_request_arrival_*`: These functions (where * is fcfs, sstf, or look) handle the arrival of new I/O requests. They either return the new request if the disk is free or add it to the queue and return the current request.

2. `handle_request_completion_*`: These functions handle the completion of the current I/O request. They select the next request to be serviced based on the respective scheduling algorithm.

Each algorithm implementation includes logic for:
- Handling request arrivals when the disk is busy or free
- Selecting the next request to service based on the specific scheduling policy
- Maintaining the request queue

## Notes

- The project uses predefined constants like QUEUEMAX. Ensure these are properly defined in your header files.
- The implementation assumes a simplistic model of disk scheduling and may not reflect all complexities of real-world systems.
- Error handling for edge cases (like full request queues) is implemented in a basic manner and may need additional refinement for production use.
- The LOOK algorithm implementation includes logic for changing scan direction when necessary.


This project serves as a educational tool for understanding basic disk scheduling algorithms in operating systems. It provides a foundation for further exploration and implementation of more complex disk management strategies.
