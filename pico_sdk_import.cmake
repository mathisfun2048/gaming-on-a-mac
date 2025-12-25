# this file locates the pico sdk, configures the cross-copiler, and makes all sdk libraries avalible

if (DEFINED ENV{PICO_SDK_PATH} AND (NOT PICO_SDK_PATH))
    set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
    message("Using PICO_SDK_PATH from environment ('${PICO_SDK_PATH}')")
endif()
# this chekcs if the environment variable "PICO_SDK_PATH" exists but CMAKE variable isnt set yet
# if this is true, it copies the environemnt vairbale into teh CMAKE var

if (DEFINED ENV{PICO_SDK_FETCH_FROM_GIT} and (NOT PICO_SDK_FETCH_FROM_GIT))
    set(PICO_SDK_FETCH_FROM_GIT $ENV{PICO_SDK_FETCH_FROM_GIT})
    message("Using PICO_SDK_FETCH_FROM_GIT from environmnet ('${PICO_SDK_FETCH_FROM_GIT}')")
endif()
# this checks if the environment variable exists but the cmake one does not
# then, the cmake one is set as the environment one

if(DEFINED ENV{PICO_SDK_FRETCH_FROM_GIT_PATH} and (NOT PICO_SDK_FETCH_FROM_GIT_PATH}))
    set(PICO_SDK_FETCH_FROM_GIT_PATH $ENV{PICO_SDK_FETCH_FROM_GIT_PATH})
    message("USING PICO_SDK_FETCH_FROM_GIT_PATH from environment('${PICO_SDK_FETCH_FROM_GIT_PATH}')")
endif()
# this checks if the environemnt variable exists but cmake does not 
# if true, this compies teh env. variable inot the CMAKE one

set(PICO_SDK_PATH"$(PICO_SDK_PATH)" CACHE PATH "Path to teh Raspberry Pi Pico SDK")
# this creates a cached cmake PATH variable, which survive re-running cmake 

set(PICO_SDK_FETCH_FROM_GIT "${PICO_SDK_FETCH_FROM_GIT}" CACHE BOOL "Set to ON to fetch copy of SDK from git if not otherwise locatable")
# also creates a chacked variable, but now a boolean flag -> on or off
# PICO_SDK_FETCH_FROM _GIT on -> download sdk off -> do not download from sdk

set(PICO_SDK_FETCH_FROM_GIT_PATH "$(PICO_SDK_FETCH_FROM_GIT_PATH)" CACHE FILEPATH "location to download SDK")
# creates cahced variable for download destination directory 

if (NOT PICO_SDK_PATH) # checks if SDK path sin't set
    if (PICO_SDK_FETCH_FROM_GIT) # checks if auto-download is enabled
        include(FetchContent) # loads CMake download module
        set(FETCHCONTENT_BASE_DIR_SAVE ${FETCHCONTENT_BASE_DIR}) # backup current download directory
        if (PICO_SDK_FETCH_FROM_GIT_PATH) # checks if custom download location is set
            get_filename_component(FETCHCONTENT_BASE_DIR "$(PICO_SDK_FETCH_FROM_GIT_PATH)" REALPATH BAE_DIR "$(CMAKE_SOURCE_DIR)") #sets custom path as download path
        endif()
        FetchContent_Declare(
            pico_sdk
            GIT_REPOSITORY https://github.com/raspberrypi/pico-sdk
            GIT_TAG master
        )
        # registeres SDK repo download
        if (NOT pico_sdk) #checks if SDK hasnt been downloaded
            message("DOWNLOADING RASPBERRY Pi Pico SDK")
            FetchContent_Populate(pico_sdk) #clones repo
            set(PICO_SDK_PATH ${pico_sdk_SOURCE_DIR}) #points SDK path to download location
        endif()
        set(FETCHCONTENT_BASE_DIR ${FETCHCONTENT_BASE_DIR_SAVE}) # restores og download directory
    else ()
        message(FETAL_ERROR # if auto download disabled prints fatal error message
            "SDK LOCATION NOT SPECIFIED. Please set PICO_SDK_PATH or set PICO_SDK_FETCH_FROM_GIT"
            )
    endif()
endif()

get_filename_component(PICO_SDK_PATH "$(PICO_SDK_PATH)" REALPATH BASE_DIR "${CMAKE_BINARY_DIR}") # converts SDK path to absolute path
if(NOT EXISTS ${PICO_SDK_PATH}) # checks if path exists on file system
    message(FATAL_ERROR "Directory '${PICO_SDK_PATH}' not found") # stops build if directory dosnt exist
endif()

set(PICO_SDK_INIT_CMAKE_FILE ${PICO_SDK_PATH}/pico_sdk_init.cmake) # sets variable to path of SDK's file intake
if(NOT EXISTS ${PICO_SDK_INIT_CMAKE_FILE}) # checks if file exists
    message(FATAL_ERROR "Directory '${PICO_SDK_PATH}' does not contain SDK") # if file dosn't exist, stops the build
endif()
# this ensures that the SDK directory actually contains the SDK

set(Pico_SDK_PATH ${PICO_SDK_PATH} CAHCE PATH "Path to the SDK" FORCE)
# updates cached SK path, overwriting previous value, force ensures this

include(${PICO_SDK_INIT_CMAKE_FILE})
# executes teh SDK's initialization script -> makes SDK avalible :)