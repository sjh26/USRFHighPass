cmake_minimum_required(VERSION 2.8.9)

project(USRFHighPass)

#-----------------------------------------------------------------------------
# Extension meta-information
set(EXTENSION_HOMEPAGE "http://slicer.org/slicerWiki/index.php/Documentation/Nightly/Extensions/USRFHighPass")
set(EXTENSION_CATEGORY "Ultrasound")
set(EXTENSION_CONTRIBUTORS "Sam Horvath (Kitware Inc)")
set(EXTENSION_DESCRIPTION "This module applies a high pass filter designed to remove noise from US RF images created by the Interson Array Linear probe.")
set(EXTENSION_ICONURL "http://www.example.com/Slicer/Extensions/USRFHighPass.png")
set(EXTENSION_SCREENSHOTURLS "http://www.example.com/Slicer/Extensions/USRFHighPass/Screenshots/1.png")
set(EXTENSION_DEPENDS "NA") # Specified as a space separated string, a list or 'NA' if any

#-----------------------------------------------------------------------------
# Extension dependencies
find_package(Slicer REQUIRED)
include(${Slicer_USE_FILE})

#-----------------------------------------------------------------------------
# Extension modules
add_subdirectory(USRFHighPassFilter)
## NEXT_MODULE

#-----------------------------------------------------------------------------
include(${Slicer_EXTENSION_CPACK})
