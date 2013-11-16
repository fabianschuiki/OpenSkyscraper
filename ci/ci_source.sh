#!/bin/bash
# Copyright © 2013 Fabian Schuiki
# This script fetches the latest changes from the OpenSkyscraper repository and
# packages them in a source tarball for distribution. It is intended to be
# called on a regular basis (e.g. every hour or day).

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
VERSION_TAGS=$(git tag -l | (grep ^v\\d || :))

# Perform self-update if requested.
if [ "$1" == "update" ]; then
	git checkout -f remotes/origin/master
	cp ci/ci_source.sh "$SCRIPTWD/$0"
	exit 0
fi

echo_sub "fetching list of deployed versions"
DEPLOYED_VERSIONS=$(curl -l "ftp://$DEPLOY_HOST/" | (grep ^[^\.] || :))

deploy() {
	local DIST_DIR="src-$VERSION"
	local DIST_ARCHIVE="openskyscraper-$VERSION-source.tar.bz2"
	cd "$CI_DIR"

	echo_sub "copying source"
	cp -aP "$REPO_DIR" "$DIST_DIR"

	echo_sub "stripping files"
	(cd $DIST_DIR && rm -rf .git)
	(cd $DIST_DIR && rm -rf *~)
	(cd $DIST_DIR && rm -rf doc/*)
	
	echo_sub "annotating"
	echo -e "# This snapshot of the source was bundled automatically by the ci_source.sh\n# script on $(date).\n$VERSION" > "$DIST_DIR/VERSION"

	echo_sub "archiving"
	tar -jcf "$DIST_ARCHIVE" "$DIST_DIR"

	echo_sub "deploying"
	curl --ftp-create-dirs -T "$DIST_ARCHIVE" "ftp://$DEPLOY_HOST/"

	cd "$REPO_DIR"
}

# Deploy tags that are new.
for TAG in $VERSION_TAGS; do
	VERSION=${TAG:1}
	if [ -z $(echo "$DEPLOYED_VERSIONS" | grep "^openskyscraper-$VERSION-source.tar.bz2$") ]; then
		echo_main "Deploying $VERSION"
		git checkout -f "$TAG"
		deploy
	fi
done

formatdate() {
	if [ $(uname | grep "^Darwin") ]; then
		date -j -r $1 +"%Y%m%d-%H%M"
	else
		date -d @$1 +"%Y%m%d-%H%M"
	fi
}

# Checkout master and check whether it has not yet been deployed.
git checkout -f remotes/origin/master
COMMIT_HASH=$(git log -n 1 --format=format:"%h")
COMMIT_DATE=$(formatdate $(git log -n 1 --format=format:"%at"))
VERSION="$COMMIT_DATE-$COMMIT_HASH"
if [ -z $(echo "$DEPLOYED_VERSIONS" | grep "^openskyscraper-$VERSION-source.tar.bz2$") ]; then
	echo_main "Deploying $VERSION"
	deploy
fi

cd "$LASTWD"