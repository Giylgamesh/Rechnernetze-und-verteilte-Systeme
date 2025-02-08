//
// Created by tinol on 07.02.2025.
//

#ifndef RN_PRAXIS_ZMQ_GENERICS_H
#define RN_PRAXIS_ZMQ_GENERICS_H

/**
* This header file contains the generics, constants and default values for the map reduce implementation
*/

#define RETURN_FAILURE -1

#define MAX_CHUNK_SIZE 1497
#define MAX_MSG_LENGTH 1500

typedef struct Worker {
    char *chunk;
    char *port;
} Worker;

typedef struct WorkerAdressList {
    char *address;
} WorkerAdressList;

typedef struct Request {
    char type[3];
    char *payload[MAX_MSG_LENGTH - 3];
} Request;

typedef struct {
    char word[MAX_MSG_LENGTH];
    int count;
} WordCount;

typedef struct {
    char word[MAX_CHUNK_SIZE];
    int frequency;
} WordFreq;

#endif //RN_PRAXIS_ZMQ_GENERICS_H