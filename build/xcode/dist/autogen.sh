#!/bin/sh

############################################################
#
#  SFML - Simple and Fast Multimedia Library
#  Copyright (C) 2007-2009 Lucas Soltic (ceylow@gmail.com) and Laurent Gomila (laurent.gom@gmail.com)
#
#  This software is provided 'as-is', without any express or implied warranty.
#  In no event will the authors be held liable for any damages arising from the use of this software.
#
#  Permission is granted to anyone to use this software for any purpose,
#  including commercial applications, and to alter it and redistribute it freely,
#  subject to the following restrictions:
#
#  1. The origin of this software must not be misrepresented;
#     you must not claim that you wrote the original software.
#     If you use this software in a product, an acknowledgment
#     in the product documentation would be appreciated but is not required.
#
#  2. Altered source versions must be plainly marked as such,
#     and must not be misrepresented as being the original software.
#
#  3. This notice may not be removed or altered from any source distribution.
#
############################################################


cd ../../../

SFML_VERSION="1.6"
OS="macosx"

ROOT_DIR=`pwd`
XCODE_C_ROOT_DIR="$ROOT_DIR/CSFML/xcode"
XCODE_CXX_ROOT_DIR="$ROOT_DIR/build/xcode"
XCODE_SAMPLES_ROOT_DIR="$ROOT_DIR/samples/build/xcode"
XCODE_FRAMEWORKS_PROJECT="SFML.xcodeproj"
XCODE_64B_FRAMEWORKS_PROJECT="SFML with Intel 64 bits.xcodeproj"
XCODE_LIBRARIES_PROJECT="SFML-bare.xcodeproj"
XCODE_SAMPLES_PROJECT="samples.xcodeproj"
XCODE_64B_SAMPLES_PROJECT="samples with Intel 64 bits.xcodeproj"
XCODE_BUILD_STYLE="Release"
XCODE_C_TARGETS=("SFML" "csfml-system" "csfml-network" "csfml-audio" "csfml-window" "csfml-graphics")
XCODE_CXX_TARGETS=("SFML" "sfml-system" "sfml-network" "sfml-audio" "sfml-window" "sfml-graphics")
XCODE_ALL_TARGETS=("All")


# Architectures
ARCH_32B="ub32"			# ppc + i386
ARCH_3264B="ub32+64"	# ppc + i386 + x86_64
ARCH_64B="$ARCH_3264B"	# same as 3264B for now

C_FRAMEWORKS_DIR="$ROOT_DIR/CSFML/lib"				# target directory for C frameworks
CXX_FRAMEWORKS_DIR="$ROOT_DIR/lib"					# target directory for C++ frameworks
CXX_32B_FRAMEWORKS_DIR="$ROOT_DIR/CSFML/lib64"		# target directory for '32b' C++ frameworks
CXX_64B_FRAMEWORKS_DIR="$ROOT_DIR/lib64"			# target directory for '3264b' C++ frameworks
CXX_SAMPLES_DIR="$ROOT_DIR/samples/bin"				# target directory for C++ samples
PACKAGES_ROOT_DIR="$ROOT_DIR/dist"					# target directory for '32b' packages
PACKAGES_ROOT_DIR_64B="$ROOT_DIR/dist64"			# target directory for '64b' packages

# Packages names (default, 32 bits, 64 bits)
CXX_SDK_PACKAGE="SFML-$SFML_VERSION-sdk-$OS-$ARCH_32B"
CXX_DEV_PACKAGE="SFML-$SFML_VERSION-dev-$OS-$ARCH_32B"
C_SDK_PACKAGE="SFML-$SFML_VERSION-c-sdk-$OS-$ARCH_32B"
C_DEV_PACKAGE="SFML-$SFML_VERSION-c-dev-$OS-$ARCH_32B"

CXX_SDK_PACKAGE_32B="$CXX_SDK_PACKAGE"
CXX_DEV_PACKAGE_32B="$CXX_DEV_PACKAGE"
C_SDK_PACKAGE_32B="$C_SDK_PACKAGE"
C_DEV_PACKAGE_32B="$C_DEV_PACKAGE"

CXX_SDK_PACKAGE_64B="SFML-$SFML_VERSION-sdk-$OS-$ARCH_64B"
CXX_DEV_PACKAGE_64B="SFML-$SFML_VERSION-dev-$OS-$ARCH_64B"
C_SDK_PACKAGE_64B="SFML-$SFML_VERSION-c-sdk-$OS-$ARCH_64B"
C_DEV_PACKAGE_64B="SFML-$SFML_VERSION-c-dev-$OS-$ARCH_64B"

# Package directories
CXX_SDK_PACKAGE_DIR="$PACKAGES_ROOT_DIR/$CXX_SDK_PACKAGE"
CXX_DEV_PACKAGE_DIR="$PACKAGES_ROOT_DIR/$CXX_DEV_PACKAGE"
C_SDK_PACKAGE_DIR="$PACKAGES_ROOT_DIR/$C_SDK_PACKAGE"
C_SDK_PACKAGE_SUB_DIR="$PACKAGES_ROOT_DIR/$C_SDK_PACKAGE/CSFML"
C_DEV_PACKAGE_DIR="$PACKAGES_ROOT_DIR/$C_DEV_PACKAGE"
C_DEV_PACKAGE_SUB_DIR="$PACKAGES_ROOT_DIR/$C_DEV_PACKAGE/CSFML"
PACKAGES_INFO_FILES_DIR="$XCODE_CXX_ROOT_DIR/dist"

CXX_SDK_PACKAGE_DIR_64B="$PACKAGES_ROOT_DIR_64B/$CXX_SDK_PACKAGE_64B"
CXX_DEV_PACKAGE_DIR_64B="$PACKAGES_ROOT_DIR_64B/$CXX_DEV_PACKAGE_64B"
C_SDK_PACKAGE_DIR_64B="$PACKAGES_ROOT_DIR_64B/$C_SDK_PACKAGE_64B"
C_DEV_PACKAGE_DIR_64B="$PACKAGES_ROOT_DIR_64B/$C_DEV_PACKAGE_64B"

CXX_SDK_DIRS=("build" "src" "include" "lib" "extlibs" "samples" "doc")
CXX_64B_SDK_DIRS=("build" "src" "include" "lib64" "extlibs" "samples" "doc")
CXX_DEV_DIRS=("lib" "extlibs" "build/xcode/templates")
CXX_64B_DEV_DIRS=("lib64" "extlibs" "build/xcode/templates")
C_SDK_DIRS=("src" "include" "extlibs")
C_SDK_SUB_DIRS=("CSFML/xcode" "CSFML/src" "CSFML/include" "CSFML/lib")
C_DEV_DIRS=("extlibs")
C_DEV_SUB_DIRS=("CSFML/xcode/templates" "CSFML/lib")
PACKAGE_INFO_FILES=("Read Me.rtf" "Release Notes.rtf" "Notes de version.rtf" "license.txt" "Lisez-moi.rtf")


# Build detection
SHOULD_CONSIDER_64B="yes"
SHOULD_CONSIDER_CXX="yes"
SHOULD_CONSIDER_CXX_64B="yes"
SHOULD_CONSIDER_C="yes"
SHOULD_CONSIDER_C_64B="yes"
SHOULD_CONSIDER_SAMPLES="yes"
SHOULD_CONSIDER_SAMPLES_64B="yes"

VERBOSE_OUTPUT="/dev/null"

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
        echo "*** Last process did not end properly. Process stopped."
        echo "*** Error occured when executed from `pwd`"
        exit 1
    fi
}

# User help
print_usage()
{
    echo "Usage: $0 [clean[-32|-64] | build[-32|-64] | build-samples[-32|-64] | build-pkg[-32|-64] | all[-32|-64] [--verbose]]"
    echo
    echo "Commands:"
    echo "    clean\t\tdeletes the C and C++ frameworks, object files and packages"
    echo "    build\t\tcompiles all the C and C++ SFML frameworks"
    echo "    build-samples\tcompiles the SFML samples"
    echo "    build-pkg\t\tbuilds the packages to be distributed"
    echo "    all\t\t\tdoes build, build-samples and build-pkg"
    echo "    help (default)\tshows this usage information"
    echo
    echo "Options:"
    echo "    --verbose\t\tdo not hide standard ouput, errors are still shown"
    echo
    echo "Command suffixes:"
    echo "    -32\t\t\tapply command for 32 bits PowerPC and Intel binaries"
    echo "    -64\t\t\tapply command for 32 bits PowerPC and Intel, and 64 bits Intel binaries"
    echo "    \t\t\tDefault applies command for both choices"
    echo "    \t\t\t(ie. build frameworks for both 32 and 64 bits platforms)"
    echo
    echo "Contents summary:"
    echo "    Found C++ frameworks project \t\t $SHOULD_CONSIDER_CXX"
    echo "    Found C frameworks project \t\t\t $SHOULD_CONSIDER_C"
    echo "    Found samples project \t\t\t $SHOULD_CONSIDER_SAMPLES"
    echo "    Can build Intel 64 bits binaries \t\t $SHOULD_CONSIDER_64B"
    echo "    Found C++ 64 bits frameworks project \t $SHOULD_CONSIDER_CXX_64B"
    echo "    Found C 64 bits frameworks project \t\t $SHOULD_CONSIDER_C_64B"
    echo "    Found 64 bits samples project \t\t $SHOULD_CONSIDER_SAMPLES_64B"
    echo "    Missing projects will be skiped without further warning."
}

# Checks that the user computer can run this script
check_config()
{
	# Check OS type and version
    os=`uname -s`
    version=`uname -r` # 10.2.0 form; 8.x = Mac OS X 10.4
    if [ $os != "Darwin" ]
      then
        echo "You're not running Mac OS X !"
        exit 1
    fi
    
   	version=${version%%.*}
   	if test $version -le 8 # 64 bits builds not supported on Mac OS X 10.4 and earlier
   	  then
   	    SHOULD_CONSIDER_64B="no"
   	fi
   	
   	# Check scripts tools
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
    
    # Check Xcode projects availability
    if ! test -d "$XCODE_CXX_ROOT_DIR/$XCODE_FRAMEWORKS_PROJECT"
      then
        SHOULD_CONSIDER_CXX="no"
    fi
    
    if ! test -d "$XCODE_C_ROOT_DIR/$XCODE_FRAMEWORKS_PROJECT"
      then
        SHOULD_CONSIDER_C="no"
    fi
    
    if ! test -d "$XCODE_SAMPLES_ROOT_DIR/$XCODE_SAMPLES_PROJECT"
      then
        SHOULD_CONSIDER_SAMPLES="no"
    fi
    
    # 64 bits projects
    if [ "$SHOULD_CONSIDER_64B" == "no" ]
      then
        SHOULD_CONSIDER_CXX_64B="no"
        SHOULD_CONSIDER_C_64B="no"
        SHOULD_CONSIDER_SAMPLES_64B="no"
    else
    	if ! test -d "$XCODE_CXX_ROOT_DIR/$XCODE_64B_FRAMEWORKS_PROJECT"
	      then
	        SHOULD_CONSIDER_CXX_64B="no"
	    fi
	    
	    if ! test -d "$XCODE_C_ROOT_DIR/$XCODE_64B_FRAMEWORKS_PROJECT"
	      then
	        SHOULD_CONSIDER_C_64B="no"
	    fi
	    
	    if ! test -d "$XCODE_SAMPLES_ROOT_DIR/$XCODE_64B_SAMPLE_PROJECT"
	      then
	        SHOULD_CONSIDER_SAMPLES_64B="no"
	    fi
    fi    
}

# Makes a directory if needed
make_dir()
{
    if ! test -d "$1"
      then
        mkdir -p "$1"
        check_last_process
    fi
}

# Checks that a directory exists and removes it
remove_dir()
{
    if test -d "$1"
      then
        rm -rfv "$1" > "$VERBOSE_OUTPUT"
    fi
}

# Checks that a file exists and removes it
remove_file()
{
    if test -f "$1"
      then
        rm -f "$1" > "$VERBOSE_OUTPUT"
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
    /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude ".*" -exclude codeblocks -exclude *vc2005 -exclude *vc2008 -exclude *mingw -exclude Makefile -exclude Main -exclude "Template for new ports" -exclude Linux -exclude Win32 -exclude win32 -exclude *.build -exclude *.dll -exclude *.dSYM -exclude qt -exclude wxwidgets -exclude X11 -exclude "$USER*" -exclude AL -strip-debug-symbols -resolve-src-symlinks "$1" "$2"
}

build_project()
{
	project_directory=$1
	project_file=$2
	target="All"

	cd "$project_directory"
	check_last_process
	
	if test -d "$project_directory/$project_file"
	  then
		printf "Building target $target from $project_file..."
		xcodebuild -project "$project_file" -target "$target" -parallelizeTargets -configuration "$XCODE_BUILD_STYLE" build > "$VERBOSE_OUTPUT"
		check_last_process " done"
	else
		echo "*** Missing file $project_directory/$project_file. Process stopped."
		exit 1
	fi
}

clean_project()
{
	project_directory=$1
	project_file=$2
	
	if test -d "$project_directory/$project_file"
	  then
		cd "$project_directory"
		xcodebuild -project "$project_file" -target "All" -configuration "$XCODE_BUILD_STYLE" clean > "$VERBOSE_OUTPUT"
		check_last_process " done"
	else
		if test -d "$project_directory"
		  then
			echo "*** Missing file $project_directory/$project_file. Process stopped."
			exit 1
		else
			echo " not found. Skiped"
		fi
	fi
}

# Delete built frameworks and intermediate object files
clean_all()
{
    cd "$ROOT_DIR"
    
    # Process cleaning for C++ project
    if [ "$SHOULD_CONSIDER_CXX" == "yes" ]
      then
        printf "Cleaning C++ products..."
        clean_project "$XCODE_CXX_ROOT_DIR" "$XCODE_FRAMEWORKS_PROJECT"
    fi
    
    
    # Process cleaning for C project
    if [ "$SHOULD_CONSIDER_C" == "yes" ]
      then
        printf "Cleaning C products..."
        clean_project "$XCODE_C_ROOT_DIR" "$XCODE_FRAMEWORKS_PROJECT"
    fi
    
    # Process cleaning for samples
    if [ "$SHOULD_CONSIDER_SAMPLES" == "yes" ]
      then
        printf "Cleaning samples..."
        clean_project "$XCODE_SAMPLES_ROOT_DIR" "$XCODE_SAMPLES_PROJECT"
    fi
    
    # Process cleaning for packages
    printf "Removing packages..."
    remove_dir "$PACKAGES_ROOT_DIR"
    check_last_process " done"
}

# Delete built frameworks and intermediate object files (64 bits)
clean_all_64b()
{
	if [ "$SHOULD_CONSIDER_64B" == "yes" ]
	  then
	    cd "$ROOT_DIR"
	    
	    # Process cleaning for C++ project
	    if [ "$SHOULD_CONSIDER_CXX_64B" == "yes" ]
	      then
	        printf "Cleaning 64 bits C++ products..."
	        clean_project "$XCODE_CXX_ROOT_DIR" "$XCODE_64B_FRAMEWORKS_PROJECT"
	    fi
	    
	    
	    # Process cleaning for C project
	    if [ "$SHOULD_CONSIDER_C_64B" == "yes" ]
	      then
	        printf "Cleaning 64 bits C products..."
	        clean_project "$XCODE_C_ROOT_DIR" "$XCODE_64B_FRAMEWORKS_PROJECT"
	    fi
	    
	    # Process cleaning for samples
	    if [ "$SHOULD_CONSIDER_64B_SAMPLES" == "yes" ]
	      then
	        printf "Cleaning 64 bits samples..."
			clean_project "$XCODE_SAMPLES_ROOT_DIR" "$XCODE_64B_SAMPLES_PROJECT"
	    fi
	    
	    # Process cleaning for packages
	    printf "Removing 64 bits packages..."
	    remove_dir "$PACKAGES_ROOT_DIR"
	    check_last_process " done"
	else
	    echo "Intel 64 bits is not supported on your computer. Process skiped"
	fi
}

# Build C and C++ frameworks
build_frameworks()
{
    # Go into the C++ project directory
    if [ "$SHOULD_CONSIDER_CXX" == "yes" ]
      then
	    echo "Building C++ SFML frameworks in $XCODE_BUILD_STYLE mode..."
		build_project "$XCODE_CXX_ROOT_DIR" "$XCODE_FRAMEWORKS_PROJECT"
		echo "All C++ SFML frameworks built."
    fi
    
    # Go into the C project directory
    if [ "$SHOULD_CONSIDER_C" == "yes" ]
      then
        echo "Building C SFML frameworks in $XCODE_BUILD_STYLE mode..."
		build_project "$XCODE_C_ROOT_DIR" "$XCODE_FRAMEWORKS_PROJECT" 
		echo "All C SFML frameworks built."
    fi
}

# Build C and C++ frameworks (with Intel 64 bits)
build_frameworks_64b()
{
	if [ "$SHOULD_CONSIDER_64B" == "yes" ]
	  then
	    # Go into the C++ project directory
	    if [ "$SHOULD_CONSIDER_CXX_64B" == "yes" ]
	      then
	        echo "Building 64 bits C++ SFML frameworks in $XCODE_BUILD_STYLE mode..."
			build_project "$XCODE_CXX_ROOT_DIR" "$XCODE_64B_FRAMEWORKS_PROJECT"
	        echo "All C++ SFML frameworks built."
	    fi
	    
	    # Go into the C project directory
	    if [ "$SHOULD_CONSIDER_C_64B" == "yes" ]
	      then
	        echo "Building C SFML frameworks in $XCODE_BUILD_STYLE mode..."
			build_project "$XCODE_C_ROOT_DIR" "$XCODE_64B_FRAMEWORKS_PROJECT"
	        echo "All C SFML frameworks built."
	    fi
	else
	    echo "Intel 64 bits is not supported on your computer. Process skiped"
	fi
}

# Build the samples
build_samples()
{
    # Go into the samples project directory
    if [ "$SHOULD_CONSIDER_SAMPLES" == "yes" ]
      then
        printf "Building SFML samples in $XCODE_BUILD_STYLE mode..."
		build_project "$XCODE_SAMPLES_ROOT_DIR" "$XCODE_SAMPLES_PROJECT" $XCODE_ALL_TARGETS
		echo "In order to run the samples, the SFML frameworks (located in \"lib\") must be copied in the /Library/Frameworks directory." > "$CXX_SAMPLES_DIR/README"
		check_last_process
    fi
}

# Build the samples (with Intel 64 bits)
build_samples_64b()
{
	if [ "$SHOULD_CONSIDER_64B" == "yes" ]
	  then
	    # Go into the samples project directory
	    if [ "$SHOULD_CONSIDER_SAMPLES_64B" == "yes" ]
	      then
	        printf "Building 64 bits SFML samples in $XCODE_BUILD_STYLE mode..."
			build_project "$XCODE_SAMPLES_ROOT_DIR" "$XCODE_64B_SAMPLES_PROJECT" $XCODE_ALL_TARGETS
			echo "In order to run the samples, the SFML frameworks (located in \"lib\") must be copied in the /Library/Frameworks directory." > "$CXX_SAMPLES_DIR/README"
	    fi
	else
	    echo "Intel 64 bits is not supported on your computer. Process skiped"
	fi
}

# Put the information files in the package
copy_info_files()
{
    for file in "${PACKAGE_INFO_FILES[@]}"
      do
        copy "$PACKAGES_INFO_FILES_DIR/$file" "$1"
        check_last_process
    done
}

# Build the archives of the C and C++ Dev and SDK packages
build_packages()
{
    cd "$ROOT_DIR"
    
    make_dir "$PACKAGES_ROOT_DIR"
    
    # Build the C++ SDK package
    if [ "$SHOULD_CONSIDER_CXX" == "yes" ]
      then
        printf "Building C++ SDK package..."
        make_dir "$CXX_SDK_PACKAGE_DIR"
        for dir in "${CXX_SDK_DIRS[@]}"
          do
            case "$dir" in
                "doc")
                    if ! test -f "$ROOT_DIR/$dir/html/index.htm"
                      then
                        echo "*** $ROOT_DIR/$dir/html/index.htm not found. Make sure the documentation has been built."
                        exit 1
                      fi
                      ;;
            esac
            
            copy "$ROOT_DIR/$dir" "$CXX_SDK_PACKAGE_DIR"
            
            # in 32 bits mode, remove the 64 bits dependencies directory
            case "$dir" in
                "extlibs")
                	remove_dir "$CXX_SDK_PACKAGE_DIR/extlibs/bin/x86_64"
                	check_last_process
                	;;
            esac
            
            check_last_process
        done
        copy_info_files "$CXX_SDK_PACKAGE_DIR"
		check_last_process
        
        # Build the archive
        cd "$PACKAGES_ROOT_DIR"
        tar -cjlf "$PACKAGES_ROOT_DIR/$CXX_SDK_PACKAGE.tar.bz2" "$CXX_SDK_PACKAGE" > "$VERBOSE_OUTPUT"
        check_last_process " done"
        
        # Build the C++ Dev package
        printf "Building C++ Development package..."
        make_dir "$CXX_DEV_PACKAGE_DIR"
        for dir in "${CXX_DEV_DIRS[@]}"
          do
            copy "$ROOT_DIR/$dir" "$CXX_DEV_PACKAGE_DIR"
            check_last_process
            
            case $dir in
                "build/xcode/templates")
                    # Special consideration for the templates folder that is to be moved in build/xcode
                    move_dir "$CXX_DEV_PACKAGE_DIR/templates" "$CXX_DEV_PACKAGE_DIR/build/xcode"
                    check_last_process
                    ;;
                "extlibs")
                    # Drop the libs-xcode and headers directories
                    remove_dir "$CXX_DEV_PACKAGE_DIR/extlibs/libs-xcode"
                    check_last_process
                    remove_dir "$CXX_DEV_PACKAGE_DIR/extlibs/headers"
                    check_last_process
                    # in 32 bits mode, remove the 64 bits dependencies directory
                    remove_dir "$CXX_DEV_PACKAGE_DIR/extlibs/bin/x86_64"
                    check_last_process
                    ;;
                *)
                    ;;
            esac
        done
        copy_info_files "$CXX_DEV_PACKAGE_DIR"
        
        # Build the archive
        cd "$PACKAGES_ROOT_DIR"
        tar -cjlf "$PACKAGES_ROOT_DIR/$CXX_DEV_PACKAGE.tar.bz2" "$CXX_DEV_PACKAGE" > "$VERBOSE_OUTPUT"
        check_last_process " done"
    fi # SHOULD_CONSIDER_CXX
    
    
    # Build the C SDK package
    if [ "$SHOULD_CONSIDER_C" == "yes" ]
      then
        printf "Building C SDK package..."
        make_dir "$C_SDK_PACKAGE_DIR"
        for dir in "${C_SDK_DIRS[@]}"
          do
            copy "$ROOT_DIR/$dir" "$C_SDK_PACKAGE_DIR"
            check_last_process
        done
        
        # in 32 bits mode, remove the 64 bits dependencies directory
        remove_dir "$C_DEV_PACKAGE_DIR/extlibs/bin/x86_64"
        check_last_process
        
        make_dir "$C_SDK_PACKAGE_SUB_DIR"
        for dir in "${C_SDK_SUB_DIRS[@]}"
          do
            copy "$ROOT_DIR/$dir" "$C_SDK_PACKAGE_SUB_DIR"
            check_last_process
        done
        copy_info_files "$C_SDK_PACKAGE_DIR"
        
        # Build the archive
        cd "$PACKAGES_ROOT_DIR"
        tar -cjlf "$PACKAGES_ROOT_DIR/$C_SDK_PACKAGE.tar.bz2" "$C_SDK_PACKAGE" > "$VERBOSE_OUTPUT"
        check_last_process " done"
        
        # Build the C Development package
        printf "Building C Development package..."
        make_dir "$C_DEV_PACKAGE_DIR"
        for dir in "${C_DEV_DIRS[@]}"
          do
            copy "$ROOT_DIR/$dir" "$C_DEV_PACKAGE_DIR"
            check_last_process
            
            case $dir in
                "extlibs")
                    # Drop the libs-xcode and headers directories
                    remove_dir "$C_DEV_PACKAGE_DIR/extlibs/libs-xcode"
                    check_last_process
                    remove_dir "$C_DEV_PACKAGE_DIR/extlibs/headers"
                    check_last_process
                    # in 32 bits mode, remove the 64 bits dependencies directory
			        remove_dir "$C_DEV_PACKAGE_DIR/extlibs/bin/x86_64"
			        check_last_process
                    ;;
            esac
        done
        
        make_dir "$C_DEV_PACKAGE_SUB_DIR"
        for dir in "${C_DEV_SUB_DIRS[@]}"
          do
            copy "$ROOT_DIR/$dir" "$C_DEV_PACKAGE_SUB_DIR"
            check_last_process
            
            case $dir in
                "CSFML/xcode/templates")
                    # Special consideration for the templates folder that is to be moved in build/xcode
                    move_dir "$C_DEV_PACKAGE_DIR/CSFML/templates" "$C_DEV_PACKAGE_DIR/CSFML/xcode"
                    check_last_process
                    ;;
            esac
        done
        copy_info_files "$C_DEV_PACKAGE_DIR"
        
        # Build the archive
        cd "$PACKAGES_ROOT_DIR"
        tar -cjlf "$PACKAGES_ROOT_DIR/$C_DEV_PACKAGE.tar.bz2" "$C_DEV_PACKAGE" > "$VERBOSE_OUTPUT"
        check_last_process " done"
    fi
    
    echo "All packages have been built. See $PACKAGES_ROOT_DIR."
}

# Build the archives of the C and C++ Dev and SDK packages (for Intel 64 bits)
build_packages_64b()
{
	if [ "$SHOULD_CONSIDER_64B" == "yes" ]
	  then
	    cd "$ROOT_DIR"
	    make_dir "$PACKAGES_ROOT_DIR_64B"
	    
	    # Build the C++ SDK package
	    if [ "$SHOULD_CONSIDER_CXX_64B" == "yes" ]
	      then
	        printf "Building 64 bits C++ SDK package..."
	        make_dir "$CXX_SDK_PACKAGE_DIR_64B"
	        for dir in "${CXX_64B_SDK_DIRS[@]}"
	          do
	            case "$dir" in
	                "doc")
	                    if ! test -f "$ROOT_DIR/$dir/html/index.htm"
	                      then
	                        echo "*** $ROOT_DIR/$dir/html/index.htm not found. Make sure the documentation has been built."
	                        exit 1
	                      fi
	                      ;;
	            esac
	            
	            copy "$ROOT_DIR/$dir" "$CXX_SDK_PACKAGE_DIR_64B"
	            check_last_process
	        done
	        copy_info_files "$CXX_SDK_PACKAGE_DIR_64B" 
	        check_last_process
	        
	        # in 64 bits mode, remove the 32 bits dependencies
	        remove_dir "$CXX_SDK_PACKAGE_DIR_64B/extlibs/bin/OpenAL.framework"
			check_last_process
			remove_dir "$CXX_SDK_PACKAGE_DIR_64B/extlibs/bin/sndfile.framework"
			check_last_process
	        
	        # Build the archive
	        cd "$PACKAGES_ROOT_DIR_64B"
	        tar -cjlf "$PACKAGES_ROOT_DIR_64B/$CXX_SDK_PACKAGE_64B.tar.bz2" "$CXX_SDK_PACKAGE_64B" > "$VERBOSE_OUTPUT"
	        check_last_process " done"
	        
	        # Build the C++ Dev package
	        printf "Building 64 bits C++ Development package..."
	        make_dir "$CXX_DEV_PACKAGE_DIR_64B"
	        for dir in "${CXX_64B_DEV_DIRS[@]}"
	          do
	            copy "$ROOT_DIR/$dir" "$CXX_DEV_PACKAGE_DIR_64B"
	            check_last_process
	            
	            case $dir in
	                "build/xcode/templates")
	                    # Special consideration for the templates folder that is to be moved in build/xcode
	                    move_dir "$CXX_DEV_PACKAGE_DIR_64B/templates" "$CXX_DEV_PACKAGE_DIR_64B/build/xcode"
	                    check_last_process
	                    ;;
	                "extlibs")
	                    # Drop the libs-xcode and headers directories
	                    remove_dir "$CXX_DEV_PACKAGE_DIR_64B/extlibs/libs-xcode"
	                    check_last_process
	                    remove_dir "$CXX_DEV_PACKAGE_DIR_64B/extlibs/headers"
	                    check_last_process
	                    ;;
	                *)
	                    ;;
	            esac
	        done
	        copy_info_files "$CXX_DEV_PACKAGE_DIR_64B"
	        check_last_process
	        
	        # in 64 bits mode, remove the 32 bits dependencies
	        remove_dir "$CXX_DEV_PACKAGE_DIR_64B/extlibs/bin/OpenAL.framework"
			check_last_process
			remove_dir "$CXX_DEV_PACKAGE_DIR_64B/extlibs/bin/sndfile.framework"
			check_last_process
	        
	        # Build the archive
	        cd "$PACKAGES_ROOT_DIR_64B"
	        tar -cjlf "$PACKAGES_ROOT_DIR_64B/$CXX_DEV_PACKAGE_64B.tar.bz2" "$CXX_DEV_PACKAGE_64B" > "$VERBOSE_OUTPUT"
	        check_last_process " done"
	    fi # SHOULD_CONSIDER_CXX
	    
	    
	    # Build the C SDK package
	    if [ "$SHOULD_CONSIDER_C_64B" == "yes" ]
	      then
	        printf "Building 64 bits C SDK package..."
	        make_dir "$C_SDK_PACKAGE_DIR_64B"
	        for dir in "${C_SDK_DIRS[@]}"
	          do
	            copy "$ROOT_DIR/$dir" "$C_SDK_PACKAGE_DIR_64B"
	            check_last_process
	        done
	        
	        make_dir "$C_SDK_PACKAGE_SUB_DIR"
	        for dir in "${C_SDK_SUB_DIRS[@]}"
	          do
	            copy "$ROOT_DIR/$dir" "$C_SDK_PACKAGE_SUB_DIR"
	            check_last_process
	        done
	        copy_info_files "$C_SDK_PACKAGE_DIR_64B"
	        
	        # Build the archive
	        cd "$PACKAGES_ROOT_DIR_64B"
	        tar -cjlf "$PACKAGES_ROOT_DIR_64B/$C_SDK_PACKAGE_64B.tar.bz2" "$C_SDK_PACKAGE_64B" > "$VERBOSE_OUTPUT"
	        check_last_process " done"
	        
	        # Build the C Development package
	        printf "Building 64 bits C Development package..."
	        make_dir "$C_DEV_PACKAGE_DIR_64B"
	        for dir in "${C_DEV_DIRS[@]}"
	          do
	            copy "$ROOT_DIR/$dir" "$C_DEV_PACKAGE_DIR_64B"
	            check_last_process
	            
	            case $dir in
	                "extlibs")
	                    # Drop the libs-xcode and headers directories
	                    remove_dir "$C_DEV_PACKAGE_DIR_64B/extlibs/libs-xcode"
	                    check_last_process
	                    remove_dir "$C_DEV_PACKAGE_DIR_64B/extlibs/headers"
	                    check_last_process
	                    ;;
	            esac
	        done
	        
	        make_dir "$C_DEV_PACKAGE_SUB_DIR"
	        for dir in "${C_DEV_SUB_DIRS[@]}"
	          do
	            copy "$ROOT_DIR/$dir" "$C_DEV_PACKAGE_SUB_DIR_64B"
	            check_last_process
	            
	            case $dir in
	                "CSFML/xcode/templates")
	                    # Special consideration for the templates folder that is to be moved in build/xcode
	                    move_dir "$C_DEV_PACKAGE_DIR_64B/CSFML/templates" "$C_DEV_PACKAGE_DIR_64B/CSFML/xcode"
	                    check_last_process
	                    ;;
	            esac
	        done
	        copy_info_files "$C_DEV_PACKAGE_DIR_64B"
	        
	        # Build the archive
	        cd "$PACKAGES_ROOT_DIR_64B"
	        tar -cjlf "$PACKAGES_ROOT_DIR_64B/$C_DEV_PACKAGE_64B.tar.bz2" "$C_DEV_PACKAGE_64B" > "$VERBOSE_OUTPUT"
	        check_last_process " done"
	    fi
	    
	    echo "All packages have been built. See $PACKAGES_ROOT_DIR_64B."
    else
	    echo "Intel 64 bits is not supported on your computer. Process skiped"
	fi
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
    
    for flag in $*
      do
        if [ "$flag" == "--verbose" ]
          then
            VERBOSE_OUTPUT="/dev/stdout"
        fi
    done
    
    case $action in
        "clean")
            clean_all $*
            clean_all_64b $*
            ;;
        
        "clean-32")
        	clean_all $*
            ;;
        
        "clean-64")
        	clean_all_64b $*
        	;;
        	
        "build")
            build_frameworks $*
            build_frameworks_64b $*
            ;;
            
        "build-32")
        	build_frameworks $*
        	;;
        	
        "build-64")
        	build_frameworks_64b $*
        	;;
        	
        "build-samples")
            build_samples $*
            build_samples_64b $*
            ;;
        "build-samples-32")
        	build_samples $*
        	;;
       	
       	"build-samples-64")
       		build_samples_64b $*
       		;;
       		
        "build-pkg")
            build_packages $*
            build_packages_64b $*
            ;;
            
        "build-pkg-32")
        	build_packages $*
        	;;
        
        "build-pkg-64")
        	build_packages_64b $*
        	;;
        	
        "all")
            build_frameworks $*
            build_frameworks_64b $*
            build_samples $*
            build_samples_64b $*
            build_packages $*
            build_packages_64b $*
            ;;
         
         "all-32")
	         build_frameworks $*
	         build_samples $*
	         build_packages $*
	         ;;
         
         "all-64")
	         build_frameworks_64b $*
	         build_samples_64b $*
	         build_packages_64b $*
	         ;;
	         
         "-h" | "--help" | "help")
            print_usage $*
            ;;
          "--verbose")
            echo "--verbose option cannot be used alone. See usage."
            ;;
        *)
            echo "Unknow action $action."
            print_usage $*
            ;;
    esac
}

main $*
