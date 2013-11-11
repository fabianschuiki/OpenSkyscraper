#!/bin/bash
# Copyright © 2013 Fabian Schuiki
# This script builds the OpenSkyscraper game by installing the necessary
# dependencies and compiling the final application.

set -e # exit if any of these commands fail

export CFLAGS="$CFLAGS -arch i386 -arch x86_64"
export LDFLAGS="$LDFLAGS -arch i386 -arch x86_64"

BUILD_DIR="$PWD/build"
PROJECT_DIR="$(cd "$(dirname "$0")/../" && pwd)"

MSPACK_VERSION="0.4alpha"
MSPACK_URL="http://www.cabextract.org.uk/libmspack/libmspack-$MSPACK_VERSION.tar.gz"
MSPACK_DIR="$BUILD_DIR/mspack-$MSPACK_VERSION"
MSPACK_BUILD_DIR="$MSPACK_DIR/build"

SFML_VERSION="1.6"
SFML_URL="http://sourceforge.net/projects/sfml/files/sfml/$SFML_VERSION/SFML-$SFML_VERSION-dev-macosx-ub32+64.tar.bz2"
SFML_DIR="$BUILD_DIR/sfml-$SFML_VERSION"
SFML_BUILD_DIR="$SFML_DIR/lib64"

ROCKET_VERSION="1.2.1"
ROCKET_URL="https://github.com/lloydw/libRocket/archive/release-$ROCKET_VERSION.zip"
ROCKET_DIR="$BUILD_DIR/rocket-$ROCKET_VERSION"
ROCKET_BUILD_DIR="$ROCKET_DIR/_build"

OSS_VERSION="0.1"
OSS_DIR="$BUILD_DIR/oss-$OSS_VERSION"
OSS_NAME="openskyscraper-$OSS_VERSION-macosx"
OSS_ARCHIVE="$OSS_NAME.tar.bz2"

echo_main() {
	echo -e "\033[1m$@\033[0m"
}

echo_sub() {
	echo -e "\033[34m==> $@\033[0m"
}

# Create the build directory if required.
if [ ! -d "$BUILD_DIR" ]; then
	mkdir "$BUILD_DIR"
fi

# Build libmspack if necessary.
if [ ! -f "$MSPACK_BUILD_DIR/lib/libmspack.a" ]; then
	echo_main "Installing libmspack $MSPACK_VERSION"

	if [ ! -d "$MSPACK_DIR" ]; then
		# Download the packaged version if necessary.
		MSPACK_PKG="$BUILD_DIR/$(basename "$MSPACK_URL")"
		if [ ! -f "$MSPACK_PKG" ]; then
			echo_sub "downloading"
			curl -L -o "$MSPACK_PKG" "$MSPACK_URL"
		fi

		# Extract the archive.
		echo_sub "unpacking"
		tar -xzf "$MSPACK_PKG" -C "$BUILD_DIR"
		mv "$(dirname "$MSPACK_PKG")/$(basename -s .tar.gz "$MSPACK_PKG")" "$MSPACK_DIR"
	fi

	echo_sub "building"
	LASTWD="$PWD"
	cd "$MSPACK_DIR"
	./configure --disable-shared --enable-static --prefix="$MSPACK_BUILD_DIR" --disable-dependency-tracking
	make
	make install
	cd "$LASTWD"
fi

# Build SFML if necessary.
if [ ! -d "$SFML_BUILD_DIR/SFML.framework" ]; then
	echo_main "Installing SFML $SFML_VERSION"

	if [ ! -d "$SFML_DIR" ]; then
		# Download the packaged version if necessary.
		SFML_PKG="$BUILD_DIR/$(basename "$SFML_URL")"
		if [ ! -f "$SFML_PKG" ]; then
			echo_sub "downloading"
			curl -L -o "$SFML_PKG" "$SFML_URL"
		fi

		# Extract the archive.
		echo_sub "unpacking"
		tar -xzf "$SFML_PKG" -C "$BUILD_DIR"
		mv "$(dirname "$SFML_PKG")/$(basename -s .tar.bz2 "$SFML_PKG")" "$SFML_DIR"
	fi
fi

# Build libRocket if necessary.
if [ ! -f "$ROCKET_BUILD_DIR/lib/libRocketCore.a" ]; then
	echo_main "Installing libRocket $ROCKET_VERSION"

	if [ ! -d "$ROCKET_DIR" ]; then
		# Download the packaged version if necessary.
		ROCKET_PKG="$BUILD_DIR/$(basename "$ROCKET_URL")"
		if [ ! -f "$ROCKET_PKG" ]; then
			echo_sub "downloading"
			curl -L -o "$ROCKET_PKG" "$ROCKET_URL"
		fi

		# Extract the archive.
		echo_sub "unpacking"
		unzip -q -d "$BUILD_DIR" "$ROCKET_PKG"
		mv "$(dirname "$ROCKET_PKG")/libRocket-$(basename -s .zip "$ROCKET_PKG")" "$ROCKET_DIR"
	fi


	echo_sub "building"
	LASTWD="$PWD"
	cd "$ROCKET_DIR/Build"
	cmake -D BUILD_PYTHON_BINDINGS=OFF -D BUILD_SAMPLES=OFF -D BUILD_SHARED_LIBS=OFF -D CMAKE_INSTALL_PREFIX="$ROCKET_BUILD_DIR" CMAKE_OSX_ARCHITECTURES="i386;x86_64" .
	make
	make install
	cd "$LASTWD"
fi

# Build OpenSkyscraper.
echo_main "Building OpenSkyscraper $OSS_VERSION"
if [ ! -d "$OSS_DIR" ]; then
	mkdir "$OSS_DIR"
fi
cd "$OSS_DIR"

echo_sub "compiling"
cmake -D SFMLDIR="$SFML_BUILD_DIR" -D MSPACKDIR="$MSPACK_BUILD_DIR" -D ROCKETDIR="$ROCKET_BUILD_DIR" -D CMAKE_BUILD_TYPE="Release" -D CMAKE_OSX_ARCHITECTURES="i386;x86_64" -D VERSION="$OSS_VERSION" "$PROJECT_DIR"
make
make install

echo_sub "archiving"
tar -jcf "$OSS_ARCHIVE" OpenSkyscraper.app

# Deploy via FTP. The DEPLOY_HOST is likely to look like <user>:<pass>@<host>.
# It is omitted from this script for security reasons.
echo_sub "deploying"
curl --ftp-create-dirs -T "$OSS_ARCHIVE" "ftp://${DEPLOY_HOST}/binaries/$OSS_VERSION/"

echo -e "\033[32;1mDone, OpenSkyscraper $OSS_VERSION for Mac OS X deployed\033[0m"
