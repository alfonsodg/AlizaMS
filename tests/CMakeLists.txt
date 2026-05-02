cmake_minimum_required(VERSION 3.11.0)

# Support standalone build (for CI unit-tests job)
if(NOT CMAKE_PROJECT_NAME OR CMAKE_PROJECT_NAME STREQUAL "alizams_tests")
  project(alizams_tests CXX)
  enable_testing()
endif()

set(TEST_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/test_structures.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/test_buffer_sizes.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/test_lut_pipeline.cpp)

# DICOM parsing tests require mdcm (only available in full project build)
if(CMAKE_PROJECT_NAME AND NOT CMAKE_PROJECT_NAME STREQUAL "alizams_tests")
  list(APPEND TEST_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/test_dicom_parsing.cpp)
endif()

# GSDF tests require common/gsdf.h (only available in full project build)
if(EXISTS "${CMAKE_SOURCE_DIR}/common/gsdf.h"
   AND CMAKE_PROJECT_NAME
   AND NOT CMAKE_PROJECT_NAME STREQUAL "alizams_tests")
  list(APPEND TEST_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/test_gsdf.cpp)
endif()

add_executable(alizams_tests ${TEST_SRCS})

target_include_directories(alizams_tests PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/common
  ${CMAKE_SOURCE_DIR}/mdcm/Source/Common
  ${CMAKE_SOURCE_DIR}/mdcm/Source/DataDictionary
  ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition
  ${CMAKE_SOURCE_DIR}/mdcm/Source/MediaStorageAndFileFormat
  ${CMAKE_BINARY_DIR}/mdcm/Source/Common)

# Link mdcm sources for DICOM tests (when building as part of full project)
if(CMAKE_PROJECT_NAME AND NOT CMAKE_PROJECT_NAME STREQUAL "alizams_tests")
  # Minimal mdcm sources needed for DICOM parsing tests
  target_sources(alizams_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/mdcm/Source/Common/mdcmVersion.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/Common/mdcmEvent.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/Common/mdcmSubject.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/Common/mdcmSwapCode.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/Common/mdcmSystem.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/Common/mdcmFilename.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/Common/mdcmFilenameGenerator.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/Common/mdcmBase64.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/Common/mdcmSHA1.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/Common/mdcmCryptoFactory.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/Common/mdcmASN1.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/Common/mdcmUnpacker12Bits.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmTag.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmVR.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmVM.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmTransferSyntax.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmMediaStorage.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmByteValue.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmValue.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmDataElement.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmDataSet.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmPreamble.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmFileMetaInformation.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmFile.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmExplicitDataElement.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmImplicitDataElement.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmSequenceOfItems.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmSequenceOfFragments.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmFragment.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmCodeString.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmPrivateTag.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmReader.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmWriter.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmCSAHeader.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmMrProtocol.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmByteSwapFilter.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmUNExplicitDataElement.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmCP246ExplicitDataElement.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmExplicitImplicitDataElement.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataStructureAndEncodingDefinition/mdcmVR16ExplicitDataElement.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataDictionary/mdcmDicts.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataDictionary/mdcmDefaultDicts.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataDictionary/mdcmPrivateDefaultDicts.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataDictionary/mdcmGlobal.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataDictionary/mdcmUIDs.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/DataDictionary/mdcmCSAHeaderDefaultDicts.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/MediaStorageAndFileFormat/mdcmPixelFormat.cxx
    ${CMAKE_SOURCE_DIR}/mdcm/Source/MediaStorageAndFileFormat/mdcmPhotometricInterpretation.cxx)
endif()

target_compile_features(alizams_tests PUBLIC cxx_std_17)

add_test(NAME alizams_tests COMMAND alizams_tests)
