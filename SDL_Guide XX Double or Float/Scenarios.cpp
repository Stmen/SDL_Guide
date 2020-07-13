#include <SDL.h>
#include "Scenarios.h"
#include <stdio.h>

int ScenarioThread(void* ptr)
{
    if (ptr == NULL) return -1;
    SCENARIO_INFO* info = (SCENARIO_INFO*)ptr;

    int32_t integer_result = 0;
    float float_result = 0.0;
    double double_result = 0.0;

    int32_t integer_values[2] = { (int32_t)info->values[0], (int32_t)info->values[1] };
    float float_values[2] = { (float)info->values[0], (float)info->values[1] };
    double double_values[2] = { info->values[0], info->values[1] };

    switch (info->type) {
    case SCENARIO_TYPE_INTEGER:
        switch (info->operation) {
        case SCENARIO_OP_MULTIPLY:
        {
            Uint64 start = SDL_GetPerformanceCounter();
            for (Uint64 i = 0; i < info->iterations; i++) integer_result = integer_values[0] * integer_values[1];
            info->duration = SDL_GetPerformanceCounter() - start;
        }
        break;
        case SCENARIO_OP_DIVIDE:
        {
            Uint64 start = SDL_GetPerformanceCounter();
            for (Uint64 i = 0; i < info->iterations; i++) integer_result = integer_values[0] / integer_values[1];
            info->duration = SDL_GetPerformanceCounter() - start;
        }
        break;
        case SCENARIO_OP_ADD:
        {
            Uint64 start = SDL_GetPerformanceCounter();
            for (Uint64 i = 0; i < info->iterations; i++) integer_result = integer_values[0] + integer_values[1];
            info->duration = SDL_GetPerformanceCounter() - start;
        }
        break;
        case SCENARIO_OP_SUBTRACT:
        {
            Uint64 start = SDL_GetPerformanceCounter();
            for (Uint64 i = 0; i < info->iterations; i++) integer_result = integer_values[0] - integer_values[1];
            info->duration = SDL_GetPerformanceCounter() - start;
        }
        break;
        }
        break;
    case SCENARIO_TYPE_DOUBLE:
        switch (info->operation) {
        case SCENARIO_OP_MULTIPLY:
        {
            Uint64 start = SDL_GetPerformanceCounter();
            for (Uint64 i = 0; i < info->iterations; i++) double_result = double_values[0] * double_values[1];
            info->duration = SDL_GetPerformanceCounter() - start;
        }
        break;
        case SCENARIO_OP_DIVIDE:
        {
            Uint64 start = SDL_GetPerformanceCounter();
            for (Uint64 i = 0; i < info->iterations; i++) double_result = double_values[0] / double_values[1];
            info->duration = SDL_GetPerformanceCounter() - start;
        }
        break;
        case SCENARIO_OP_ADD:
        {
            Uint64 start = SDL_GetPerformanceCounter();
            for (Uint64 i = 0; i < info->iterations; i++) double_result = double_values[0] + double_values[1];
            info->duration = SDL_GetPerformanceCounter() - start;
        }
        break;
        case SCENARIO_OP_SUBTRACT:
        {
            Uint64 start = SDL_GetPerformanceCounter();
            for (Uint64 i = 0; i < info->iterations; i++) double_result = double_values[0] - double_values[1];
            info->duration = SDL_GetPerformanceCounter() - start;
        }
        break;
        }
        break;
    case SCENARIO_TYPE_FLOAT:
        switch (info->operation) {
        case SCENARIO_OP_MULTIPLY:
        {
            Uint64 start = SDL_GetPerformanceCounter();
            for (Uint64 i = 0; i < info->iterations; i++) float_result = float_values[0] * float_values[1];
            info->duration = SDL_GetPerformanceCounter() - start;
        }
        break;
        case SCENARIO_OP_DIVIDE:
        {
            Uint64 start = SDL_GetPerformanceCounter();
            for (Uint64 i = 0; i < info->iterations; i++) float_result = float_values[0] / float_values[1];
            info->duration = SDL_GetPerformanceCounter() - start;
        }
        break;
        case SCENARIO_OP_ADD:
        {
            Uint64 start = SDL_GetPerformanceCounter();
            for (Uint64 i = 0; i < info->iterations; i++) float_result = float_values[0] + float_values[1];
            info->duration = SDL_GetPerformanceCounter() - start;
        }
        break;
        case SCENARIO_OP_SUBTRACT:
        {
            Uint64 start = SDL_GetPerformanceCounter();
            for (Uint64 i = 0; i < info->iterations; i++) float_result = float_values[0] - float_values[1];
            info->duration = SDL_GetPerformanceCounter() - start;
        }
        break;
        }
        break;
    }

    return 0;
}

const char* scenario_type_to_str(SCENARIO_TYPE type)
{
    switch (type) {
    case SCENARIO_TYPE_INTEGER: return "Integer";
    case SCENARIO_TYPE_DOUBLE: return "Double";
    case SCENARIO_TYPE_FLOAT: return "Float";
    }

    return "<Not Implemented>";
}

const char* scenario_op_to_str(SCENARIO_OPERATION op)
{
    switch (op) {
    case SCENARIO_OP_MULTIPLY: return "Multiply";
    case SCENARIO_OP_DIVIDE: return "Divide";
    case SCENARIO_OP_ADD: return "Add";
    case SCENARIO_OP_SUBTRACT: return "Subtract";
    }

    return "<Not Implemented>";
}