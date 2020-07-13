#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <inttypes.h>
#include "Scenarios.h"

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64 1
#define ENVIRONMENT32 0
#else
#define ENVIRONMENT32 1
#define ENVIRONMENT64 0
#endif
#ifdef _DEBUG
#define CONFIGURATION_DEBUG 1
#define CONFIGURATION_RELEASE 0
#else
#define CONFIGURATION_RELEASE 1
#define CONFIGURATION_DEBUG 0
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64 1
#define ENVIRONMENT32 0
#else
#define ENVIRONMENT32 1
#define ENVIRONMENT64 0
#endif
#ifdef NDEBUG
#define CONFIGURATION_RELEASE 1
#define CONFIGURATION_DEBUG 0
#else
#define CONFIGURATION_DEBUG 1
#define CONFIGURATION_RELEASE 0
#endif
#endif

void printsep(int length);
void printsepd();

#define STRING_MAX_LEN 256
const size_t SCENARIO_COUNT_OVERRIDE = 0;
const uint64_t SCENARIO_ITERATIONS = 250000000;
const unsigned PRERUN_COUNT = 5;

int main(int argc, char* argv[]) {

    size_t scenario_count = SCENARIO_COUNT_OVERRIDE ? SCENARIO_COUNT_OVERRIDE : (size_t)SDL_GetCPUCount();

    srand((unsigned)time(NULL));
    float test_values[2] = { (float)(rand() % 10000 + 1), (float)(rand() % 10000 + 1) };

    printf("SDL_Guide Doubles Versus Floats (Configuration: %s, Architecture: %s)\n", CONFIGURATION_DEBUG ? "Debug" : "Release", ENVIRONMENT64 ? "x64" : "x86");
    printf("Scenario (thread) count set to: %zd\n", scenario_count);
    printf("Scenario iterations set to: %zd\n", SCENARIO_ITERATIONS);

    char* results[SCENARIO_OP_LAST * SCENARIO_TYPE_LAST] = { 0 };
    for (size_t i = 0; i < SCENARIO_OP_LAST * SCENARIO_TYPE_LAST; i++)
#ifdef __cplusplus
        results[i] = new char[STRING_MAX_LEN];
#else
        results[i] = malloc(STRING_MAX_LEN);
#endif

    for (unsigned prerun = 0; prerun < PRERUN_COUNT + 1; prerun++) {
        int is_prerun = prerun < PRERUN_COUNT;
        if (is_prerun) printf("Prerunning to throttle CPU up: %u\n", PRERUN_COUNT - prerun);
        else printf("Prerun finished, starting tests...\n");

        for (int selected_operation = 0, current_result = 0; selected_operation < SCENARIO_OP_LAST; selected_operation++) {
            for (int selected_type = 0; selected_type < SCENARIO_TYPE_LAST; selected_type++, current_result++) {
                if (!is_prerun) {
                    printsepd();
                    printf("%s (%s) Scenarios\n",
                        scenario_op_to_str((SCENARIO_OPERATION)selected_operation),
                        scenario_type_to_str((SCENARIO_TYPE)selected_type));
                    printsepd();
                }
#ifdef __cplusplus
                SCENARIO_INFO* scenarios = new SCENARIO_INFO[scenario_count];
#else
                SCENARIO_INFO* scenarios = malloc(sizeof(SCENARIO_INFO) * scenario_count);
#endif
                if (!is_prerun) printf("Started %zd threads: ", scenario_count);
                char threadName[24] = { 0 };
                for (size_t i = 0; i < scenario_count; i++) {
                    snprintf(threadName, 24, "T%d", i);

                    scenarios[i].duration = 0;
                    scenarios[i].iterations = SCENARIO_ITERATIONS;
                    scenarios[i].values[0] = test_values[0];
                    scenarios[i].values[1] = test_values[1];
                    scenarios[i].type = (SCENARIO_TYPE)selected_type;
                    scenarios[i].operation = (SCENARIO_OPERATION)selected_operation;
                    scenarios[i].thread = SDL_CreateThread(ScenarioThread, threadName, &scenarios[i]);

                    if (!is_prerun) printf("%s%s", i ? ", " : "", threadName);
                }

                if (!is_prerun) printf("\n");

                int sepwidth = 10;
                if (!is_prerun) {
                    sepwidth = printf("| %-11s | %14s |\n", "Thread Name", "Duration (ms)");
                    printsep(sepwidth - 1); // -2 for pipe and newline
                }
                for (size_t i = 0; i < scenario_count; i++) {
                    SDL_strlcpy(threadName, SDL_GetThreadName(scenarios[i].thread), 24);
                    SDL_WaitThread(scenarios[i].thread, NULL);
                    if (!is_prerun) printf("| %-11s | %14.3f |\n", threadName,
                        (scenarios[i].duration / (double)SDL_GetPerformanceFrequency()) * 1000.0);
                }
                if (!is_prerun) printsep(sepwidth - 1); // -2 for pipe and newline

                // Calculate average:
                double average = 0.0;
                for (size_t i = 0; i < scenario_count; i++)
                    average += (scenarios[i].duration / (double)SDL_GetPerformanceFrequency()) * 1000.0;
                average = average / scenario_count;

                if (!is_prerun) {
                    //printf("Average duration: %f milliseconds\n", average);
                    char tmp[STRING_MAX_LEN / 2] = { 0 };
                    snprintf(tmp, STRING_MAX_LEN / 2, "Average duration for %s %s:",
                        scenario_op_to_str((SCENARIO_OPERATION)selected_operation),
                        scenario_type_to_str((SCENARIO_TYPE)selected_type));
                    snprintf(results[current_result], STRING_MAX_LEN, "%-40s %10f ms\n", tmp, average);
                    printf("\n");
                }

#ifdef __cplusplus
                delete[] scenarios;
#else
                free(scenarios);
#endif
            }
        }
    }

    printf("\n");
    for (size_t i = 0; i < SCENARIO_OP_LAST * SCENARIO_TYPE_LAST; i++) {
        printf(results[i]);
#ifdef __cplusplus
        delete[] results[i];
#else
        free(results[i]);
#endif
    }

    return 0;
}

void printsep(int length)
{
    for (int i = 0; i < length; i++) { printf("-"); }
    printf("\n");
}

void printsepd()
{
    printsep(79);
}