install(
    TARGETS aoc_exe
    RUNTIME COMPONENT aoc_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
