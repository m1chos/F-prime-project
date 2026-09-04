####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding files
# MOD_DEPS: (optional) module dependencies
# UT_SOURCE_FILES: list of source files for unit tests
#
####
set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/MotionSensor.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/MotionSensor.cpp"
)
if(FPRIME_ARDUINO)
target_use_arduino_libraries("FastIMU")
endif()

register_fprime_module()
