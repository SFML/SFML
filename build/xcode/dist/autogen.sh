#!/bin/sh
cd ../../../

SFML_VERSION="1.6"
OS="macosx"

ROOT_DIR=`pwd`
XCODE_C_ROOT_DIR="$ROOT_DIR/CSFML/xcode"
XCODE_CXX_ROOT_DIR="$ROOT_DIR/build/xcode"
XCODE_SAMPLES_ROOT_DIR="$ROOT_DIR/samples/build/xcode"
XCODE_FRAMEWORKS_PROJECT="SFML.xcodeproj"
XCODE_LIBRARIES_PROJECT="SFML-bare.xcodeproj"
XCODE_SAMPLES_PROJECT="samples.xcodeproj"
XCODE_BUILD_STYLE="Release"
XCODE_C_TARGETS=("SFML" "csfml-system" "csfml-network" "csfml-audio" "csfml-window" "csfml-graphics")
XCODE_CXX_TARGETS=("SFML" "sfml-system" "sfml-network" "sfml-audio" "sfml-window" "sfml-graphics")
XCODE_SAMPLES_DIR="$ROOT_DIR/samples/build/xcode"

TARGET_C_FRAMEWORKS_DIR="$ROOT_DIR/CSFML/lib"
TARGET_CXX_FRAMEWORKS_DIR="$ROOT_DIR/lib"
TARGET_CXX_SAMPLES_DIR="$ROOT_DIR/samples/bin"
PACKAGES_ROOT_DIR="$ROOT_DIR/dist"
CXX_SDK_PACKAGE="SFML-$SFML_VERSION-sdk-$OS"
CXX_DEV_PACKAGE="SFML-$SFML_VERSION-dev-$OS"
C_SDK_PACKAGE="SFML-$SFML_VERSION-c-sdk-$OS"
C_DEV_PACKAGE="SFML-$SFML_VERSION-c-dev-$OS"
PACKAGES_CXX_SDK_DIR="$PACKAGES_ROOT_DIR/$CXX_SDK_PACKAGE"
PACKAGES_CXX_DEV_DIR="$PACKAGES_ROOT_DIR/$CXX_DEV_PACKAGE"
PACKAGES_C_SDK_DIR="$PACKAGES_ROOT_DIR/$C_SDK_PACKAGE"
PACKAGES_C_SDK_SUB_DIR="$PACKAGES_ROOT_DIR/$C_SDK_PACKAGE/CSFML"
PACKAGES_C_DEV_DIR="$PACKAGES_ROOT_DIR/$C_DEV_PACKAGE"
PACKAGES_C_DEV_SUB_DIR="$PACKAGES_ROOT_DIR/$C_DEV_PACKAGE/CSFML"

CXX_SDK_DIRS=("build" "src" "include" "lib" "extlibs" "samples")
CXX_DEV_DIRS=("lib" "extlibs" "build/xcode/templates")
C_SDK_DIRS=("src" "include" "extlibs")
C_SDK_SUB_DIRS=("CSFML/xcode" "CSFML/src" "CSFML/include" "CSFML/lib")
C_DEV_DIRS=("extlibs")
C_DEV_SUB_DIRS=("CSFML/xcode/templates" "CSFML/lib")


# Checks that last command ended normally. Prints an error message and exists if not.
check_last_process()
{
    if [ $? -eq 0 ]
      then
        if [ "$1" != "" ]
          then
            echo "$1"
        fi
    else
        echo " *** Last process did not end properly. Process stopped."
        exit 1
    fi
}

# User help
print_usage()
{
    echo "Usage: $0 [clean | build | build-samples | build-pkg | all]"
    echo "    Default shows this help"
    echo
    echo "    clean\t\tdeletes all the intermediates object files and built frameworks"
    echo "    build\t\tcompiles all the C and C++ SFML frameworks"
    echo "    build-samples\tcompiles the SFML samples"
    echo "    build-pkg\t\tbuilds the Disk Image Files to be distributed"
    echo "    all\t\t\tdoes build, build-samples and build-pkg"
}

# Checks that the user computer can run this script
check_config()
{
    if ! test -f "/Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp"
      then
        echo "Missing tool pbxcp needed by this script (should be located at /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp). Make sure Apple Developer Tools are correctly installed."
        exit 1
    fi
    
    if ! test -f "/usr/bin/xcodebuild"
      then
        echo "Missing tool xcodebuild needed by this script (should be located at /usr/bin/xcodebuild). Make sure Apple Developer Tools are correctly installed."
        exit 1
    fi
    
    sdk=`xcodebuild -showsdks | grep "Mac OS X 10.4"`
    if [ "$sdk" == "" ]
      then
        echo "Missing Mac OS X 10.4 SDK needed by SFML. Please install it."
        exit 1
    fi
}

# Makes a directory if needed
make_dir()
{
    if ! test -d "$1"
      then
        mkdir -p "$1"
    fi
}

# Checks that a directory exists and removes it
remove_dir()
{
    if test -d "$1"
      then
        rm -rfv "$1" > /dev/null
    fi
}

# Moves a directory, removing the destination before moving if already existing
move_dir()
{
    remove_dir "$2"
    check_last_process
    make_dir "$2/.."
    check_last_process
    mv "$1" "$2"
    check_last_process
}

copy()
{
    /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude .DS_Store -exclude .svn -exclude codeblocks -exclude *vc2005 -exclude *vc2008 -exclude *mingw -exclude Makefile -exclude Main -exclude "Template for new ports" -exclude Linux -exclude Win32 -exclude win32 -exclude *.build -exclude *.dll -exclude *.dSYM -exclude qt -exclude wxwidgets -exclude X11 -exclude lucas* -strip-debug-symbols -resolve-src-symlinks "$1" "$2"
}

# Delete built frameworks and intermediate object files
clean_all()
{
    cd "$ROOT_DIR"
    
    # Process cleaning for C++ project
    printf "Cleaning C++ products..."
    if test -d "$XCODE_CXX_ROOT_DIR/$XCODE_FRAMEWORKS_PROJECT"
      then
        cd "$XCODE_CXX_ROOT_DIR"
        xcodebuild -project "$XCODE_FRAMEWORKS_PROJECT" -target "All" -configuration "$XCODE_BUILD_STYLE" clean > /dev/null
        check_last_process " done"
    else
        echo "*** Missing file $XCODE_CXX_ROOT_DIR/$XCODE_FRAMEWORKS_PROJECT. Process stopped."
        exit 1
    fi
    
    # Process cleaning for C project
    printf "Cleaning C products..."
    if test -d "$XCODE_C_ROOT_DIR/$XCODE_FRAMEWORKS_PROJECT"
      then
        cd "$XCODE_C_ROOT_DIR"
        xcodebuild -project "$XCODE_FRAMEWORKS_PROJECT" -target "All" -configuration "$XCODE_BUILD_STYLE" clean > /dev/null
        check_last_process " done"
    else
        echo "*** Missing file $XCODE_C_ROOT_DIR/$XCODE_FRAMEWORKS_PROJECT. Process stopped."
        exit 1
    fi
    
    # Process cleaning for samples
    printf "Cleaning samples..."
    if test -d "$XCODE_SAMPLES_ROOT_DIR/$XCODE_SAMPLES_PROJECT"
      then
        cd "$XCODE_SAMPLES_ROOT_DIR"
        xcodebuild -project "$XCODE_SAMPLES_PROJECT" -target "All" -configuration "$XCODE_BUILD_STYLE" clean > /dev/null
        check_last_process " done"
    else
        echo "*** Missing file $XCODE_SAMPLES_ROOT_DIR/$XCODE_SAMPLES_PROJECT. Process stopped."
        exit 1
    fi
    
    # Process cleaning for packages
    printf "Removing packages..."
    remove_dir "$PACKAGES_ROOT_DIR"
    check_last_process " done"
}

# Build C and C++ frameworks
build_frameworks()
{
    # Go into the C++ project directory
    cd "$XCODE_CXX_ROOT_DIR"
    check_last_process
    echo "Building C++ SFML frameworks in $XCODE_BUILD_STYLE mode..."
    
    if test -d "$XCODE_CXX_ROOT_DIR/$XCODE_FRAMEWORKS_PROJECT"
      then
        # Build every C++ target (building manually each target to be able to show progress)
        for target in ${XCODE_CXX_TARGETS[@]}
          do
            printf "Building $target framework..."
            xcodebuild -project "$XCODE_FRAMEWORKS_PROJECT" -target "$target" -parallelizeTargets -configuration "$XCODE_BUILD_STYLE" build > /dev/null
            check_last_process " done"
        done
        
        echo "All C++ SFML frameworks built."
    else
        echo "*** Missing file $XCODE_CXX_ROOT_DIR/$XCODE_FRAMEWORKS_PROJECT. Process stopped."
        exit 1
    fi
    
    # Go into the C project directory
    cd "$XCODE_C_ROOT_DIR"
    check_last_process
    echo "Building C SFML frameworks in $XCODE_BUILD_STYLE mode..."
    
    if test -d "$XCODE_C_ROOT_DIR/$XCODE_FRAMEWORKS_PROJECT"
      then
        # Build every C target 
        for target in ${XCODE_C_TARGETS[@]}
          do
            printf "Building $target framework..."
            xcodebuild -project "$XCODE_FRAMEWORKS_PROJECT" -target "$target" -parallelizeTargets -configuration "$XCODE_BUILD_STYLE" build > /dev/null
            check_last_process " done"
        done
        
        echo "All C SFML frameworks built."
    else
        echo "*** Missing file $XCODE_C_ROOT_DIR/$XCODE_FRAMEWORKS_PROJECT. Process stopped."
        exit 1
    fi
}

# Build the samples
build_samples()
{
    # Go into the samples project directory
    cd "$XCODE_SAMPLES_DIR"
    check_last_process
    
    printf "Building SFML samples in $XCODE_BUILD_STYLE mode..."
    if test -d "$XCODE_SAMPLES_DIR/$XCODE_SAMPLES_PROJECT"
      then
        # Build the samples
        xcodebuild -project "$XCODE_SAMPLES_PROJECT" -target "All" -parallelizeTargets -configuration "$XCODE_BUILD_STYLE" build > /dev/null
        check_last_process " done"
        echo "In order to run the samples, the SFML frameworks (located in \"lib\") must be copied in the /Library/Frameworks directory." > "$TARGET_CXX_SAMPLES_DIR/README"
        check_last_process
    else
        echo "*** Missing file $XCODE_SAMPLES_DIR/$XCODE_SAMPLES_PROJECT. Process stopped."
        exit 1
    fi
}

# Build the disk image files of the C and C++ Dev and SDK packages
build_packages()
{
    cd "$ROOT_DIR"
    
    make_dir "$PACKAGES_ROOT_DIR"
    
    # Build the C++ SDK package
    printf "Building C++ SDK package..."
    make_dir "$PACKAGES_CXX_SDK_DIR"
    for dir in ${CXX_SDK_DIRS[@]}
      do
        copy "$ROOT_DIR/$dir" "$PACKAGES_CXX_SDK_DIR"
        check_last_process
    done
    
    # Build the disk image file
    hdiutil create -ov -format UDBZ -volname "$CXX_SDK_PACKAGE" -srcfolder "$PACKAGES_CXX_SDK_DIR" "$PACKAGES_ROOT_DIR/$CXX_SDK_PACKAGE.dmg" > /dev/null
    check_last_process " done"
    
    # Build the C++ Dev package
    printf "Building C++ Development package..."
    make_dir "$PACKAGES_CXX_DEV_DIR"
    for dir in ${CXX_DEV_DIRS[@]}
      do
        copy "$ROOT_DIR/$dir" "$PACKAGES_CXX_DEV_DIR"
        check_last_process
        
        case $dir in
            "build/xcode/templates")
                # Special consideration for the templates folder that is to be moved in build/xcode
                move_dir "$PACKAGES_CXX_DEV_DIR/templates" "$PACKAGES_CXX_DEV_DIR/build/xcode"
                check_last_process
                ;;
            "extlibs")
                # Drop the libs-xcode and headers directories
                remove_dir "$PACKAGES_CXX_DEV_DIR/extlibs/libs-xcode"
                check_last_process
                remove_dir "$PACKAGES_CXX_DEV_DIR/extlibs/headers"
                check_last_process
                ;;
            *)
                ;;
        esac
    done
    
    # Build the disk image file
    hdiutil create -ov -format UDBZ -volname "$CXX_DEV_PACKAGE" -srcfolder "$PACKAGES_CXX_DEV_DIR" "$PACKAGES_ROOT_DIR/$CXX_DEV_PACKAGE.dmg" > /dev/null
    check_last_process " done"
    
    
    # Build the C SDK package
    printf "Building C SDK package..."
    make_dir "$PACKAGES_C_SDK_DIR"
    for dir in ${C_SDK_DIRS[@]}
      do
        copy "$ROOT_DIR/$dir" "$PACKAGES_C_SDK_DIR"
        check_last_process
    done
    
    make_dir "$PACKAGES_C_SDK_SUB_DIR"
    for dir in ${C_SDK_SUB_DIRS[@]}
      do
        copy "$ROOT_DIR/$dir" "$PACKAGES_C_SDK_SUB_DIR"
        check_last_process
    done
    
    # Build the disk image file
    hdiutil create -ov -format UDBZ -volname "$C_SDK_PACKAGE" -srcfolder "$PACKAGES_C_SDK_DIR" "$PACKAGES_ROOT_DIR/$C_SDK_PACKAGE.dmg" > /dev/null
    check_last_process " done"
    
    # Build the C Development package
    printf "Building C Development package..."
    make_dir "$PACKAGES_C_DEV_DIR"
    for dir in ${C_DEV_DIRS[@]}
      do
        copy "$ROOT_DIR/$dir" "$PACKAGES_C_DEV_DIR"
        check_last_process
        
        case $dir in
            "extlibs")
                # Drop the libs-xcode and headers directories
                remove_dir "$PACKAGES_C_DEV_DIR/extlibs/libs-xcode"
                check_last_process
                remove_dir "$PACKAGES_C_DEV_DIR/extlibs/headers"
                check_last_process
                ;;
        esac
    done
    
    make_dir "$PACKAGES_C_DEV_SUB_DIR"
    for dir in ${C_DEV_SUB_DIRS[@]}
      do
        copy "$ROOT_DIR/$dir" "$PACKAGES_C_DEV_SUB_DIR"
        check_last_process
        
        case $dir in
            "CSFML/xcode/templates")
                # Special consideration for the templates folder that is to be moved in build/xcode
                move_dir "$PACKAGES_C_DEV_DIR/CSFML/templates" "$PACKAGES_C_DEV_DIR/CSFML/xcode"
                check_last_process
                ;;
        esac
    done
    
    # Build the disk image file
    hdiutil create -ov -format UDBZ -volname "$C_DEV_PACKAGE" -srcfolder "$PACKAGES_C_DEV_DIR" "$PACKAGES_ROOT_DIR/$C_DEV_PACKAGE.dmg" > /dev/null
    check_last_process " done"

}

main()
{
    # First make sure the user will be able to run the script
    check_config
    
    # Check which action the user wants to execute (default is help)
    action="help"
    if test "$1" != ""
      then
        action="$1"
    fi
    
    case $action in
        "clean")
            clean_all $*
            ;;
        "build")
            build_frameworks $*
            ;;
        "build-samples")
            build_samples $*
            ;;
        "build-pkg")
            build_packages $*
            ;;
        "all")
            build_frameworks $*
            build_samples $*
            build_packages $*
            ;;
         "-h")
            print_usage $*
            ;;
         "--help")
            print_usage $*
            ;;
         "help")
            print_usage $*
            ;;
        *)
            echo "Unknow action $action."
            print_usage $*
            ;;
    esac
}

main $*
