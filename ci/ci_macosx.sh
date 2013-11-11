#!/bin/bash
# Copyright © 2013 Fabian Schuiki
# This script fetches the latest changes from the OpenSkyscraper repository and
# calls the build script for new tags and commits. It is intended to be run on
# a regular basis (e.g. every hour).

set -e # exit if any of the commands fail
SCRIPTWD="$PWD"

if [ -z "$DEPLOY_HOST" ]; then
	echo "environment variable DEPLOY_HOST must be set"
	exit 1
fi

if [ -z "$CI_DIR" ]; then
	CI_DIR=/tmp/openskyscraper-ci
	echo "# using default directory $CI_DIR"
fi
if [ ! -d "$CI_DIR" ]; then
	mkdir "$CI_DIR"
fi
cd "$CI_DIR"

REPO_DIR="repo"
REPO_URL="https://github.com/fabianschuiki/OpenSkyscraper.git"

echo_main() {
	echo -e "\033[1m$@\033[0m"
}

echo_sub() {
	echo -e "\033[34m==> $@\033[0m"
}

echo_error() {
	echo -e "\033[31;1m*** $@\033[0m"
}

# Clone the repository if required.
if [ ! -d "$REPO_DIR/.git" ]; then
	echo_main "Cloning repository"
	git clone "$REPO_URL" "$REPO_DIR"
fi

# Change into the repository and fetch the latest changes.
echo_main "Detecting changes"
LASTWD="$PWD"
cd "$REPO_DIR"

echo_sub "updating submodules"
git submodule update --init

echo_sub "fetching commits and tags"
git fetch origin
git fetch --tags
VERSION_TAGS=$(git tag -l | grep ^v\\d)
echo "$VERSION_TAGS"

# Perform self-update if requested.
if [ "$1" == "update" ]; then
	git checkout -f remotes/origin/master
	cp ci/ci_macosx.sh "$SCRIPTWD/$0"
	exit 0
fi

echo_sub "fetching list of deployed versions"
DEPLOYED_VERSIONS=$(curl -l "ftp://$DEPLOY_HOST/") # | grep ^[^\.]
echo "$DEPLOYED_VERSIONS"

deploy() {
	echo_sub "compiling"
	cd "$CI_DIR"
	LOG_TEMP="build.log"
	LOG_PATH="openskyscraper-$VERSION-macosx.log"
	set +e # don't exit if the build breaks
	T0=`date +%s`
	"$REPO_DIR/ci/build_macosx.sh" "$VERSION" 1> "$LOG_TEMP" 2>&1
	if [ $? -ne 0 ]; then
		echo_error "Deploying $VERSION failed"
		RESULT="failed"
	else
		RESULT="passed"
	fi
	DURATION=$(expr $(date +%s) - $T0)
	set -e
	echo "# Build $RESULT ($DURATION seconds)" | cat - "$LOG_TEMP" > "$LOG_PATH"
	echo_sub "uploading log"
	curl --ftp-create-dirs -T "$LOG_PATH" "ftp://$DEPLOY_HOST/"
	cd "$REPO_DIR"
}

# Deploy tags that are new.
for TAG in $VERSION_TAGS; do
	VERSION=${TAG:1}
	if [ -z $(echo "$DEPLOYED_VERSIONS" | grep "^openskyscraper-$VERSION-macosx.tar.bz2$") ]; then
		echo_main "Deploying $VERSION"
		git checkout -f "$TAG"
		deploy
	fi
done

# Checkout master and check whether it has not yet been deployed.
git checkout -f remotes/origin/master
COMMIT_HASH=$(git log -n 1 --format=format:"%h")
COMMIT_DATE=$(date -j -r $(git log -n 1 --format=format:"%at") +"%Y%m%d-%H%M")
VERSION="$COMMIT_DATE-$COMMIT_HASH"
if [ -z $(echo "$DEPLOYED_VERSIONS" | grep "^openskyscraper-$VERSION-macosx.tar.bz2$") ]; then
	echo_main "Deploying $VERSION"
	deploy
fi

cd "$LASTWD"