cmake_minimum_required(VERSION 3.22)

project(GSF_FIGHTER VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- JUCE ----
# Set JUCE_DIR to your local JUCE installation, or use CPM/FetchContent
# Option 1: Local JUCE install
# set(JUCE_DIR "/path/to/JUCE")
# add_subdirectory(${JUCE_DIR} JUCE)

# Option 2: FetchContent (recommended for reproducible builds)
include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        8.0.4
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(JUCE)

# ---- Plugin target ----
juce_add_plugin(GSFFighter
    COMPANY_NAME               "GSF"
    PLUGIN_MANUFACTURER_CODE   GSFF
    PLUGIN_CODE                GsFt
    IS_SYNTH                   FALSE
    NEEDS_MIDI_INPUT           FALSE
    NEEDS_MIDI_OUTPUT          FALSE
    IS_MIDI_EFFECT             FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD    TRUE
    FORMATS                    VST3 AU AAX Standalone
    PRODUCT_NAME               "GSF FIGHTER"
    BUNDLE_ID                  "com.gsf.fighter"
    AAX_IDENTIFIER             "com.gsf.fighter"

    # Plugin characteristics
    VST3_CATEGORIES            "Analyzer" "Tools"
    AU_MAIN_TYPE               "kAudioUnitType_Effect"
    AAX_CATEGORY               "AAX_ePlugInCategory_None"
)

# ---- Source files ----
target_sources(GSFFighter PRIVATE
    # Core
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp

    # Network
    Source/Network/StreamServer.cpp
    Source/Network/DeviceDiscovery.cpp
    Source/Network/ClientSession.cpp

    # Presets / Translation EQ
    Source/Presets/TranslationEngine.cpp

    # Arcade UI
    Source/ArcadeUI/GSFLookAndFeel.cpp
    Source/ArcadeUI/HealthBarMeter.cpp
    Source/ArcadeUI/ArcadeButton.cpp
    Source/ArcadeUI/ConnectionPanel.cpp
    Source/ArcadeUI/PresetPanel.cpp
    Source/ArcadeUI/FightScreen.cpp
)

# ---- Include paths ----
target_include_directories(GSFFighter PRIVATE
    Source
    ../Shared
)

# ---- JUCE modules ----
target_compile_definitions(GSFFighter PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
)

target_link_libraries(GSFFighter
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
