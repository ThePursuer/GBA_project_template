file(GLOB WAV_MOD_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.wav" "${CMAKE_CURRENT_SOURCE_DIR}/*.mod")

set(MMOUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/.dkp-generated/soundbank.bin")
set(MMHEADER "${CMAKE_CURRENT_SOURCE_DIR}/.dkp-generated/soundbank.h")
set(AUDIOOBJECT "${CMAKE_CURRENT_SOURCE_DIR}/.dkp-generated/soundbank_bin/soundbank_bin.s")
set(AUDIOHEADER "${CMAKE_CURRENT_SOURCE_DIR}/.dkp-generated/soundbank_bin/soundbank_bin.h")

# Create a placeholder file for soundbank.bin to ensure it exists
if (NOT EXISTS ${MMOUTPUT})
  file(WRITE ${MMOUTPUT} "")
endif()

mm_add_soundbank_target(
    soundbank
    ${WAV_MOD_FILES}
    OUTPUT ${MMOUTPUT}
    HEADER ${MMHEADER}
)

dkp_add_embedded_binary_library(
    soundbank_bin
    ${MMOUTPUT}
)

add_dependencies(soundbank_bin soundbank)

install(FILES ${AUDIOOBJECT}
    DESTINATION lib)
install(FILES ${MMHEADER} ${AUDIOHEADER}
        DESTINATION include)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${MMHEADER};${AUDIOHEADER};${AUDIOOBJECT};${MMOUTPUT}")