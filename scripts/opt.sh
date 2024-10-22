#!/bin/bash
 opt -passes="instrumentation" --load-pass-plugin=${INSTRUMENTATION_LIB_DIR}/libInstrumentation.so $@
