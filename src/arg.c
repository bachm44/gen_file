#include "arg.h"
#include <argp.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define ARGUMENT_MAX_SIZE 255

typedef enum bool {
  FALSE = 0,
  TRUE = 1,
} bool_t;

const char *argp_program_version = "gen_file 1.0.3";
const char *argp_program_bug_address = "bachm44@gmail.com";

static struct argp_option options[] = {
    {"verbose", 'v', 0, OPTION_ARG_OPTIONAL, "Verbose output", 0},
    {"seed", 'z', "SEED", OPTION_ARG_OPTIONAL,
     "Integer used for generating random file content", 0},
    {"size", 's', "SIZE", OPTION_ARG_OPTIONAL, "File size", 0},
    {0}};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;

  switch (key) {
  case 'v':
    arguments->verbose = 1;
    break;
  case 'z':
    if (arg == NULL) {
      argp_usage(state);
    }

    for (size_t i = 0; i < strlen(arg); ++i) {
      if (!isdigit(arg[i])) {
        argp_usage(state);
      }
    }

    arguments->seed = atoi(arg);
    break;
  case 's':
    arguments->size = arg;
    break;
  case ARGP_KEY_ARG:
    if (state->arg_num >= 4) {
      argp_usage(state);
    }
    arguments->filename = arg;
    break;
  case ARGP_KEY_END:
    if (state->arg_num < 1) {
      argp_usage(state);
    }
    break;
  default:
    return ARGP_ERR_UNKNOWN;
  }

  return EXIT_SUCCESS;
}

static const char args_doc[] = "FILENAME";

static const char doc[] =
    "gen_file -- A program to generate random file with fixed seed ";

static struct argp argp = {options, parse_opt, args_doc, doc, .children = NULL};

void validate(const struct arguments *arguments) {
  bool_t is_error = FALSE;

  if (strlen(arguments->filename) > ARGUMENT_MAX_SIZE) {
    fprintf(stderr, "Filename exceeds maximum length: %d\n", ARGUMENT_MAX_SIZE);
    is_error = TRUE;
  }

  if (strlen(arguments->size) > ARGUMENT_MAX_SIZE) {
    fprintf(stderr, "Size exceeds maximum length: %d\n", ARGUMENT_MAX_SIZE);
    is_error = TRUE;
  }

  if (arguments->seed < 0 || arguments->seed >= INT_MAX) {
    fprintf(stderr, "Seed should be a positive integer: %d\n", arguments->seed);
    is_error = TRUE;
  }

  if (is_error) {
    exit(EXIT_FAILURE);
  }
}

void parse(int argc, char *argv[], struct arguments *arguments) {
  argp_parse(&argp, argc, argv, 0, 0, arguments);
  validate(arguments);
}