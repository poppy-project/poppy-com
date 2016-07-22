#!/bin/sh

if [ "$SUITE" = "build" ]; then
    ## the following automatically builds the doxygen
    ## documentation and pushes it to the gh_pages branch

    # First, set up credentials using the environment variables
    # GIT_NAME, GIT_EMAIL and GH_TOKEN. These were passed
    # encrypted to travis and should have been decrypted
    # using travis' private key before this script was run.
    git config --global user.name $GIT_NAME
    git config --global user.email $GIT_EMAIL

    # Clone the whole repo again.
    git checkout $TRAVIS_BRANCH
    OWNER_NAME=`echo $TRAVIS_REPO_SLUG | awk -F"/" '{print $1}'`
    REPO_NAME=`echo $TRAVIS_REPO_SLUG | awk -F"/" '{print $2}'`
    echo $TRAVIS_BRANCH
    sed -i '/Build Status/c[![Build Status](https://travis-ci.org/'$TRAVIS_REPO_SLUG'.svg?branch='$TRAVIS_BRANCH')](https://travis-ci.org/'$TRAVIS_REPO_SLUG')[![Coverage Status](https://coveralls.io/repos/'$TRAVIS_REPO_SLUG'/badge.svg?branch='$TRAVIS_BRANCH')](https://coveralls.io/github/'$TRAVIS_REPO_SLUG'?branch='$TRAVIS_BRANCH')' README.md
    sed -i '/Please read /cPlease read [the code documentation](http://'$OWNER_NAME'.github.io/'$REPO_NAME'/)\' README.md
    cat README.md
    git add README.md
    git commit -m "Auto-updating README badges."
    git push --quiet https://$GH_TOKEN@github.com/$TRAVIS_REPO_SLUG $TRAVIS_BRANCH
    # Switch to gh_pages branch
    make docs
    git clone -b gh-pages https://$GH_TOKEN@github.com/$TRAVIS_REPO_SLUG.git full
    cp -r Docs full/
    cd full
    # cd into the docs dir and commit and push the new docs.
    git status
    git add Docs/*
    git commit -m "Auto-updating $TRAVIS_REPO_SLUG documentation"
    git push --quiet https://$GH_TOKEN@github.com/$TRAVIS_REPO_SLUG gh-pages

elif [ "$SUITE" = "tests" ]; then
    coveralls --exclude stdio2 -e test -e test_mngmnt -e hal --gcov-options '\-lp'


else
    echo "Unknown suite variable - '$SUITE'. Terminating ..."
    exit 1
fi
