project(G15-ping)
cmake_minimum_required(VERSION 2.8.8)

set(Program_SOURCES src/G15-ping.cpp src/SDK/EZ_LCD.cpp src/SDK/EZ_LCD_Page.cpp src/SDK/LCDGUI/LCDAnimatedBitmap.cpp src/SDK/LCDGUI/LCDBase.cpp src/SDK/LCDGUI/LCDBitmap.cpp src/SDK/LCDGUI/LCDCollection.cpp src/SDK/LCDGUI/LCDGfx.cpp src/SDK/LCDGUI/LCDIcon.cpp src/SDK/LCDGUI/LCDManager.cpp src/SDK/LCDGUI/LCDOutput.cpp src/SDK/LCDGUI/LCDProgressBar.cpp src/SDK/LCDGUI/LCDScrollingText.cpp src/SDK/LCDGUI/LCDStreamingText.cpp src/SDK/LCDGUI/LCDText.cpp)

include_directories(include include/SDK include/SDK/LCDGUI)

add_executable(${PROJECT_NAME} ${Program_SOURCES})

target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/SDKLibs/x64/lgLcd.lib ${CMAKE_SOURCE_DIR}/SDKLibs/x86/lgLcd.lib)

set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
