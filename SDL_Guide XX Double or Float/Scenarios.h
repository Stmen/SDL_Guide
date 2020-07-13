#pragma once

typedef enum {
    SCENARIO_OP_MULTIPLY,
    SCENARIO_OP_DIVIDE,
    SCENARIO_OP_ADD,
    SCENARIO_OP_SUBTRACT,
    SCENARIO_OP_LAST
} SCENARIO_OPERATION;

typedef enum {
    SCENARIO_TYPE_INTEGER,
    SCENARIO_TYPE_DOUBLE,
    SCENARIO_TYPE_FLOAT,
    SCENARIO_TYPE_LAST
} SCENARIO_TYPE;

typedef struct {
    SDL_Thread* thread;
    SCENARIO_OPERATION operation;
    SCENARIO_TYPE type;
    Uint64 iterations;
    double values[2];
    Uint64 duration;
} SCENARIO_INFO;

int ScenarioThread(void* ptr);

const char* scenario_type_to_str(SCENARIO_TYPE type);
const char* scenario_op_to_str(SCENARIO_OPERATION type);
