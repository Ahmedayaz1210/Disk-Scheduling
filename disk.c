#include "rcb.h"
#include <stdlib.h>
#include <limits.h>

struct RCB NULLRCB = {0, 0, 0, 0, 0};


// The method returns the RCB of the newly-arriving request if the disk is free (indicated by the third parameter being a NULLRCB), otherwise, it returns the RCB of the currently-serviced request after adding the newly-arriving request to the request queue.

struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    if (current_request.request_id == NULLRCB.request_id) {
        return new_request;
    } else {
        if (*queue_cnt < QUEUEMAX) {
            request_queue[*queue_cnt] = new_request;
            (*queue_cnt)++;
        }
        return current_request;
    }
}

// This method implements the logic to handle the completion of servicing an IO request in a First-Come-First-Served Scheduler. The method determines the request to service next and returns its RCB.
struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt) {
    // If the request queue is empty, the method returns a NULLRCB, indicating that there is no request to service next. 
    if (*queue_cnt == 0) {
        return NULLRCB;
    }

    // Otherwise, the method finds the RCB in the request queue that has the earliest arrival time 
    struct RCB next_request = request_queue[0];
    int next_index = 0;

    for (int i = 1; i < *queue_cnt; i++) {
        if (request_queue[i].arrival_timestamp < next_request.arrival_timestamp) {
            next_request = request_queue[i];
            next_index = i;
        }
    }

    // It then removes this RCB from the request queue and returns it.
    for (int i = next_index; i < *queue_cnt - 1; i++) {
        request_queue[i] = request_queue[i + 1];
    }

    (*queue_cnt)--;

    return next_request;
}




// This method implements the logic to handle the arrival of a new IO request in a Shortest-Seek-First (also known as Shortest-Seek-Time-First) Scheduler. 
struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    // The method returns the RCB of the newly-arriving request if the disk is free (indicated by the third parameter being NULLRCB)
    if (current_request.request_id == NULLRCB.request_id) {
        return new_request;
    } 
    // Otherwise, it returns the RCB of the currently-serviced request after adding the newly-arriving request to the request queue. 
    else {
        if (*queue_cnt < QUEUEMAX) {
            request_queue[*queue_cnt] = new_request;
            (*queue_cnt)++;
        }
        return current_request;
    }
}

// This method implements the logic to handle the completion of servicing an IO request in a Shortest-Seek-Time-First Scheduler.

struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder) {
    // If the request queue is empty, the method returns NULLRCB, indicating that there is no request to service next. 
    if (*queue_cnt == 0) {
        return NULLRCB;
    }


    struct RCB selected_request;
    int min_distance = INT_MAX;
    int index_to_remove = -1;

    // Otherwise, the method finds the RCB in the request queue whose cylinder is closest to the current cylinder. If there are multiple requests with the closest cylinder, then the method picks the request among these that has the earliest arrival_timestamp. 
    for (int i = 0; i < *queue_cnt; i++) {
        int distance = abs(request_queue[i].cylinder - current_cylinder);
        if (distance < min_distance || (distance == min_distance && request_queue[i].arrival_timestamp < selected_request.arrival_timestamp)) {
            min_distance = distance;
            selected_request = request_queue[i];
            index_to_remove = i;
        }
    }

    // The method then removes the RCB of the selected request from the request queue and returns it
    if (index_to_remove != -1) {
        for (int i = index_to_remove; i < *queue_cnt - 1; i++) {
            request_queue[i] = request_queue[i + 1];
        }
        (*queue_cnt)--;
    }

    return selected_request;
}

// This method implements the logic to handle the arrival of a new IO request in a LOOK (also known as Elevator) Scheduler

struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    // The method returns the RCB of the newly-arriving request if the disk is free (indicated by the third parameter being NULLRCB)
    if (current_request.request_id == NULLRCB.request_id) {
        return new_request;
    } 
    // Otherwise, it returns the RCB of the currently-serviced request after adding the newly-arriving request to the request queue
    else {
        if (*queue_cnt < QUEUEMAX) {
            request_queue[*queue_cnt] = new_request;
            (*queue_cnt)++;
        }
        return current_request;
    }
}

// This method implements the logic to handle the completion of servicing an IO request in a LOOK Scheduler.

struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder, int scan_direction) {
    // If the request queue is empty, the method returns NULLRCB, indicating that there is no request to service next
    if (*queue_cnt == 0) {
        return NULLRCB;
    }

    int index = -1;
    int closest_distance = __INT_MAX__;
    int target_cylinder = current_cylinder;
    // Otherwise, it picks the next request to service from the request queue.

    for (int i = 0; i < *queue_cnt; i++) {
        if (request_queue[i].cylinder == target_cylinder) {
            if (index == -1 || request_queue[i].arrival_timestamp < request_queue[index].arrival_timestamp) {
                index = i;
            }
        }
    }

    // If there are requests in the queue with the same cylinder as the current cylinder, the method picks the one among these requests with the earliest arrival time. Otherwise, if the scan direction is 1 and there are requests with cylinders larger than the current cylinder, the method picks the one among these whose cylinder is closest to the current cylinder. Otherwise, if the scan direction is 1 and there are no requests with cylinders larger than the current cylinder, the method picks the request whose cylinder is closest to the current cylinder. Otherwise, if the scan direction is 0 and there are requests with cylinders smaller than the current cylinder, the method picks the one among these whose cylinder is closest to the current cylinder. Otherwise, if the scan direction is 0 and there are requests with cylinders larger than the current cylinder, the method picks the request whose cylinder is closest to the current cylinder. 

    if (index == -1) {
        if (scan_direction == 1) {
            for (int i = 0; i < *queue_cnt; i++) {
                if (request_queue[i].cylinder > target_cylinder) {
                    int distance = request_queue[i].cylinder - target_cylinder;
                    if (distance < closest_distance) {
                        closest_distance = distance;
                        index = i;
                    }
                }
            }
            if (index == -1) {
                for (int i = 0; i < *queue_cnt; i++) {
                    int distance = abs(request_queue[i].cylinder - target_cylinder);
                    if (distance < closest_distance) {
                        closest_distance = distance;
                        index = i;
                    }
                }
            }
        } else {
            for (int i = 0; i < *queue_cnt; i++) {
                if (request_queue[i].cylinder < target_cylinder) {
                    int distance = target_cylinder - request_queue[i].cylinder;
                    if (distance < closest_distance) {
                        closest_distance = distance;
                        index = i;
                    }
                }
            }
            if (index == -1) {
                for (int i = 0; i < *queue_cnt; i++) {
                    int distance = abs(request_queue[i].cylinder - target_cylinder);
                    if (distance < closest_distance) {
                        closest_distance = distance;
                        index = i;
                    }
                }
            }
        }
    }

    // After picking the RCB from the request queue, as described above, the method removes the RCB from the queue and returns it. 
    struct RCB next_request = request_queue[index];
    for (int i = index; i < *queue_cnt - 1; i++) {
        request_queue[i] = request_queue[i + 1];
    }
    (*queue_cnt)--;

    return next_request;
}