cmake_minimum_required(VERSION 3.22)
project(AILS_Filter_HP VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  JUCE
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  GIT_TAG 8.0.8
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(AILS_Filter_HP
    COMPANY_NAME "AILS Audio"
    BUNDLE_ID "com.ailsaudio.ailsfilterhp"
    PLUGIN_MANUFACTURER_CODE Ails
    PLUGIN_CODE Ahpf
    FORMATS VST3
    PRODUCT_NAME "AILS Filter HP"
    VST3_CATEGORIES Fx Filter
    COPY_PLUGIN_AFTER_BUILD FALSE
)

target_sources(AILS_Filter_HP PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
)

target_compile_definitions(AILS_Filter_HP PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(AILS_Filter_HP PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
